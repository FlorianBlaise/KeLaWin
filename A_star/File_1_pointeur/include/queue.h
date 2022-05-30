#include "cell.h"

/*! The queue structure, contains a size to know how many element are in the queue and a pointer to the last entered element*/
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
 * @brief Add a new element to the queue
 * @return int 0 if all gone well
 */
int queue_Enqueue(Queue,element);

/**
 * @brief Remove the first in element of the queue
 * @return element the value taht has been removed from the queue
 */
element queue_Dequeue(Queue);


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