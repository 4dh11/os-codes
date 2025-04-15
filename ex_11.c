#include <stdio.h>

// Function prototypes
void fifo(int pages[], int n, int frames);
void lru(int pages[], int n, int frames);
void optimal(int pages[], int n, int frames);

int main() {
    // Test case - standard reference string
    int pages[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    int n = sizeof(pages) / sizeof(pages[0]);
    int frames = 3; // Number of memory frames
    
    printf("Page Replacement Algorithms Simulation\n");
    printf("Reference String: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", pages[i]);
    }
    printf("\nNumber of Frames: %d\n\n", frames);
    
    // Run all algorithms
    fifo(pages, n, frames);
    lru(pages, n, frames);
    optimal(pages, n, frames);
    
    return 0;
}

// FIFO (First-In-First-Out) Page Replacement
void fifo(int pages[], int n, int frames) {
    int memory[frames], time[frames];
    int page_faults = 0, ptr = 0;
    
    // Initialize memory and time arrays
    for (int i = 0; i < frames; i++) {
        memory[i] = -1; // -1 indicates empty frame
        time[i] = 0;
    }

    printf("\nFIFO Page Replacement:\n");
    
    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int found = 0;
        
        // Check if page is already in memory
        for (int j = 0; j < frames; j++) {
            if (memory[j] == page) {
                found = 1;
                break;
            }
        }
        
        if (!found) {
            // Replace the oldest page (FIFO)
            memory[ptr] = page;
            ptr = (ptr + 1) % frames; // Circular pointer
            page_faults++;
            
            // Display current memory state
            printf("Page %d -> [", page);
            for (int j = 0; j < frames; j++) {
                if (memory[j] == -1) printf(" -");
                else printf(" %d", memory[j]);
            }
            printf(" ] (Fault)\n");
        } else {
            printf("Page %d -> [Hit]\n", page);
        }
    }
    
    printf("Total Page Faults: %d\n", page_faults);
}

// LRU (Least Recently Used) Page Replacement
void lru(int pages[], int n, int frames) {
    int memory[frames], counter[frames];
    int page_faults = 0;
    
    // Initialize memory and counter arrays
    for (int i = 0; i < frames; i++) {
        memory[i] = -1;
        counter[i] = 0;
    }

    printf("\nLRU Page Replacement:\n");
    
    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int found = 0;
        
        // Check if page is already in memory
        for (int j = 0; j < frames; j++) {
            if (memory[j] == page) {
                found = 1;
                counter[j] = i + 1; // Update last used time
                break;
            }
        }
        
        if (!found) {
            // Find LRU page to replace
            int lru_index = 0;
            for (int j = 1; j < frames; j++) {
                if (counter[j] < counter[lru_index]) {
                    lru_index = j;
                }
            }
            
            memory[lru_index] = page;
            counter[lru_index] = i + 1; // Set to current time
            page_faults++;
            
            // Display current memory state
            printf("Page %d -> [", page);
            for (int j = 0; j < frames; j++) {
                if (memory[j] == -1) printf(" -");
                else printf(" %d", memory[j]);
            }
            printf(" ] (Fault)\n");
        } else {
            printf("Page %d -> [Hit]\n", page);
        }
    }
    
    printf("Total Page Faults: %d\n", page_faults);
}

// Optimal Page Replacement
void optimal(int pages[], int n, int frames) {
    int memory[frames];
    int page_faults = 0;
    
    // Initialize memory array
    for (int i = 0; i < frames; i++) {
        memory[i] = -1;
    }

    printf("\nOptimal Page Replacement:\n");
    
    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int found = 0;
        
        // Check if page is already in memory
        for (int j = 0; j < frames; j++) {
            if (memory[j] == page) {
                found = 1;
                break;
            }
        }
        
        if (!found) {
            // Find page to replace (optimal)
            int replace_index = -1, farthest = i;
            for (int j = 0; j < frames; j++) {
                int k;
                for (k = i + 1; k < n; k++) {
                    if (memory[j] == pages[k]) {
                        if (k > farthest) {
                            farthest = k;
                            replace_index = j;
                        }
                        break;
                    }
                }
                // If page not found in future, it's the best to replace
                if (k == n) {
                    replace_index = j;
                    break;
                }
            }
            
            // If all pages are used in future, replace the first one
            if (replace_index == -1) replace_index = 0;
            
            memory[replace_index] = page;
            page_faults++;
            
            // Display current memory state
            printf("Page %d -> [", page);
            for (int j = 0; j < frames; j++) {
                if (memory[j] == -1) printf(" -");
                else printf(" %d", memory[j]);
            }
            printf(" ] (Fault)\n");
        } else {
            printf("Page %d -> [Hit]\n", page);
        }
    }
    
    printf("Total Page Faults: %d\n", page_faults);
}