all:
	gcc -o metronome metronome.c

install:
	mv metronome /usr/bin/metronome

uninstall:
	rm -f /usr/bin/metronome

clean:
	rm -f metronome
