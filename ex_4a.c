#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

int main(void) {
    int pipefd[2];
    pid_t pid;
    char buffer[100];

    // Create pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork process
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // Child process (reader)
        close(pipefd[1]);  // Close write end
        
        // Read from pipe
        while (read(pipefd[0], buffer, sizeof(buffer)) > 0) {
            printf("Child received: %s", buffer);
        }
        
        close(pipefd[0]);
    }

    else {  // Parent process (writer)
        close(pipefd[0]);  // Close read end
        
        // Write to pipe
        char *messages[] = {"hello, world!\n", "goodbye, world!\n"};
        for (int i = 0; i < 2; i++) {
            write(pipefd[1], messages[i], strlen(messages[i]));
        }
        
        close(pipefd[1]);
    }

    return EXIT_SUCCESS;
}