#include "sjf.h"
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

void shortest_job_first(struct task_t *task, int size) {
    // Hints:
    // 1. Create Queue based on the task array in the correct order
    // 2. Process each "task" until completion
    // 3. You can process by popping items from the queue
    struct task_t temp;
    //simple bubble sort before putting into queue
    int i, j; 
    for (i = 0; i < size - 1; i++)
        for (j = 0; j < size - i - 1; j++) {
            if (task[j].execution_time > task[j + 1].execution_time) {
                temp = task[j];
                task[j] = task[j+1];
                task[j+1] = temp;
            }
        }
    //create queue with sorted task array
    struct node_t* queue = create_queue(task,size);
    int wait_time = 0;
    while(!is_empty(&queue)) {
        queue->task->waiting_time = wait_time;  // - start_time;
        queue->task->turnaround_time = wait_time + queue->task->execution_time;
        wait_time = wait_time + queue->task->execution_time;
        pop(&queue);
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
    // printf("%f\n",average_wait_time);
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
    // printf("%f\n",average_turnaround_time);
    return average_turnaround_time;
}