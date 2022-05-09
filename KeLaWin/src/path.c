#include "path.h"
#include "map.h"

/*###########################################################################################*/
/*#################################### print ################################################*/
/*###########################################################################################*/

int nbNode(Path* path) {
    int cpt = 0;
    while(path != NULL) {
        cpt ++;
        path = path->next;
    }
    return cpt;
}

void printPath(Path* path) {
    int i;
    int n;
    n= nbNode(path);
    for(i=0; i<n; i++) {
        printf("(%d ; %d) / ", path->coord.x, path->coord.y);
        path = path->next;
    }
    printf("\n");
}

void fprintPath(Path* path, char* file_txt) {
    int i;
    int n;
    
    FILE* file = fopen(file_txt,"a");

    n= nbNode(path);

    for(i=0; i<n; i++) {
        fprintf(file,"(%d ; %d) / ", path->coord.x, path->coord.y);
        path = path->next;
    }
    fprintf(file,"\n\n");
    fclose(file);
}

void displayPath(Path* path, Map* atlas) {
    int i;
    int n;
    Map* atlasToPrint;
    char cpt = 'a';

    n= nbNode(path);

    atlasToPrint = copyMap(atlas);
    
    for(i=0; i<n; i++){
        atlasToPrint->map[path->coord.y][path->coord.x] = cpt;
        path = path->next;
        cpt++;
        if (cpt =='z'+1) {
            cpt = 'a';
        }
    }
    printMap(atlasToPrint);
    free(atlasToPrint);
}

/*###########################################################################################*/
/*#################################### copy #################################################*/
/*###########################################################################################*/

Path* copy(Path* initialPath) {
    int i;
    int n;
    Path* copiedPath = (Path *) malloc(sizeof(Path));
    Path* start = copiedPath;

    n = nbNode(initialPath); 

    for (i=0; i<n;i++) {
        copiedPath->content = initialPath->content;
        copiedPath->coord = initialPath->coord;

        if (i!=n-1) { /*plus grand bout de scotch de la terre entiere*/
            copiedPath->next = (Path *) malloc(sizeof(Path));
            copiedPath = copiedPath->next;
            initialPath = initialPath->next;
        }
    }


    return start;
}

/*###########################################################################################*/
/*################################### create ################################################*/
/*###########################################################################################*/

Path* createNode(char content, Coord coord) {
    Path* next = (Path*) malloc(sizeof(Path));
    next->content = content;
    next->coord = coord;
    return next;
}

/*###########################################################################################*/
/*################################## distance ###############################################*/
/*###########################################################################################*/

double distEucli(int x1, int y1, int x2, int y2) {
    return sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );
}

double pathLength(Path* path) {
    int i;
    int n;
    double dist = 0;
    n = nbNode(path);
    for(i=0; i< n-1; i++) {
        dist += distEucli(path->coord.x, path->coord.y, path->next->coord.x, path->next->coord.y);
        path = path->next;
    }
    return dist;
}

double fitness(Path* path) {
    return pathLength(path);
}

/*###########################################################################################*/
/*################################ suivons le mur ###########################################*/
/*###########################################################################################*/

void folowDir(Map* atlas, int* x, int* y, enum DIRECTION dir) {
    if (atlas->map[*y][*x] != '.') {
        if (dir == N) {
            (*y) = (*y)-1;
        } else if (dir == E) {
            (*x) = (*x)+1;
        } else if (dir == S) {
            (*y) = (*y)+1;
        } else if (dir == W) {
            (*x) = (*x)-1;
        }
    }
}

enum DIRECTION validStartingDir(Map* atlas, int x, int y) {
    if ( atlas->map[y-1][x] != '.') {
        return N;
    } else if ( atlas->map[y][x+1] != '.') {
        return E;
    } else if ( atlas->map[y+1][x] != '.') {
        return S;
    }
    return W;
}

