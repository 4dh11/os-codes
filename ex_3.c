#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define SHM_KEY 1234
#define SHM_SIZE 1024

int main() {
    // Create shared memory
    int shmid = shmget(SHM_KEY, SHM_SIZE, 0666|IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach shared memory
    char *shm = shmat(shmid, NULL, 0);
    if (shm == (char*)-1) {
        perror("shmat");
        exit(1);
    }

    if (fork() == 0) { // Child process (reader)
        sleep(1); // Wait for parent to write
        printf("Child read: %s\n", shm);
        shmdt(shm);
    } 
    else { // Parent process (writer)
        strcpy(shm, "Hello from parent!");
        printf("Parent wrote to shared memory\n");
        wait(NULL); // Wait for child to finish
        shmdt(shm);
        shmctl(shmid, IPC_RMID, NULL); // Clean up
    }

    return 0;
}