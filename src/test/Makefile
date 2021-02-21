# Makefile

CC= gcc #-fsanitize=address

CFLAGS=-Wall -Wextra -g -std=gnu99 -O3 -O0
LDFLAGS= -lm
LDLIBS=-lSDL -lSDL_image

SRC= rgb_to_hsi.c ../tools.c main.c#Source files here
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: main #Replace by 'main' if only one .exe

main: ${OBJ}
	$(CC) $(CFLAGS) $(OBJ) -o main $(LDFLAGS) $(LDLIBS)

.PHONY: clean

clean:
	rm -f ${OBJ} ${DEP} ${BIN} main

-include ${DEP}

# END
