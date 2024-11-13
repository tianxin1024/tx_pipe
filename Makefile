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
	$(CMAKE_CMD) && make -j8

build:
	@$(CMAKE_MAKE) && make -s -j$(NUM_JOBS)

run :
	@cd build/bin && ./$(PRO) 

clean:
	read -r -p "This will delete the contents of build/*. Are you sure? [CRAL-C to abort]" response && rm -rf build/*

.PHONY: all run test clean check-python
