#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "queue.h"

#define MAX_LINE_LENGTH 512


/* La structure des noeud est dans le point h des cellules car on a besoin pour modifier les cellules*/

typedef struct _Path {
    char content;
    Coord coord;
    struct _Path* next;
} Path;

enum DIRECTION {N, E, S, W};

typedef struct _Map {
    int heigth;
    int width;
    int startingGas;
    char** map;
} Map;

/******************************************************************************************************************************************/
/*                                                       -Prototypes des fonctions-                                                       */
/******************************************************************************************************************************************/

Noeud* Create_Noeud(Noeud*);

Noeud* Constru_Noeud(Noeud*, int, int, int, int,Noeud* );

int affiche_Noeud(Noeud*);

int compareParHeuristique(Noeud* ,Noeud* );