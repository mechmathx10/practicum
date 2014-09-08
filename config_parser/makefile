CFLAGS= -Wall -Werror -Wextra -O3 -g -std=gnu99
CC=gcc

OBJECTIVES=input.o config_parser.o main.o

all : main

main : $(OBJECTIVES)
	$(CC) $(OBJECTIVES) -o main $(CFLAGS)

input.o : input.[hc]
	$(CC) input.c -c $(CFLAGS)

config_parser.o : config_parser.[hc]
	$(CC) config_parser.c -c $(CFLAGS)

main.o : main.c
	$(CC) main.c -c $(CFLAGS)

clean :
	rm *~ *.o main -rf
