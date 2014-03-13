# Makefile
CC=g++
CFLAGS=-I. -std=c++11 -Werror -Wall 

all: main.cpp 
	$(CC) main.cpp $(CFLAGS)
clean:
	rm simpleCross
