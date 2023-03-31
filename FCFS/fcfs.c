#include "fcfs.h"
#include "queue.h"
#include <stdio.h>

void init(struct task_t *task, int *execution, int size) {
    for(int i = 0; i < size; i++) {
        task->execution_time = execution[i];
        task->process_id = i; //arbitrary name id based on order in task array
        task->waiting_time = 0; //unused initially
        task->turnaround_time = 0; //unused initially
        task++;
    }
    return;
}

void first_come_first_served(struct task_t *task, int size) {
    struct node_t* queue = create_queue(task,size);
    // double start_time = 0;
    int wait_time = 0;
    while(!is_empty(&queue)) {
        queue->task->waiting_time = wait_time;  // - start_time;
        queue->task->turnaround_time = wait_time + queue->task->execution_time;
        wait_time = wait_time + queue->task->execution_time;
        pop(&queue);
    }
    // Hints:
    // 1. Create Queue based on the task array
    // 2. Pop off the front item until there are no more tasks
    return;
}

float calculate_average_wait_time(struct task_t *task, int size) {
    float total_wait_time = 0;
    float average_wait_time = 0;
    for(int i = 0; i < size; i++) {
        total_wait_time = (total_wait_time + task->waiting_time); //sum all wait times
        task++; //next task in taskarray
    }
    average_wait_time = total_wait_time / size; //take average
    printf("%f\n",average_wait_time);
    return average_wait_time;
}

float calculate_average_turn_around_time(struct task_t *task, int size) {
    float total_turnaround_time = 0;
    float average_turnaround_time = 0;
    for(int i = 0; i < size; i++) {
        total_turnaround_time = (total_turnaround_time + task->turnaround_time); //sum all turnaround times
        task++;
    }
    average_turnaround_time = total_turnaround_time / size; //take average
    return average_turnaround_time;
}