#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "map.h"
#include "path.h"
#include "queue.h"

/* La structure des noeud est dans le point h des cellules car on a besoin pour modifier les cellules*/


/******************************************************************************************************************************************/
/*                                                       -Prototypes des fonctions-                                                       */
/******************************************************************************************************************************************/

Noeud* Create_Noeud(void);

Noeud* Constru_Noeud(Noeud*, int, int, int, int,Noeud* );

int affiche_Noeud(Noeud*);

int compareParHeuristique(Noeud* ,Noeud* );

Queue cheminPlusCourt(Noeud* objectif, Noeud* depart);