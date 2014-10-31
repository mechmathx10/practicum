# makefile for minimal residual problem

CC=gcc
CFLAGS=-g -O3 -std=c99
CPPFLAGS=-Wall -Wextra -Werror -pedantic -fgnu89-inline

OBJDIR=obj
SRCDIR=src
EXEDIR=exe

OBJECTS=$(addprefix $(OBJDIR)/, input.o output.o matrix_utils.o solver.o generator.o)
EXEOBJECT=$(addprefix $(OBJDIR)/, main.o)

NOIST_HEADERS=$(addprefix $(SRCDIR)/, datatypes.h)

EXECUTABLE=$(addprefix $(EXEDIR)/, mrm_solver)

all : exe

$(OBJDIR)/matrix_utils.o : $(SRCDIR)/matrix_utils.h $(SRCDIR)/matrix_utils.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/matrix_utils.c -o $(OBJDIR)/matrix_utils.o $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/input.o : $(SRCDIR)/input.h $(SRCDIR)/input.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/input.c -o $(OBJDIR)/input.o $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/output.o : $(SRCDIR)/output.h $(SRCDIR)/output.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/output.c -o $(OBJDIR)/output.o $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/solver.o : $(SRCDIR)/solver.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/solver.c -o $(OBJDIR)/solver.o $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/generator.o : $(SRCDIR)/generator.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/generator.c -o $(OBJDIR)/generator.o $(CFLAGS) $(CPPFLAGS)

$(EXEOBJECT) : $(SRCDIR)/main.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/main.c -o $(OBJDIR)/main.o $(CFLAGS) $(CPPFLAGS)

clean:
	@ rm obj/* -rfv
	@ rm src/*~ -fv
	@ rm $(EXECUTABLE) -fv
	@ find $(EXEDIR)/ -type f -not -name "test_*" -not -name "out_*" -exec rm -fv {} \;
	@ find . -maxdepth 0 -type f -not -name "(README|makefile|run)" -exec rm -fv {} \;

exe: $(OBJECTS) $(EXEOBJECT)
	$(CC) $(OBJECTS) $(EXEOBJECT) -o $(EXECUTABLE) $(CFLAGS) $(CPPFLAGS)
	@ echo -e "\e[0;32mExecutable compiled: ${EXECUTABLE}\033[0m" ;\
