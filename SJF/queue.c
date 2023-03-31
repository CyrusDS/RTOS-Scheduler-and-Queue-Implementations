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