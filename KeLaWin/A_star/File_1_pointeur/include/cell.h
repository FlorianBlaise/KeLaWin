#include <stdlib.h>
#include <stdio.h>

/*! Element is the type that the queue will store */
typedef int element;

/*! Struct cell, constitued by an element a pointer to the next cell */
typedef struct cell{
    struct cell* next;
    element value;
}cell, *Cell;

/**
 * Create a new cell.
 * @return a pointer to the created cell.
 * @param element, an element that will be inside the create cell (it's  value).
*/
Cell createCell(element);

/**
 * Delete a given cell.
 * @return an int (0 if the delete was succesfull).
 * @param Cell a pointer to the cell to delete.
*/
int deleteCell(Cell);



