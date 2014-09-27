# makefile for linear system problem

CC=gcc
CFLAGS=-g -O3
CPPFLAGS=-Wall -Wextra -Werror -pedantic

vpath %.c src
vpath %.h src
vpath %.o obj

OBJDIR=obj
SRCDIR=src

OBJECTS=$(addprefix $(OBJDIR)/, input.o output.o main.o)
NOIST_HEADERS=$(addprefix $(SRCDIR)/, datatypes.h)

all : main clean

$(OBJECTS) : $(NOIST_HEADERS) | $(OBJDIR)

input.o : input.h input.o
	$(CC) input.c -c $(CFLAGS) $(CPPFLAGS)

output.o : output.h output.o
	$(CC) output.c -c $(CFLAGS) $(CPPFLAGS)

main.o : main.c
	$(CC) main.c -c $(CFLAGS) $(CPPFLAGS)

clean:
	rm obj/* -rf
	rm src/*~ -f
	rm main

main : $(OBJECTS)
	$(CC) $(OBJECTS) -o main $(CFLAGS) $(CPPFLAGS)