CC=mpicc
CFLAGS=-O2

tspgen:
	$(CC) -O3 -o tspgen *.c -lm -DDEBUG

.PHONY: clean
clean:
	rm -f tspgen

.PHONY: all
all: tspgen
