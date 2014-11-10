# makefile for minimal residual problem

CC=gcc
CFLAGS=-g -O3 -std=gnu99
CPPFLAGS=-Wall -Wextra -Werror -pedantic -fgnu89-inline

OBJDIR=obj
SRCDIR=src
EXEDIR=exe

THREAD_DIR=thread
FORK_DIR=fork

OBJECTS=$(addprefix $(OBJDIR)/, )
THREAD_OBJECT=$(addprefix $(OBJDIR)/, main_thread.o)
FORK_OBJECT=$(addprefix $(OBJDIR)/, main_fork.o)

NOIST_HEADERS=$(addprefix $(SRCDIR)/, datatypes.h)

THREAD_EXE=$(addprefix $(EXEDIR)/, thread_exe)
FORK_EXE=$(addprefix $(EXEDIR)/, fork_exe)

all : thread fork

############### Thread part ###############

$(THREAD_OBJECT) : $(SRCDIR)/$(THREAD_DIR)/main_thread.c $(NOIST_HEADERS)
	$(CC) -c -pthread $(SRCDIR)/$(THREAD_DIR)/main_thread.c -o $(THREAD_OBJECT) $(CFLAGS) $(CPPFLAGS)

###############  Fork part  ###############

$(FORK_OBJECT) : $(SRCDIR)/$(FORK_DIR)/main_fork.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/$(FORK_DIR)/main_fork.c -o $(FORK_OBJECT) $(CFLAGS) $(CPPFLAGS)

###########################################

clean:
	@ rm $(OBJDIR)/* -rfv
	@ rm $(SRCDIR)/*~ -fv
	@ rm $(SRCDIR)/$(THREAD_DIR)/*~ -fv
	@ rm $(SRCDIR)/$(FORK_DIR)/*~ -fv
	@ rm $(THREAD_EXE) -fv
	@ find $(EXEDIR)/ -type f -not -name "test_*" -not -name "out_*" -exec rm -fv {} \;
	@ find . -maxdepth 0 -type f -not -name "(README|makefile|run)" -exec rm -fv {} \;

###########################################

thread: $(THREAD_OBJECT) $(OBJECTS)
	$(CC) $(THREAD_OBJECT) $(OBJECTS) -o $(THREAD_EXE) $(CFLAGS) $(CPPFLAGS) -pthread
	@ echo -e "\e[0;32mThread executable compiled: ${THREAD_EXE}\033[0m" ;\

fork: $(FORK_OBJECT) $(OBJECTS)
	$(CC) $(FORK_OBJECT) $(OBJECTS) -o $(FORK_EXE) $(CFLAGS) $(CPPFLAGS)
	@ echo -e "\e[0;32mFork executable compiled: ${FORK_EXE}\033[0m" ;\
