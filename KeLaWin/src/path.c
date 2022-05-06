#include "path.h"

/*###########################################################################################*/
/*#################################### print ################################################*/
/*###########################################################################################*/

void printPath(Path* path) {
    while(path->content != '=') {
        printf("(%d ; %d) / ", path->coord.x, path->coord.y);
        path = path->next;
    }
    printf("\n");
}

void fprintPath(Path* path, char* file_txt) {
    FILE* file = fopen(file_txt,"a");

    while(path->content != '=') {
        fprintf(file,"(%d ; %d) / ", path->coord.x, path->coord.y);
        path = path->next;
    }
    fprintf(file,"\n\n");
    fclose(file);
}

void displayPath(Path* path, Map* atlas) {
    char cpt = 'a';
    while( path->content != '=') {
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
/*################################## distance ###############################################*/
/*###########################################################################################*/

double distEucli(int x1, int y1, int x2, int y2) {
    return sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );
}

double pathLength(Path* path) {
    double dist = 0;
    while(path->content != '=') {
        dist += distEucli(path->coord.x, path->coord.y, path->next->coord.x, path->next->coord.y);
        path = path->next;
    }
    return dist;
}

double fitness(Path* path) {
    return pathLength(path);
}

int nbNode(Path* path) {
    int dist = 0;
    while(path->content != '=') {
        dist += 1;
        path = path->next;
    }
    return dist;
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
    Path* next_path;

    dir = validStartingDir(atlas, x, y);
    
    next_path = (Path *) malloc(sizeof(Path));

    path->content = atlas->map[y][x];
    path->coord.x = x;
    path->coord.y = y;
    path->next = next_path;

    while( x != endX || y != endY) {
        
        folowDir(atlas, &x, &y, dir);

        next_path->content = atlas->map[y][x];
        next_path->coord.x = x;
        next_path->coord.y = y;
        next_path->next = (Path *)malloc(sizeof(Path));

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

        next_path = next_path->next;
    }
}

/*###########################################################################################*/
/*################################ Recuit Simulé ############################################*/
/*###########################################################################################*/

void randomPop(Path* path) {
    if ( isMooveValid(path->coord.x, path->coord.y, path->next->next->coord.x, path->next->next->coord.y) ) {
        path->next = path->next->next;
    }
}

void randomMorph(Path* path, int rx, int ry, Map* atlas) {
    if ( isMooveValid(path->coord.x, path->coord.y, path->next->coord.x+rx, path->next->coord.y+ry) && !isAWall(path->coord.x+rx, path->coord.y+ry, atlas) ) {
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

    if ( isMooveValid(path->coord.x, path->coord.y, new_path->coord.x, new_path->coord.y) && !isAWall(new_path->coord.x, new_path->coord.y, atlas) ) {
        path->next = new_path;
    }
}

void mutate(Path* path, Map* atlas) {
    double r;

    int rx;
    int ry;

    rx = (rand()%5)-2;
    ry = (rand()%5)-2;

    r = rand()/(RAND_MAX+1.0);

    while( isAWall(path->coord.x+rx, path->coord.y+ry, atlas) ) {
        rx = rand()%5-2;
        ry = rand()%5-2;
    }

    if (r< 0.7) {
        randomMorph(path, rx, ry, atlas);
        printf("1");
    } else if (r>0.7 && r<0.9) {
        randomPop(path);
        printf("2");
    } else {
        randomAdd(path, rx, ry, atlas);
        printf("3");
    }
}

Path* generateNeighbor(Path* path, Map* atlas) {
    Path* neighbor = (Path *) malloc(sizeof(Path));
    double r;
    double mutatingProbabilitie = 0.3;
    
    neighbor->content = path->content;
    neighbor->coord = path->coord;
    neighbor->next = path->next;

    while (neighbor->next->content != '=') {
        r = rand()/(RAND_MAX+1.0);
        if (r < mutatingProbabilitie ) {
            mutate(neighbor, atlas);
        }
        neighbor = neighbor->next;
    }
    return neighbor;
}

void simulatedAnnealing(Path* path, Map* atlas) {
    int i;
    int k;
    double r;

    double temperature;
    double coolingFactor;
    double epsilon;

    Path* bestPath = path;

    Path neighbors[10];

    bestPath->content = path->content;
    bestPath->coord = path->coord;
    bestPath->next = path->next;
    
    temperature = 0.7;
    coolingFactor = 0.9;
    epsilon = 0.000001;

    srand(time(NULL));

    while (temperature > epsilon) {
        for(i=0; i<10; i++) {
            neighbors[i] = *generateNeighbor(bestPath, atlas);
        }
        k = rand()%10;

        if (fitness(&neighbors[k]) < fitness(bestPath)) {
            bestPath = &neighbors[k];
        } else {
            r = rand()/(RAND_MAX+1.0);
            if (r < exp( (fitness(bestPath) - fitness(&neighbors[k]))/temperature )) {
                bestPath = &neighbors[k];
            }
        }

        temperature *= coolingFactor;
        printf("nb node : %d\n",nbNode(path));
        /*printf("path length bestPath: %f\n\n", pathLength(bestPath));*/

        /*fprintPath(&bestPath,"err.txt");*/
    }
    /*path = bestPath;*/
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

    simulatedAnnealing(path, atlas);
    printf("\n");
    displayPath(path, &cleanAtlas);
    printf("path length (recuit): %f\n", pathLength(path));
}