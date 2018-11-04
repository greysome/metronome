CC = cc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -Os
LDFLAGS =
LDLIBS = 
PREFIX = /usr/local

all: metronome

metronome: metronome.c
	$(CC) $(LDFLAGS) $(CFLAGS) -o metronome metronome.c $(LDLIBS)

clean:
	rm -f metronome

install:
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m 755 metronome $(DESTDIR)$(PREFIX)/bin

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/metronome
