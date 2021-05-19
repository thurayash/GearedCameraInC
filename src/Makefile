# Makefile

CC= gcc #-fsanitize=address

CFLAGS=-Wall -Wextra -g -std=gnu99 -O3 -O0
LDFLAGS= -lm
LDLIBS=-lSDL -lSDL_image

SRC=$(shell find libs/ -type f -name '*.c' ! -iname "verif_line.c")
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}
MYDIR=libs/src
MYOBJ=libs/obj/

all: main #Replace by 'main' if only one .exe

object:${SRC}
	@for f in $(shell ls ${MYDIR}/*.c | grep -v 'verif_line.c$$' | xargs -n 1 basename); do echo \
	"Compilling $${f} ..." >> libs/LOGS; \
	$(CC) $(CFLAGS) -o libs/obj/$${f%.*}.o -c libs/src/$${f} $(LDFLAGS) $(LDLIBS); done
	echo "\nObjects compilling done !" >> libs/LOGS

main: object
	$(CC) $(CFLAGS) $(MYOBJ)* -o main $(LDFLAGS) $(LDLIBS)

.PHONY: clean

clean:
	echo  > libs/LOGS
	rm -f libs/obj/*.o
	rm -f ${OBJ} ${DEP} ${BIN} main

verif_line:
	./verif ./libs/src/*.c
	./verif ./libs/header/*.h
-include ${DEP}

verif_space :
	python3 .space_verif.py *.c *.h

remove_space:
	sed -i "s/\s*$$//" *.c *.h

# END
