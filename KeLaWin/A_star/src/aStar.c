#include "a_star.h"
#define MAX(a,b) (((a)>(b))?(a):(b))

/* Dans le main ? */
/* On récupère les coord du départ.Puis :
    Noeud depart; = Nœud(x=_, y=_, cout=0, heuristique=0)*/

Noeud* Create_Noeud(Noeud* n){
    n=(Noeud *) malloc(sizeof(Noeud));
    n->parent=(Noeud *) malloc(sizeof(Noeud));
    n->coordo.x=0;
    n->coordo.y=0;
    n->cout=0;
    n->heuristique=0;
    n->parent=NULL;
    return n;
}
Noeud* Constru_Noeud(Noeud* n3,int x, int y, int cout, int heuristique, Noeud* parent) {
    n3->coordo.x=x;
    n3->coordo.y=y;
    n3->cout=cout;
    n3->heuristique=heuristique;
    n3->parent=parent;
    return n3;
}

int affiche_Noeud(Noeud* n){
    printf("\n------------------------------------------------\n- Ce Noeud est aux coordonées (x=%d ; y=%d).\n- Il a un cout de %d.\n- Il a un heuristique de %d.\n------------------------------------------------\n",n->coordo.x,n->coordo.y,n->cout,n->heuristique);
    return 0;
}


Queue reconstituerChemin(Noeud* u){
    int i;
    Queue chemininv;
    queue_CreateQueue(chemininv);
    Queue chemin;
    queue_CreateQueue(chemin);
    Noeud* tmp = Create_Noeud(tmp);
    tmp= Constru_Noeud(tmp ,u->coordo.x ,u->coordo.y ,u->cout ,u->heuristique ,u->parent);
    queue_Enqueue(chemininv,u);
    while ( tmp->parent != NULL){
        queue_Enqueue(chemininv,tmp->parent);
        tmp=tmp->parent;
    }
    for (i=0;i<chemininv->size;i++) {
        tmp=queue_Dequeue(chemininv);
        queue_Enqueue(chemin,tmp);
    }
    free(tmp);
    if (queue_IsEmpty(chemininv)){
        free(chemininv);
    }
    return chemin;                                   
}


int compareParHeuristique(Noeud* n1,Noeud* n2){
    if( n1->heuristique < n2->heuristique){ 
        return 1;
    } else if (n1->heuristique == n2->heuristique) {
        return 0;
    } else {
        return -1;
    }
}


int distance(int x1, int y1, int x2, int y2){
    int manh;
    int eucli;
    int octile;
    int cheby;
    int dist;
    float F;
    int dx;
    int dy;

    dx=abs(x1-x2);
    dy=abs(y1-y2);

    /* Distance de Manhattan */
    //manh = dx+ dy; 
    //dist=manh;

   /* Distance d'Euclide */
    eucli=sqrt(dx * dx + dy * dy);
    dist=eucli;
  
   /* distance d'Octile */
    //F = (sqrt(2)-1);
    //octile = (dx < dy) ? (F*dx + dy) : (F*dy + dx);
    //dist = octile;

  /* distance de chebyshev */
    //cheby= MAX(dx,dy);
    //dist = cheby;

    return dist;

}

   
Queue cheminPlusCourt(Map map, Noeud* objectif, Noeud* depart){
    Queue openList; 
    queue_CreateQueue(openList);
    Queue closedList;
    queue_CreateQueue(closedList);
    Queue u;
    queue_CreateQueue(u);
    Queue voisin;
    queue_CreateQueue(voisin);
    Queue resultat;
    queue_CreateQueue(resultat);
    Cell tmp;
    tmp=createCell(NULL);
    /* A FAIRE LA PRIO TRI RAPIDE PAR HEURISTIQUE ?*/
    //openList = FilePrioritaire(comparateur = compareParHeuristique)
    queue_Enqueue_heuristique(openList,depart);
    while(openList->size!=0){ 
        u = queue_Dequeue(openList);
        if ((u->current->n->coordo.x == objectif->coordo.x) && (u->current->n->coordo.y == objectif->coordo.y)){
               resultat=reconstituerChemin(u);
               /* terminer le programme car objectif atteint */
               return resultat;
        }
        /* A FAIRE POUR REGARDER LES VOISINS */
        voisin=queue_Voisin(u);
        tmp=voisin->current;
        while (tmp){
            /* A FAIRE UN NOEUD EST T'IL PRESENT DANS UNE QUEUE (+ AVEC CONDITION) */

            if ( queue_Exists(closedList,tmp->n)==false && queue_Exists_cout(openList,tmp->n) ){
                /* On calcul le nouveau cout*/
                tmp->n->cout = u->current->n->cout+1; 
                /* On calcul le nouvel heuristique */
                tmp->n->heuristique = tmp->n->cout + distance(tmp->n->coordo.x, tmp->n->coordo.y, objectif->coordo.x, objectif->coordo.x);
                /* openList.ajouter(v)*/
                queue_Enqueue_heuristique(openList,tmp->n);
            }
            tmp=tmp->suivant;
        }
        queue_Enqueue(closedList,u);
    } 
    /*terminer le programme (avec erreur)*/
    perror("Erreur dans l'obtention du chemin : Arrivée jamais trouvée.\n");
    exit(EXIT_FAILURE);
}





void print(){
    printf("\n-HELLO-\n");
}

int main(){

/*********************************************************/
/*                -Tests fonctions a_star-               */
/*********************************************************/

    Noeud* n1=Create_Noeud(n1);
    Noeud* n2=Create_Noeud(n2);
    Constru_Noeud(n1,0,0,0,0,NULL);
    Constru_Noeud(n2,0,0,0,1,NULL);
    affiche_Noeud(n1);
    if (compareParHeuristique(n1,n2)==1){
        print();
    } else {
        printf("\nça marche \n");
    }



/*********************************************************/
/*                -Tests fonctions queue-                */
/*********************************************************/

    return 0;
}














