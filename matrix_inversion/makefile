# makefile for matrix inversion problem

CC=gcc
CFLAGS=-g -O3 -std=c99
CPPFLAGS=-Wall -Wextra -Werror -pedantic -fgnu89-inline

OBJDIR=obj
SRCDIR=src
EXEDIR=exe
TEXDIR=latex
TESTDIR=test

TESTSRCDIR=src/test
IODIR=io
UTILSDIR=utils
SOLVERDIR=solver

OBJECTS=$(addprefix $(OBJDIR)/, input.o output.o block_utils.o matrix_utils.o solver.o permutation.o)
TESTOBJECTS=$(addprefix $(OBJDIR)/, test_solver.o)
EXEOBJECT=$(addprefix $(OBJDIR)/, main.o)
TESTEXEOBJECT=$(addprefix $(OBJDIR)/, test_main.o)

NOIST_HEADERS=$(addprefix $(SRCDIR)/, datatypes.h)

EXECUTABLE=$(addprefix $(EXEDIR)/, main)
TESTEXE=$(addprefix $(TESTDIR)/, main)

TEXCOMMAND=pdflatex -interaction=nonstopmode -output-directory $(TEXDIR) $(TEXDIR)/report.tex > /dev/null

all : test exe tex

############## IO #####################

$(OBJDIR)/input.o : $(SRCDIR)/$(IODIR)/input.h $(SRCDIR)/$(IODIR)/input.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/$(IODIR)/input.c -o $(OBJDIR)/input.o $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/output.o : $(SRCDIR)/$(IODIR)/output.h $(SRCDIR)/$(IODIR)/output.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/$(IODIR)/output.c -o $(OBJDIR)/output.o $(CFLAGS) $(CPPFLAGS)

############## Utils ##################

$(OBJDIR)/block_utils.o : $(SRCDIR)/$(UTILSDIR)/block_utils.h $(SRCDIR)/$(UTILSDIR)/block_utils.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/$(UTILSDIR)/block_utils.c -o $(OBJDIR)/block_utils.o $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/matrix_utils.o : $(SRCDIR)/$(UTILSDIR)/matrix_utils.h $(SRCDIR)/$(UTILSDIR)/matrix_utils.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/$(UTILSDIR)/matrix_utils.c -o $(OBJDIR)/matrix_utils.o $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/permutation.o : $(SRCDIR)/$(UTILSDIR)/permutation.h $(SRCDIR)/$(UTILSDIR)/permutation.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/$(UTILSDIR)/permutation.c -o $(OBJDIR)/permutation.o $(CFLAGS) $(CPPFLAGS)

############## Test  ##################

$(OBJDIR)/test_solver.o : $(TESTSRCDIR)/test_solver.h $(TESTSRCDIR)/test_solver.c $(NOIST_HEADERS)
	$(CC) -c $(TESTSRCDIR)/test_solver.c -o $(OBJDIR)/test_solver.o $(CFLAGS) $(CPPFLAGS)

$(TESTEXEOBJECT) : $(TESTSRCDIR)/test_main.c $(NOIST_HEADERS)
	$(CC) -c $(TESTSRCDIR)/test_main.c -o $(OBJDIR)/test_main.o $(CFLAGS) $(CPPFLAGS)

############## Solver #################

$(OBJDIR)/solver.o : $(SRCDIR)/$(SOLVERDIR)/solver.h $(SRCDIR)/$(SOLVERDIR)/solver.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/$(SOLVERDIR)/solver.c -o $(OBJDIR)/solver.o $(CFLAGS) $(CPPFLAGS)

############## Main  ##################

$(EXEOBJECT) : $(SRCDIR)/main.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/main.c -o $(OBJDIR)/main.o $(CFLAGS) $(CPPFLAGS)

##########################################

clean:
	@ rm obj/* -rfv
	@ rm src/*~ -fv
	@ rm src/$(IODIR)/*~ -fv
	@ rm src/$(UTILSDIR)/*~ -fv
	@ rm src/$(SOLVERDIR)/*~ -fv
	@ rm $(EXECUTABLE) -fv
	@ find $(EXEDIR)/ -type f -not -name "test_*" -not -name "data_*" -exec rm -fv {} \;
	@ rm $(TESTEXE) -fv
	@ find $(TESTDIR)/ -type f -not -name "test_*" -not -name "data_*" -exec rm -fv {} \;
	@ find $(TEXDIR)/ -type f -not -name "*.tex" -exec rm -fv {} \;
	@ find . -maxdepth 0 -type f -not -name "(README|makefile|run)" -exec rm -fv {} \;

tex: $(TEXDIR)/report.tex
	@ echo "$(TEXCOMMAND)" ; \
	$(TEXCOMMAND) ; \
	if [ $$? = 0 ] ; then \
		echo -e "\e[0;32mReport compiled: ${TEXDIR}/report.pdf\033[0m"; \
	else \
		echo -e "\e[0;31mReport compilation failed\033[0m"; \
	fi;

exe: $(OBJECTS) $(EXEOBJECT)
	$(CC) $(OBJECTS) $(EXEOBJECT) -o $(EXECUTABLE) $(CFLAGS) $(CPPFLAGS)
	@ echo -e "\e[0;32mExecutable compiled: ${EXECUTABLE}\033[0m" ;\

test: $(OBJECTS) $(TESTEXEOBJECT) $(TESTOBJECTS)
	$(CC) $(OBJECTS) $(TESTOBJECTS) $(TESTEXEOBJECT) -o $(TESTEXE) $(CFLAGS) $(CPPFLAGS)
	@ echo -e "\e[0;32mTest executable compiled: ${TESTEXE}\033[0m" ;\
