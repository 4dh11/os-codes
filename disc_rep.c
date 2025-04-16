#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

// Function prototypes
void fcfs(int requests[], int n, int head);
void sstf(int requests[], int n, int head);
void scan(int requests[], int n, int head, int direction, int disk_size);
void cscan(int requests[], int n, int head, int direction, int disk_size);
void sort(int arr[], int n);

int main() {
    
    int n = 8;
    int requests[] = {98, 183, 37, 122, 14, 124, 65, 67};
    int head = 53;
    int disk_size = 200;
    int direction = 1; // 1 for right, 0 for left
    
    printf("\nFCFS:\n");
    fcfs(requests, n, head);
    
    printf("\nSSTF:\n");
    sstf(requests, n, head);
    
    printf("\nSCAN:\n");
    scan(requests, n, head, direction, disk_size);
    
    printf("\nC-SCAN:\n");
    cscan(requests, n, head, direction, disk_size);
    
    return 0;
}

// First Come First Serve (FCFS) algorithm
void fcfs(int requests[], int n, int head) {
    int total_seek = 0;
    int current = head;
    
    printf("Seek Sequence: %d", current);
    
    for(int i = 0; i < n; i++) {
        total_seek += abs(current - requests[i]);
        current = requests[i];
        printf(" -> %d", current);
    }
    
    printf("\nTotal seek time: %d\n", total_seek);
}

// Shortest Seek Time First (SSTF) algorithm
void sstf(int requests[], int n, int head) {
    int total_seek = 0;
    int current = head;
    int visited[n];
    
    for(int i = 0; i < n; i++) {
        visited[i] = 0;
    }
    
    printf("Seek Sequence: %d", current);
    
    for(int i = 0; i < n; i++) {
        int min_dist = INT_MAX;
        int min_index = -1;
        
        for(int j = 0; j < n; j++) {
            if(!visited[j]) {
                int dist = abs(current - requests[j]);
                if(dist < min_dist) {
                    min_dist = dist;
                    min_index = j;
                }
            }
        }
        
        if(min_index != -1) {
            visited[min_index] = 1;
            total_seek += min_dist;
            current = requests[min_index];
            printf(" -> %d", current);
        }
    }
    
    printf("\nTotal seek time: %d\n", total_seek);
}

// SCAN (Elevator) algorithm
void scan(int requests[], int n, int head, int direction, int disk_size) {
    int total_seek = 0;
    int current = head;
    int sorted[n];
    
    for(int i = 0; i < n; i++) {
        sorted[i] = requests[i];
    }
    sort(sorted, n);
    
    printf("Seek Sequence: %d", current);
    
    if(direction == 1) { // Moving right
        // Serve requests on the right of head
        for(int i = 0; i < n; i++) {
            if(sorted[i] >= current) {
                total_seek += abs(current - sorted[i]);
                current = sorted[i];
                printf(" -> %d", current);
            }
        }
        // Go to end
        total_seek += abs(current - (disk_size - 1));
        current = disk_size - 1;
        printf(" -> %d", current);
        // Serve requests on the left
        for(int i = n - 1; i >= 0; i--) {
            if(sorted[i] < head) {
                total_seek += abs(current - sorted[i]);
                current = sorted[i];
                printf(" -> %d", current);
            }
        }
    } else { // Moving left
        // Serve requests on the left of head
        for(int i = n - 1; i >= 0; i--) {
            if(sorted[i] <= current) {
                total_seek += abs(current - sorted[i]);
                current = sorted[i];
                printf(" -> %d", current);
            }
        }
        // Go to start
        total_seek += abs(current - 0);
        current = 0;
        printf(" -> %d", current);
        // Serve requests on the right
        for(int i = 0; i < n; i++) {
            if(sorted[i] > head) {
                total_seek += abs(current - sorted[i]);
                current = sorted[i];
                printf(" -> %d", current);
            }
        }
    }
    
    printf("\nTotal seek time: %d\n", total_seek);
}

// Circular SCAN (C-SCAN) algorithm
void cscan(int requests[], int n, int head, int direction, int disk_size) {
    int total_seek = 0;
    int current = head;
    int sorted[n];
    
    for(int i = 0; i < n; i++) {
        sorted[i] = requests[i];
    }
    sort(sorted, n);
    
    printf("Seek Sequence: %d", current);
    
    if(direction == 1) { // Moving right
        // Serve requests on the right of head
        for(int i = 0; i < n; i++) {
            if(sorted[i] >= current) {
                total_seek += abs(current - sorted[i]);
                current = sorted[i];
                printf(" -> %d", current);
            }
        }
        // Go to end and then to start
        total_seek += abs(current - (disk_size - 1)) + (disk_size - 1);
        printf(" -> %d -> 0", disk_size - 1);
        current = 0;
        // Serve requests on the left
        for(int i = 0; i < n; i++) {
            if(sorted[i] < head) {
                total_seek += abs(current - sorted[i]);
                current = sorted[i];
                printf(" -> %d", current);
            }
        }
    } else { // Moving left
        // Serve requests on the left of head
        for(int i = n - 1; i >= 0; i--) {
            if(sorted[i] <= current) {
                total_seek += abs(current - sorted[i]);
                current = sorted[i];
                printf(" -> %d", current);
            }
        }
        // Go to start and then to end
        total_seek += abs(current - 0) + (disk_size - 1);
        printf(" -> 0 -> %d", disk_size - 1);
        current = disk_size - 1;
        // Serve requests on the right
        for(int i = n - 1; i >= 0; i--) {
            if(sorted[i] > head) {
                total_seek += abs(current - sorted[i]);
                current = sorted[i];
                printf(" -> %d", current);
            }
        }
    }
    
    printf("\nTotal seek time: %d\n", total_seek);
}

// Helper function to sort array
void sort(int arr[], int n) {
    for(int i = 0; i < n - 1; i++) {
        for(int j = 0; j < n - i - 1; j++) {
            if(arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}