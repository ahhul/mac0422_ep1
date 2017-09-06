#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include "process.h"

void* processing(void* process) {
	double i = time(NULL);
	Process *p = (Process *) process;
	printf("Thread %.1f entrou e saiu!\n", p->t0);
}