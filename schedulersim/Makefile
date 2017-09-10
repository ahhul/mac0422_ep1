CC     = gcc
CFLAGS = -g -Wall -ansi -Wno-unused-result
RM     = rm
#------------------------------------------------

ep1: ep1.o scheduler.o process.o
	$(CC) $(CFLAGS) ep1.o process.o scheduler.o -lpthread -o ep1

ep1.o: ep1.c process.h scheduler.h
	$(CC) $(CFLAGS) -lpthread -c ep1.c

scheduler.o: scheduler.c process.h
	$(CC) $(CFLAGS) -lpthread -c scheduler.c

process.o: process.c process.h
	$(CC) $(CFLAGS) -lpthread -c process.c

clean: 
	rm -f *.o 
