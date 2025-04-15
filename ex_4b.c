#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    // Create a new process
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        printf("Child process (PID: %d) started\n", getpid());
        // Simulate some work
        sleep(2);
        printf("Child process (PID: %d) finished\n", getpid());
        // Exit without being waited for by parent
        exit(0);
    } else {
        // Parent process
        printf("Parent process (PID: %d) started\n", getpid());
        // Don't wait for child process to finish
        // This will create a zombie process
        // sleep(3); // Uncomment this line to prevent zombie process
        printf("Parent process (PID: %d) finished\n", getpid());
        // Exit
        exit(0);
    }
    return 0;
}