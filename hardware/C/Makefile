CC=gcc
LDFLAGS= -g -std=gnu99 -Werror -Wall -pthread -O0
LDLIBS=
EXECV= comp
SRC=$(wildcard *.c)
all: main

main :
	$(CC) $(LDFLAGS) -o $(EXECV) $(SRC)

clean:
	rm -rf *.o $(EXECV)




