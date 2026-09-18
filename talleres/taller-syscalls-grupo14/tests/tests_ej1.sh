#!/usr/bin/env bash
#
# SSOO - Ejercicio 1 (relay) - tests
#
# Uso (desde la raíz del taller, adentro de la VM):
#   bash tests/tests_ej1.sh ./bin/relay_espejo [--check=NOMBRE]
#
# NOMBRE puede ser: salida | parametros | syscalls | huerfanos
# Si no se pasa --check, corren las cuatro secciones.
#
# Normalmente no hace falta invocarlo a mano: el Makefile lo corre
# adentro de la VM con "make test-ej1" (y "make test-ej1-salida", etc.
# para una sola sección).
#
# Variables de entorno:
#   REF             binario de referencia de la cátedra (default: bin/relay)
#   TIMEOUT_FACTOR  multiplica todos los timeouts en máquinas lentas
#
# A diferencia de La Cadena de Mando, acá NO hay componente aleatorio: la
# salida de 'relay' es determinística salvo por los PIDs. Por eso se
# compara la salida de la entrega contra la del binario de referencia,
# línea por línea, después de normalizar todos los números (PIDs) a "#".
# Esa normalización tiene un efecto lateral útil: las N líneas
# "[HIJO <pid>] Esperando señal..." quedan idénticas entre si, así que el
# orden en que los hijos ganan la CPU deja de importar.

set -u

# shellcheck source=lib.sh
. "$(dirname "$0")/lib.sh"

tests_init "$@"

REF="${REF:-bin/relay}"

# El binario de referencia viene versionado en el repo y puede perder el
# bit de ejecución al clonar o descomprimir.
[ -f "$REF" ] && [ ! -x "$REF" ] && chmod +x "$REF" 2>/dev/null

correr_alumno() { correr "$1" "$2" "$BIN" "${@:3}"; }
correr_ref()    { correr "$1" "$2" "$REF" "${@:3}"; }

# Deja la salida en una forma comparable entre la referencia y la entrega:
#   - el nombre/ruta del ejecutable (argv[0], que aparece en el "Uso:")
#     pasa a ser "PROG": no tiene por que llamarse igual;
#   - todos los números pasan a "#": los PIDs son arbitrarios.
# El orden importa: las rutas pueden tener dígitos.
normalizar() {
    local archivo="$1" prog="$2"
    sed -E -e "s#(\./)?${prog}#PROG#g" \
           -e "s#(\./)?$(basename "$prog")#PROG#g" \
           -e 's/[0-9]+/#/g' "$archivo"
}

# ---------------------------------------------------------------------
# ¿Podemos correr el binario de referencia? Si no, las secciones que
# comparan contra el se saltean con un mensaje claro (es un problema del
# entorno, no de la entrega).
REF_OK=1
MOTIVO_REF=""
if [ ! -x "$REF" ]; then
    REF_OK=0
    MOTIVO_REF="no encontré el binario de referencia en '$REF' (pasalo con REF=...)"
elif ! correr_ref 5 "$TMPDIR/ref_smoke.txt" 1 >/dev/null 2>&1; then
    REF_OK=0
    MOTIVO_REF="el binario de referencia '$REF' no se puede ejecutar acá"
fi

# ---------------------------------------------------------------------
if run_seccion salida; then
echo "=== Salida idéntica a la del binario de referencia (N=1..5) ==="
if [ "$REF_OK" -eq 0 ]; then
    skip "$MOTIVO_REF"
else
    # Cada N se corre 3 veces: si la entrega tiene una carrera (por
    # ejemplo le falta el fflush, o manda las señales sin esperar a que
    # los hijos instalen el handler), es probable que al menos una de las
    # corridas salga distinta.
    REPS=3
    for N in 1 2 3 4 5; do
        check
        correr_ref 10 "$TMPDIR/ref.txt" "$N"
        rc_ref=$?
        normalizar "$TMPDIR/ref.txt" "$REF" > "$TMPDIR/ref.norm"

        ok_n=1
        for _ in $(seq 1 $REPS); do
            correr_alumno 10 "$TMPDIR/alu.txt" "$N"
            rc_alu=$?

            if [ $rc_alu -eq 124 ]; then
                fail "N=$N: la entrega se cuelga (timeout)"
                ok_n=0; break
            fi
            if [ $rc_alu -ge 128 ]; then
                fail "N=$N: la entrega termina por señal (posible segfault), rc=$rc_alu"
                ok_n=0; break
            fi
            if [ $rc_alu -ne $rc_ref ]; then
                fail "N=$N: exit code $rc_alu, el de referencia es $rc_ref"
                ok_n=0; break
            fi

            normalizar "$TMPDIR/alu.txt" "$BIN" > "$TMPDIR/alu.norm"
            if ! diff -u "$TMPDIR/ref.norm" "$TMPDIR/alu.norm" > "$TMPDIR/diff.txt"; then
                fail "N=$N: la salida no coincide con la de 'relay' (PIDs normalizados a #):"
                sed 's/^/         /' "$TMPDIR/diff.txt" >&2
                ok_n=0; break
            fi
        done

        if [ $ok_n -eq 1 ]; then
            echo "  [OK] N=$N: salida idéntica en $REPS corridas"
            pass
        fi
    done

    # Chequeo extra que la normalización se come: todas las líneas de
    # [PADRE ...] tienen que hablar del mismo PID, y ese PID no puede ser
    # el de ningún hijo.
    check
    correr_alumno 10 "$TMPDIR/alu.txt" 3
    PADRES=$(grep -oE '\[PADRE [0-9]+\]' "$TMPDIR/alu.txt" | sort -u | wc -l)
    HIJOS=$(grep -oE '\[HIJO [0-9]+\]' "$TMPDIR/alu.txt" | sort -u | wc -l)
    PID_PADRE=$(grep -oE '\[PADRE [0-9]+\]' "$TMPDIR/alu.txt" | head -1 | grep -oE '[0-9]+')
    if [ "$PADRES" -ne 1 ]; then
        fail "aparecen $PADRES PIDs distintos como [PADRE ...] (debería ser uno solo)"
    elif [ "$HIJOS" -ne 3 ]; then
        fail "con N=3 aparecen $HIJOS PIDs distintos como [HIJO ...] (deberían ser 3)"
    elif grep -qE "\[HIJO $PID_PADRE\]" "$TMPDIR/alu.txt"; then
        fail "el PID del padre ($PID_PADRE) aparece también como [HIJO ...]"
    else
        echo "  [OK] 1 padre y 3 hijos con PIDs distintos"
        pass
    fi
