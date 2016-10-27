BIN = afnd
OBJ = afnd.o alfabeto.o estado.o ftrans.o tools.o main.o
CFLAGS = -Wall -ansi -pedantic -g

all: ${BIN}

${BIN}:%: ${OBJ}
	gcc ${CFLAGS} -o $@ $^

${OBJ}:%.o: %.c
	gcc ${CFLAGS} -c -o $@ $<

.PHONY: clean

clean:
	rm -v ${BIN} ${OBJ}
