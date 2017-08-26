#ifndef AUX_HEADER
#define AUX_HEADER

#include <stdlib.h>

#define MAX_COMMAND_SIZE 200
#define MAX_ARGS 10

void* mallocc(size_t nbytes);
char* prompt_terminal();
char** parse_command(char* command);


#endif