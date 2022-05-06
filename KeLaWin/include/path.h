#ifndef PATH_H
#define PATH_H

#include "map.h"

typedef struct _Coord{
    int x;
    int y;
} Coord;

typedef struct _Path {
    char content;
    Coord coord;
    struct _Path* next;
} Path;

void printPath(Path* path);
void fprintPath(Path* path, char* file_txt);
void displayPath(Path* path, Map* atlas);

double distEucli(int x1, int y1, int x2, int y2);
double pathLength(Path* path);
double fitness(Path* path);

void folowDir(Map* atlas, int* x, int* y, enum DIRECTION dir);
enum DIRECTION validStartingDir(Map* atlas, int x, int y);
void generateFirstPath(Map* atlas, int startX, int startY, int endX, int endY, Path* path);

void mutate(Path* path, Map* atlas);
Path* generateNeighbor(Path* path, Map* atlas);
void simulatedAnnealing(Path* path, Map* atlas);

void LookForPath(Map* atlas, Path* path);

#endif