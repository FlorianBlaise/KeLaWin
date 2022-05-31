#include "a_star.h"
#include <stdio.h>
#include <stdlib.h>


void queue_CreateQueue(Queue q){
    q=(Queue)calloc(1,sizeof(queue));
    q->size = 0;
    q->current = NULL;
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
    tmp=createCell(NULL);
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
    perror("Erreur dans l'insertion du noeud.\n");
    exit(EXIT_FAILURE);
}



Noeud* queue_Dequeue(Queue queue){

    Noeud* n;
    n=Create_Noeud(n);
    n = queue->current->suivant->n;
    if(queue->size == 1){
        queue->size = 0;
        deleteCell(queue->current);
        queue->current = NULL;
    }
    else{
        Cell tmp;
        tmp = queue->current->suivant;
        n = tmp->n;
        
        queue->current->suivant = queue->current->suivant->suivant;
        deleteCell(tmp);
        queue->size --;
    }
    return n;

}
int  queue_DisplayQueue(Queue queue){
    Cell tmp;
    printf("Displaying queue:\n");
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
        printf("The queue is empty\n");
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
    tmp= createCell(tmp);
    tmp = q->current;
    if ((v->coordo.x==tmp->n->coordo.x)&&(v->coordo.y==tmp->n->coordo.y)){
        return true;
    }
    while (tmp->suivant){
        if ((v->coordo.x==tmp->suivant->n->coordo.x)&&(v->coordo.y==tmp->suivant->n->coordo.y)){
            return true;
        }
        tmp = tmp->suivant;
    }
    return false;
}

int queue_Exists_cout(Queue q, Noeud* v){
    Cell tmp;
    tmp= (Cell)malloc(sizeof(cell));
    tmp = q->current;
    if ((v->coordo.x==tmp->n->coordo.x)&&(v->coordo.y==tmp->n->coordo.y)){
        if (v->cout<=tmp->n->cout){
            q->current->n->parent=v->parent;
            q->current->n->heuristique=v->heuristique;
            return true; 
        }
        return false;
    }
    while (tmp->suivant){
        if ((v->coordo.x==tmp->n->coordo.x)&&(v->coordo.y==tmp->n->coordo.y)){
            if (v->cout<=tmp->n->cout){
                tmp->n->parent=v->parent;
                tmp->n->heuristique=v->heuristique;
                tmp->n->cout=v->cout;
                return true; 
            }
            return false;
        }
    }
}

Queue queue_Copy(Queue q){
    Queue q2;
    Cell tmp;
    int i;
    q2 = (Queue) malloc(sizeof(queue));
    queue_CreateQueue(q2);
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
    Queue voisin;
    queue_CreateQueue(voisin);
    Noeud* N;
    Noeud* S;
    Noeud* E;
    Noeud* W;

    N=Create_Noeud(N);
    S=Create_Noeud(S);
    E=Create_Noeud(E);
    W=Create_Noeud(W);

    N=Constru_Noeud(N,u->coordo.x,u->coordo.y-1,u->cout,u->heuristique,u->parent);
    S=Constru_Noeud(N,u->coordo.x,u->coordo.y+1,u->cout,u->heuristique,u->parent);
    E=Constru_Noeud(N,u->coordo.x+1,u->coordo.y,u->cout,u->heuristique,u->parent);
    W=Constru_Noeud(N,u->coordo.x-1,u->coordo.y,u->cout,u->heuristique,u->parent);



    queue_Enqueue(voisin,N);
    queue_Enqueue(voisin,S);
    queue_Enqueue(voisin,E);
    queue_Enqueue(voisin,W);

    return voisin;
}