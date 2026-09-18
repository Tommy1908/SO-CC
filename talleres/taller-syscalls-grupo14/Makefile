# SSOO - Taller de syscalls y señales
#
# Todo se compila y se ejecuta adentro de una VM Linux, no en tu máquina.
# Así el entorno es idéntico para todos (Linux, macOS y Windows con WSL2)
# y herramientas como strace funcionan siempre.
#
# La carpeta del taller se comparte con la VM, así que podés editar los
# archivos con tu editor de siempre y compilar/correr con make.
#
# Arranca con:   make install
# Y después:     make help

.DEFAULT_GOAL := help

# ---------------------------------------------------------------------
# Herramientas compartidas por todos los talleres de la materia (la VM,
# principalmente). Viven en su propio repo y las baja make solo, la primera
# vez.  https://github.com/SSOO-Exactas-2026-2C/taller-tools
TOOLS_REPO ?= https://github.com/SSOO-Exactas-2026-2C/taller-tools.git
TOOLS_REF  ?= main

# Se clona en una carpeta temporal y recién ahí se mueve a tools/, para que un
# Ctrl-C a mitad de la descarga no deje un tools/ incompleto (que después haría
# fallar todos los clones siguientes con "destination path already exists").
#
# Ojo: el resultado se chequea con el exit status del $(shell) y NO con un
# segundo $(wildcard). Make cachea el listado de cada directorio y no lo
# invalida después de un $(shell), así que el wildcard seguiría dando vacío y
# abortaría un clone que en realidad anduvo bien.
ifeq ($(wildcard tools/vm.mk),)
$(info ==> Bajando las herramientas compartidas del taller...)
TOOLS_OK := $(shell rm -rf tools tools.tmp && \
	git clone --quiet --depth 1 --branch $(TOOLS_REF) $(TOOLS_REPO) tools.tmp 1>&2 && \
	mv tools.tmp tools && echo ok)
ifneq ($(TOOLS_OK),ok)
$(error No pude bajar las herramientas del taller desde $(TOOLS_REPO) -- necesitás git y conexión a internet)
endif
endif

include tools/vm.mk

# ---------------------------------------------------------------------
# Carpeta con los fuentes a compilar. Por defecto la del alumno; la cátedra
# puede apuntar a la solución sin copiar archivos a src/:
#   make test SRC_DIR=src-solucion
SOL_DIR := src-solucion
SRC_DIR ?= src

# Ejercicio 1: replicar el binario 'relay'
SRC_EJ1  := $(SRC_DIR)/ej1/relay_espejo.c
BIN_EJ1  := bin/relay_espejo
REF_EJ1  := bin/relay              # binario de referencia de la cátedra
TEST_EJ1 := tests/tests_ej1.sh

# Ejercicio 2: la cadena de mando
SRC_EJ2  := $(SRC_DIR)/ej2/cadenamando.c
BIN_EJ2  := bin/cadenamando
TEST_EJ2 := tests/tests_ej2.sh

# Argumentos de los programas. Se pueden pisar desde la línea de comandos:
#   make run-ej1 N=5
#   make run-ej2 N=4 K=5 J=1
N ?=
K ?=
J ?=
ARGS_EJ1 := $(or $(N),3)
ARGS_EJ2 := $(or $(N),4) $(or $(K),5) $(or $(J),1)

# Traza de syscalls: la completa queda en TRACE_LOG (lo define tools/vm.mk),
# por pantalla se muestra solo lo filtrado por FILTRO.
#
# Ojo: con "strace -f" cada línea arranca con el PID, incluidas las de
# entrega de señal ("441   --- SIGUSR1 {...} ---"), así que el filtro no
# puede anclarse al principio de la línea.
FILTRO := rt_sigaction|rt_sigprocmask|rt_sigsuspend|pause|kill|clone|fork|wait4|waitid|exit_group|--- SIG
STRACE := strace -f -e trace=%signal,%process

.PHONY: help \
        build build-ej1 build-ej2 \
        run-ej1 run-ej2 run-relay \
        strace-ej1 strace-ej2 strace-relay \
        test test-ej1 test-ej1-salida test-ej1-parametros test-ej1-syscalls \
        test-ej1-huerfanos test-ej2 test-ej2-simbolos test-ej2-parametros \
        test-ej2-invariantes test-ej2-huerfanos \
        clean

help:
	@echo "Taller de syscalls y señales"
	@echo ""
	$(VM_HELP)
	@echo ""
	@echo "  Ejercicio 1 - relay"
	@echo "    make run-relay N=3        corre el binario de la cátedra"
	@echo "    make strace-relay N=3     traza sus syscalls (Parte A)"
	@echo "    make build-ej1            compila tu $(SRC_EJ1)"
	@echo "    make run-ej1 N=3          corre tu implementación"
	@echo "    make strace-ej1 N=3       traza tu implementación (para debuggear)"
	@echo "    make test-ej1             corre los tests del ejercicio 1"
	@echo ""
	@echo "  Ejercicio 2 - la cadena de mando"
	@echo "    make build-ej2            compila tu $(SRC_EJ2)"
	@echo "    make run-ej2 N=4 K=5 J=1  corre tu implementación"
	@echo "    make strace-ej2 N=4 K=5 J=1"
	@echo "    make test-ej2             corre los tests del ejercicio 2"
	@echo ""
	@echo "  Todo junto"
	@echo "    make build / make test"
	@echo ""
	$(VM_HELP_LIMPIEZA)

