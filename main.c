#include "aux.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
	pid_t child_pid;
	int i, j;
	int status;
	char* command = NULL;
	char** parsed_command = NULL;

	while(1) {
		/* ler comando */
		command = prompt_terminal();

		/* parsear comando em "programa" e "argumentos" */
		parsed_command = parse_command(command);

		if (strcmp(parsed_command[0], "exit") == 0) {
			free(command);
			free(parsed_command);
			exit(0);
		}

		/* código do processo filho */
		else if ((child_pid = fork()) == 0) {
			execve(parsed_command[0], parsed_command, 0);
			free(command);
			free(parsed_command);
		}
		/* código do processo pai */
		else {
			waitpid(-1, &status, 0);
			free(command);
			free(parsed_command);
		}
	}
	return 0;
}