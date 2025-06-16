#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 3

// Thread function
typedef void *(*THREAD_FUNC)(void *);

void *print_message(void *arg) {
    int thread_id = *(int *)arg;
    printf("Hello from thread %d!\n", thread_id);
    pthread_exit(NULL);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];  // Declare thread variables
    int thread_ids[NUM_THREADS];
    int result;

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        result = pthread_create(&threads[i], NULL,  (THREAD_FUNC)print_message, &thread_ids[i]);
        if (result) {
            printf("Error creating thread %d. Error code: %d\n", i + 1, result);
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All threads execution completed.\n");
    return 0;
}
