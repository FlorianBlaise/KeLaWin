#include "a_star.h"
#include <stdlib.h>

Cell createCell(Noeud* v){
    Cell cell;
    cell = (Cell)malloc(sizeof(struct cell));
    cell->n = v;
    return cell;
}

int affiche_cellule(Cell cell){
    fprintf(stderr,"\nCette cellule contient le noeud:");
    affiche_Noeud(cell->n);
    return 0;
}

int deleteCell(Cell cell){
    free(cell->n);
    free(cell);
    return 0;
}
