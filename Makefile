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

ep1: ep1.o scheduler.o
	$(CC) ep1.o scheduler.o -lpthread -o ep1
	rm -f *.o

ep1.o: schedulersim/ep1.c schedulersim/process.h schedulersim/scheduler.h
	$(CC) $(CCFLAGS) -lpthread -c schedulersim/ep1.c

scheduler.o: schedulersim/scheduler.c schedulersim/process.h
	$(CC) $(CCFLAGS) -lpthread -c schedulersim/scheduler.c

clean: 
	rm -f *.o 
