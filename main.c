#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "aux.h"

int main(int argc, char* argv[]) {
	struct timeval *crude_time =  mallocc(sizeof(struct timeval));
	pid_t child_pid;
	int err, status;
	char *gname, *date_str, *command = NULL;
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
			free(crude_time);
			break;
		}

		/* comando "chown :<gid> <nome do arquivo>" */
		else if (strcmp(parsed_command[0], "chown") == 0) {
			/* tira o ":" do group ID */
			gname = strtok(parsed_command[1], ":");
			err = chown(parsed_command[3], -1, atoi(gname));

			if (err == -1) {
				printf("chown: Operação cancelada.\n");
			}
			free(command);
			free(parsed_command);
		}

		else if (strcmp(parsed_command[0], "date") == 0) {
			date_str = mallocc(50 * sizeof(char));

			/* chamada de sistema gettimeofday() para pegar o tempo do sistema*/
			gettimeofday(crude_time, NULL);

			/* Formata os dados da estrutura "crude_time" do jeito que queremos,
			usando o campo "tv_sec" e formatando para o horario local */
			strftime(date_str, 50, "%a %b %d %X %Z %Y\n", localtime(&(crude_time->tv_sec)));

			printf("%s", date_str);

			free(command);
			free(parsed_command);
			free(date_str);
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