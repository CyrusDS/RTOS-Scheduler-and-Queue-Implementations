#include "rr.h"
#include "queue.h"
#include <stdio.h>

void init(struct task_t *task, int *execution, int size) {
    for(int i = 0; i < size; i++) {
        task->execution_time = execution[i];
        task->process_id = i; //arbitrary name id based on order in task array
        task->waiting_time = 0; //unused initially
        task->turnaround_time = 0; //unused initially
        task->left_to_execute = execution[i];
        task++;
    }
    return;
}

void round_robin(struct task_t *task, int quantum, int size) {
    struct node_t* queue = create_queue(task,size);
    int wait_time[size];
        for(int i = 0; i < size; i++) { //set initial value of wait times to be zero so no undefined behavior
        wait_time[i] = 0;
    }
    // Hints:
    // 1. Create Queue based on the task array
    // 2. Process each "task" in round robin fashion
    // 3. You can process by pushing and popping items from the queue
    while(!is_empty(&queue)) {
        if(queue->task->left_to_execute > quantum) { //we will use the entire quantum of time given to us
            //printf("\nTime left to execute was > quantum\n");
            //printf("CURR TASK ID: %d, WAIT TIME: %d, TIME LEFT: %d\n",queue->task->process_id, wait_time[queue->task->process_id], queue->task->left_to_execute);
            queue->task->left_to_execute = queue->task->left_to_execute - quantum; //amount of time dedicated to task this run 
            queue->task->waiting_time = wait_time[queue->task->process_id]; //update wait time for current process
            //printf("CURR TASK ID: %d,TURNAROUND TIME: %d\n",queue->task->process_id, queue->task->turnaround_time);
            for(int i = 0; i < size; i++) {
                wait_time[i] = wait_time[i] + quantum; //add time spent to ALL processes, then remove wait time for current process after.
            }
            wait_time[queue->task->process_id] = wait_time[queue->task->process_id] - quantum; //this is the task being executed
            queue->task->turnaround_time = (queue->task->execution_time - queue->task->left_to_execute) + queue->task->waiting_time;
            //printf("CURR TASK ID: %d, WAIT TIME: %d, TIME LEFT: %d\n",queue->task->process_id, wait_time[queue->task->process_id], queue->task->left_to_execute);
            push(&queue,queue->task);
            pop(&queue);
        }
        else {
            //printf("\nTime left to execute was less than or equal quanta - will add %d to waiting time of other tasks instead \n", queue->task->left_to_execute);
            //printf("CURR TASK ID: %d, WAIT TIME: %d, TIME LEFT: %d\n",queue->task->process_id, wait_time[queue->task->process_id], queue->task->left_to_execute);
            queue->task->waiting_time = wait_time[queue->task->process_id];
            //printf("CURR TASK ID: %d,TURNAROUND TIME: %d\n",queue->task->process_id, queue->task->turnaround_time);
             for(int i = 0; i < size; i++) {
                wait_time[i] = wait_time[i] + queue->task->left_to_execute;
            }
            wait_time[queue->task->process_id] = wait_time[queue->task->process_id] - queue->task->left_to_execute; //this is the task being executed, so its not waiting
            queue->task->waiting_time = wait_time[queue->task->process_id];
            queue->task->left_to_execute = 0;
            queue->task->turnaround_time = (queue->task->execution_time - queue->task->left_to_execute) + queue->task->waiting_time;
            // printf("CURR TASK ID: %d, WAIT TIME: %d, TIME LEFT: %d\n",queue->task->process_id, wait_time[queue->task->process_id], queue->task->left_to_execute);
            pop(&queue);   
        }
    }
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
    //printf("%f\n",average_wait_time);
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