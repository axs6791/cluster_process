# Makefile
CC=g++
CFLAGS=-I. -std=c++11 -Werror -Wall -g -ggdb -O3

all: main.cpp cluster_generator.hpp configuration.hpp
	$(CC) -o cluster_process main.cpp $(CFLAGS)
clean:
	rm cluster_process
