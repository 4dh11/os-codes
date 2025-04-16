#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5 // Number of philosophers

// Shared structures for both implementations
enum { THINKING, HUNGRY, EATING };

// Semaphore implementation
void semaphore_solution();
void *philosopher_sem(void *num);
sem_t chopstick_sem[N];
sem_t mutex_sem;
int state_sem[N];

// Monitor implementation
void monitor_solution();
void *philosopher_mon(void *num);
void pickup(int phil);
void putdown(int phil);
pthread_mutex_t monitor;
pthread_cond_t cond[N];
int state_mon[N];

int main() {
    int choice;
    
    printf("Dining Philosophers Problem Solutions\n");
    printf("1. Semaphore Solution\n");
    printf("2. Monitor Solution\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &choice);
    
    switch(choice) {
        case 1:
            semaphore_solution();
            break;
        case 2:
            monitor_solution();
            break;
        default:
            printf("Invalid choice\n");
    }
    
    return 0;
}

/********************** SEMAPHORE SOLUTION **********************/

void semaphore_solution() {
    pthread_t thread[N];
    int phil_num[N];
    
    sem_init(&mutex_sem, 0, 1);
    
    for(int i = 0; i < N; i++) {
        sem_init(&chopstick_sem[i], 0, 1);
        state_sem[i] = THINKING;
        phil_num[i] = i;
    }
    
    printf("\nStarting Semaphore Solution...\n");
    
    for(int i = 0; i < N; i++) {
        pthread_create(&thread[i], NULL, philosopher_sem, &phil_num[i]);
    }
    
    for(int i = 0; i < N; i++) {
        pthread_join(thread[i], NULL);
    }
}

void *philosopher_sem(void *num) {
    int phil = *(int *)num;
    
    while(1) {
        printf("Philosopher %d is thinking\n", phil);
        sleep(1);
        
        sem_wait(&mutex_sem);
        state_sem[phil] = HUNGRY;
        printf("Philosopher %d is hungry\n", phil);
        
        // Try to acquire chopsticks
        sem_wait(&chopstick_sem[phil]);
        sem_wait(&chopstick_sem[(phil+1)%N]);
        
        state_sem[phil] = EATING;
        printf("Philosopher %d is eating\n", phil);
        sleep(2);
        
        sem_post(&chopstick_sem[phil]);
        sem_post(&chopstick_sem[(phil+1)%N]);
        
        state_sem[phil] = THINKING;
        sem_post(&mutex_sem);
    }
}

/********************** MONITOR SOLUTION **********************/

void monitor_solution() {
    pthread_t thread[N];
    int phil_num[N];
    
    pthread_mutex_init(&monitor, NULL);
    
    for(int i = 0; i < N; i++) {
        pthread_cond_init(&cond[i], NULL);
        state_mon[i] = THINKING;
        phil_num[i] = i;
    }
    
    printf("\nStarting Monitor Solution...\n");
    
    for(int i = 0; i < N; i++) {
        pthread_create(&thread[i], NULL, philosopher_mon, &phil_num[i]);
    }
    
    for(int i = 0; i < N; i++) {
        pthread_join(thread[i], NULL);
    }
}

void *philosopher_mon(void *num) {
    int phil = *(int *)num;
    
    while(1) {
        sleep(1);
        pickup(phil);
        sleep(1);
        putdown(phil);
    }
}

void pickup(int phil) {
    pthread_mutex_lock(&monitor);
    state_mon[phil] = HUNGRY;
    printf("Philosopher %d is hungry\n", phil);
    
    while(state_mon[(phil+1)%N] == EATING || state_mon[(phil+N-1)%N] == EATING) {
        pthread_cond_wait(&cond[phil], &monitor);
    }
    
    state_mon[phil] = EATING;
    printf("Philosopher %d is eating\n", phil);
    pthread_mutex_unlock(&monitor);
}

void putdown(int phil) {
    pthread_mutex_lock(&monitor);
    state_mon[phil] = THINKING;
    printf("Philosopher %d is thinking\n", phil);
    
    pthread_cond_signal(&cond[(phil+1)%N]);
    pthread_cond_signal(&cond[(phil+N-1)%N]);
    pthread_mutex_unlock(&monitor);
}