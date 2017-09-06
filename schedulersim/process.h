#ifndef PROCESS_HEADER
#define PROCESS_HEADER

#include <pthread.h>

/* Um processo, além dos requisitos do EP, tem um campo
   para falar em qual sua thread de execução */
struct process {
	char* name;
	double t0;
	double dt;
	double deadline;
	pthread_t thread;
};
typedef struct process Process;

#endif