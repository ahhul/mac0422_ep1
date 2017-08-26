#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include "aux.h"

/* mallocc - wrapper do malloc da stdlib, 
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
	fgets(input, MAX_COMMAND_SIZE * sizeof(char), stdin);

	return input;
}

char** parse_command(char* command) {
	int i;
	char** args;
	char* token;
	const char* delimiter = " \n";

	args = mallocc(MAX_ARGS * sizeof(char*));

	/* pega o primeiro token pra setar as variaveis internas da strtok */
	token = strtok(command, delimiter);

	/* agora é só tokenizar o resto da linha de comando */
	for (i = 0; i < MAX_ARGS; i++) {
		args[i] = token;
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