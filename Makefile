CC=gcc
CFLAGS=-Wall
DEPS=fonctions.h
EXEC=program

all : $(EXEC)

fonctions.o : fonctions.c $(DEPS)
	$(CC) $(CFLAGS) -c fonctions.c
			
main2.o : main2.c $(DEPS)
	$(CC) $(CFLAGS) -c main2.c
		
program : fonctions.o main2.o 
	$(CC) $(CFLAGS) -o $(EXEC) main2.c fonctions.c

clean : 
	rm -rf $(EXEC) *.o
