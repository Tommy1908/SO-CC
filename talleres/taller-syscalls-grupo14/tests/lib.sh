#!/usr/bin/env bash
#
# Funciones comunes a los tests de los dos ejercicios.
#
# Los tests corren SIEMPRE adentro de la VM (ver el Makefile), o sea sobre
# Linux, parados en la raíz del taller. Por eso todas las rutas son
# relativas y se pueden dar por sentadas herramientas de GNU/Linux:
# setsid, timeout, pkill, ps, nm, strace.

# En máquinas lentas (por ejemplo una Mac con Apple Silicon, donde la VM
# corre emulada por software) los timeouts se pueden estirar:
#   TIMEOUT_FACTOR=3 make test-ej2
TIMEOUT_FACTOR="${TIMEOUT_FACTOR:-1}"

TOTAL=0
OK=0
FAIL=0

BIN=""
CHECK="todos"

fail()  { FAIL=$((FAIL+1)); echo "  [FAIL] $1" >&2; }
pass()  { OK=$((OK+1)); }
check() { TOTAL=$((TOTAL+1)); }
skip()  { echo "  [SKIP] $1"; }

# Parsea los argumentos comunes: la ruta al binario y un --check=SECCION
# opcional (para poder correr una sola seccion, útil para corregir con
# puntaje por partes).
tests_init() {
    local arg
    for arg in "$@"; do
        case "$arg" in
            --check=*) CHECK="${arg#--check=}" ;;
            *) [ -z "$BIN" ] && BIN="$arg" ;;
        esac
    done

    if [ -z "$BIN" ] || [ ! -x "$BIN" ]; then
        echo "uso: $0 /ruta/al/binario [--check=SECCION]" >&2
        exit 1
    fi

    TMPDIR=$(mktemp -d)
    trap 'rm -rf "$TMPDIR"' EXIT
}

# ¿Toca correr esta sección?
run_seccion() {
    case "$CHECK" in
        todos|"$1") return 0 ;;
        *) return 1 ;;
    esac
}

# Corre un programa con timeout, deja stdout+stderr en $2 y devuelve su
# exit code (124 si se colgó).
#
# IMPORTANTE: si la entrega se cuelga (el clásico pause() que nunca
# despierta), "timeout" solo mata al proceso que arrancó directamente;
# los hijos ya forkeados quedarían huérfanos. Por eso se corre todo bajo
# "setsid" (grupo de procesos propio) y después se barre el grupo entero
# con pkill -g. Se barre SIEMPRE, no solo si hubo timeout: una entrega
# rota puede terminar el padre sin esperar a los hijos y dejarlos
# huérfanos aunque el timeout nunca haya disparado.
correr() {
    local segundos="$1" out="$2" prog="$3"
    shift 3

    segundos=$((segundos * TIMEOUT_FACTOR))

    setsid timeout -k 1 "$segundos" "$prog" "$@" > "$out" 2>&1 &
    local pid=$!
    wait "$pid"
    local rc=$?

    pkill -9 -g "$pid" 2>/dev/null || true

    return "$rc"
}

# Chequeo final compartido: que no hayan quedado procesos vivos del
# binario después de todas las corridas.
seccion_huerfanos() {
    echo "=== Sin procesos huérfanos/zombies después de correr ==="
    check
    sleep 1
    local binname restantes
    binname=$(basename "$BIN")
    restantes=$(ps -eo pid,comm,args | awk -v b="$binname" '$2 == b {print}')
    if [ -n "$restantes" ]; then
        fail "quedaron procesos vivos del binario después de terminar las corridas:"
        echo "$restantes" | sed 's/^/         /' >&2
    else
        echo "  [OK] no quedaron procesos huérfanos"
        pass
    fi
    echo ""
}

# Resumen y exit code de los tests.
resumen() {
    local raya="================================================"
    if [ "$FAIL" -gt 0 ]; then
        {
            echo "$raya"
            echo " Resultado: $OK/$TOTAL checks OK, $FAIL fallidos"
            echo "$raya"
        } >&2
        exit 1
    fi
    echo "$raya"
    echo " Resultado: $OK/$TOTAL checks OK, $FAIL fallidos"
    echo "$raya"
    exit 0
}
