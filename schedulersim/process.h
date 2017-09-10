#ifndef PROCESS_HEADER
#define PROCESS_HEADER

#include <pthread.h>
#include <semaphore.h>

/* Um processo, além dos requisitos do EP, tem um campo
   para falar em qual sua thread de execução, um campo next para lista ligada */
struct process {
	char* name;
	double t0, dt, deadline, tf, start_time, priority;
	sem_t *cpu_lock;
	pthread_t *exec_thread;
	struct process *next;
};
typedef struct process Process;

/* Cria um novo processo*/
Process *new_process (double t0, double dt, double deadline, char *name);

/* Conta o número de processos do arquivo de trace */
int count_processes (char *trace);

/* Retorna tr, o tempo de execução "do relógio" do processo */
double real_time(Process *proc);


/* mallocc - wrapper do malloc da stdlib, 
com manejamento de erro. Inspirado no site de
Paulo Feofiloff: https://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html
*/
void* mallocc(size_t nbytes);



#endif
