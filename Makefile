# Makefile

CC= gcc -fsanitize=address

CFLAGS= -Wall -Wextra -lSDL -std=gnu99 -g -O3
LDFLAGS=
LDLIBS=

SRC= vidtest.c #Source files here
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}
BIN= ${OBJ:.o=}

all: ${BIN} #Replace by 'main' if only one .exe

-include ${DEP}

main: ${OBJ}

.PHONY: clean

clean:
	rm -f ${OBJ} ${DEP} ${BIN}

# END
