#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

struct node_t* create_queue(struct task_t* task, int size) {
    struct node_t *queue = create_new_node(task); //declare queue 
    struct node_t **tempnode = &queue; //needs to be ** to be passable
    for(int i = 0; i < (size-1); i++){ //first element added, so iterate one less
        task++;
        push(tempnode, task);
    }
    return queue;
}

struct node_t* create_new_node(struct task_t* task) {
    struct node_t *node; //declare node to be put in queue 
	node = (struct node_t*) malloc(sizeof(struct node_t)); //allocate memory for queue, depending on size of queue

    node->next = NULL;
    node->task = task;
    // return node 
    return node ;
}

struct task_t* peek(struct node_t** head) {
    if(*head == NULL) {
        printf("head null in peek");
        return NULL;
    }
    // return null so it compiles
    return (*head)->task;
}

void pop(struct node_t** head) {
    if(*head == NULL) {
        printf("head null in pop");
        return;
    }
    struct node_t *temp_head = *head; //declare node 
    *head = temp_head->next; //move head to next node
    free(temp_head); //free memory for node being popped
    return;
}

void push(struct node_t** head, struct task_t* task) {
    if((*head) == NULL) {
        printf("head null in push");
        return;
    }
    struct node_t *temp = create_new_node(task); //declare temp
    struct node_t *iter; //declare iterator

    iter = (*head);
    while((iter)->next != NULL) {
        iter = iter->next; //go to end of linked list
    }

    iter->next = temp; //assign end of linked list to temp node to push.

    return;
}

int is_empty(struct node_t** head) {
    if(*head == NULL) {
        return 1;
    }
    // return 0 so it compiles
    return 0;
}

void empty_queue(struct node_t** head) {
    while(*head != NULL) {
        pop(head);
    }
    return;
}
void update_priority(struct node_t** head, int time) {

    // Note: see hints in main.c and queue.h
        if(is_empty(head)){
        printf("Reached end");
        return;
        }

//     struct node_t *new_node = *head; //used to print queue before updating priority, reordering
//     struct node_t *new_node2 = *head; //used to print queue after priority updates, reordering
    
    struct node_t *iter = *head; //used to update priority of all tasks in the queue, and ALSO used to determine size of the queue (important)
    struct node_t *iter2 = *head; //used to place all tasks in new task array AFTER we know the size of the queue
    struct task_t* temp; //used to switch elements of tasks in task array as a temp holder
    int curr_queue_size = 0; //size of current queue, determined by iterating through
    //updates priorities based on execution time, time left to execute, compared to time.
    //also determine size of queue (important)
    while(iter != NULL) {
        if(iter->task->execution_time == time){
            // printf("\n STYST T=%d, ET=%d \n", time,iter->task->execution_time);
            iter->task->priority = iter->task->priority * 4;
        }
        else if(iter->task->left_to_execute == time){
            // printf("\n STYST T=%d, LTE=%d \n", time,iter->task->execution_time);
            iter->task->priority = iter->task->priority * 2;
        }
        curr_queue_size++;
        iter = iter->next;
    }

    struct task_t *requeue[curr_queue_size]; //put all tasks back in a task array
   
    int i = 0;
    while(iter2 != NULL) {
        requeue[i] = iter2->task; //iterate through queue to put into task array
        iter2 = iter2->next;
        i++;
    }
    
    
    //BUBBLE SORT TO RESORT QUEUE BASED ON PRIORITY ONCE ALL ELEMENTS ARE NOW RECOVERED
    for (int i = 0; i < curr_queue_size - 1; i++) {
        for (int j = 0; j < curr_queue_size - i - 1; j++) {
            if (requeue[j]->priority > requeue[j + 1]->priority) {
                temp = requeue[j];
                requeue[j] = requeue[j+1];
                requeue[j+1] = temp;
            }
        }
    }
    
    for(int i = 0; i < curr_queue_size; i++) {
        push(head,requeue[i]); //Requeue all elements that were placed in, based on queue size
    }
//     int aj = 0;
    
//     printf("\n CURRENT RUN \n");
//     printf("\n =========== \n");
//     while(new_node2 != NULL) {
//         if(aj >= curr_queue_size) {
//         printf("\n ID: %d ",new_node2->task->process_id);
//         printf("PRIO: %d ",new_node2->task->priority);
//         printf("EXEC TIME: %d ",new_node2->task->execution_time);
//         printf("TIME LEFT: %d \n",new_node2->task->left_to_execute);
//         }
//         new_node2 = new_node2->next;
//         aj++;
//     }
}