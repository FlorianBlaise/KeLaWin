#include <stdio.h>
#include "queue.h"
#include <stdlib.h>

/**
 * @brief Test if the queue structure is well coded
 * @return int 1 if all the tests are passed.
 */
int tests(Queue);


int main()
{
    queue q;
    int res;
    queue_CreateQueue(&q);
    res = tests(&q);
    printf((res == 1) ? "Tests passed ! \n" : "Tests failed ... %d errors so far\n",res-1);
    return 0;
}

int tests(Queue q){

    element* tab;
    Queue q2;
    int i;
    int res;
    res = 0;

    tab = (element*) malloc(sizeof(element)*3);
    tab[0] = 1;
    tab[1] = 7;
    tab[2] = 78;
    

    res = 0;

    for(i = 0;i<3;i++){
        queue_Enqueue(q,tab[i]);      
    }


    (q->current->value) == tab[2] ?  printf("queue_Enqueue ___ Tests Passed\n") : res ++;
    queue_Dequeue(q);


    q->current->next->value == tab[1] ? printf("queue_Dequeue ___ Tests Passed\n") : res ++;
    queue_Dequeue(q);


    q->size == 1 ? printf("size of the queue ___ Tests Passed\n") : res ++;

    queue_IsEmpty(q) == 0 ? res = res: res ++;

    queue_Dequeue(q);


    queue_IsEmpty(q) == 1 ? printf("queue_IsEmpty ___ Test Passed\n") : res ++;

    for(i = 0;i<3;i++){
        queue_Enqueue(q,tab[i]);      
    }

    q2 = queue_Copy(q);
    q2->current->value == q->current->value? printf("queue_Copy ___ Test Passed\n") : res ++;

    queue_Concat(q2,q);
    q2->current->value == q->current->next->next->next->value ? printf("queue_Concat ___ Test Passed\n") : res ++;

    queue_Destruct(q2);
    return res+1;
}