fi
echo ""
fi

# ---------------------------------------------------------------------
if run_seccion parametros; then
echo "=== Parámetros inválidos: mismo rechazo que el binario de referencia ==="
# Sin argumentos, N fuera de [1,5], no numérico, negativo y de más.
for args in "" "0" "6" "-1" "abc" "3 3"; do
    check
    # shellcheck disable=SC2086
    correr_alumno 5 "$TMPDIR/alu.txt" $args
    rc_alu=$?

    if [ $rc_alu -eq 124 ]; then
        fail "con parámetros ($args) el programa se cuelga"
        continue
    fi
    if [ $rc_alu -ge 128 ]; then
        fail "con parámetros ($args) el programa termina por señal (posible segfault), rc=$rc_alu"
        continue
    fi

    if [ "$REF_OK" -eq 0 ]; then
        # Sin referencia disponible, al menos exigimos que no explote y
        # que avise el error con exit code distinto de 0.
        if [ "$args" != "3 3" ] && [ $rc_alu -eq 0 ]; then
            fail "con parámetros ($args) el programa devuelve 0 (debería rechazarlos)"
        else
            echo "  [OK] parámetros ($args) rechazados prolijamente (rc=$rc_alu)"
            pass
        fi
        continue
    fi

    # shellcheck disable=SC2086
    correr_ref 5 "$TMPDIR/ref.txt" $args
    rc_ref=$?
    normalizar "$TMPDIR/ref.txt" "$REF" > "$TMPDIR/ref.norm"
    normalizar "$TMPDIR/alu.txt" "$BIN" > "$TMPDIR/alu.norm"

    if [ $rc_alu -ne $rc_ref ]; then
        fail "con parámetros ($args): exit code $rc_alu, el de referencia es $rc_ref"
    elif ! diff -u "$TMPDIR/ref.norm" "$TMPDIR/alu.norm" > "$TMPDIR/diff.txt"; then
        fail "con parámetros ($args): el mensaje de error no coincide con el de 'relay':"
        sed 's/^/         /' "$TMPDIR/diff.txt" >&2
    else
        echo "  [OK] parámetros ($args) rechazados igual que la referencia (rc=$rc_alu)"
        pass
    fi
done
echo ""
fi

# ---------------------------------------------------------------------
if run_seccion syscalls; then
echo "=== Uso de las syscalls pedidas (fork/signal/kill/pause/wait) ==="
if ! command -v strace >/dev/null 2>&1; then
    skip "no hay strace instalado en la VM (algo salió mal en 'make install')"
else
    SYSCALLS="clone,clone3,fork,vfork,kill,tgkill,rt_sigaction,signal,pause,rt_sigsuspend,wait4,waitid"
    if ! correr 15 "$TMPDIR/strace_err.txt" strace -f -e trace="$SYSCALLS" \
            -o "$TMPDIR/trace.txt" "$BIN" 3; then
        skip "no pude correr strace acá ($(head -1 "$TMPDIR/strace_err.txt" 2>/dev/null))"
    else
        # (etiqueta legible ; regex de syscalls que la satisfacen)
        for req in "fork/clone:(clone[3]?|fork|vfork)" \
                   "instalación de handler (signal/sigaction):(rt_sigaction|signal)" \
                   "kill:(kill|tgkill)" \
                   "espera pasiva (pause/sigsuspend):(pause|rt_sigsuspend)" \
                   "wait:(wait4|waitid)"; do
            check
            etiqueta="${req%%:*}"
            regex="${req#*:}"
            if grep -qE "(^|[0-9] +)$regex\(" "$TMPDIR/trace.txt"; then
                echo "  [OK] usa $etiqueta"
                pass
            else
                fail "no se observa $etiqueta en la traza de syscalls"
            fi
        done
    fi
fi
echo ""
fi

# ---------------------------------------------------------------------
if run_seccion huerfanos; then
    seccion_huerfanos
fi

resumen
