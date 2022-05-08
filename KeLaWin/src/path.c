#include "path.h"

/*###########################################################################################*/
/*#################################### print ################################################*/
/*###########################################################################################*/

void printPath(Path* path) {
    while(path->next != NULL) {
        printf("(%d ; %d) / ", path->coord.x, path->coord.y);
        path = path->next;
    }
    printf("\n");
}

void fprintPath(Path* path, char* file_txt) {
    FILE* file = fopen(file_txt,"a");

    while(path->next != NULL) {
        fprintf(file,"(%d ; %d) / ", path->coord.x, path->coord.y);
        path = path->next;
    }
    fprintf(file,"\n\n");
    fclose(file);
}

void displayPath(Path* path, Map* atlas) {
    char cpt = 'a';
    while(path->next != NULL) {
        atlas->map[path->coord.y][path->coord.x] = cpt;
        path = path->next;
        cpt++;
        if (cpt =='z'+1) {
            cpt = 'a';
        }
    }
    printMap(atlas);
}

/*###########################################################################################*/
/*#################################### copy #################################################*/
/*###########################################################################################*/

Path* copy(Path* initialPath) {
    Path* copiedPath = (Path *) malloc(sizeof(Path));
    Path* start = copiedPath;

    do {
        copiedPath->content = initialPath->content;
        copiedPath->coord = initialPath->coord;

        copiedPath->next = (Path *) malloc(sizeof(Path));
        copiedPath = copiedPath->next;
        initialPath = initialPath->next;
    } while (initialPath->next != NULL);

    return start;
}

/*###########################################################################################*/
/*################################## distance ###############################################*/
/*###########################################################################################*/

double distEucli(int x1, int y1, int x2, int y2) {
    return sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );
}

int nbNode(Path* path) {
    int cpt = 0;
    while(path->next != NULL) {
        cpt += 1;
        path = path->next;
    }
    return cpt;
}

double pathLength(Path* path) {
    int i;
    double dist = 0;
    for(i=0; i< nbNode(path); i++) {
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
        if (dir == S) {
            (*y) = (*y)-1;
        } else if (dir == E) {
            (*x) = (*x)+1;
        } else if (dir == N) {
            (*y) = (*y)+1;
        } else if (dir == W) {
            (*x) = (*x)-1;
        }
    }
}

enum DIRECTION validStartingDir(Map* atlas, int x, int y) {
    if ( atlas->map[y-1][x] != '.') {
        return S;
    } else if ( atlas->map[y][x+1] != '.') {
        return E;
    } else if ( atlas->map[y+1][x] != '.') {
        return N;
    }
    return W;
}

void generateFirstPath(Map* atlas, int startX, int startY, int endX, int endY, Path* path) {
    int x = startX;
    int y = startY;
    int sumAngl = 0;
    enum DIRECTION dir;

    dir = validStartingDir(atlas, x, y);
    
    path->next = (Path *) malloc(sizeof(Path));

    path->content = atlas->map[y][x];
    path->coord.x = x;
    path->coord.y = y;

    while( x != endX || y != endY) {
        
        folowDir(atlas, &x, &y, dir);

        path->next->content = atlas->map[y][x];
        path->next->coord.x = x;
        path->next->coord.y = y;
        path->next->next = (Path *)malloc(sizeof(Path));

        if (atlas->map[y][x] == '#') {
            atlas->map[y][x] = '$';
        } else {
            atlas->map[y][x] = '&';
        }
        /*fprintMap(atlas, "err.txt");*/

        if (isThereWallForward(atlas, x, y, dir) == 1) {
            dir = (dir+1)%4;
            sumAngl++;
            continue;
        }
        if (isThereWallLeft(atlas, x, y, dir) == 0) {
            if (sumAngl != 0) {
                dir = (dir-1)%4;
                sumAngl--;
            }
        }

        path = path->next;
    }
}

/*###########################################################################################*/
/*################################ Recuit Simulé ############################################*/
/*###########################################################################################*/
int isNodeInPath(int x, int y, Path* path) {
    while (path->next != NULL) {
        if ( path->coord.x == x && path->coord.y == y ) {
            return 1;
        }
        path = path->next;
    }
    return 0;
}

void randomPop(Path* path) {
    if ( isMooveValid(path->coord.x, path->coord.y, path->next->next->coord.x, path->next->next->coord.y) ) {
        path->next = path->next->next;
    }
}

void randomMorph(Path* path, int rx, int ry, Map* atlas) {
    if ( isMooveValid(path->coord.x, path->coord.y, path->next->coord.x+rx, path->next->coord.y+ry) && !isAWall(path->coord.x+rx, path->coord.y+ry, atlas) && !isNodeInPath(path->coord.x+rx, path->coord.y+ry, path)) {
        path->coord.y += ry;
        path->coord.x += rx;
        path->content = atlas->map[path->coord.y][path->coord.x];
    }

}

void randomAdd(Path* path, int rx, int ry, Map* atlas) {
    Path* new_path;

    new_path = (Path *) malloc(sizeof(Path));
    new_path->coord = path->coord;

    new_path->coord.y += ry;
    new_path->coord.x += rx;
    new_path->content = atlas->map[path->coord.y][path->coord.x];

    if ( isMooveValid(path->coord.x, path->coord.y, new_path->coord.x, new_path->coord.y) && !isAWall(new_path->coord.x, new_path->coord.y, atlas) && !isNodeInPath(new_path->coord.x, new_path->coord.y, path) ) {
        path->next = new_path;
    }
}

void mutate(Path* path, Map* atlas) {
    double r;

    int rx;
    int ry;

    rx = (rand()%3)-1;
    ry = (rand()%3)-1;

    r = rand()/(RAND_MAX+1.0);

    while( isAWall(path->coord.x+rx, path->coord.y+ry, atlas) ) {
        rx = rand()%3-1;
        ry = rand()%3-1;
    }

    if (r< 0.7) {
        printf("1\n");
        randomMorph(path, rx, ry, atlas);
    } else if (r>0.7 && r<0.9) {
        printf("2\n");
        randomPop(path);
    } else {
        printf("3\n");
        randomAdd(path, rx, ry, atlas);
    }
}

Path* generateNeighbor(Path* path, Map* atlas) {
    Path* neighbor = copy(path);
    Path* start = neighbor;
    double r;
    double mutatingProbabilitie = 0.3;

    while (neighbor->next != NULL) {
        r = rand()/(RAND_MAX+1.0);
        if (r < mutatingProbabilitie ) {
            mutate(neighbor, atlas);
        }
        neighbor = neighbor->next;
    }
    return start;
}

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

    srand(time(NULL));

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
            if (r < exp( (fitness(bestPath) - fitness(bestNeighbor))/temperature )) {
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

void LookForPath(Map* atlas, Path* path) {
    int j, i;
    int startX = 0;
    int startY = 0;
    int endX = 0;
    int endY = 0;

    Map cleanAtlas;

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

    findStartingPoint(atlas, &startX, &startY);
    findEndPoint(atlas, &endX, &endY);

    generateFirstPath(atlas, startX, startY, endX, endY, path);
    atlas = &cleanAtlas;

    printf("\n");
    displayPath(path, atlas);
    printf("path length (longer mur): %f\n", pathLength(path));

    simulatedAnnealing(path, &cleanAtlas);
    atlas = &cleanAtlas;

    printf("\n");
    displayPath(path, atlas);
    printf("path length (recuit): %f\n", pathLength(path));
}