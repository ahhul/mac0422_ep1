#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include "process.h"
#include "scheduler.h"

int main(int argc, char* argv[]){
	
	printf("Começando simulação.\n");
	scheduler_simulator(ROUND_ROBIN, "trace.txt");
	

	return 0;
}
