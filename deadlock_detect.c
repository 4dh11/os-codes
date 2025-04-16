#include <stdio.h>
#include <stdbool.h>

#define PROCESSES 5
#define RESOURCES 3

// Function to detect deadlock
bool detectDeadlock(int allocation[PROCESSES][RESOURCES], 
                    int request[PROCESSES][RESOURCES], 
                    int available[RESOURCES]) 
{
    int work[RESOURCES];
    bool finish[PROCESSES] = {false};
    int i, j, k;
    bool deadlock = false;
    bool found;

    // Initialize work with available resources
    for (i = 0; i < RESOURCES; i++)
        work[i] = available[i];

    // Find processes that can finish
    for (k = 0; k < PROCESSES; k++) {
        found = false;
        for (i = 0; i < PROCESSES; i++) {
            if (!finish[i]) {
                bool canExecute = true;
                for (j = 0; j < RESOURCES; j++) {
                    if (request[i][j] > work[j]) {
                        canExecute = false;
                        break;
                    }
                }

                if (canExecute) {
                    // Process can execute, release its resources
                    for (j = 0; j < RESOURCES; j++)
                        work[j] += allocation[i][j];
                    finish[i] = true;
                    found = true;
                }
            }
        }
        if (!found) break;
    }

    // Check for deadlock
    deadlock = false;
    for (i = 0; i < PROCESSES; i++) {
        if (!finish[i]) {
            printf("Process P%d is deadlocked\n", i);
            deadlock = true;
        }
    }

    return deadlock;
}

int main() {
    // Sample data structures
    int allocation[PROCESSES][RESOURCES] = {
        {0, 1, 0},  // P0
        {2, 0, 0},  // P1
        {3, 0, 3},  // P2
        {2, 1, 1},  // P3
        {0, 0, 2}   // P4
    };

    int request[PROCESSES][RESOURCES] = {
        {0, 0, 0},  // P0
        {2, 0, 2},  // P1
        {0, 0, 0},  // P2
        {1, 0, 0},  // P3
        {0, 0, 2}   // P4
    };

    int available[RESOURCES] = {0, 0, 0};  // Available resources

    printf("Deadlock Detection Algorithm\n");
    printf("----------------------------\n");

    if (detectDeadlock(allocation, request, available)) {
        printf("\nDeadlock detected in the system!\n");
    } else {
        printf("\nNo deadlock detected.\n");
    }

    return 0;
}