# ---------------------------------------------------------------------
# Ejercicio 1: relay
# ---------------------------------------------------------------------

# Los binarios se compilan adentro de la VM. Como la carpeta es compartida,
# los .c se leen y los binarios aparecen del lado del host también.
build-ej1:
	@test -f $(SRC_EJ1) || { \
		echo "Falta $(SRC_EJ1): es tu entrega del ejercicio 1."; exit 1; }
	@$(VM) exec "gcc $(CFLAGS) -o $(BIN_EJ1) $(SRC_EJ1)"
	@echo "compilado: $(BIN_EJ1)"

run-ej1: build-ej1
	@$(VM) run "./$(BIN_EJ1) $(ARGS_EJ1)" $(INTERACTIVO)

run-relay:
	@$(VM) run "chmod +x $(REF_EJ1); ./$(REF_EJ1) $(ARGS_EJ1)" $(INTERACTIVO)

# Parte A del ejercicio 1: mirar que syscalls usa el binario de la cátedra.
strace-relay:
	@$(VM) run "chmod +x $(REF_EJ1); rm -f $(TRACE_LOG); \
		$(STRACE) -o $(TRACE_LOG) ./$(REF_EJ1) $(ARGS_EJ1)" $(INTERACTIVO)
	$(VM_SHOW_TRACE)

# Lo mismo pero sobre tu implementación, para comparar y debuggear.
strace-ej1: build-ej1
	@$(VM) run "rm -f $(TRACE_LOG); $(STRACE) -o $(TRACE_LOG) ./$(BIN_EJ1) $(ARGS_EJ1)" $(INTERACTIVO)
	$(VM_SHOW_TRACE)

test-ej1: build-ej1
	@$(VM) run "REF=$(REF_EJ1) bash $(TEST_EJ1) ./$(BIN_EJ1)"

test-ej1-salida: build-ej1
	@$(VM) run "REF=$(REF_EJ1) bash $(TEST_EJ1) ./$(BIN_EJ1) --check=salida"

test-ej1-parametros: build-ej1
	@$(VM) run "REF=$(REF_EJ1) bash $(TEST_EJ1) ./$(BIN_EJ1) --check=parametros"

test-ej1-syscalls: build-ej1
	@$(VM) run "REF=$(REF_EJ1) bash $(TEST_EJ1) ./$(BIN_EJ1) --check=syscalls"

test-ej1-huerfanos: build-ej1
	@$(VM) run "REF=$(REF_EJ1) bash $(TEST_EJ1) ./$(BIN_EJ1) --check=huerfanos"

# ---------------------------------------------------------------------
# Ejercicio 2: la cadena de mando
# ---------------------------------------------------------------------

build-ej2:
	@test -f $(SRC_EJ2) || { \
		echo "Falta $(SRC_EJ2): es tu entrega del ejercicio 2."; exit 1; }
	@$(VM) exec "gcc $(CFLAGS) -o $(BIN_EJ2) $(SRC_EJ2)"
	@echo "compilado: $(BIN_EJ2)"

run-ej2: build-ej2
	@$(VM) run "./$(BIN_EJ2) $(ARGS_EJ2)" $(INTERACTIVO)

strace-ej2: build-ej2
	@$(VM) run "rm -f $(TRACE_LOG); $(STRACE) -o $(TRACE_LOG) ./$(BIN_EJ2) $(ARGS_EJ2)" $(INTERACTIVO)
	$(VM_SHOW_TRACE)

test-ej2: build-ej2
	@$(VM) run "bash $(TEST_EJ2) ./$(BIN_EJ2)"

test-ej2-simbolos: build-ej2
	@$(VM) run "bash $(TEST_EJ2) ./$(BIN_EJ2) --check=simbolos"

test-ej2-parametros: build-ej2
	@$(VM) run "bash $(TEST_EJ2) ./$(BIN_EJ2) --check=parametros"

test-ej2-invariantes: build-ej2
	@$(VM) run "bash $(TEST_EJ2) ./$(BIN_EJ2) --check=invariantes"

test-ej2-huerfanos: build-ej2
	@$(VM) run "bash $(TEST_EJ2) ./$(BIN_EJ2) --check=huerfanos"

# ---------------------------------------------------------------------

build: build-ej1 build-ej2

test: test-ej1 test-ej2

clean:
	rm -f $(BIN_EJ1) $(BIN_EJ2) $(TRACE_LOG)

# Uso interno de la cátedra: regenera el binario de referencia a partir
# de la solución. Solo funciona en la rama con las soluciones.
.PHONY: ref-relay
ref-relay:
	@test -f $(SOL_DIR)/ej1/relay_espejo.c \
		|| { echo "Este target es de la cátedra: falta $(SOL_DIR)/ej1/relay_espejo.c"; exit 1; }
	@$(VM) exec "gcc -static -O2 -Wall -s -o $(REF_EJ1) $(SOL_DIR)/ej1/relay_espejo.c"
	@echo "regenerado: $(REF_EJ1)"
