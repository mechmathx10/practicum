# makefile for linear system problem

CC=gcc
CFLAGS=-g -O3 -std=gnu99
CPPFLAGS=-Wall -Wextra -Werror -pedantic

OBJDIR=obj
SRCDIR=src
EXEDIR=exe

OBJECTS=$(addprefix $(OBJDIR)/, input.o output.o main.o)
NOIST_HEADERS=$(addprefix $(SRCDIR)/, datatypes.h)
EXECUTABLE=$(addprefix $(EXEDIR)/, main)

all : main

$(OBJDIR)/input.o : $(SRCDIR)/input.h $(SRCDIR)/input.c
	$(CC) -c $(SRCDIR)/input.c -o $(OBJDIR)/input.o $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/output.o : $(SRCDIR)/output.h $(SRCDIR)/output.c
	$(CC) -c $(SRCDIR)/output.c -o $(OBJDIR)/output.o $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/main.o : $(SRCDIR)/main.c
	$(CC) -c $(SRCDIR)/main.c -o $(OBJDIR)/main.o $(CFLAGS) $(CPPFLAGS)

#$(OBJECTS) : $(NOIST_HEADERS) | $(OBJDIR)

clean:
	rm obj/* -rf
	rm src/*~ -f
	rm $(EXECUTABLE) -f

main : $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(CFLAGS) $(CPPFLAGS)
