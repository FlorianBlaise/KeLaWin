#include "a_star.h"
#define MAX(a,b) (((a)>(b))?(a):(b))

/* Dans le main ? */
/* On récupère les coord du départ.Puis :
    Noeud depart; = Nœud(x=_, y=_, cout=0, heuristique=0)*/

Noeud* Create_Noeud(){
    Noeud* n=(Noeud *) malloc(sizeof(Noeud));
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
    fprintf(stderr,"\n------------------------------------------------\n- Ce Noeud est aux coordonées (x=%d ; y=%d).\n- Il a un cout de %d.\n- Il a un heuristique de %d.\n------------------------------------------------\n",n->coordo.x,n->coordo.y,n->cout,n->heuristique);
    return 0;
}

Queue reconstituerChemin(Noeud* u){
    int i;
    Queue chemininv;
    Queue chemin;
    Noeud* tmp = Create_Noeud();

    chemininv = queue_CreateQueue();
    chemin = queue_CreateQueue();
    
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
    int eucli;
    int dist;
    int dx;
    int dy;

    dx=abs(x1-x2);
    dy=abs(y1-y2);

   /* Distance d'Euclide */
    eucli=sqrt(dx * dx + dy * dy);
    dist=eucli;

    return dist;

}

   
Queue cheminPlusCourt(Noeud* objectif, Noeud* depart){
    int i;
    Queue openList; 
    Queue closedList;
    Noeud* u;
    Queue voisin;
    Queue resultat;
    Cell tmp;

    openList = queue_CreateQueue();
    closedList = queue_CreateQueue();
    u = Create_Noeud();
    voisin = queue_CreateQueue();
    resultat = queue_CreateQueue();
    
    tmp=createCell(NULL);
    /* A FAIRE LA PRIO TRI RAPIDE PAR HEURISTIQUE ?*/
    /*openList = FilePrioritaire(comparateur = compareParHeuristique)*/
    queue_Enqueue(openList,depart);
    while(openList->size!=0){
        u = queue_Dequeue(openList);
        if ((u->coordo.x == objectif->coordo.x) && (u->coordo.y == objectif->coordo.y)){
               resultat=reconstituerChemin(u);
               /* terminer le programme car objectif atteint */
               return resultat;
        }
        /* A FAIRE POUR REGARDER LES VOISINS */
        voisin=queue_Voisin(u);
        queue_DisplayQueue(voisin);
        tmp=voisin->current;
        for(i=0; i<4; i++) {
            /* A FAIRE UN NOEUD EST T'IL PRESENT DANS UNE QUEUE (+ AVEC CONDITION) */
            fprintf(stderr,"tmp");
            affiche_cellule(tmp);
            fprintf(stderr,"tmp->s");
            affiche_cellule(tmp->suivant);

            if ( queue_Exists(closedList,tmp->n)==false && queue_Exists_cout(openList,tmp->n) ){

                /* On calcul le nouveau cout*/
                tmp->n->cout = u->cout+1; 
                /* On calcul le nouvel heuristique */
                tmp->n->heuristique = tmp->n->cout + distance(tmp->n->coordo.x, tmp->n->coordo.y, objectif->coordo.x, objectif->coordo.x);
                /* openList.ajouter(v)*/
                fprintf(stderr,"open");
                queue_DisplayQueue(openList);
                queue_Enqueue_heuristique(openList,tmp->n);
                queue_DisplayQueue(openList);
            }
            tmp=tmp->suivant;
        }
        fprintf(stderr,"oui");
        queue_Enqueue(closedList,u);
    } 
    fprintf(stderr,"Erreur dans l'obtention du chemin : Arrivée jamais trouvée.\n");
    return NULL;
}

void print(){
    printf("\n-HELLO-\n");
}
/*
int main(){

    Noeud* n1=Create_Noeud();
    Noeud* n2=Create_Noeud();
    Constru_Noeud(n1,0,0,0,0,NULL);
    Constru_Noeud(n2,0,0,0,1,NULL);
    affiche_Noeud(n1);
    if (compareParHeuristique(n1,n2)==1){
        print();
    } else {
        printf("\nça marche \n");
    }
    return 0;
}*/














