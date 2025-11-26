MPICC = mpicc
CFLAGS = -g -O3

BIN_DIR = ../bin
LIB_DIR = ../lib
SRC_LIB = $(LIB_DIR)/libplugin.so

TESTS = $(wildcard test*.c)
EXECUTABLES = $(patsubst %.c,$(BIN_DIR)/%,$(TESTS))

YELLOW=\033[1;33m
RESET=\033[0m

.PHONY: all clean cleanall

all: $(BIN_DIR) $(EXECUTABLES)

$(BIN_DIR)/%: %.c $(SRC_LIB)
	@echo "$(YELLOW)Compilation du fichier $<$(RESET)"
	$(MPICC) $< $(CFLAGS) -o $@ -fplugin=$(SRC_LIB)
	@echo "\n"

clean:
	rm -f *.dot
	rm -f $(EXECUTABLES)

cleanall: clean
