#include "aux.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
	pid_t child_pid;
	int err;
	int status;
	char* command = NULL;
	char* gid;
	char** parsed_command = NULL;

	while(1) {
		/* ler comando */
		command = prompt_terminal();

		/* parsear comando em "programa" e "argumentos" */
		parsed_command = parse_command(command);

		/* sai do shell */
		if (strcmp(parsed_command[0], "exit") == 0) {
			free(command);
			free(parsed_command);
			exit(0);
		}

		/* comando "chown :<gid> <nome do arquivo>" */
		else if (strcmp(parsed_command[0], "chown") == 0) {
			/* tira o ":" do group ID */
			gid = strtok(parsed_command[1], ":");
			err = chown(parsed_command[3], -1, atoi(gid));

			if (err == -1) {
				printf("chown deu ruim\n");
			}
			free(command);
			free(parsed_command);
		}

		else if (strcmp(parsed_command[0], "date") == 0) {

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