/* João Henrique Luciano (NUSP: 8535957) */
/* Ludmila Ferreira      (NUSP: 7557136) */
/* EP 1 de Sistemas Operacionais - 2017  */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include "process.h"
#include "scheduler.h"

int main(int argc, char* argv[]){
	int schel_policy;
	char trace_file[20];

	schel_policy = atoi(argv[1]);
	strcpy(trace_file, argv[2]);
	
	scheduler_simulator(schel_policy, trace_file);
	
	return 0;
}
