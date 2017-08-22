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
	char** parsed_command;

	while(1) {
		command = prompt_terminal();
		printf("%s\n", command);

		free(command);
	}
	return 0;
}