Path* generateFirstPath(Map* atlas, Coord start, Coord end) {
    Coord cur = start;
    int sumAngl = 0;
    enum DIRECTION dir;
    Path* path;
    Path* startPath;

    dir = validStartingDir(atlas, cur.x, cur.y);

    path = createNode(atlas->map[cur.y][cur.x], cur);
    path->next = (Path *) malloc(sizeof(Path));
    startPath = path;

    while( cur.x != end.x || cur.y != end.y) {
        
        folowDir(atlas, &cur.x, &cur.y, dir);

        path->next = createNode(atlas->map[cur.y][cur.x], cur);

        if (isThereWallForward(atlas, cur.x, cur.y, dir) == 1) {
            dir = (dir+1)%4;
            sumAngl++;
            continue;
        }
        if (isThereWallLeft(atlas, cur.x, cur.y, dir) == 0) {
            if (sumAngl != 0) {
                dir = (dir-1)%4;
                sumAngl--;
            }
        }
        path = path->next;
    }
    return startPath;
}

/*###########################################################################################*/
/*################################ Recuit Simulé ############################################*/
/*###########################################################################################*/
int isNodeInPath(Coord coord, Path* path) {
    while (path != NULL) {
        if ( path->coord.x == coord.x && path->coord.y == coord.y ) {
            return 1;
        }
        path = path->next;
    }
    return 0;
}

void pop(Path* path, int n) {
    int i;
    for (i=0; i<n-1; i++) {
        path = path->next;
    }
    path->next = path->next->next;
}

int isPopPossible(Path* path, int ri) {
    int i;
    for (i=0; i<ri; i++) {
        path = path->next;
    }
    if ( isMooveValid(path->coord.x, path->coord.y, path->next->next->coord.x, path->next->next->coord.y) ) {
        return 1;
    }
    return 0;
}

void morph(Path* path, int rx, int ry, int n, Map* atlas) {
    int i;
    for (i=0; i<n; i++) {
        path = path->next;
    }   
    path->coord.y += ry;
    path->coord.x += rx;
    path->content = atlas->map[path->coord.y][path->coord.x];
}

int isMorphPossible(Path* path, int rx, int ry, int ri, Map* atlas) {
    int i;
    for (i=0; i<ri; i++) {
        path = path->next;
    }
    if ( !isAWall(path->coord.x+rx, path->coord.y+ry, atlas) && isMooveValid(path->coord.x, path->coord.y, path->coord.x+rx, path->coord.y+ry) ) {
        return 1;
    }
    return 0;
}

void add(Path* path, int rx, int ry, int n, Map* atlas) {
    int i;
    Path* newPath;

    newPath = (Path *) malloc(sizeof(Path));

    for (i=0; i<n; i++) {
        path = path->next;
    }

    newPath->coord = path->coord;
    newPath->coord.x += rx;
    newPath->coord.y += ry;

    newPath->content = atlas->map[newPath->coord.y][newPath->coord.x];

    newPath->next = path->next;
    path->next = newPath;
}

int isAddPossible(Path* path, int rx, int ry, int ri, Map* atlas) {
    int i;
    for (i=0; i<ri; i++) {
        path = path->next;
    }
    if ( !isAWall(path->coord.x+rx, path->coord.y+ry, atlas) && isMooveValid(path->coord.x, path->coord.y, path->coord.x+rx, path->coord.y+ry) ) {
        return 1;
    }
    return 0;
}

void mutate(Path* path, Map* atlas) {
    int i = 1;
    double r;

    int rx;
    int ry;

    int ri;

    int n;

    rx = (rand()%3)-1;
    ry = (rand()%3)-1;

    r = rand()/(RAND_MAX+1.0);

    n = nbNode(path);

    ri = (rand()%n);

    if (ri > 0) {
        if (r< 0.7) {
            if ( isMorphPossible(path, rx, ry, ri, atlas) )  {
                morph(path, rx, ry, ri, atlas);
            }
        } else if (r>0.7 && r<0.9) {
            if ( isPopPossible(path, ri) && ri < n-i) {
                pop(path, ri);
                i++;
            }
        } else {
            if ( isAddPossible(path, rx, ry, ri, atlas)) {
                add(path, rx, ry, ri, atlas);
                i--;
            }
        }
    }
}

