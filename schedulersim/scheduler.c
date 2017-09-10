#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include "process.h"
#include "scheduler.h"

Process *process_table;
Process *head;
int n_proc = 0, remaining_process = 0, schel_policy = ROUND_ROBIN;
clock_t start_time;
pthread_mutex_t queue_lock, cpu0, cpu1, cpu2, cpu3, process_count;

void scheduler_simulator(int scheduler_option, char *trace) {
	int i;
	char name[200]; 
	double t0, dt, deadline;
	FILE *trace_in;
	Process *tmp, *p;

	printf("Inicializando lista de processos no scheduler_simulator...\n");
	n_proc = remaining_process = count_processes (trace);
	process_table = mallocc(sizeof (Process));
	process_table->next = NULL;
	printf("SCHEDULER_SIMULATOR: remaining_process = %d!\n", remaining_process);

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
}

void shortest_job_first (int nproc){
}

void round_robin (int nproc) {
	pthread_t q_manager, cpus[N_CPUS];
	pthread_mutex_t *cpu_lock;
	int i;

	printf("Cheguei até aqui, pelo menos! Na round_robin()\n");

	/* prepara as coisas do queue_manager e lança ele */

	pthread_mutex_init(&queue_lock, NULL);
	printf("Lançando queue_manager...\n");
	pthread_create(&q_manager, NULL, queue_manager, NULL);

	printf("Inicializando mutexes.\n");

	pthread_mutex_init(&process_count, NULL);
	pthread_mutex_init(&cpu0, NULL);
	pthread_mutex_init(&cpu1, NULL);
	pthread_mutex_init(&cpu2, NULL);
	pthread_mutex_init(&cpu3, NULL);

	sleep(3);
	printf("Distribuindo threads das CPUs.\n");
	for (i = 0; i < N_CPUS; i++) {
		printf("Lançando cpu_scheduler's...\n");
		pthread_create(&(cpus[i]), NULL, cpu_scheduler, (void *) i);
	}

	/* a simulação acaba quando o queue_manager voltar */
	pthread_join(q_manager, NULL);
	pthread_join(cpus[0], NULL);
	pthread_join(cpus[1], NULL);
	pthread_join(cpus[2], NULL);
	pthread_join(cpus[3], NULL);

	return;
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
			printf("Q MANAGER: iniciando verificacao de processos pendentes.\n");

			/* verifica se tem novos processos tentando entrar na fila
				de processos */
			sim_time = (double) (clock() - start_time) * 1000. / CLOCKS_PER_SEC;
			printf("Tempo de simulação atual: %f.\n", sim_time);
			dummy = head;
			if (dummy->next != NULL && sim_time >= dummy->next->t0) {

				/*trava a fila para podermos mexer nela */
				pthread_mutex_lock(&queue_lock);

				/*adiciona todos os processos na tabela de processos se
				  ja tiver passado o tempo t0 desses processos*/
				while (sim_time >= dummy->next->t0 && dummy->next != NULL) {
					printf("Processo novo na tabela!\n");
					tmp = dummy->next->next;
					dummy->next->next = NULL;
					/* prioridade do processo é definido pelo tido de escalonador */
					/* Round-robin: prioridade 3 para todos
					   SJF: prioridade = dt*10
					   Com prioridade: prioridade = (dt + t0) / deadline
					*/
					dummy->next->priority = 3.0;
					printf("processo adicionado de nome %s.\n", dummy->next->name);
					put_process(dummy->next);
					dummy->next = tmp;
				}
				pthread_mutex_unlock(&queue_lock);
			}
			/* dorme por 1 quantum */
			nanosleep(&nsleep_time, NULL);
		}
	}

	else if (schel_policy == SHORTEST_JOB_FIRST){
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

	else {
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
	return;
}

/* Função
*/
void* cpu_scheduler(void* n_cpu) {
	struct timespec nsleep_time;
	pthread_t process_thread;
	Process *process;
	pthread_mutex_t *cpu_lock;
	int cpu_id = *(int *) n_cpu;
	double simulated_time;

	printf("Entrou no cpu scheduler!\n");

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

	printf("CPU SCHEDULER: trava setada!\n");

	/* enquanto tiver processos na fila, escalona eles pra essa cpu */
	while (remaining_process > 0) {
		pthread_mutex_unlock(&process_count);

		pthread_mutex_lock(&queue_lock);
		/* pega o processo e reorganizar a fila */
		process = get_process(); 
		pthread_mutex_unlock(&queue_lock);

		/* libera a trava da cpu para o processo poder executar */
		pthread_mutex_unlock(cpu_lock);
		/* cria thread pra simular a execução do processo na CPU 
		   e dorme em seguida */
		pthread_create(&process_thread, NULL, processing, process);
		nsleep_time = (struct timespec) {QUANTUM_SEC, (long) process->priority * QUANTUM_NSEC};
		nanosleep(&nsleep_time, NULL);

		/* simulação da preempção, usando mutex para travar a thread da cpu
		   e cancelando sua execução */
		pthread_mutex_lock(cpu_lock);
		pthread_cancel(process_thread);
		simulated_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
		/* se o processo fez o que deveria, termina ele */
		if (simulated_time >= process->dt) {
			process->tf = simulated_time;
			printf("Processo %s terminou sua execução no tempo %.1f.\n", process->name, process->tf);

			pthread_mutex_lock(&process_count);
			remaining_process--;
			pthread_mutex_unlock(&process_count);
		}
		else {
			put_process(process);
			printf("Troca de contexto pra acontecer!\n");
		}
		pthread_mutex_lock(&process_count);
	}
	return;
}
/* Função para simular o processo na CPU 
*/
void* processing(void* process) {
	Process *p = (Process *) process;
	double simulated_time;

	while(1) {
		pthread_mutex_lock(p->cpu_lock);
		simulated_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
		pthread_mutex_unlock(p->cpu_lock);
	}

	return;
}

Process *get_process() {
	Process *result;

	result = process_table->next;
	process_table->next = result->next;

	return result;
}

void put_process(Process *p) {
	Process *dummy, *tmp;

	/* no caso do SJF, precisamos garantir que os novos
	   processos sejam ordenados */
	if (schel_policy == SHORTEST_JOB_FIRST) {
		dummy = process_table;
		while (dummy->next != NULL) {
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
			dummy = dummy->next;
		}
		dummy->next = p;
	}
}
