#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

#define MAX_ARGUMENTS 200
#define MAX_PIPES 10

int readcommand(char *command) {
    if (fgets(command, 20000, stdin) == NULL) {
        return 0;
    }
    return 1;
}

void parsecmd(char *command, char **argument, int *numarg) {
    *numarg = 0;
    char *word = strtok(command, " \n");
    while (word != NULL && *numarg < MAX_ARGUMENTS) {
        argument[(*numarg)++] = word;
        word = strtok(NULL, " \n");
    }
    argument[*numarg] = NULL;
}

void execute_piped_commands(int num_of_pipes, char *pipe_commands[MAX_PIPES]) {
    int pipes[MAX_PIPES][2];
    pid_t pids[MAX_PIPES];

    for (int i = 0; i < num_of_pipes; i++) {
        if (pipe(pipes[i]) < 0) {
            perror("Pipe creation failed");
            exit(1);
        }

        pids[i] = fork();
        if (pids[i] < 0) {
            perror("Fork failed");
            exit(1);
        } else if (pids[i] == 0) {
            if (i > 0) {
                dup2(pipes[i - 1][0], 0); // Redirect stdin from the previous pipe
                close(pipes[i - 1][0]);
            }

            if (i < num_of_pipes - 1) {
                dup2(pipes[i][1], 1); // Redirect stdout to the current pipe
                close(pipes[i][1]);
            }

            for (int j = 0; j < num_of_pipes; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            char *cmd_args[MAX_ARGUMENTS];
            parsecmd(pipe_commands[i], cmd_args, &num_of_pipes);
            if (execvp(cmd_args[0], cmd_args) < 0) {
                perror("Exec failed");
                exit(1);
            }
        }
    }

    // Close all pipes in the parent process
    for (int i = 0; i < num_of_pipes; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for child processes to finish
    for (int i = 0; i < num_of_pipes; i++) {
        wait(NULL);
    }
}

int checkpipe(char *command) {
    int length = strlen(command);
    for (int i = 0; i < length; i++) {
        if (command[i] == '|') {
            return 1; // Return 1 to indicate a pipe is found
        }
    }
    return 0; // Return 0 if no pipe is found
}

int parsepipe(char *command, char *pipe_commands[MAX_PIPES]) {
    int num = 0;
    char *cmds = strtok(command, "|");
    while ((cmds != NULL) && num < MAX_PIPES) {
        pipe_commands[num++] = cmds;
        cmds = strtok(NULL, "|");
    }
    return num;
}

int main() {
    char command[20000];
    char *argument[MAX_ARGUMENTS];
    char *pipe_commands[MAX_PIPES];
    int numarg;
    int num_of_pipes;

    while (true) {
        printf("\nJarvis@system:~$ ");
        if (readcommand(command) == 0) {
            break;
        }

        int pipecheck = checkpipe(command);
        if (pipecheck) {
            num_of_pipes = parsepipe(command, pipe_commands);
            execute_piped_commands(num_of_pipes, pipe_commands);
        } else {
            parsecmd(command, argument, &numarg);
            pid_t pid = fork();
            if (pid < 0) {
                perror("Fork failed");
                exit(1);
            } else if (pid == 0) {
                if (execvp(argument[0], argument) < 0) {
                    perror("Exec failed");
                    exit(1);
                }
            } else {
                wait(NULL);
            }
        }
    }
    return 0;
}
