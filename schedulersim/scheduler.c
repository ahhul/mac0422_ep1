#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include "process.h"
#include "aux.h"


void scheduler_simulator(int scheduler_option, char *trace) {

	int i, n_proc;
	char *name; 
	double t0, dt, deadline;
	FILE trace_in;
	Process *table;
	/* Ponteiro da lista de processos */
	Process *head, *tmp;

	head->next = NULL;
	num_proc = count_processes (trace);
	trace_in = fopen(trace, "r");

	/* Insere as informações do arquivo de trace na lista head */
	tmp = head;
	for (i = 0; i < n; i++) {
		fscanf (trace_in, "%f %f %f %s", t0, dt, deadline, name);
		tmp->next = new_process (t0, dt, deadline, name);
		tmp = tmp->next;	
	}

	/* Aloca a tabela de processos */
	table = mallocc (n_proc * sizeof (Process));
	/* Precisa colocar os roles nela*/
	
	if (scheduler_option == 1) {
		shortest_job_first();
	}else if (scheduler_option == 2) {
		round_robin();
	}else if(scheduler_option == 3) {
		priority_scheduling();
	}else {
		/* dá pra colocar umas mensagens de erro do sistema aqui né?*/
		printf("Opção de escalonamento inválida. \n");
	}

void shortest_job_first (){
}

void round_robin () {
}

void priority_scheduling () {
}

void* processing(void* process) {
	double i = time(NULL);
	Process *p = (Process *) process;
	printf("Thread %.1f entrou e saiu!\n", p->t0);
}




