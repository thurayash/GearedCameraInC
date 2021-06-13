CC = gcc -g -O0 -fsanitize=address
CFLAGS = `pkg-config --cflags glib-2.0` -Wall -Wextra --coverage
LDLIBS = `pkg-config --libs glib-2.0`

EXE = test
SRC=test.c json_parser.c

all:
	$(CC) $(CFLAGS) $(SRC) $(LDLIBS) -o $(EXE)

.PHONY: clean

clean:
	${RM} $(EXE)

