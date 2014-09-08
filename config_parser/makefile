CFLAGS= -Wall -Werror -Wextra -O3 -g -std=gnu99
CC=gcc

EXECUTUBLE=main
SOURCES=input.c config_parser.c array.c main.c 
OBJECTS=$(SOURCES:.c=.o)

all : $(EXECUTUBLE)

$(EXECUTUBLE) : $(OBJECTS)
	$(CC) $(OBJECTS) -o main $(CFLAGS)

.[hc].o:
	$(CC) $(CFLAGS) $< -o $@

clean :
	rm *~ *.o main -rf
