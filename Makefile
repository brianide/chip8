CC=clang
CFLAGS=-Wall
LDFLAGS=-lncurses

all: demo

demo: demo.c cpu.c
