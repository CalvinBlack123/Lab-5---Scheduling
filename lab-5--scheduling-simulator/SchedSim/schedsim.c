#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct Process {
    int pid;       // Process ID
    int bt;        // Burst Time
    int art;       // Arrival Time
    int wt;        // Waiting Time
    int tat;       // Turnaround Time
    int pri;       // Priority
    int rem_bt;    // Remaining Burst Time
} ProcessType;

// Comparison function for priority sorting
int comparePriority(const void* a, const void* b) {
    ProcessType* p1 = (ProcessType*)a;
    ProcessType* p2 = (ProcessType*)b;
    
    // If priorities are different, sort by priority
    if (p1->pri != p2->pri)
        return p1->pri - p2->pri;
    
    // If priorities are same, sort by arrival time (FCFS)
    return p1->art - p2->art;
}

// Priority Scheduling
void priorityScheduling(ProcessType* processes, int n) {
    // Sort processes by priority
    qsort(processes, n, sizeof(ProcessType), comparePriority);
    
    int current_time = 0;
    for (int i = 0; i < n; i++) {
        // Wait if process hasn't arrived yet
        if (current_time < processes[i].art)
            current_time = processes[i].art;
        
        // Calculate waiting time and turnaround time
        processes[i].wt = current_time - processes[i].art;
        processes[i].tat = processes[i].wt + processes[i].bt;
        
        // Update current time
        current_time += processes[i].bt;
    }
}

// Shortest Job First (Shortest Remaining Time First)
void sjfScheduling(ProcessType* processes, int n) {
    int total_time = 0;
    int completed = 0;
    
    // Initialize remaining burst times
    for (int i = 0; i < n; i++) {
        processes[i].rem_bt = processes[i].bt;
        total_time += processes[i].bt;
    }
    
    int current_time = 0;
    int shortest_process = -1;
    int min_rem_time = INT_MAX;
    
    while (completed < n) {
        // Find process with shortest remaining time
        shortest_process = -1;
        min_rem_time = INT_MAX;
        
        for (int i = 0; i < n; i++) {
            // Check if process has arrived and not completed
            if (processes[i].art <= current_time && 
                processes[i].rem_bt > 0 && 
                processes[i].rem_bt < min_rem_time) {
                shortest_process = i;
                min_rem_time = processes[i].rem_bt;
            }
        }
        
        // If no process found, increment time
        if (shortest_process == -1) {
            current_time++;
            continue;
        }
        
        // Execute process for 1 time unit
        processes[shortest_process].rem_bt--;
        current_time++;
        
        // Check if process is completed
        if (processes[shortest_process].rem_bt == 0) {
            completed++;
            
            // Calculate waiting time and turnaround time
            processes[shortest_process].tat = 
                current_time - processes[shortest_process].art;
            processes[shortest_process].wt = 
                processes[shortest_process].tat - processes[shortest_process].bt;
        }
    }
}

// Round Robin Scheduling
void roundRobinScheduling(ProcessType* processes, int n, int quantum) {
    int* rem_bt = malloc(n * sizeof(int));
    int current_time = 0;
    
    // Initialize remaining burst times
    for (int i = 0; i < n; i++) {
        rem_bt[i] = processes[i].bt;
    }
    
    while (1) {
        int done = 1;
        
        // Cycle through all processes
        for (int i = 0; i < n; i++) {
            // Skip if process is already completed
            if (rem_bt[i] > 0) {
                done = 0;
                
                // If remaining burst time > quantum
                if (rem_bt[i] > quantum) {
                    current_time += quantum;
                    rem_bt[i] -= quantum;
                }
                // Final execution of process
                else {
                    current_time += rem_bt[i];
                    
                    // Calculate waiting time
                    processes[i].wt = current_time - 
                        processes[i].bt - processes[i].art;
                    
                    // Calculate turnaround time
                    processes[i].tat = current_time - processes[i].art;
                    
                    // Mark process as completed
                    rem_bt[i] = 0;
                }
            }
        }
        
        // Exit if all processes completed
        if (done) break;
    }
    
    free(rem_bt);
}

// Utility function to print process details
void printProcessDetails(ProcessType* processes, int n) {
    printf("PID\tBT\tAT\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\n", 
               processes[i].pid, 
               processes[i].bt, 
               processes[i].art, 
               processes[i].wt, 
               processes[i].tat);
    }
}

int main() {
    // Example usage
    ProcessType processes[] = {
        {1, 10, 0, 0, 0, 3},
        {2, 5, 1, 0, 0, 1},
        {3, 8, 2, 0, 0, 2}
    };
    int n = sizeof(processes) / sizeof(processes[0]);
    int quantum = 2;  // For Round Robin
    
    // Priority Scheduling
    printf("Priority Scheduling:\n");
    priorityScheduling(processes, n);
    printProcessDetails(processes, n);
    
    // Reset waiting and turnaround times
    for (int i = 0; i < n; i++) {
        processes[i].wt = 0;
        processes[i].tat = 0;
    }
    
    // SJF Scheduling
    printf("\nShortest Job First (SRTF):\n");
    sjfScheduling(processes, n);
    printProcessDetails(processes, n);
    
    // Reset waiting and turnaround times
    for (int i = 0; i < n; i++) {
        processes[i].wt = 0;
        processes[i].tat = 0;
    }
    
    // Round Robin Scheduling
    printf("\nRound Robin Scheduling:\n");
    roundRobinScheduling(processes, n, quantum);
    printProcessDetails(processes, n);
    
    return 0;
}