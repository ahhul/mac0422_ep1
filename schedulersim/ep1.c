#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include "process.h"
#include "scheduler.h"

Process **ptable = NULL;
pthread_mutex_t table_lock;

int main(int argc, char* argv[]){
	int i;
	char c;
	ptable = malloc(100 * sizeof(Process *));

	for (i = 0; i < 100; i++) {
		ptable[i] = malloc(sizeof(Process));
		ptable[i]->name = "processo";
		ptable[i]->t0 = i;
	}

	for (i = 0; i < 100; i++) {
		pthread_create(&(ptable[i]->thread), NULL, processing, (void *) ptable[i]);
	}

	for (i = 0; i < 100; i++) {
		free(ptable[i]);
	}

	free(ptable);
	return 0;
}