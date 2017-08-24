#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include "aux.h"

/* mallocc - função wrapper do malloc da stdlib, 
com manejamento de erro. Inspirado no site de
Paulo Feofiloff: https://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html
*/
void* mallocc(size_t nbytes) {
	void* ptr;
	ptr = malloc(nbytes);
	if (ptr == NULL) {
		printf("Erro ao alocar memoria. Saindo...\n");
		exit(1);
	}
	return ptr;
}


char* prompt_terminal() {
	char* input = mallocc(MAX_COMMAND_SIZE*sizeof(char));

	printf("$ ");
	scanf("%s", input);

	return input;
}

char** parse_command(char* command) {
	int i;
	char** args;
	char* token;
	const char* delimiter = " ";

	args = mallocc(MAX_ARGS * sizeof(char*));
	/*for (i = 0; i < MAX_ARGS; i++) {
		args[i] = mallocc(MAX_ARG_SIZE * sizeof(char));
	}*/

	token = strtok(command, delimiter);
	/* args[0] = <nome do programa> */
	args[0] = token;

	for (i = 1; token != NULL && i < MAX_ARGS; i++) {
		args[i] = token;
		printf("token - %s\n", args[i]);
		token = strtok(NULL, delimiter);
	}
	return args;
}

void open_process(char* process, char** args) {
	return;
}

void exec_command(char* command, char** args) {
	return;
}