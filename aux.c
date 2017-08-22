#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include "aux.h"

char* prompt_terminal() {
	char* input = malloc(MAX_COMMAND_SIZE*sizeof(char));

	printf("$ ");
	scanf("%s", input);

	return input;
}

char** parse_command(char* command) {
	int i, j;
	char** parsed_command;

	/* primeiro: alocar espaço pro comando parseado */
	parsed_command = malloc(MAX_ARGS * sizeof(char *));

	for (i = 0; i < MAX_ARGS; i++) {
		parsed_command[i] = malloc(MAX_ARG_SIZE * sizeof(char));
	}

	/* segundo: parsear o comando */
	for (i = 0; i < MAX_ARGS; i++) {
		for (j = 0; (j < MAX_COMMAND_SIZE) && (command[j] != ' ') && (command[j] != '\0'); j++) {
			parsed_command[i][j] = command[j];
		}
		/* precisamos colocar um \0 no final para indicar fim de string */
		parsed_command[i][j] = '\0';
	}

	return parsed_command;
}

void open_process(char* process, char** args) {
	return;
}

void exec_command(char* command, char** args) {
	return;
}