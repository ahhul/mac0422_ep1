/* João Henrique Luciano (NUSP: 8535957) */
/* Ludmila Ferreira      (NUSP: 7557136) */
/* EP 1 de Sistemas Operacionais - 2017  */

#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


Process *new_process (double t0, double dt, double deadline, char *name) {
    Process *proc = mallocc (sizeof (Process));
    proc->t0 = t0;
    proc->dt = dt;
    proc->deadline = deadline;
    proc->name = name;
    proc->priority = 0.;
    proc->next = NULL;
    proc->exec_thread = NULL;
    proc->cpu_lock = NULL;
    return proc;
}

int count_processes (char *file_name) {
	int lines, ch;
	FILE *trace = fopen(file_name, "r");
	lines = ch = 0;

	while(!feof(trace)){
		ch = fgetc(trace);
		if(ch == '\n')	lines++;
	}

	fclose(trace);

	return lines;
}

double real_time (Process *proc){
    return proc->tf - proc->start_time;
}

void* mallocc(size_t nbytes) {
	void* ptr;
	ptr = malloc(nbytes);
	if (ptr == NULL) {
		printf("Erro ao alocar memoria. Saindo...\n");
		exit(1);
	}
	return ptr;
}

