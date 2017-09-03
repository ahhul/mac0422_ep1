#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <grp.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "aux.h"

int main(int argc, char* argv[]) {
	struct timeval *crude_time = mallocc(sizeof(struct timeval));
	struct group *grp = NULL;
	pid_t child_pid;
	int err, status;
	char *grpname, *date_str, *command = NULL, dir[300] = "", prompt[310] = "";
	char** parsed_command = NULL;

	/* inicializa o history e o readline */
	rl_initialize();
	using_history();
	while(1) {
		/* limpa o prompt */
		strcpy(prompt, "");
		/* pega o diretorio atual e monta o prompt novo*/
		getcwd(dir, sizeof(dir));
		strcat(prompt, "[");
		strcat(prompt, dir);
		strcat(prompt, "]$ ");
		/* le comando e salva no historico */
		command = readline(prompt);
		add_history(command);

		parsed_command = parse_command(command);

		/* sai do shell, desalocando a memoria alocada */
		if (strcmp(parsed_command[0], "exit") == 0) {
			free(parsed_command);
			free(command);
			free(crude_time);
			clear_history();
			break;
		}

		/* comando interno "chown :<gname> <nome do arquivo>" */
		else if (strcmp(parsed_command[0], "chown") == 0) {
			/* tira o ":" do nome do grupo*/
			grpname = strtok(parsed_command[1], ":");

			/* Procura por informações sobre o grupo de nome "gname".
			   Usaremos o GID (group ID) contido na estrutura devolvida pela função */
			grp = getgrnam(grpname);
			if (grp == NULL) {
				printf("chown: nome de grupo não encontrado.\n");
			}

			err = chown(parsed_command[2], -1, grp->gr_gid);
			if (err == -1) {
				printf("chown: Operação cancelada.\n");
			}
			free(parsed_command);
			free(command);
		}

		/* comando interno "date" */
		else if (strcmp(parsed_command[0], "date") == 0) {
			date_str = mallocc(50 * sizeof(char));

			/* chamada de sistema gettimeofday() para pegar o tempo do sistema*/
			gettimeofday(crude_time, NULL);

			/* Formata os dados da estrutura "crude_time" do jeito que queremos,
			usando o campo "tv_sec" e formatando para o horario local */
			strftime(date_str, 50, "%a %b %d %X %Z %Y\n", localtime(&(crude_time->tv_sec)));

			printf("%s", date_str);

			free(parsed_command);
			free(date_str);
			free(command);
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
			free(parsed_command);
			free(command);
		}
	}

	return 0;
}