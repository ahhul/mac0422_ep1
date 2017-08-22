CC     = gcc
CFLAGS = -g -Wall -ansi -pedantic -Wno-unused-result
RM     = rm
#------------------------------------------------

# chama o linker
ep1:  main.o aux.o
	$(CC) main.o aux.o -o ep1

main.o: main.c aux.h
	$(CC) $(CFLAGS) -c main.c 

aux.o: aux.c aux.h
	$(CC) $(CFLAGS) -c aux.c  

clean: 
	rm -f *.o 
