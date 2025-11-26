DINC_CPP = ../src
DSRC_CPP = ../src

SRC = $(wildcard $(DSRC_CPP)/*.cpp) # all source files
INC = $(wildcard $(DINC)/*.h) # all include files

YELLOW=\033[1;33m
RESET=\033[0m

.PHONY: all doc

all: doc

doc:
	doxygen
	@cd latex && make
	@echo "$(YELLOW)Open the documentation with the command 'open doc/html/index.html' or 'open doc/latex/refman.pdf'.$(RESET)"

clean:
	rm -rf html
	rm -rf latex
