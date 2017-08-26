#include "aux.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
	pid_t child_pid;
	int i, j;
	char* command = NULL;
	char* command_cpy = NULL;
	char** parsed_command = NULL;

	while(1) {
		/* ler comando */
		command = prompt_terminal();

		/* parsear comando em "programa" e "argumentos" */
		parsed_command = parse_command(command);

		for (i = 0; parsed_command[i] != NULL ; i++) {
			printf("%s\n", parsed_command[i]);
		}

		/* melhor dar free nas coisas pra não estourar memória... */
		free(command);
		free(parsed_command);
	}
	return 0;
}