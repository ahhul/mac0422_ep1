#ifndef H_SCHEDULER
#define H_SCHEDULER

#include <pthread.h>
#include <sys/types.h>
#include "process.h"

void* processing(void* process);

#endif