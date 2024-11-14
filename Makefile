CXX      = g++
PRO      = demo

SPACE   ?= $(PWD)
LIBDIR  ?= $(SPACE)/lib
LIBSPEC ?= $(SPACE)/include

CMAKE_CMD = mkdir -p build && cd build && cmake ..
CMAKE_MAKE = cd build

FLAGS = 
DEBUG_FLAGS = $(FLAGS) -DCMAKE_BUILD_TYPE:STRING=Debug
RELEASE_FLAGS = $(FLAGS) -DCMAKE_BUILD_TYPE:STRING=Release

all : 
	@$(CMAKE_CMD) && make -s -j8

run :
	@cd build/bin && ./$(PRO) 

debug :
	cd debug && gdb -x ./demo.gdb

clean:
	read -r -p "This will delete the contents of build/*. Are you sure? [CRAL-C to abort]" response && rm -rf build/*

.PHONY: all run debug test clean check-python
