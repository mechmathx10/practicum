# makefile for linear system problem

CC=gcc
CFLAGS=-g -O3 -std=gnu99
CPPFLAGS=-Wall -Wextra -Werror -pedantic -fgnu89-inline

OBJDIR=obj
SRCDIR=src
EXEDIR=exe
TEXDIR=latex
TESTDIR=test
TESTSRCDIR=src/test

OBJECTS=$(addprefix $(OBJDIR)/, input.o output.o block_utils.o matrix_utils.o solver.o permutation.o)
TESTOBJECTS=$(addprefix $(OBJDIR)/, test_solver.o)
EXEOBJECT=$(addprefix $(OBJDIR)/, main.o)
TESTEXEOBJECT=$(addprefix $(OBJDIR)/, test_main.o)

NOIST_HEADERS=$(addprefix $(SRCDIR)/, datatypes.h)

EXECUTABLE=$(addprefix $(EXEDIR)/, main)
TESTEXE=$(addprefix $(TESTDIR)/, main)

all : tex exe test

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

$(OBJDIR)/test_solver.o : $(TESTSRCDIR)/test_solver.c $(NOIST_HEADERS)
	$(CC) -c $(TESTSRCDIR)/test_solver.c -o $(OBJDIR)/test_solver.o $(CFLAGS) $(CPPFLAGS)

$(TESTEXEOBJECT) : $(TESTSRCDIR)/test_main.c $(NOIST_HEADERS)
	$(CC) -c $(TESTSRCDIR)/test_main.c -o $(OBJDIR)/test_main.o $(CFLAGS) $(CPPFLAGS)

$(EXEOBJECT) : $(SRCDIR)/main.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/main.c -o $(OBJDIR)/main.o $(CFLAGS) $(CPPFLAGS)

clean:
	@ rm obj/* -rfv
	@ rm src/*~ -fv
	@ rm $(EXECUTABLE) -fv
	@ find $(EXEDIR)/ -type f -not -name "test_*" -not -name "data_*" -exec rm -fv {} \;
	@ rm $(TESTEXE) -fv
	@ find $(TESTDIR)/ -type f -not -name "test_*" -not -name "data_*" -exec rm -fv {} \;
	@ find $(TEXDIR)/ -type f -not -name "*.tex" -not -name "*.pdf" -exec rm -fv {} \;
	@ find . -maxdepth 0 -type f -not -name "(README|makefile|run)" -exec rm -fv {} \;

tex:
	pdflatex -output-directory $(TEXDIR) $(TEXDIR)/report.tex

exe: $(OBJECTS) $(EXEOBJECT)
	$(CC) $(OBJECTS) $(EXEOBJECT) -o $(EXECUTABLE) $(CFLAGS) $(CPPFLAGS)

test: $(OBJECTS) $(TESTEXEOBJECT) $(TESTOBJECTS)
	$(CC) $(OBJECTS) $(TESTOBJECTS) $(TESTEXEOBJECT) -o $(TESTEXE) $(CFLAGS) $(CPPFLAGS)
