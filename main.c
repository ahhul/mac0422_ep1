#include "aux.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
	pid_t child_pid;
	int i;
	char* command = NULL;
	char** parsed_command;

	while(i < 5) {
		/* ler comando */
		command = prompt_terminal();

		/* parsear comando em "programa" e "argumentos" */
		parsed_command = parse_command(command);

		/* melhor dar free nas coisas pra não estourar memória... */
		free(command);
		free(parsed_command);
	}
	return 0;
}