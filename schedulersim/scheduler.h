#ifndef H_SCHEDULER
#define H_SCHEDULER

#include <pthread.h>
#include <sys/types.h>
#include "process.h"

void* processing(void* process);

void scheduler_simulator(int scheduler_option, char *trace);

void shortest_job_first ();

void round_robin ();

void priority_scheduling ();

#endif