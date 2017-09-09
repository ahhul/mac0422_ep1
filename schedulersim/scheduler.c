#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include "process.h"
#include "scheduler.h"

Process **process_table = NULL;
Process *head = NULL;
int n_proc = 0, remaining_process = 0;
clock_t start_time;
pthread_mutex_t queue_lock, cpu0, cpu1, cpu2, cpu3;

void scheduler_simulator(int scheduler_option, char *trace) {
	int i;
	char *name; 
	double t0, dt, deadline;
	FILE *trace_in;
	Process *tmp, *p;

	printf("Inicializando lista de processos no scheduler_simulator...\n");
	n_proc = remaining_process = count_processes (trace);
	process_table = mallocc(n_proc * sizeof (Process *));

	trace_in = fopen(trace, "r");
	/* Guarda as informações do arquivo de trace na lista head */
	tmp = head;
	for (i = 0; i < n_proc; i++) {
		fscanf (trace_in, "%f %f %f %s", &t0, &dt, &deadline, name);
		p = new_process(t0, dt, deadline, name);
		tmp->next = p;
		p->next = NULL;
		tmp = p;
	}
	
	/* agora que está tudo setado, a simulação em si começa */
	start_time = clock();
	if (scheduler_option == SHORTEST_JOB_FIRST) {
		shortest_job_first(n_proc);
	}else if (scheduler_option == ROUND_ROBIN) {
		round_robin(n_proc);
	}else if(scheduler_option == PRIORITY_SCHEDULING) {
		priority_scheduling(n_proc);
	}else {
		/* dá pra colocar umas mensagens de erro do sistema aqui né?*/
		printf("Opção de escalonamento inválida. \n");
	}

	printf("Fim de escalonamento! :)");

	free(process_table);
}

void shortest_job_first (int nproc){
}

void round_robin (int nproc) {
	pthread_t q_manager, cpus[N_CPUS];
	pthread_mutex_t *cpu_lock;
	int i, *aux;

	printf("Cheguei até aqui, pelo menos! Na round_robin()\n");

	/* prepara as coisas do queue_manager e lança ele */
	pthread_mutex_init(&queue_lock, NULL);
	pthread_create(&q_manager, NULL, queue_manager, (void *) ROUND_ROBIN);

	pthread_mutex_init(&cpu0, NULL);
	pthread_mutex_init(&cpu1, NULL);
	pthread_mutex_init(&cpu2, NULL);
	pthread_mutex_init(&cpu3, NULL);
	for (i = 0; i < N_CPUS; i++) {
		*aux = i;
		pthread_create(&(cpus[i]), NULL, cpu_scheduler, (void *) aux);
	}

	/* a simulação acaba quando o queue_manager voltar */
	pthread_join(q_manager, NULL);
}

void priority_scheduling (int nproc) {
}

/* Função para a thread que será responsável por
   gerenciar a fila de processos. Ela vai verificar
   periodicamente (a cada quantum, de 0.1s) se há novos
   processos querendo entrar na fila
 */
void* queue_manager(void* schel_policy) {
	pthread_t *threads = mallocc(n_proc * sizeof(pthread_t));
	struct timespec nsleep_time = (struct timespec) {QUANTUM_SEC, QUANTUM_NSEC};
	Process *dummy;
	double sim_time;

	if (*(int *) schel_policy == ROUND_ROBIN){

		while(remaining_process > 0) {

			sim_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
			if (sim_time >= head->t0) {

				/*trava a fila para podermos mexer nela */
				pthread_mutex_lock(&queue_lock);

				/*adiciona todos os processos na tabela de processos se
				  ja tiver passado o tempo t0 desses processos*/
				while (sim_time >= head->t0) {
					/* pegar processo e setar prioridade pra 3 (prioridade normal)
					 pra todos eles (round robin)*/
					break;
				}

				pthread_mutex_unlock(&queue_lock);
			}
			/* dorme por 1 quantum */
			nanosleep(&nsleep_time, NULL);
		}
	}

	else if (*(int *) schel_policy == SHORTEST_JOB_FIRST){
		while(remaining_process > 0) {
			pthread_mutex_lock(&queue_lock);

			sim_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
			if (sim_time >= head->t0) {
				/*adiciona todos os processos na tabela de processos se
				  ja tiver passado o tempo t0 desses processos*/
				while (sim_time >= head->t0) {
					break;
				}
			}

			pthread_mutex_unlock(&queue_lock);
			nanosleep(&nsleep_time, NULL);
		}
	}

	else {
		while(remaining_process > 0) {
			pthread_mutex_lock(&queue_lock);

			sim_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
			if (sim_time >= head->t0) {
				/*adiciona todos os processos na tabela de processos se
				  ja tiver passado o tempo t0 desses processos*/
				while (sim_time >= head->t0) {
					break;
				}
			}

			pthread_mutex_unlock(&queue_lock);
			nanosleep(&nsleep_time, NULL);
		}
	}
	free(threads);
}

/* Função
*/
void* cpu_scheduler(void* n_cpu) {
	struct timespec nsleep_time;
	pthread_t process_thread;
	Process *process;
	pthread_mutex_t *cpu_lock;
	int cpu_id = *(int *) n_cpu;

	/* recebe a trava da cpu certa */
	if (cpu_id == 0) {
		cpu_lock = &cpu0;
	}
	else if (cpu_id == 1) {
		cpu_lock = &cpu1;
	}
	else if (cpu_id == 2) {
		cpu_lock = &cpu2;
	}
	else {
		cpu_lock = &cpu3;
	}

	/* enquanto tiver processos na fila, escalona eles pra essa cpu */
	while (remaining_process > 0) {

		pthread_mutex_lock(&queue_lock);
		/* alguma coisa pra pegar o processo e reorganizar a fila */
		process = NULL; 
		pthread_mutex_unlock(&queue_lock);

		pthread_create(&process_thread, NULL, processing, process);
	
		nsleep_time = (struct timespec) {QUANTUM_SEC, (long) (6 - process->priority) * QUANTUM_NSEC};
		nanosleep(&nsleep_time, NULL);
	}


}
/* Função para simular o processo na CPU 
*/
void* processing(void* process) {
	Process *p = (Process *) process;
	int i = 0;

	while(1) {
		pthread_mutex_lock(p->cpu_lock);
		i = i+1;
		i = 0;
		pthread_mutex_unlock(p->cpu_lock);
	}
}

