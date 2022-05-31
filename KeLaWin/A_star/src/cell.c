#include "a_star.h"
#include <stdlib.h>

Cell createCell(Noeud* v){
    Cell cell;
    cell->n=Create_Noeud(cell->n);
    cell = (Cell)malloc(sizeof(struct cell));
    cell->n = v; 
    return cell;
}

int affiche_cellule(Cell cell){
    printf("\nCette cellule contient le noeud:");
    affiche_Noeud(cell->n);
    return 0;
}

int deleteCell(Cell cell){
    free(cell->n);
    free(cell);
    return 0;
}


int main(){
    /*********************************************************/
    /*                 -Tests fonctions cell-                */
    /********************************************************
    Noeud* n1=Create_Noeud(n1);
    Constru_Noeud(n1,0,0,0,0);
    Cell cellule;
    cellule = createCell(n1);
    affiche_cellule(cellule);
    deleteCell(cellule);
    */
    return 0;
}