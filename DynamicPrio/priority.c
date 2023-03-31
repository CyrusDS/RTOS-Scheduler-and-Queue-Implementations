#include "priority.h"
#include "queue.h"
#include <stdio.h>

void init(struct task_t *task, int *execution, int *priority, int size) {
    for(int i = 0; i < size; i++) {
        task->execution_time = execution[i];
        task->process_id = i; //arbitrary name id based on order in task array
        task->waiting_time = 0; //unused initially
        task->turnaround_time = 0; //unused initially
        task->left_to_execute = execution[i];
        task->priority = priority[i];
        task++;
    }
    return;
}

void priority_schedule(struct task_t *task, int size) {
    // Hints:
    // 1. Create Queue based on the task array in the correct order
    // 2. Each task can be processed for a time interval of 1 (i.e quantum time of 1)
    // 3. You can process by pushing and popping items from the queue
    // 4. You must recalculate the priorities after every turn
    struct task_t temp; //temp task for sorting initial input queue
    int quantum = 1;
    int system_time = 0; //time to pass to update priority
    int curr_queue_size = size; //size of queue, updated after a pop occurs.
    
    int wait_time[size]; //all wait times for input queue
        for(int i = 0; i < size; i++) { //set initial value of wait times to be zero so no undefined behavior
        wait_time[i] = 0;
    }
    
    //simple bubble sort before putting into queue for correct priority
    int i, j; 
    for (i = 0; i < size - 1; i++) {
        for (j = 0; j < size - i - 1; j++) {
            if (task[j].priority > task[j + 1].priority) {
                temp = task[j];
                task[j] = task[j+1];
                task[j+1] = temp;
            }
        }
    }
    
    
    struct node_t* queue = create_queue(task,size); //create new queue sorted by (now correct) priority
    
    while(!is_empty(&queue)) {
        // struct node_t *iter2 = queue;
        // printf("\n --------- \n");
        // printf("\n QUEUE BEFORE RUN, T=%d \n", system_time+1);
        // while(iter2 != NULL) {
        //     printf(" ET%d_LTE%d(P%d){ID%d}(WT%d),",iter2->task->execution_time, iter2->task->left_to_execute,iter2->task->priority,iter2->task->process_id, iter2->task->waiting_time);
        //     iter2 = iter2->next;
        // }
        // printf("\n --------- \n");
        system_time = system_time + 1;
        
        if(queue->task->left_to_execute > quantum) { //we will use the entire quantum of time given to us
            queue->task->left_to_execute = queue->task->left_to_execute - quantum; //amount of time dedicated to task this run 
            queue->task->waiting_time = wait_time[queue->task->process_id]; //update wait time for current process
            for(int i = 0; i < size; i++) {
                wait_time[i] = wait_time[i] + quantum; //add time spent to ALL processes, then remove wait time for current process after.
            }
            wait_time[queue->task->process_id] = wait_time[queue->task->process_id] - quantum; //this is the task being executed
            queue->task->turnaround_time = (queue->task->execution_time - queue->task->left_to_execute) + queue->task->waiting_time;
            update_priority(&queue,system_time); //update priorities based on new system time, 0 initially (postincremented)
            
            for(int i = 0; i < curr_queue_size; i++) {
                pop(&queue); //THIS IS WHAT "REORDERS" QUEUE. Removed old entries so that only new entries remain after update_priority
            }
        } //end if left_to_execute > quantum
        else { //time left_to_execute < quantum (pop a task and increment time equal to how much time was needed)
            queue->task->waiting_time = wait_time[queue->task->process_id]; //does not wait any more time, but update to current.
             for(int i = 0; i < size; i++) {
                wait_time[i] = wait_time[i] + queue->task->left_to_execute; //increment all wait times
            }
            queue->task->left_to_execute = 0;
            queue->task->turnaround_time = (queue->task->execution_time - queue->task->left_to_execute) + queue->task->waiting_time;
            // printf("End WT: %d",queue->task->waiting_time);
            pop(&queue); 
            curr_queue_size--;
            update_priority(&queue,system_time);
            for(int i = 0; i < curr_queue_size; i++) {
                pop(&queue); //THIS IS WHAT "REORDERS" QUEUE. Removed old entries so that only new entries remain after update_priority
            } 
        } //end pop operation
        // iter2 = queue;
        // printf("\n QUEUE AFTER RUN, T=%d \n", system_time);
        // while(iter2 != NULL) {
        //     printf(" ET%d_LTE%d(P%d){ID%d}(WT%d),",iter2->task->execution_time, iter2->task->left_to_execute,iter2->task->priority,iter2->task->process_id, iter2->task->waiting_time);
        //     iter2 = iter2->next;
        // }
    } //end queue not empty
    return;
}

float calculate_average_wait_time(struct task_t *task, int size) {
    float total_wait_time = 0;
    float average_wait_time = 0;
    for(int i = 0; i < size; i++) {
        total_wait_time = (total_wait_time + task->waiting_time); //sum all wait times
        task++; //next task in taskarray
    }
    average_wait_time = total_wait_time / (float)size; //take average
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
    average_turnaround_time = total_turnaround_time / (float)size; //take average
    // printf("%f\n",average_turnaround_time);
    return average_turnaround_time;
}