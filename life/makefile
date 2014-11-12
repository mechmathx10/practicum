# makefile for "life" cellular automaton

CC=gcc
CFLAGS=-g -O3 -std=gnu99
CPPFLAGS=-Wall -Wextra -Werror -pedantic -fgnu89-inline

SEQDIR=sequential
PARDIR=parallel

OBJDIR=obj
SRCDIR=src
EXEDIR=exe
HEADERDIR=headers

NOIST_HEADERS=$(addprefix $(HEADERDIR)/, datatypes.h)

#############################################

SEQ_OBJECTS=$(addprefix $(SEQDIR)/$(OBJDIR)/, life_sequential.o)
SEQ_EXEOBJECT=$(addprefix $(SEQDIR)/$(OBJDIR)/, main.o)

SEQ_EXEC=$(addprefix $(SEQDIR)/$(EXEDIR)/, life_sequential)

#############################################

PAR_OBJECTS=$(addprefix $(PARDIR)/$(OBJDIR)/, life_parallel.o)
PAR_EXEOBJECT=$(addprefix $(PARDIR)/$(OBJDIR)/, main.o)

PAR_EXEC=$(addprefix $(PARDIR)/$(EXEDIR)/, life_parallel)

#############################################

all: sequential parallel

#############################################

$(SEQDIR)/$(OBJDIR)/life_sequential.o : $(SEQDIR)/$(SRCDIR)/life_sequential.c $(NOIST_HEADERS)
	$(CC) -c $(SEQDIR)/$(SRCDIR)/life_sequential.c -o $(SEQDIR)/$(OBJDIR)/life_sequential.o $(CFLAGS) $(CPPFLAGS)

$(SEQ_EXEOBJECT) : $(SEQDIR)/$(SRCDIR)/main.c $(NOIST_HEADERS)
	$(CC) -c $(SEQDIR)/$(SRCDIR)/main.c -o $(SEQDIR)/$(OBJDIR)/main.o $(CFLAGS) $(CPPFLAGS)

#############################################

$(PARDIR)/$(OBJDIR)/life_parallel.o : $(PARDIR)/$(SRCDIR)/life_parallel.c $(NOIST_HEADERS)
	$(CC) -c $(PARDIR)/$(SRCDIR)/life_parallel.c -o $(PARDIR)/$(OBJDIR)/life_parallel.o $(CFLAGS) $(CPPFLAGS)

$(PAR_EXEOBJECT) : $(PARDIR)/$(SRCDIR)/main.c $(NOIST_HEADERS)
	$(CC) -c $(PARDIR)/$(SRCDIR)/main.c -o $(PARDIR)/$(OBJDIR)/main.o $(CFLAGS) $(CPPFLAGS)

#############################################



clean:
	@ rm $(SEQDIR)/$(OBJDIR)/* -rfv
	@ rm $(SEQDIR)/$(SRCDIR)/*~ -fv
	@ rm $(PARDIR)/$(OBJDIR)/* -rfv
	@ rm $(PARDIR)/$(SRCDIR)/*~ -fv
	@ rm $(SEQ_EXEC) -fv
	@ rm $(PAR_EXEC) -fv
	@ find $(SEQDIR)/$(EXEDIR)/ -type f -not -name "test_*" -not -name "out_*" -exec rm -fv {} \;
	@ find $(PARDIR)/$(EXEDIR)/ -type f -not -name "test_*" -not -name "out_*" -exec rm -fv {} \;
	@ find . -maxdepth 0 -type f -not -name "(README|makefile|run)" -exec rm -fv {} \;
	@ echo -e "\e[0;32mDirectory clean\033[0m" ;\

sequential: $(SEQ_OBJECTS) $(SEQ_EXEOBJECT)
	$(CC) $(SEQ_OBJECTS) $(SEQ_EXEOBJECT) -o $(SEQ_EXEC) $(CFLAGS) $(CPPFLAGS)
	@ echo -e "\e[0;32mSequential executable compiled: ${SEQ_EXEC}\033[0m" ;\

parallel: $(PAR_OBJECTS) $(PAR_EXEOBJECT)
	$(CC) $(PAR_OBJECTS) $(PAR_EXEOBJECT) -o $(PAR_EXEC) $(CFLAGS) $(CPPFLAGS)
	@ echo -e "\e[0;32mParallel executable compiled: ${PAR_EXEC}\033[0m" ;\
