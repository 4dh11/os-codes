//System basic calls: open, read, write, lseek, fork, execvp, wait
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    pid_t pid;
    int fd, status;
    char buffer[100];

    // Get current process ID
    printf("Parent PID: %d\n", getpid());

    // Open a file (or create if it doesn't exist)
    fd = open("example.txt", O_CREAT | O_RDWR, 0644);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Write to the file
    char *msg = "Hello, System Calls!\n";
    if (write(fd, msg, strlen(msg)) == -1) {
        perror("write");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Move file pointer to the beginning
    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("lseek");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Read from the file
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
    if (bytes_read == -1) {
        perror("read");
        close(fd);
        exit(EXIT_FAILURE);
    }
    buffer[bytes_read] = '\0';
    printf("File content: %s", buffer);

    // Close the file
    close(fd);

    // Fork a child process
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        printf("Child PID: %d\n", getpid());
        sleep(2); // Sleep for 2 seconds

        // Execute 'ls' command
        char *args[] = {"ls", "-l", NULL};
        if (execvp("ls", args) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else { // Parent process
        printf("Parent waiting for child...\n");
        wait(&status); // Wait for child to finish
        printf("Child exited with status: %d\n", WEXITSTATUS(status));
    }

    return 0;
}
