#include "cell.h"
#include<stdlib.h>

Cell createCell(int v){
    Cell cell;
    cell = (Cell)malloc(sizeof(struct cell));
    cell->value = v; 
    return cell;
}

int deleteCell(Cell cell){
    free(cell);
    return 0;
}