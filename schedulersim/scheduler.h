#ifndef H_SCHEDULER
#define H_SCHEDULER

#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include "process.h"

/* número padrão de cpus do simulador */
#define N_CPUS 4
#define SHORTEST_JOB_FIRST 1
#define ROUND_ROBIN 2
#define PRIORITY_SCHEDULING 3
/* 0.1 segundo = 0 segundos + 10^8 nanossegundo
   Precisamos disso pro nanosleep() */
#define QUANTUM_SEC (time_t)0
#define QUANTUM_NSEC 1e8


void* processing(void* process);
void* queue_manager();
void* cpu_scheduler(void*);
void scheduler_simulator(int scheduler_option, char *trace);
void shortest_job_first ();
void round_robin ();
void priority_scheduling ();

#endif