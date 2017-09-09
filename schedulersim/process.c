#include "process.h"
#include <stdio.h>
#include <stdlib.h>


Process *new_process (double t0, double dt, double deadline, char *name) {
    Process *proc = mallocc (sizeof (Process));
    proc->t0 = t0;
    proc->dt = dt;
    proc->deadline = deadline;
    proc->name = name;
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

	return lines;
}

double real_time (Process *proc){
    return proc->tf - proc->t0;
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

