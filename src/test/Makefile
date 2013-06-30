CFLAGS+=-Wall -Werror -std=c99 -pedantic -pedantic-errors -g
LDFLAGS+=
MAIN=filetest
SRC=filetest.c
OBJ=$(SRC:.c=.o)
EXTRAOBJ=../util.o ../netfile.o
CC?=gcc

all: data $(SRC) $(MAIN)

data:
	python generate.py

$(MAIN): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(EXTRAOBJ)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	-rm -f $(MAIN) $(OBJ) *part0 *part1 generateddata.txt

.PHONY: clean install
