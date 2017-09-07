#include "process.h"


Process *new_process (double t0, double dt, double deadline, char *name) {

    Process *proc = mallocc (sizeof (Process));
    proc->t0 = t0;
    proc->dt = dt;
    proc->deadline = deadline;
    proc->name = name;
    job->next = NULL;
    return proc;
}

int count_processes (char *file_name) {

	int lines, ch;
	FILE trace = fopen(file_name, "r");
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
