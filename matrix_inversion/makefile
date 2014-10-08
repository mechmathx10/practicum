# makefile for linear system problem

CC=gcc
CFLAGS=-g -O3 -std=gnu99
CPPFLAGS=-Wall -Wextra -Werror -pedantic -fgnu89-inline

OBJDIR=obj
SRCDIR=src
EXEDIR=exe

OBJECTS=$(addprefix $(OBJDIR)/, input.o output.o main.o block_utils.o)
NOIST_HEADERS=$(addprefix $(SRCDIR)/, datatypes.h)
EXECUTABLE=$(addprefix $(EXEDIR)/, main)

all : $(EXECUTABLE)

$(OBJDIR)/block_utils.o : $(SRCDIR)/block_utils.h $(SRCDIR)/block_utils.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/block_utils.c -o $(OBJDIR)/block_utils.o $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/input.o : $(SRCDIR)/input.h $(SRCDIR)/input.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/input.c -o $(OBJDIR)/input.o $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/output.o : $(SRCDIR)/output.h $(SRCDIR)/output.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/output.c -o $(OBJDIR)/output.o $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/main.o : $(SRCDIR)/main.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/main.c -o $(OBJDIR)/main.o $(CFLAGS) $(CPPFLAGS)

#$(OBJECTS) : $(NOIST_HEADERS)

clean:
	rm obj/* -rf
	rm src/*~ -f
	rm $(EXECUTABLE) -f

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(CFLAGS) $(CPPFLAGS)
