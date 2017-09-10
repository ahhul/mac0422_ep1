CC     = gcc
CFLAGS = -g -Wall -ansi -Wno-unused-result
RM     = rm
#------------------------------------------------

ep1sh: ep1sh.o aux.o
	$(CC) ep1sh.o aux.o -lreadline -o ep1sh

ep1sh.o: bash/ep1sh.c bash/aux.h
	$(CC) $(CFLAGS) -lreadline -c bash/ep1sh.c 

aux.o: bash/aux.c bash/aux.h
	$(CC) $(CFLAGS) -lreadline -c bash/aux.c

ep1: ep1.o scheduler.o process.o
	$(CC) $(CFLAGS) ep1.o process.o scheduler.o -lpthread -o ep1

ep1.o: schedulersim/ep1.c schedulersim/process.h schedulersim/scheduler.h
	$(CC) $(CFLAGS) -lpthread -c schedulersim/ep1.c

scheduler.o: schedulersim/scheduler.c schedulersim/scheduler.h schedulersim/process.h
	$(CC) $(CFLAGS) -lpthread -c schedulersim/scheduler.c

process.o: schedulersim/process.c schedulersim/process.h
	$(CC) $(CFLAGS) -lpthread -c schedulersim/process.c

clean: 
	rm -f *.o 
