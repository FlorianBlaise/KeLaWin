#include "queue.h"
#include <stdio.h>
#include <stdlib.h>


void queue_CreateQueue(Queue q){
    q->size = 0;
    q->current = NULL;
}

int queue_IsEmpty(Queue queue){
    return queue->size == 0 ? 1 : 0;

}
int queue_Enqueue(Queue queue,element value){
    Cell cell;
    cell = createCell(value);
    if((queue->size) > 0){
        cell->next = queue->current->next;
        queue->current->next = cell;
    }else{
        cell->next = cell;
    }
    queue->current = cell;
    queue->size ++;
    return 0;
}
element queue_Dequeue(Queue queue){

    element e;
    e = queue->current->next->value;
    if(queue->size == 1){
        queue->size = 0;
        deleteCell(queue->current);
        queue->current = NULL;
    }
    else{
        Cell tmp;
        tmp = queue->current->next;
        e = tmp->value;
        
        queue->current->next = queue->current->next->next;
        deleteCell(tmp);
        queue->size --;
    }
    return e;

}
int  queue_DisplayQueue(Queue queue){
    Cell tmp;
    printf("Displaying queue:\n");
    if(queue->size > 0){
        printf("%d ",queue->current->next->value);
        tmp  = queue->current->next->next;
        while(queue->current != tmp){
            printf(" -> %d",tmp->value);
            tmp = tmp->next;
        }
        if(queue->size >1){
        printf(" -> %d",tmp->value);
        }
    }
    else{
        printf("Queue is empty\n");
    }
    return 0;
}    



int queue_Concat(Queue q1, Queue q2){
    Cell tmp;
    if(q1->current == NULL){
        q1->current = q2->current;
    }else if(q2->current == NULL){
        q2->current = q1->current;
    }else{
        tmp = q2->current->next;
        q2->current->next = q1->current->next;
        q1->current->next = tmp;
        q2->current = q1->current;
    }
    return 0;
}


Queue queue_Copy(Queue q){
    Queue q2;
    Cell tmp;
    int i;
    q2 = (Queue) malloc(sizeof(queue));
    queue_CreateQueue(q2);
    tmp = q->current->next;
    
    for(i = 0; i<q->size;i++){
        queue_Enqueue(q2,tmp->value);
        tmp = tmp->next;
    }
    return q2;

}


void queue_Destruct(Queue q){

    do{
        queue_Dequeue(q);
    }while(q->size >0);
    free(q);
}