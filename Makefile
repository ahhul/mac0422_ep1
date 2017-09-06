CC     = gcc
CFLAGS = -g -Wall -ansi -pedantic -Wno-unused-result
RM     = rm
#------------------------------------------------

ep1sh: ep1sh.o aux.o
	$(CC) ep1sh.o aux.o -lreadline -o ep1sh
	rm -f *.o

ep1sh.o: bash/ep1sh.c bash/aux.h
	$(CC) $(CFLAGS) -lreadline -c bash/ep1sh.c 

aux.o: bash/aux.c bash/aux.h
	$(CC) $(CFLAGS) -c bash/aux.c  

clean: 
	rm -f *.o 
