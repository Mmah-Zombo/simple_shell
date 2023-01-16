#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define PROMPT "cisfun$ "

int main() {
    char command[256];

    while (1) {
        printf("%s", PROMPT);
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0; // remove newline character

        pid_t pid = fork();
        if (pid == 0) { // child process
            if (execve(command, NULL, NULL) == -1) {
                printf("%s: %s\n", command, "No such file or directory");
                exit(1);
            }
        } else if (pid > 0) { // parent process
            int status;
            wait(&status);
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                printf("%s: %s\n", command, "Error");
            }
        } else { // fork failed
            perror("fork");
            exit(1);
        }
    }

    return 0;
}
