# Makefile

CC= gcc #-fsanitize=address

CFLAGS=-Wall -Wextra -g -std=gnu99 -O3 -O0
LDFLAGS= -lm
LDLIBS=-lSDL -lSDL_image

SRC= dilation_erosion.c hsi_to_BW.c main.c superpostion.c tools.c ycbcr_to_BW.c hybridation.c circle_detection.c structs.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: main #Replace by 'main' if only one .exe

main: ${OBJ}
	$(CC) $(CFLAGS) $(OBJ) -o main $(LDFLAGS) $(LDLIBS)

.PHONY: clean

clean:
	rm -f ${OBJ} ${DEP} ${BIN} main

verif_line:
	./verif *.c *.h
-include ${DEP}

verif_space :
	python3 .space_verif.py *.c *.h

remove_space:
	sed -i "s/\s*$$//" *.c *.h

# END
