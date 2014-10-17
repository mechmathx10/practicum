# makefile for linear system problem

CC=gcc
CFLAGS=-g -O3 -std=gnu99
CPPFLAGS=-Wall -Wextra -Werror -pedantic -fgnu89-inline

OBJDIR=obj
SRCDIR=src
EXEDIR=exe
TEXDIR=latex

OBJECTS=$(addprefix $(OBJDIR)/, input.o output.o main.o block_utils.o matrix_utils.o solver.o permutation.o)
NOIST_HEADERS=$(addprefix $(SRCDIR)/, datatypes.h)
EXECUTABLE=$(addprefix $(EXEDIR)/, main)

all : tex exe

$(OBJDIR)/block_utils.o : $(SRCDIR)/block_utils.h $(SRCDIR)/block_utils.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/block_utils.c -o $(OBJDIR)/block_utils.o $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/matrix_utils.o : $(SRCDIR)/matrix_utils.h $(SRCDIR)/matrix_utils.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/matrix_utils.c -o $(OBJDIR)/matrix_utils.o $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/input.o : $(SRCDIR)/input.h $(SRCDIR)/input.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/input.c -o $(OBJDIR)/input.o $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/output.o : $(SRCDIR)/output.h $(SRCDIR)/output.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/output.c -o $(OBJDIR)/output.o $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/solver.o : $(SRCDIR)/solver.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/solver.c -o $(OBJDIR)/solver.o $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/permutation.o : $(SRCDIR)/permutation.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/permutation.c -o $(OBJDIR)/permutation.o $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/main.o : $(SRCDIR)/main.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/main.c -o $(OBJDIR)/main.o $(CFLAGS) $(CPPFLAGS)

clean:
	@ rm obj/* -rfv
	@ rm src/*~ -fv
	@ rm $(EXECUTABLE) -fv
	@ find $(EXEDIR)/ -type f -not -name "test_*" -not -name "data_*" -exec rm -fv {} \;
	@ find $(TEXDIR)/ -type f -not -name "*.tex" -not -name "*.pdf" -exec rm -fv {} \;
	@ find . -maxdepth 0 -type f -not -name "(README|makefile|run)" -exec rm -fv {} \;

tex:
	pdflatex -output-directory $(TEXDIR) $(TEXDIR)/report.tex

exe : $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(CFLAGS) $(CPPFLAGS)
