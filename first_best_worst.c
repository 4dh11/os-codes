#include <stdio.h>

// Function prototypes
void firstFit(int blockSize[], int m, int processSize[], int n);
void bestFit(int blockSize[], int m, int processSize[], int n);
void worstFit(int blockSize[], int m, int processSize[], int n);

int main() {
    int blockSize[] = {100, 500, 200, 300, 600};
    int processSize[] = {212, 417, 112, 426};
    int m = sizeof(blockSize)/sizeof(blockSize[0]);
    int n = sizeof(processSize)/sizeof(processSize[0]);

    printf("Memory Allocation Strategies:\n");
    
    printf("\nFirst Fit:\n");
    firstFit(blockSize, m, processSize, n);
    
    printf("\nBest Fit:\n");
    bestFit(blockSize, m, processSize, n);
    
    printf("\nWorst Fit:\n");
    worstFit(blockSize, m, processSize, n);
    
    return 0;
}

// First Fit Allocation
void firstFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n];
    
    // Initially no block is assigned to any process
    for (int i = 0; i < n; i++)
        allocation[i] = -1;
    
    // Pick each process and find first suitable block
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                // Allocate block j to process i
                allocation[i] = j;
                // Reduce available memory in this block
                blockSize[j] -= processSize[i];
                break;
            }
        }
    }
    
    // Print allocation results
    printf("Process No.\tProcess Size\tBlock no.\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t", i+1, processSize[i]);
        if (allocation[i] != -1)
            printf("%d\n", allocation[i]+1);
        else
            printf("Not Allocated\n");
    }
}

// Best Fit Allocation
void bestFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n];
    
    // Initially no block is assigned to any process
    for (int i = 0; i < n; i++)
        allocation[i] = -1;
    
    // Pick each process and find best suitable block
    for (int i = 0; i < n; i++) {
        int bestIdx = -1;
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (bestIdx == -1 || blockSize[j] < blockSize[bestIdx])
                    bestIdx = j;
            }
        }
        
        if (bestIdx != -1) {
            allocation[i] = bestIdx;
            blockSize[bestIdx] -= processSize[i];
        }
    }
    
    // Print allocation results
    printf("Process No.\tProcess Size\tBlock no.\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t", i+1, processSize[i]);
        if (allocation[i] != -1)
            printf("%d\n", allocation[i]+1);
        else
            printf("Not Allocated\n");
    }
}

// Worst Fit Allocation
void worstFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n];
    
    // Initially no block is assigned to any process
    for (int i = 0; i < n; i++)
        allocation[i] = -1;
    
    // Pick each process and find worst suitable block
    for (int i = 0; i < n; i++) {
        int worstIdx = -1;
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (worstIdx == -1 || blockSize[j] > blockSize[worstIdx])
                    worstIdx = j;
            }
        }
        
        if (worstIdx != -1) {
            allocation[i] = worstIdx;
            blockSize[worstIdx] -= processSize[i];
        }
    }
    
    // Print allocation results
    printf("Process No.\tProcess Size\tBlock no.\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t", i+1, processSize[i]);
        if (allocation[i] != -1)
            printf("%d\n", allocation[i]+1);
        else
            printf("Not Allocated\n");
    }
}