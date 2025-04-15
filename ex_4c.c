#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        printf("Child process (PID: %d) started\n", getpid());
        sleep(10); // Simulate some work
    } else {
        // Parent process
        printf("Parent process (PID: %d) started\n", getpid());
        exit(0); // Terminate parent process
    }
    return 0;
}