# makefile for small parallel programs

CC=gcc
CFLAGS=-g -O3 -std=gnu99
CPPFLAGS=-Wall -Wextra -Werror -pedantic -fgnu89-inline

OBJDIR=obj
SRCDIR=src
EXEDIR=exe

THREAD_DIR=thread
FORK_DIR=fork
DAEMON_DIR=daemon
SHARED_DIR=shared_data

OBJECTS=$(addprefix $(OBJDIR)/, )
NOIST_HEADERS=$(addprefix $(SRCDIR)/, datatypes.h)

THREAD_OBJECT=$(addprefix $(OBJDIR)/, main_thread.o)
FORK_OBJECT=$(addprefix $(OBJDIR)/, main_fork.o)
DAEMON_CALLER_OBJECT=$(addprefix $(OBJDIR)/, main_daemon.o)
DAEMON_OBJECT=$(addprefix $(OBJDIR)/, daemon.o)
SHARED_OBJECT=$(addprefix $(OBJDIR)/, shared_data.o)
SHARED_READER_OBJECT=$(addprefix $(OBJDIR)/, reader.o)
SHARED_WRITER_OBJECT=$(addprefix $(OBJDIR)/, writer.o)

THREAD_EXE=$(addprefix $(EXEDIR)/, thread_exe)
FORK_EXE=$(addprefix $(EXEDIR)/, fork_exe)
DAEMON_CALLER_EXE=$(addprefix $(EXEDIR)/, daemon_exe)
DAEMON_EXE=$(addprefix $(EXEDIR)/, daemon)
SHARED_READER_EXE=$(addprefix $(EXEDIR)/, reader)
SHARED_WRITER_EXE=$(addprefix $(EXEDIR)/, writer)

all : thread fork daemon shared_data

############### Thread part ###############

$(THREAD_OBJECT) : $(SRCDIR)/$(THREAD_DIR)/main_thread.c $(NOIST_HEADERS)
	$(CC) -c -pthread $(SRCDIR)/$(THREAD_DIR)/main_thread.c -o $(THREAD_OBJECT) $(CFLAGS) $(CPPFLAGS)

###############  Fork part  ###############

$(FORK_OBJECT) : $(SRCDIR)/$(FORK_DIR)/main_fork.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/$(FORK_DIR)/main_fork.c -o $(FORK_OBJECT) $(CFLAGS) $(CPPFLAGS)

###########  Survivable daemon ############

$(DAEMON_CALLER_OBJECT) : $(SRCDIR)/$(DAEMON_DIR)/main_daemon.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/$(DAEMON_DIR)/main_daemon.c -o $(DAEMON_CALLER_OBJECT) $(CFLAGS) $(CPPFLAGS)

$(DAEMON_OBJECT) : $(SRCDIR)/$(DAEMON_DIR)/daemon.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/$(DAEMON_DIR)/daemon.c -o $(DAEMON_OBJECT) $(CFLAGS) $(CPPFLAGS)

############### Shared data ###############

$(SHARED_OBJECT) : $(SRCDIR)/$(SHARED_DIR)/shared_data.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/$(SHARED_DIR)/shared_data.c -o $(SHARED_OBJECT) $(CFLAGS) $(CPPFLAGS)

$(SHARED_WRITER_OBJECT) : $(SRCDIR)/$(SHARED_DIR)/writer.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/$(SHARED_DIR)/writer.c -o $(SHARED_WRITER_OBJECT) $(CFLAGS) $(CPPFLAGS)

$(SHARED_READER_OBJECT) : $(SRCDIR)/$(SHARED_DIR)/reader.c $(NOIST_HEADERS)
	$(CC) -c $(SRCDIR)/$(SHARED_DIR)/reader.c -o $(SHARED_READER_OBJECT) $(CFLAGS) $(CPPFLAGS)

###########################################

clean:
	@ rm $(OBJDIR)/* -rfv
	@ rm $(SRCDIR)/*~ -fv
	@ rm $(SRCDIR)/$(THREAD_DIR)/*~ -fv
	@ rm $(SRCDIR)/$(FORK_DIR)/*~ -fv
	@ rm $(SRCDIR)/$(DAEMON_DIR)/*~ -fv
	@ rm $(SRCDIR)/$(SHARED_DIR)/*~ -fv
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

daemon: _daemon _daemon_caller

_daemon: $(DAEMON_OBJECT) $(OBJECTS)
	$(CC) $(DAEMON_OBJECT) $(OBJECTS) -o $(DAEMON_EXE) $(CFLAGS) $(CPPFLAGS)
	@ echo -e "\e[0;32mDaemon executable compiled: ${DAEMON_EXE}\033[0m" ;\

_daemon_caller: $(DAEMON_CALLER_OBJECT) $(OBJECTS)
	$(CC) $(DAEMON_CALLER_OBJECT) $(OBJECTS) -o $(DAEMON_CALLER_EXE) $(CFLAGS) $(CPPFLAGS)
	@ echo -e "\e[0;32mDaemon caller executable compiled: ${DAEMON_CALLER_EXE}\033[0m" ;\

shared_data: _shared_reader _shared_writer

_shared_reader: $(SHARED_READER_OBJECT) $(SHARED_OBJECT) $(OBJECTS)
	$(CC) $(SHARED_READER_OBJECT) $(OBJECTS) $(SHARED_OBJECT) -o $(SHARED_READER_EXE) $(CFLAGS) $(CPPFLAGS)
	@ echo -e "\e[0;32mShared reader executable compiled: ${SHARED_READER_EXE}\033[0m" ;\

_shared_writer: $(SHARED_WRITER_OBJECT) $(SHARED_OBJECT) $(OBJECTS)
	$(CC) $(SHARED_WRITER_OBJECT) $(OBJECTS) $(SHARED_OBJECT) -o $(SHARED_WRITER_EXE) $(CFLAGS) $(CPPFLAGS)
	@ echo -e "\e[0;32mShared writer executable compiled: ${SHARED_WRITER_EXE}\033[0m" ;\
