#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "process.h"
#include "scheduler.h"

Process *process_table, *last_process = NULL;
Process *head;
int n_proc = 0, schel_policy = ROUND_ROBIN;
clock_t start_time;
pthread_mutex_t queue_lock, cpu_locks[N_CPUS];
pthread_cond_t cpu_conds[N_CPUS];

void scheduler_simulator(int scheduler_option, char *trace) {
	int i;
	char *name = malloc(20 * sizeof(char)); 
	double t0, dt, deadline;
	FILE *trace_in;
	Process *tmp, *p;

	n_proc = count_processes (trace);
	process_table = mallocc(sizeof (Process));
	process_table->next = NULL;

	trace_in = fopen(trace, "r");
	/* Guarda as informações do arquivo de trace na lista head */
	head = mallocc(sizeof(Process));
	head->next = NULL;
	tmp = head;
	for (i = 0; i < n_proc; i++) {
		fscanf (trace_in, "%lf %lf %lf %s", &t0, &dt, &deadline, name);
		p = new_process(t0, dt, deadline, name);
		p->next = tmp->next;
		tmp->next = p;
		tmp = p;
	}

	fclose(trace_in);

	printf("Inicializando escalonador...\n");
	schel_policy = scheduler_option;
	
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
	free(head);
	free(name);
}

void shortest_job_first (int nproc){
}

void round_robin (int nproc) {
	pthread_t q_manager, cpus[N_CPUS];
	int i, cpu_ids[N_CPUS];

	/* prepara as coisas do queue_manager e lança ele */

	pthread_mutex_init(&queue_lock, NULL);
	pthread_create(&q_manager, NULL, queue_manager, NULL);


	sleep(3);
	for (i = 0; i < N_CPUS; i++) {
		pthread_cond_init(&cpu_conds[i], NULL);
		pthread_mutex_init(&cpu_locks[i], NULL);
		cpu_ids[i] = i;
		pthread_create(&(cpus[i]), NULL, cpu_scheduler, (void *) &cpu_ids[i]);
	}

	/* a simulação acaba quando o escalonamento das CPUs acabar */
	pthread_join(cpus[0], NULL);
	pthread_join(cpus[1], NULL);
	pthread_join(cpus[2], NULL);
	pthread_join(cpus[3], NULL);
}

void priority_scheduling (int nproc) {
}

/* Função para a thread que será responsável por
   gerenciar a fila de processos. Ela vai verificar
   periodicamente (a cada quantum, de 0.1s) se há novos
   processos querendo entrar na fila
 */
void* queue_manager() {
	struct timespec nsleep_time = (struct timespec) {QUANTUM_SEC, QUANTUM_NSEC};
	Process *dummy, *tmp;
	double sim_time;


	if (schel_policy == ROUND_ROBIN){

		/* enquanto tiver novos processos para entrar na tabela */
		while(head->next != NULL) {

			/* verifica se tem novos processos tentando entrar na fila
				de processos */
			sim_time = (double) (clock() - start_time) * 1000. / CLOCKS_PER_SEC;
			dummy = head;
			if (dummy->next != NULL && sim_time >= dummy->next->t0) {

				/*adiciona todos os processos na tabela de processos se
				  ja tiver passado o tempo t0 desses processos*/
				while (dummy->next != NULL && sim_time >= dummy->next->t0) {
					tmp = new_process(dummy->next->t0, dummy->next->dt, dummy->next->deadline, dummy->next->name);
					/* prioridade do processo é definido pelo tido de escalonador */
					/* Round-robin: prioridade 1 para todos
					   SJF: prioridade = dt*10
					   Com prioridade: prioridade = (dt + t0) / deadline
					*/
					tmp->priority = 2.0;

					sem_wait(&queue_lock);
					put_process(tmp);
					sem_wait(&queue_lock);

					tmp = dummy->next->next;
					free(dummy->next);
					dummy->next = tmp;
				}
			}
			/* dorme por 1 quantum */
			nanosleep(&nsleep_time, NULL);
		}
	}

	else if (schel_policy == SHORTEST_JOB_FIRST){

		/* enquanto tiver novos processos para entrar na tabela */
		while(head->next != NULL) {

			/* verifica se tem novos processos tentando entrar na fila
				de processos */
			sim_time = (double) (clock() - start_time) * 1000. / CLOCKS_PER_SEC;
			dummy = head;
			if (dummy->next != NULL && sim_time >= dummy->next->t0) {

				/*trava a fila para podermos mexer nela */
				while(pthread_mutex_trylock(&queue_lock) != 0);

				/*adiciona todos os processos na tabela de processos se
				  ja tiver passado o tempo t0 desses processos*/
				while (dummy->next != NULL && sim_time >= dummy->next->t0) {
					tmp = new_process(dummy->next->t0, dummy->next->dt, dummy->next->deadline, dummy->next->name);
					/* prioridade do processo é definido pelo tido de escalonador */
					/* Round-robin: prioridade 3 para todos
					   SJF: prioridade = dt*10
					   Com prioridade: prioridade = (dt + t0) / deadline
					*/
					tmp->priority = tmp->dt * 10.;
					put_process(tmp);

					tmp = dummy->next->next;
					free(dummy->next);
					dummy->next = tmp;
				}
				pthread_mutex_unlock(&queue_lock);
			}
			/* dorme por 1 quantum */
			nanosleep(&nsleep_time, NULL);
		}
	}

	else {

		/* enquanto tiver novos processos para entrar na tabela */
		while(head->next != NULL) {

			/* verifica se tem novos processos tentando entrar na fila
				de processos */
			sim_time = (double) (clock() - start_time) * 1000. / CLOCKS_PER_SEC;
			dummy = head;
			if (dummy->next != NULL && sim_time >= dummy->next->t0) {

				/*adiciona todos os processos na tabela de processos se
				  ja tiver passado o tempo t0 desses processos*/
				while (dummy->next != NULL && sim_time >= dummy->next->t0) {
					tmp = new_process(dummy->next->t0, dummy->next->dt, dummy->next->deadline, dummy->next->name);
					/* prioridade do processo é definido pelo tido de escalonador */
					/* Round-robin: prioridade 3 para todos
					   SJF: prioridade = dt*10
					   Com prioridade: prioridade = (dt + t0) / deadline
					*/
					tmp->priority = (tmp->dt + tmp->t0) / tmp->deadline;
					tmp->start_time = sim_time;
					put_process(tmp);

					tmp = dummy->next->next;
					free(dummy->next);
					dummy->next = tmp;
				}
			}
			/* dorme por 1 quantum */
			nanosleep(&nsleep_time, NULL);
		}
	}

	printf("Fim do queue_manager! :)\n");
	pthread_exit(NULL);
}

