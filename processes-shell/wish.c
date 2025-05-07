#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // fork, execv, execvp, access, chdir
#include <sys/types.h>  // pid_t
#include <sys/wait.h>   // wait, waitpid
const int MAX_ARGS = 100;
const int MAX_COMMANDS = 100;

#define MAX_ARGS 10
#define MAX_COMMANDS 10

// Function to trim newline and extra spaces from a string
void trim_newline(char *str) {
    char *end = str + strlen(str) - 1;
    while (end >= str && (*end == '\n' || *end == '\r')) {
        *end = '\0';
        end--;
    }
}

// Function to parse the input line into a 2D array of arguments
int parse_line(char *line, char *commands[MAX_COMMANDS][MAX_ARGS]) {
    int command_count = 0;
    char *command_token;
    char *rest = line;

    // Trim any newline character at the end of the line
    trim_newline(line);

    // First, split the line into commands using '&' as the delimiter
    while ((command_token = strsep(&rest, "&")) != NULL) {
        if (*command_token == '\0') continue;  // Skip empty commands (if any)

        // Now split the command into arguments using space or tab as the delimiter
        int argc = 0;
        char *args[MAX_ARGS];
        char *arg_token;
        char *command_rest = command_token;

        while ((arg_token = strsep(&command_rest, " \t")) != NULL) {
            if (*arg_token == '\0') continue;  // Skip empty tokens

            // Trim any unwanted characters like '$' or newline within tokens
            trim_newline(arg_token);

            args[argc++] = arg_token;
            if (argc >= MAX_ARGS - 1) break;  // Don't exceed MAX_ARGS
        }
        args[argc] = NULL;  // Null-terminate the argument list

        // Store the arguments in the 2D array for this command
        for (int i = 0; i < argc; i++) {
            commands[command_count][i] = args[i];
        }
        commands[command_count][argc] = NULL;  // Null-terminate the command's arguments

        command_count++;
        if (command_count >= MAX_COMMANDS) break;  // Don't exceed MAX_COMMANDS
    }

    return command_count;  // Return the number of commands parsed
}

int main() {
    char *line = NULL;
    size_t len = 0;

    while (1) {
        printf("wish> ");
        fflush(stdout);

        ssize_t nread = getline(&line, &len, stdin);
        char *commands[MAX_COMMANDS][MAX_ARGS];  // 2D array to store commands and arguments
    
        int num_commands = parse_line(line, commands);
        int status;

        for(int i = 0; i < num_commands; i++){
            pid_t pid = fork();

            if (pid < 0) {
                // Fork failed
                perror("fork");
                exit(1);
            }
        
            if (pid == 0) {
                // Child process
                if (access("/bin/ls", X_OK) == 0) {
                    printf("executing %s %s\n", commands[i][0], commands[i][1] ? commands[i][1] : "");
                    fflush(stdout);
        
                    execv("/bin/ls", commands[i]);
        
                    // If execv returns, it failed
                    perror("execv");
                    exit(1);
                } else {
                    fprintf(stderr, "Error: Command not found or not executable: %s\n", "/bin/ls");
                    exit(1);
                }
            }

    }
    wait(&status);

    }

    free(line);
    return 0;
}
