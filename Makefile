all: DET

DET: DET.o
	gcc -o DET DET.o

DET.o: DET.c DET.h
	gcc -c DET.c

