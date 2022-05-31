#include "a_star.h"
#include <stdio.h>
#include <stdlib.h>


Queue queue_CreateQueue(void){
    Queue q=(Queue)calloc(1,sizeof(queue));
    q->size = 0;
    q->current = NULL;
    return q;
}

int queue_IsEmpty(Queue queue){
    return queue->size == 0 ? 1 : 0;

}

int queue_Enqueue(Queue queue,Noeud* node){
    Cell cell;
    cell = createCell(node);
    if((queue->size) > 0){
        cell->suivant = queue->current->suivant; /* prepare l'emplacement du prochain élément */
        queue->current->suivant = cell;
    }else{
        cell->suivant = cell;
    }
    queue->current = cell;
    queue->size ++;
    return 0;
}

int queue_Enqueue_heuristique(Queue queue,Noeud* node){
    Cell cell;
    Cell tmp;
    cell = createCell(node);

    tmp=queue->current;
    
    while (tmp){
        if (tmp->n->heuristique<=cell->n->heuristique && tmp->suivant->n->heuristique>cell->n->heuristique ){
            cell->suivant = tmp->suivant;
            tmp->suivant=cell;
            return EXIT_SUCCESS; 
        } else if (tmp->n->heuristique<=cell->n->heuristique && tmp->suivant==NULL){
            tmp->suivant=cell;
            return EXIT_SUCCESS;
        }
        tmp=tmp->suivant;
    }
    fprintf(stderr,"Erreur dans l'insertion du noeud.\n");
    return EXIT_FAILURE;
}



Noeud* queue_Dequeue(Queue queue){
    Noeud* node;
    Cell tmp;
    node = queue->current->n;
    if(queue->size == 1){
        queue->size = 0;
        queue->current = NULL;
    }
    else{
        tmp = queue->current->suivant;
        node = tmp->n;
        
        queue->current->suivant = queue->current->suivant->suivant;
        deleteCell(tmp);
        queue->size --;
    }
    return node;
}

int queue_DisplayQueue(Queue queue){
    Cell tmp;
    fprintf(stderr,"Displaying queue:\n");
    if(queue->size > 0){
        affiche_Noeud(queue->current->suivant->n);
        tmp  = queue->current->suivant->suivant;
        while(queue->current != tmp){
            affiche_Noeud(tmp->n);
            tmp = tmp->suivant;
        }
        if(queue->size >1){
            affiche_Noeud(tmp->n);
        }
    }
    else{
        fprintf(stderr,"The queue is empty\n");
    }
    return 0;
}    



int queue_Concat(Queue q1, Queue q2){
    Cell tmp;
    if(q1->current == NULL){
        q1->current = q2->current;
    }else if(q2->current == NULL){
        q2->current = q1->current;
    }else{
        tmp = q2->current->suivant;
        q2->current->suivant = q1->current->suivant;
        q1->current->suivant = tmp;
        q2->current = q1->current;
    }
    return 0;
}


int queue_Exists(Queue q, Noeud* v){
    Cell tmp;
    tmp = q->current;
    if (tmp != NULL) {
        if ((v->coordo.x==tmp->n->coordo.x)&&(v->coordo.y==tmp->n->coordo.y)){
            fprintf(stderr,"1\n");
            return true;
        }
        fprintf(stderr,"2\n");
        while (tmp->suivant){
            fprintf(stderr,"3\n");
            if ((v->coordo.x==tmp->suivant->n->coordo.x)&&(v->coordo.y==tmp->suivant->n->coordo.y)){
                fprintf(stderr,"4\n");
                return true;
            }
            fprintf(stderr,"5\n");
            tmp = tmp->suivant;
        }
    }
    fprintf(stderr,"6\n");
    return false;
}

int queue_Exists_cout(Queue q, Noeud* v){
    Cell tmp;
    tmp= (Cell)malloc(sizeof(cell));
    tmp = q->current;
    fprintf(stderr,"1*\n");
    if(q->current != NULL) {
        if ((v->coordo.x==tmp->n->coordo.x)&&(v->coordo.y==tmp->n->coordo.y)){
            fprintf(stderr,"2*\n");
            if (v->cout<=tmp->n->cout){
                fprintf(stderr,"3*\n");
                q->current->n->parent=v->parent;
                q->current->n->heuristique=v->heuristique;
                return true; 
            }
            fprintf(stderr,"4*\n");
            return false;
        }
        fprintf(stderr,"5*\n");
        while (tmp->suivant){
            fprintf(stderr,"6*\n");
            if ((v->coordo.x==tmp->n->coordo.x)&&(v->coordo.y==tmp->n->coordo.y)){
                fprintf(stderr,"7*\n");
                if (v->cout<=tmp->n->cout){
                    fprintf(stderr,"8*\n");
                    tmp->n->parent=v->parent;
                    tmp->n->heuristique=v->heuristique;
                    tmp->n->cout=v->cout;
                    return true; 
                }
                fprintf(stderr,"9*\n");
                return false;
            }
        }
    }
    fprintf(stderr,"10*\n");
    return false;
}

Queue queue_Copy(Queue q){
    Queue q2;
    Cell tmp;
    int i;
    q2 = (Queue) malloc(sizeof(queue));
    q2 = queue_CreateQueue();
    tmp = q->current->suivant;
    
    for(i = 0; i<q->size;i++){
        queue_Enqueue(q2,tmp->n);
        tmp = tmp->suivant;
    }
    return q2;

}


void queue_Destruct(Queue q){

    do{
        queue_Dequeue(q);
    }while(q->size >0);
    free(q);
}

Queue queue_Voisin(Noeud* u){
    Queue voisin = (Queue )malloc(sizeof(Queue));
    Noeud* N;
    Noeud* S;
    Noeud* E;
    Noeud* W;

    voisin = queue_CreateQueue();

    N=Create_Noeud();
    S=Create_Noeud();
    E=Create_Noeud();
    W=Create_Noeud();

    N=Constru_Noeud(N,u->coordo.x,u->coordo.y-1,u->cout,u->heuristique,u->parent);
    S=Constru_Noeud(S,u->coordo.x,u->coordo.y+1,u->cout,u->heuristique,u->parent);
    E=Constru_Noeud(E,u->coordo.x+1,u->coordo.y,u->cout,u->heuristique,u->parent);
    W=Constru_Noeud(W,u->coordo.x-1,u->coordo.y,u->cout,u->heuristique,u->parent);

    fprintf(stderr,"noeud N");
    affiche_Noeud(N);
    fprintf(stderr,"noeud S");
    affiche_Noeud(S);
    fprintf(stderr,"noeud E");
    affiche_Noeud(E);
    fprintf(stderr,"noeud W");
    affiche_Noeud(W);

    queue_Enqueue(voisin,N);
    queue_Enqueue(voisin,S);
    queue_Enqueue(voisin,E);
    queue_Enqueue(voisin,W);

    return voisin;
}