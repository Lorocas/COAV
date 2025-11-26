# GCC_ROOT=${HOME}/tools/gcc-12.2.0/
# CXX=$(GCC_ROOT)/bin/g++
# CC=$(GCC_ROOT)/bin/gcc

CXX = g++
CC = gcc

MPICC = mpicc

PLUGIN_FLAGS = `$(CC) -print-file-name=plugin`/include -g -Wall -fno-rtti -shared -fPIC
CFLAGS = -g -O3

OBJ_DIR = ../obj
BIN_DIR = ../bin
LIB_DIR = ../lib

OBJS = $(OBJ_DIR)/plugin.o \
       $(OBJ_DIR)/mpi_collectives.o \
       $(OBJ_DIR)/graphviz.o \
       $(OBJ_DIR)/rank.o \
       $(OBJ_DIR)/post_dominance.o\
	   $(OBJ_DIR)/pragma.o

LIB = $(LIB_DIR)/libplugin.so

.PHONY: all clean cleanall

all: $(LIB)

$(OBJ_DIR)/%.o: %.cpp %.h
	$(CXX) -I$(PLUGIN_FLAGS) -c -o $@ $<

$(LIB): $(OBJS)
	$(CXX) -I$(PLUGIN_FLAGS) -o $@ $^

clean:
	rm -f $(OBJ_DIR)/*.o

cleanall: clean
	rm -f $(LIB)