Path* generateNeighbor(Path* path, Map* atlas) {
    Path* neighbor = copy(path);
    Path* start = neighbor;
    double r;
    double mutatingProbabilitie = 1;

    while (neighbor != NULL) {
        r = rand()/(RAND_MAX+1.0);
        if (r < mutatingProbabilitie ) {
            mutate(neighbor, atlas);
        }
        neighbor = neighbor->next;
    }
    return start;
}
/*
Path* chooseBestNeighbor(Path** neighbors, int size ) {
    int i;

    Path* bestPath = neighbors[0];

    for( i=0; i<size; i++) {
        if( fitness(neighbors[i]) < fitness(bestPath)) {
            bestPath = neighbors[i];
        }
    }

    return bestPath;
}

/*
void simulatedAnnealing(Path* path, Map* atlas) {
    int i;
    double r;

    double temperature;
    double coolingFactor;
    double epsilon;

    Path* bestPath = copy(path);

    Path* neighbors[10];
    Path* bestNeighbor;
    
    temperature = 10;
    coolingFactor = 0.9;
    epsilon = 0.0001;

    while (temperature > epsilon) {
        for(i=0; i<10; i++) {
            neighbors[i] = generateNeighbor(bestPath, atlas);
        }
        bestNeighbor = chooseBestNeighbor(neighbors, 10);

        if (fitness(bestNeighbor) < fitness(bestPath)) {
            bestPath = bestNeighbor;
            printf("meilleur\n");
        } else {
            r = rand()/(RAND_MAX+1.0);
            printf("r=%f, fit1=%f/fit2=%f/temp=%f\n", r, fitness(bestPath) ,fitness(bestNeighbor), temperature);
            if (r < exp( -(fitness(bestNeighbor) - fitness(bestPath))/temperature )) {
                bestPath = bestNeighbor;
                printf("moins bon\n");
            }
        }

        temperature *= coolingFactor;
        printf("nb node : %d\n",nbNode(path));
        printf("path length bestPath: %f\n\n", pathLength(bestPath));

        fprintPath(bestPath, "err.txt");

        for(i=0; i<10; i++) {
            if ( neighbors[i] != bestNeighbor ) {
                free(neighbors[i]);
            }
        }
        
    }
    path = bestPath;
}

/*###########################################################################################*/
/*############################ Cherchons notre chemin #######################################*/
/*###########################################################################################*/
/*
void LookForPath(Map* atlas, Path* path) {
    int j, i;
    int startX = 0;
    int startY = 0;
    int endX = 0;
    int endY = 0;

    Map cleanAtlas;
    Map cleanAtlas1;

    cleanAtlas.heigth = atlas->heigth;
    cleanAtlas.width = atlas->width;
    cleanAtlas.startingGas = atlas->startingGas;
    cleanAtlas.map = (char **) malloc( cleanAtlas.heigth*sizeof(char *));

    for ( j=0; j<cleanAtlas.heigth; j++) {
        cleanAtlas.map[j] = (char *) malloc( cleanAtlas.width*sizeof(char));
        for (i=0; i<atlas->width; i++) {
            cleanAtlas.map[j][i] = atlas->map[j][i];
        }
    }

    cleanAtlas1.heigth = atlas->heigth;
    cleanAtlas1.width = atlas->width;
    cleanAtlas1.startingGas = atlas->startingGas;
    cleanAtlas1.map = (char **) malloc( cleanAtlas.heigth*sizeof(char *));

    for ( j=0; j<cleanAtlas1.heigth; j++) {
        cleanAtlas1.map[j] = (char *) malloc( cleanAtlas1.width*sizeof(char));
        for (i=0; i<atlas->width; i++) {
            cleanAtlas1.map[j][i] = atlas->map[j][i];
        }
    }

    findStartingPoint(atlas, &startX, &startY);
    findEndPoint(atlas, &endX, &endY);

    generateFirstPath(atlas, startX, startY, endX, endY, path);

    printf("\n");
    displayPath(path, &cleanAtlas1);
    printf("path length (longer mur): %f\n", pathLength(path));

    simulatedAnnealing(path, &cleanAtlas);
    printf("\n");
    displayPath(path, &cleanAtlas);
    printf("path length (recuit): %f\n", pathLength(path));
}
*/