/* Função pra simular o escalonador de cada CPU
*/
void* cpu_scheduler(void* n_cpu) {
	struct timespec nsleep_time;
	Process *process;
	pthread_mutex_t *cpu_lock;
	int cpu_id = *(int *) n_cpu;
	double simulated_time;
	pthread_cond_t *cpu_cond;

	/* pega a trava correspondente ao numero da cpu */
	cpu_lock = &cpu_locks[cpu_id];
	cpu_cond = &cpu_conds[cpu_id];

	printf("Trava do scheduler (CPU %d) recebida!\n", cpu_id);

	/* enquanto tiver processos na fila, escalona eles pra essa cpu */
	while (process_table->next != NULL) {

		/* pega o processo e reorganizar a fila */
		sem_wait(&queue_lock);
		process = get_process();
		sem_post(&queue_lock);

		if (process->exec_thread == NULL) {
			process->exec_thread = mallocc(sizeof(pthread_t));
		}

		process->cpu_lock = cpu_lock;
		process->cpu_cond = cpu_cond;

		printf("Entrou no escalonador o processo de t0 = %.1f na CPU %d.\n", process->t0, cpu_id);

		/* cria thread pra simular a execução do processo na CPU 
		   e dorme em seguida */
		/*nsleep_time = (struct timespec) {QUANTUM_SEC, (long) process->priority * QUANTUM_NSEC};*/

		pthread_mutex_lock(process->cpu_lock);
		pthread_cond_wait(cpu_cond, process->cpu_lock);
		pthread_create(process->exec_thread, NULL, processing, (void *) process);
		pthread_mutex_unlock(process->cpu_lock);


		/* simulação da preempção, usando mutex para travar a thread da cpu
		   e cancelando sua execução */
		simulated_time = (double) (clock() - process->start_time) / CLOCKS_PER_SEC;
		/* se o processo fez o que deveria, termina ele */
		if (simulated_time >= process->dt) {
			process->tf = simulated_time;
			printf("Processo %s terminou sua execução no tempo %.1f.\n", process->name, process->tf);
		}
		else {
			sem_wait(&queue_lock);
			put_process(process);
			sem_post(&queue_lock);

			printf("Troca de contexto pra acontecer com o processo de t0 = %f\n", process->t0);
		}
	}
	pthread_exit(NULL);
}

/* Função para simular o processo na CPU 
*/
void* processing(void* process) {
	struct timespec nsleep_time;
	Process *p = (Process *) process;

	printf("Entrou no processamento o processo de t0 = %.1f\n", p->t0);
	nsleep_time = (struct timespec) {QUANTUM_SEC, (long) p->priority * QUANTUM_NSEC};
	nanosleep(&nsleep_time, NULL);
	pthread_cond_signal(p->cpu_cond);
}

/* pega processo na fila de processos. 
   Usa a trava para impedir que outras threads mexam 
   na fila enquanto essa função é executada */
Process *get_process() {
	Process *result;

	if (process_table->next != NULL) {
		result = process_table->next;
		process_table->next = result->next;
		return result;
	}
	else {
		return NULL;
	}
}

/* Coloca processos na fila de processos. 
   Usa a trava para impedir que outras threads mexam 
   na fila enquanto essa função é executada */
void put_process(Process *p) {
	Process *dummy, *tmp;

	/* no caso do SJF, precisamos garantir que os novos
	   processos sejam ordenados */
	if (schel_policy == SHORTEST_JOB_FIRST) {
		dummy = process_table;
		while (dummy->next != NULL) {
			printf("put process rodando no loop.\n");
			if (p->dt < dummy->next->dt) {
				tmp = dummy->next;
				dummy->next = p;
				p->next = tmp;

				break;
			}
			dummy = dummy->next;
		}
	}

	else {
		dummy = process_table;
		while (dummy->next != NULL) {
			printf("put process rodando no loop.\n");

			dummy = dummy->next;
		}
		dummy->next = p;
	}
}
