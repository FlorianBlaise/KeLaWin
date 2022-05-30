#include "path.h"
#include "map.h"

/*###########################################################################################*/
/*#################################### print ################################################*/
/*###########################################################################################*/

int nbNode(Path* path) {
    if(path == NULL) {
        return 0;
    }
    return nbNode(path->next)+1;
}

void printPath(Path* path) {
    int i;
    int n;
    if (path == NULL) {
        printf("je suis null\n");
        return;
    }
    n= nbNode(path);
    for(i=0; i<n; i++) {
        printf("coord = (%d ; %d) & spd = (%d ; %d)/ ", path->coord.x, path->coord.y, path->speed.x, path->speed.y);
        path = path->next;
    }
    printf("\n");
}

void fprintPath(Path* path, char* file_txt) {
    int i;
    int n;
    
    FILE* file;
    if(strcmp(file_txt,"out") == 0) {
        file = stderr;
    } else {
        file = fopen(file_txt,"a");
    }

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

void fdisplayPath(Path* path, Map* atlas) {
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
    fprintMap(atlasToPrint, "err.txt");
    free(atlasToPrint);
}

/*###########################################################################################*/
/*#################################### copy #################################################*/
/*###########################################################################################*/

Path* copy(Path* initialPath) {
    int i;
    int n = 0;
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
    next->speed.x = 0;
    next->speed.y = 0;
    return next;
}

/*###########################################################################################*/
/*#################################### gas ##################################################*/
/*###########################################################################################*/

int gasConsumption(int accX, int accY, int speedX, int speedY, int inSand)
{
  int gas = accX * accX + accY * accY;
  gas += (int)(sqrt(speedX * speedX + speedY * speedY) * 3.0 / 2.0);
  if (inSand) {
    gas += 1;
  }
  return -gas;
}

int isNodeSand(Path* path) {
    return path->content == '~' ? 1 : 0;
}

void updateNextSpeed(Path* path) {
    path->next->speed.x = abs(path->coord.x - path->next->coord.x);
    path->next->speed.y = abs(path->coord.y - path->next->coord.y);
}

int howManySandInPath(Path* path) {
    int i;
    int n = nbNode(path);
    int nbSand=0;

    for(i=0; i<n; i++) {
        if (path->content == '~') {
            nbSand++;
        }
        path = path->next;
    }
    return nbSand;
}

int gasConsumptionForPath(Path* path) {
    int i;
    int gas = 0;
    int n = nbNode(path);

    int speedX;
    int speedY;

    int oldSpeedX = 0;
    int oldSpeedY = 0;

    int accX;
    int accY;

    for(i=0; i<n-1; i++) {
        speedX = abs(path->coord.x - path->next->coord.x);
        speedY = abs(path->coord.y - path->next->coord.y);

        accX = abs(speedX - oldSpeedX);
        accY = abs(speedY - oldSpeedY);

        gas += gasConsumption(accX, accY, speedX, speedY, isNodeSand(path));
         
        oldSpeedX = speedX;
        oldSpeedY = speedY;

        path = path->next;
    }

    return -gas;
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

double fitness(Path* path, Map* atlas) {
    double n = (double) nbNode(path);
    /*
    double dist = pathLength(path);
    double nbSand = (double) howManySandInPath(path);
    double gas = (double) gasConsumptionForPath(path);
    */

    return n;
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
    if ( y!=0 && atlas->map[y-1][x] != '.') {
        return N;
    } else if ( x != atlas->width && atlas->map[y][x+1] != '.') {
        return E;
    } else if ( y != atlas->heigth && atlas->map[y+1][x] != '.') {
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
    startPath = path;

    while( cur.x != end.x || cur.y != end.y) {
        
        folowDir(atlas, &cur.x, &cur.y, dir);
        path->next = createNode(atlas->map[cur.y][cur.x], cur);
        path = path->next;

        if (isThereWallForward(atlas, cur.x, cur.y, dir) == 1) {
            dir = (dir+1)%4;
            sumAngl++;
            continue;
        } else if (isThereWallLeft(atlas, cur.x, cur.y, dir) == 0) {
            if (sumAngl != 0) {
                dir = (dir-1)%4;
                sumAngl--;
            } else {
                dir = (dir+1)%4;
                sumAngl++;
            }
        }
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

int isMooveValid(Path* path, int nextX, int nextY) {
    int speedNorm = sqrt(path->speed.x*path->speed.x + path->speed.y*path->speed.y);
    if ( abs(abs(nextX - path->coord.x) - path->speed.x) == 1 && abs(abs(path->coord.y - nextY) - path->speed.y) == 1) {
        if ( isNodeSand(path) ) {
            return speedNorm > 1 ? 0 : 1;
        } else {
            return speedNorm > 5 ? 0 : 1;
        }
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

int isPopPossible(Path* path) {
    if ( isMooveValid(path, path->next->next->coord.x, path->next->next->coord.y) ) {
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

int isMorphPossible(Path* path, int rx, int ry, Map* atlas) {
    if (isMooveValid(path, path->next->coord.x+rx, path->next->coord.y+ry)) {
        path = path->next;

        path->coord.x += rx;
        path->coord.y += ry;
        if ( !isAWall(path->coord.x, path->coord.y, atlas) && isMooveValid(path, path->next->coord.x, path->next->coord.y) ) {
            path->coord.x -= rx;
            path->coord.y -= ry;
            return 1;
        }
        path->coord.x -= rx;
        path->coord.y -= ry;
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

int isAddPossible(Path* path, int rx, int ry, Map* atlas) {

    if (isMooveValid(path, path->next->coord.x+rx, path->next->coord.y+ry)) {
        path = path->next;

        path->coord.x += rx;
        path->coord.y += ry;
        if ( !isAWall(path->coord.x, path->coord.y, atlas) && isMooveValid(path, path->next->coord.x, path->next->coord.y) ) {
            path->coord.x -= rx;
            path->coord.y -= ry;
            return 1;
        }
        path->coord.x -= rx;
        path->coord.y -= ry;
    }
   
    return 0;
}

void mutate(Path* path, Map* atlas) {
    double r;

    int rx;
    int ry;

    rx = (rand()%3)-1;
    ry = (rand()%3)-1;

    r = rand()/(RAND_MAX+1.0);

    if (r< 0.333) {
        if ( isMorphPossible(path, rx, ry, atlas) )  {
            morph(path, rx, ry, 0, atlas);
            printf("-->morph");
        } else {
        }
    } else if (r>0.333 && r<0.666) {
        if ( isPopPossible(path) ) {
            pop(path, 0);
            printf("-->pop");
        } else {
        }
    } else {
        if ( isAddPossible(path, rx, ry, atlas) ) {
            add(path, rx, ry, 0, atlas);
            printf("-->add");
        } 
    }

}

Path* generateNeighbor(Path* path, Map* atlas) {
    Path* neighbor = copy(path);
    Path* start = neighbor;
    double r;
    double mutatingProbabilitie = 0.3;

    while(neighbor != NULL) {
        if (neighbor->content != '=' && neighbor->next->content != '=') { /*pour ne pas modifier l'arrivé*/
            r = rand()/(RAND_MAX+1.0);
            /*updateNextSpeed(neighbor);*/
            if (r < mutatingProbabilitie ) {
                mutate(neighbor, atlas);
            }
            /*updateNextSpeed(neighbor);*/
        }
        neighbor = neighbor->next;
    }
    printf("oui\n");
    correctPath(start);
    return start;
}

void correctPath(Path* path) {
    int i;
    int n = nbNode(path);

    for(i=0; i<n; i++) {
        if (path != NULL && path->next != NULL) {
            if (isNodeInPath(path->coord, path->next)) {
                pop(path, 0);
            } 
            if (path->next->content == '.') {
                pop(path,0);
            }
        path = path->next;
        }
    }
}

Path* chooseBestNeighbor(Path** neighbors, int size, Map* atlas ) {
    int i;

    Path* bestPath = neighbors[0];

    for( i=0; i<size; i++) {
        if( fitness(neighbors[i], atlas) < fitness(bestPath, atlas)) {
            bestPath = neighbors[i];
        }
    }
    return bestPath;
}


Path* simulatedAnnealing(Path* path, Map* atlas) {
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

    printf("\nfitness du chemin de base : %f\n", fitness(path, atlas));
    fdisplayPath(path, atlas);

    while (temperature > epsilon) {
        
        for(i=0; i<10; i++) {
            neighbors[i] = generateNeighbor(bestPath, atlas);
            printf("%d\n",i);
            
        }
        bestNeighbor = chooseBestNeighbor(neighbors, 10, atlas);

        printf("fitness du meilleur voisin: %f\n", fitness(bestNeighbor, atlas));

        if (fitness(bestNeighbor, atlas) <= fitness(bestPath, atlas)) {
            bestPath = bestNeighbor;
        } else {
            r = rand()/(RAND_MAX+1.0);
            if (r < exp( -(fitness(bestNeighbor, atlas) - fitness(bestPath, atlas))/temperature )) {
                bestPath = bestNeighbor;
            }
        }

        temperature *= coolingFactor;
        fdisplayPath(bestPath, atlas);
        printf("tmp = %f\n\n", temperature);
    }
    fdisplayPath(bestPath, atlas);
    printf("fitness du meilleur chemin: %f\n\n", fitness(bestPath, atlas));
    return bestPath;
}

/*###########################################################################################*/
/*############################ Cherchons notre chemin #######################################*/
/*###########################################################################################*/

void LookForPath(Map* atlas, Path* path) {
    int startX = 0;
    int startY = 0;
    int endX = 0;
    int endY = 0;
    Coord start;
    Coord end;

    findStartingPoint(atlas, &startX, &startY);
    findEndPoint(atlas, &endX, &endY);

    start.x = startX;
    start.y = startY;
    end.x = endX;
    end.y = endY;

    path = generateFirstPath(atlas, start, end);

    printf("path length (longer mur): %f\n", fitness(path, atlas));

    path = simulatedAnnealing(path, atlas);
    printf("path length (recuit): %f\n", fitness(path, atlas));
}

/*###########################################################################################*/
/*########################### fonction du gestionnaire ######################################*/
/*###########################################################################################*/

Speed getSpd(Path* path) {
    Speed spd;
    spd.x = path->coord.x-path->next->coord.x;
    spd.y = path->coord.y-path->next->coord.y;
    return spd;
}

Accel getAcc(Speed speed1, Speed speed2) {
    Accel acc;
    acc.x = speed1.x-speed2.x;
    acc.y = speed1.y-speed2.y;
    return acc;
}