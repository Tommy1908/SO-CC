#!/usr/bin/env bash
#
# SSOO - Ejercicio 2 (La Cadena de Mando) - tests
#
# Uso (desde la raíz del taller, adentro de la VM):
#   bash tests/tests_ej2.sh ./bin/cadenamando [--check=NOMBRE]
#
# NOMBRE puede ser: simbolos | parametros | invariantes | huerfanos
# Si no se pasa --check, corren las cuatro secciones.
#
# Normalmente no hace falta invocarlo a mano: el Makefile lo corre
# adentro de la VM con "make test-ej2" (y "make test-ej2-invariantes",
# etc. para una sola sección).
#
# Variables de entorno:
#   NM              herramienta para inspeccionar símbolos (default: nm)
#   TIMEOUT_FACTOR  multiplica todos los timeouts en máquinas lentas
#
# Como el juego tiene un componente aleatorio (cada hijo sortea un
# número), NO se puede testear contra un output exacto. En cambio se
# verifican invariantes que TIENEN que cumplirse en cualquier corrida
# válida, sobre muchas corridas y varias combinaciones de N/K/J.

set -u

# shellcheck source=lib.sh
. "$(dirname "$0")/lib.sh"

tests_init "$@"

NM="${NM:-nm}"

# ---------------------------------------------------------------------
if run_seccion simbolos; then
echo "=== Símbolos prohibidos en el binario ==="
check
PROHIBIDOS="pipe|pipe2|mkfifo|shmget|shmat|shmctl|msgget|msgsnd|msgrcv|semget|semop|socket|socketpair"
# La consigna pide que toda la comunicación sea con señales: nada de
# pipes, memoria compartida ni sockets. Se mira la tabla de símbolos
# dinámicos (nm -D, lo que el binario le pide a libc) y también los
# símbolos propios, por si alguien linkeó estático.
ENCONTRADOS=$(
    { "$NM" -D --undefined-only "$BIN" 2>/dev/null; "$NM" "$BIN" 2>/dev/null; } \
        | grep -E "^[0-9a-fA-F ]* *[UtTwW] ($PROHIBIDOS)$" || true
)
if [ -n "$ENCONTRADOS" ]; then
    fail "el binario usa símbolos prohibidos (pipes/shm/sockets):"
    echo "$ENCONTRADOS" | sed 's/^/         /' >&2
else
    echo "  [OK] no se detectaron pipes/memoria compartida/sockets"
    pass
fi
echo ""
fi

# ---------------------------------------------------------------------
if run_seccion parametros; then
echo "=== Validación de parámetros inválidos (no debe colgarse ni segfaultear) ==="
for args in "10 3 0" "5 3 5" "3 3 -1" "3 0 1" "abc 3 1"; do
    check
    # shellcheck disable=SC2086
    correr 3 "$TMPDIR/out.txt" "$BIN" $args
    rc=$?
    if [ $rc -eq 124 ]; then
        fail "con parámetros ($args) el programa se cuelga"
    elif [ $rc -ge 128 ]; then
        fail "con parámetros ($args) el programa termina por señal (posible segfault), rc=$rc"
    else
        echo "  [OK] parámetros ($args) rechazados prolijamente (rc=$rc)"
        pass
    fi
done
echo ""
fi

# ---------------------------------------------------------------------
if run_seccion invariantes; then
echo "=== Corridas válidas: invariantes de conteo, sin cuelgues ==="
COMBOS=(
    "3 2 1"
    "4 3 0"
    "5 4 2"
    "6 5 3"
    "8 6 4"
    "9 7 8"
    # Casos de borde: el juego termina "antes de empezar" (N=1, donde ya
    # hay un solo sobreviviente) o a la primera muerte (N=2). Son los que
    # destapan que el padre reparta el mando sin chequear antes la
    # condición de fin.
    "1 1 0"
    "1 5 0"
    "2 1 1"
    "2 5 0"
)
REPS_POR_COMBO=8

