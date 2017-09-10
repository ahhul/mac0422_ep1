/* João Henrique Luciano (NUSP: 8535957) */
/* Ludmila Ferreira      (NUSP: 7557136) */
/* EP 1 de Sistemas Operacionais - 2017  */

#ifndef H_SCHEDULER
#define H_SCHEDULER

#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include "process.h"

/* número padrão de cpus do simulador */
#define N_CPUS 4
#define SHORTEST_JOB_FIRST 1
#define ROUND_ROBIN 2
#define PRIORITY_SCHEDULING 3
/* 0.1 segundo = 0 segundos + 10^8 nanossegundo
   Precisamos disso pro nanosleep() */
#define QUANTUM_SEC (time_t)0
#define QUANTUM_NSEC 1e8


/* executa a simulação do escalonador propriamente dita */
void scheduler_simulator(int scheduler_option, char *trace);

/* lança as threads para simulação do escalonador
   usando a politica shortest-job-first */
void shortest_job_first (int nproc);

/* lança as threads para simulação do escalonador
   usando a politica round-robin */
void round_robin (int nproc);


void priority_scheduling (int nproc);

/* Função para a thread que será responsável por
   gerenciar a fila de processos. Ela vai verificar
   periodicamente (a cada quantum, de 0.1s) se há novos
   processos querendo entrar na fila
 */
void* queue_manager();

/* Função pra simular o escalonador de cada CPU
*/
void* cpu_scheduler(void* n_cpu);

/* Função para simular o processo na CPU 
*/
void* processing(void* process);

/* pega processo na fila de processos.  */
Process *get_process();

/* Coloca processos na fila de processos. */
void put_process(Process *p);

#endif