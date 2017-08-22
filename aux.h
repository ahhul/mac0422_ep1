#ifndef AUX_HEADER
#define AUX_HEADER

#define MAX_COMMAND_SIZE 200
#define MAX_ARGS 5
#define MAX_ARG_SIZE 100

char* prompt_terminal();
char** parse_command(char* command);
void open_process(char* process, char** args);
void exec_command(char* command, char** args);


#endif