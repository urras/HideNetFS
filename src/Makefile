CFLAGS+=-Wall -std=c99 -pedantic -pedantic-errors -g
LDFLAGS+=
#MAIN=
SRC=netfile.c util.c
OBJ=$(SRC:.c=.o)
CC?=gcc
DESTDIR?=/
PREFIX?=/usr/local

all: $(SRC) $(OBJ) $(MAIN)

$(MAIN): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ)

tests: all
	cd test; make

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	-rm -f $(MAIN) $(OBJ)
	cd test; make clean

install: $(MAIN)
	mkdir -p $(DESTDIR)/$(PREFIX)/bin
	cp -v $(MAIN) $(DESTDIR)/$(PREFIX)/bin/$(MAIN)

.PHONY: clean install
