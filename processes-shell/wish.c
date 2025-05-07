#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // fork, execv, execvp, access, chdir
#include <sys/types.h>  // pid_t
#include <sys/wait.h>   // wait, waitpid
const int MAX_ARGS = 100;
const char *PATHS[] = {"/bin/ls", "/usr/bin/ls"};
int main() {
    char *line = NULL;
    size_t len = 0;

    while (1) {
        printf("wish> ");
        fflush(stdout);

        ssize_t nread = getline(&line, &len, stdin);
        if (nread == -1) {
            break;  // EOF or error
        }

        // Remove newline character if present
        if (line[nread - 1] == '\n') {
            line[nread - 1] = '\0';
        }

        // Skip empty input
        if (strlen(line) == 0) {
            continue;
        }
        // Parse line into tokens
        char *args[MAX_ARGS];
        int argc = 0;
        char *token;
        char *rest = line;

        while ((token = strsep(&rest, " \t")) != NULL) {
            if (*token == '\0') continue;  // skip empty tokens (multiple spaces)
            args[argc++] = token;
            if (argc >= MAX_ARGS - 1) break;
        }
        args[argc] = NULL;  // Null-terminate the array

        if(strcmp(args[0],"ls") == 0){
            int status;
            pid_t pid = fork();
            if(pid==0){

            for(int i =0; i<2; i++) {
                   if(access(PATHS[i], X_OK)==0){
                        execv(PATHS[i],args);
                   }
                }
            }
            else {
                wait(&status);
            }
        }
    }

    free(line);
    return 0;
}