for combo in "${COMBOS[@]}"; do
    read -r N K J <<< "$combo"
    for rep in $(seq 1 $REPS_POR_COMBO); do
        check
        OUT="$TMPDIR/out_${N}_${K}_${J}_${rep}.txt"
        correr 5 "$OUT" "$BIN" "$N" "$K" "$J"
        rc=$?

        if [ $rc -eq 124 ]; then
            fail "(N=$N K=$K J=$J rep=$rep) se colgó (timeout)"
            continue
        fi
        if [ $rc -ge 128 ]; then
            fail "(N=$N K=$K J=$J rep=$rep) terminó por señal (posible segfault), rc=$rc"
            continue
        fi

        n_hijos=$(grep -c '^HIJO ' "$OUT")
        n_sobrev=$(grep -c '^SOBREVIVIENTE ' "$OUT")
        n_padre=$(grep -c '^PADRE mando_total=' "$OUT")
        total=$((n_hijos + n_sobrev))

        # Cada hijo tiene que aparecer UNA sola vez: o muerto (HIJO ...) o
        # vivo (SOBREVIVIENTE ...), nunca las dos cosas ni dos veces. No
        # alcanza con contar: un id duplicado y otro faltante se
        # compensarían en el total. El caso clásico que esto detecta es
        # que el padre liste a un hijo como sobreviviente mientras ese
        # mismo hijo, que ya tenía el mando, está imprimiendo sus últimas
        # palabras.
        ids=$( { grep -oE '^HIJO [0-9]+' "$OUT"; grep -oE '^SOBREVIVIENTE [0-9]+' "$OUT"; } \
                 | awk '{print $2}' | sort -n )
        ids_esperados=$(seq 0 $((N - 1)))

        pids=$( { grep -oE '^HIJO [0-9]+ PID [0-9]+' "$OUT"; grep -oE '^SOBREVIVIENTE [0-9]+ PID [0-9]+' "$OUT"; } \
                  | awk '{print $4}' )
        pids_distintos=$(echo "$pids" | sort -u | grep -c .)

        # El orden importa: primero los chequeos de formato, así un
        # formato mal da el mensaje claro y no un "los ids no son ..."
        # confuso derivado de no haber podido parsear nada.
        problema=""
        if [ "$n_hijos" -gt 0 ] && \
             [ "$(grep -cE '^HIJO [0-9]+ PID [0-9]+ ULTIMAS_PALABRAS mando_total=[0-9]+$' "$OUT")" -ne "$n_hijos" ]; then
            problema="alguna línea HIJO no respeta el formato esperado"
        elif [ "$n_sobrev" -gt 0 ] && \
             [ "$(grep -cE '^SOBREVIVIENTE [0-9]+ PID [0-9]+$' "$OUT")" -ne "$n_sobrev" ]; then
            problema="alguna línea SOBREVIVIENTE no respeta el formato esperado"
        elif [ "$n_padre" -ne 1 ]; then
            problema="no imprimió exactamente una línea PADRE mando_total=... (encontró $n_padre)"
        elif [ "$total" -ne "$N" ]; then
            problema="total de hijos+sobrevivientes ($total) != N ($N)"
        elif [ "$ids" != "$ids_esperados" ]; then
            problema="los ids reportados no son exactamente 0..$((N - 1)) sin repetir (encontré: $(echo $ids | tr '\n' ' '))"
        elif [ "$pids_distintos" -ne "$N" ]; then
            problema="los $N hijos deberían reportar $N PIDs distintos, se reportaron $pids_distintos"
        fi

        if [ -n "$problema" ]; then
            fail "(N=$N K=$K J=$J rep=$rep): $problema"
            echo "         --- output ---" >&2
            sed 's/^/         /' "$OUT" >&2
        else
            pass
        fi
    done
    echo "  [combo N=$N K=$K J=$J] $REPS_POR_COMBO corridas listas"
done
echo ""
fi

# ---------------------------------------------------------------------
if run_seccion huerfanos; then
    seccion_huerfanos
fi

resumen
