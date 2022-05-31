#include <stdlib.h>
#include <stdio.h>


/*
typedef struct _Coord{
    int x;
    int y;
} Coord;
*/
typedef struct Noeud{
      Coord coordo;
      int cout;
      int heuristique;
      struct Noeud* parent;
} Noeud;

/*! Struct cell, constitued by an Node a pointer to the next cell */
typedef struct cell{
    struct cell* suivant;
    Noeud* n;
}cell, *Cell;

/**
 * Create a new cell.
 * @return a pointer to the created cell.
 * @param Noeud*, an Node that will be inside the create cell (it's  value).
*/
Cell createCell(Noeud*);

/**
 * Delete a given cell.
 * @return an int (0 if the delete was succesfull).
 * @param Cell a pointer to the cell to delete.
*/
int affiche_cellule(Cell);

/**
 * Delete a given cell.
 * @return an int (0 if the delete was succesfull).
 * @param Cell a pointer to the cell to delete.
*/
int deleteCell(Cell);






