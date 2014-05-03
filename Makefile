# Makefile
PROGRAM=cluster_process
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
CC=g++
# LDFLAGS=-lboost_program_options
LDFLAGS=
CFLAGS=-I. -std=c++11 -Werror -Wall -g -ggdb -O3

all: main.cpp $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CC) -O3 -o $@ $(OBJECTS) $(LDFLAGS) $(CFLAGS)
.cpp.o:	
	$(CC) -O3 $(CFLAGS) -c $<
clean:
	rm -f $(PROGRAM) $(OBJECTS)
.PHONY: clean
