# makefile for minimal residual problem

CC=gcc
CFLAGS=-g -O3 -std=c99
CPPFLAGS=-Wall -Wextra -Werror -pedantic -fgnu89-inline

OBJDIR=obj
SRCDIR=src
EXEDIR=exe

THREAD_DIR=thread

OBJECTS=$(addprefix $(OBJDIR)/, )
THREAD_OBJECT=$(addprefix $(OBJDIR)/, main_thread.o)

NOIST_HEADERS=$(addprefix $(SRCDIR)/, datatypes.h)

THREAD_EXE=$(addprefix $(EXEDIR)/, thread_exe)

all : thread

############### Thread part ###############

$(THREAD_OBJECT) : $(SRCDIR)/$(THREAD_DIR)/main_thread.c $(NOIST_HEADERS)
	$(CC) -c -pthread $(SRCDIR)/$(THREAD_DIR)/main_thread.c -o $(THREAD_OBJECT) $(CFLAGS) $(CPPFLAGS)

###########################################

clean:
	@ rm $(OBJDIR)/* -rfv
	@ rm $(SRCDIR)/*~ -fv
	@ rm $(SRCDIR)/$(THREAD_DIR)/*~ -fv
	@ rm $(THREAD_EXE) -fv
	@ find $(EXEDIR)/ -type f -not -name "test_*" -not -name "out_*" -exec rm -fv {} \;
	@ find . -maxdepth 0 -type f -not -name "(README|makefile|run)" -exec rm -fv {} \;

thread: $(THREAD_OBJECT) $(OBJECTS)
	$(CC) $(THREAD_OBJECT) $(OBJECTS) -o $(THREAD_EXE) $(CFLAGS) $(CPPFLAGS) -pthread
	@ echo -e "\e[0;32mThread executable compiled: ${EXECUTABLE}\033[0m" ;\
