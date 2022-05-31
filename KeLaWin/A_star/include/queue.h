#include "cell.h"

/*! The queue structure, contains a size to know how many Nodes are in the queue and a pointer to the last entered Node*/
typedef struct queue
{
    Cell current;
    int size;

}queue, *Queue;

/**
 * @brief A function which create an queue on a given pointer
 * @param Queue a pointer to a queue.
 * 
 */
void queue_CreateQueue(Queue);

/**
 * @brief Check if the queue is empty or not
 * @return int 1 if the queue is empty, 0 else.
 */
int queue_IsEmpty(Queue);

/**
 * @brief Add a new Node to the queue
 * @return int 0 if all gone well
 */
int queue_Enqueue(Queue,Noeud*);

/**
 * @brief Remove the first in Node of the queue
 * @return Node the value that has been removed from the queue
 */
Noeud* queue_Dequeue(Queue);


/**
 * @brief Display the queue
 * @return int 0 if everything gone well
 */
int  queue_DisplayQueue(Queue);

/**
 * @brief Concatenate two queues (the two queue pointer will point to the same qeueue)
 * @return int 0 if all gone well
 */
int queue_Concat(Queue, Queue);

/**
 * @brief Copy a queue an return a pointer to the copied one
 * @return Queue the copied queue
 */
Queue queue_Copy(Queue);

/**
 * @brief Destroy a queue
 * @return void
 */
void queue_Destruct(Queue);



/**
 * @brief tells if a node is within the queue.
 * @return Queue the copied queue
 */
int queue_Exists(Queue q, Noeud* v);

int queue_Exists_cout(Queue , Noeud* );

Queue queue_Voisin(Noeud* );


int queue_Enqueue_heuristique(Queue ,Noeud* );
