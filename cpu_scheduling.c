#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_PROCESSES 20

// Structure to hold process information
struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int completion_time;
    int waiting_time;
    int turnaround_time;
    int response_time;
    bool responded; // Flag to track first response
};

// Function prototypes
void fcfs();
void sjf();
void srtf();
void priority_scheduling();
void round_robin();
void print_results(struct Process processes[], int n);

int main() {
    int choice;
    
    while(1) {
        printf("\nCPU Scheduling Algorithms\n");
        printf("1. First-Come, First-Served (FCFS)\n");
        printf("2. Shortest Job First (SJF) - Non-Preemptive\n");
        printf("3. Shortest Remaining Time First (SRTF) - Preemptive\n");
        printf("4. Priority Scheduling - Non-Preemptive\n");
        printf("5. Round Robin (RR)\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: fcfs(); break;
            case 2: sjf(); break;
            case 3: srtf(); break;
            case 4: priority_scheduling(); break;
            case 5: round_robin(); break;
            case 6: return 0;
            default: printf("Invalid choice!\n");
        }
    }
}

void fcfs() {
    int n;
    struct Process processes[MAX_PROCESSES];
    float avg_wt = 0, avg_tat = 0, avg_rt = 0;
    
    printf("\nFCFS Scheduling\n");
    printf("Enter number of processes: ");
    scanf("%d", &n);
    
    printf("Enter arrival times and burst times:\n");
    for(int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        processes[i].responded = false;
        printf("P[%d]: ", i + 1);
        scanf("%d %d", &processes[i].arrival_time, &processes[i].burst_time);
    }
    
    // Sort processes by arrival time (FCFS)
    for(int i = 0; i < n - 1; i++) {
        for(int j = 0; j < n - i - 1; j++) {
            if(processes[j].arrival_time > processes[j + 1].arrival_time) {
                struct Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
    
    // Calculate metrics
    processes[0].completion_time = processes[0].arrival_time + processes[0].burst_time;
    processes[0].turnaround_time = processes[0].completion_time - processes[0].arrival_time;
    processes[0].waiting_time = 0;
    processes[0].response_time = 0; // FCFS response time = waiting time (0 for first process)
    avg_wt = processes[0].waiting_time;
    avg_tat = processes[0].turnaround_time;
    avg_rt = processes[0].response_time;
    
    for(int i = 1; i < n; i++) {
        processes[i].completion_time = processes[i - 1].completion_time + processes[i].burst_time;
        processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
        processes[i].response_time = processes[i].waiting_time; // For FCFS, response time = waiting time
        avg_wt += processes[i].waiting_time;
        avg_tat += processes[i].turnaround_time;
        avg_rt += processes[i].response_time;
    }
    
    print_results(processes, n);
    printf("\nAverage Waiting Time: %.2f", avg_wt / n);
    printf("\nAverage Turnaround Time: %.2f", avg_tat / n);
    printf("\nAverage Response Time: %.2f\n", avg_rt / n);
}

void sjf() {
    int n, current_time = 0, completed = 0;
    struct Process processes[MAX_PROCESSES];
    bool done[MAX_PROCESSES] = {false};
    float avg_wt = 0, avg_tat = 0, avg_rt = 0;
    
    printf("\nSJF Scheduling (Non-Preemptive)\n");
    printf("Enter number of processes: ");
    scanf("%d", &n);
    
    printf("Enter arrival times and burst times:\n");
    for(int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        processes[i].responded = false;
        printf("P[%d]: ", i + 1);
        scanf("%d %d", &processes[i].arrival_time, &processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time;
    }
    
    while(completed < n) {
        int shortest = -1;
        int min_burst = INT_MAX;
        
        // Find shortest job available at current time
        for(int i = 0; i < n; i++) {
            if(!done[i] && processes[i].arrival_time <= current_time && 
               processes[i].burst_time < min_burst) {
                min_burst = processes[i].burst_time;
                shortest = i;
            }
        }
        
        if(shortest == -1) {
            current_time++;
            continue;
        }
        
        // Calculate response time (time first scheduled - arrival time)
        if(!processes[shortest].responded) {
            processes[shortest].response_time = current_time - processes[shortest].arrival_time;
            processes[shortest].responded = true;
            avg_rt += processes[shortest].response_time;
        }
        
        // Execute process to completion (non-preemptive)
        processes[shortest].completion_time = current_time + processes[shortest].burst_time;
        processes[shortest].turnaround_time = processes[shortest].completion_time - processes[shortest].arrival_time;
        processes[shortest].waiting_time = processes[shortest].turnaround_time - processes[shortest].burst_time;
        avg_wt += processes[shortest].waiting_time;
        avg_tat += processes[shortest].turnaround_time;
        done[shortest] = true;
        completed++;
        current_time = processes[shortest].completion_time;
    }
    
    print_results(processes, n);
    printf("\nAverage Waiting Time: %.2f", avg_wt / n);
    printf("\nAverage Turnaround Time: %.2f", avg_tat / n);
    printf("\nAverage Response Time: %.2f\n", avg_rt / n);
}

void srtf() {
    int n, current_time = 0, completed = 0;
    struct Process processes[MAX_PROCESSES];
    float avg_wt = 0, avg_tat = 0, avg_rt = 0;
    
    printf("\nSRTF Scheduling (Preemptive)\n");
    printf("Enter number of processes: ");
    scanf("%d", &n);
    
    printf("Enter arrival times and burst times:\n");
    for(int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        processes[i].responded = false;
        printf("P[%d]: ", i + 1);
        scanf("%d %d", &processes[i].arrival_time, &processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time;
    }
    
    while(completed < n) {
        int shortest = -1;
        int min_remaining = INT_MAX;
        
        // Find process with shortest remaining time
        for(int i = 0; i < n; i++) {
            if(processes[i].remaining_time > 0 && processes[i].arrival_time <= current_time && 
               processes[i].remaining_time < min_remaining) {
                min_remaining = processes[i].remaining_time;
                shortest = i;
            }
        }
        
        if(shortest == -1) {
            current_time++;
            continue;
        }
        
        // Record response time when process first gets CPU
        if(!processes[shortest].responded) {
            processes[shortest].response_time = current_time - processes[shortest].arrival_time;
            processes[shortest].responded = true;
            avg_rt += processes[shortest].response_time;
        }
        
        // Execute for 1 time unit (preemptive)
        processes[shortest].remaining_time--;
        current_time++;
        
        // Process completed
        if(processes[shortest].remaining_time == 0) {
            completed++;
            processes[shortest].completion_time = current_time;
            processes[shortest].turnaround_time = processes[shortest].completion_time - processes[shortest].arrival_time;
            processes[shortest].waiting_time = processes[shortest].turnaround_time - processes[shortest].burst_time;
            avg_wt += processes[shortest].waiting_time;
            avg_tat += processes[shortest].turnaround_time;
        }
    }
    
    print_results(processes, n);
    printf("\nAverage Waiting Time: %.2f", avg_wt / n);
    printf("\nAverage Turnaround Time: %.2f", avg_tat / n);
    printf("\nAverage Response Time: %.2f\n", avg_rt / n);
}

void priority_scheduling() {
    int n, current_time = 0, completed = 0;
    struct Process processes[MAX_PROCESSES];
    bool done[MAX_PROCESSES] = {false};
    float avg_wt = 0, avg_tat = 0, avg_rt = 0;
    
    printf("\nPriority Scheduling (Non-Preemptive)\n");
    printf("Enter number of processes: ");
    scanf("%d", &n);
    
    printf("Enter arrival times, burst times and priorities (lower number = higher priority):\n");
    for(int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        processes[i].responded = false;
        printf("P[%d]: ", i + 1);
        scanf("%d %d %d", &processes[i].arrival_time, &processes[i].burst_time, &processes[i].priority);
    }
    
    while(completed < n) {
        int highest_priority = -1;
        int min_priority = INT_MAX;
        
        // Find highest priority process available
        for(int i = 0; i < n; i++) {
            if(!done[i] && processes[i].arrival_time <= current_time && 
               processes[i].priority < min_priority) {
                min_priority = processes[i].priority;
                highest_priority = i;
            }
        }
        
        if(highest_priority == -1) {
            current_time++;
            continue;
        }
        
        // Record response time
        if(!processes[highest_priority].responded) {
            processes[highest_priority].response_time = current_time - processes[highest_priority].arrival_time;
            processes[highest_priority].responded = true;
            avg_rt += processes[highest_priority].response_time;
        }
        
        // Execute to completion (non-preemptive)
        processes[highest_priority].completion_time = current_time + processes[highest_priority].burst_time;
        processes[highest_priority].turnaround_time = processes[highest_priority].completion_time - processes[highest_priority].arrival_time;
        processes[highest_priority].waiting_time = processes[highest_priority].turnaround_time - processes[highest_priority].burst_time;
        avg_wt += processes[highest_priority].waiting_time;
        avg_tat += processes[highest_priority].turnaround_time;
        done[highest_priority] = true;
        completed++;
        current_time = processes[highest_priority].completion_time;
    }
    
    print_results(processes, n);
    printf("\nAverage Waiting Time: %.2f", avg_wt / n);
    printf("\nAverage Turnaround Time: %.2f", avg_tat / n);
    printf("\nAverage Response Time: %.2f\n", avg_rt / n);
}

void round_robin() {
    int n, quantum, current_time = 0, completed = 0;
    struct Process processes[MAX_PROCESSES];
    float avg_wt = 0, avg_tat = 0, avg_rt = 0;
    
    printf("\nRound Robin Scheduling\n");
    printf("Enter number of processes: ");
    scanf("%d", &n);
    
    printf("Enter arrival times and burst times:\n");
    for(int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        processes[i].responded = false;
        printf("P[%d]: ", i + 1);
        scanf("%d %d", &processes[i].arrival_time, &processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time;
    }
    
    printf("Enter time quantum: ");
    scanf("%d", &quantum);
    
    while(completed < n) {
        for(int i = 0; i < n; i++) {
            if(processes[i].remaining_time > 0 && processes[i].arrival_time <= current_time) {
                // Record response time when process first gets CPU
                if(!processes[i].responded) {
                    processes[i].response_time = current_time - processes[i].arrival_time;
                    processes[i].responded = true;
                    avg_rt += processes[i].response_time;
                }
                
                // Execute for quantum or remaining time
                int execute_time = (processes[i].remaining_time > quantum) ? quantum : processes[i].remaining_time;
                current_time += execute_time;
                processes[i].remaining_time -= execute_time;
                
                // Process completed
                if(processes[i].remaining_time == 0) {
                    completed++;
                    processes[i].completion_time = current_time;
                    processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
                    processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
                    avg_wt += processes[i].waiting_time;
                    avg_tat += processes[i].turnaround_time;
                }
            }
        }
    }
    
    print_results(processes, n);
    printf("\nAverage Waiting Time: %.2f", avg_wt / n);
    printf("\nAverage Turnaround Time: %.2f", avg_tat / n);
    printf("\nAverage Response Time: %.2f\n", avg_rt / n);
}

void print_results(struct Process processes[], int n) {
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\tRT\n");
    for(int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", 
               processes[i].pid,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].completion_time,
               processes[i].turnaround_time,
               processes[i].waiting_time,
               processes[i].response_time);
    }
}
