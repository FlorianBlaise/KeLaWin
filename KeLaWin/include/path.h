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

int nbNode(Path* path);

void printPath(Path* path);
void fprintPath(Path* path, char* file_txt);
void displayPath(Path* path, Map* atlas);

Path* createNode(char content, Coord coord);
Path* copy(Path* initialPath);

double distEucli(int x1, int y1, int x2, int y2);
double pathLength(Path* path);
double fitness(Path* path);

void folowDir(Map* atlas, int* x, int* y, enum DIRECTION dir);
enum DIRECTION validStartingDir(Map* atlas, int x, int y);
Path* generateFirstPath(Map* atlas, Coord start, Coord end);

int isNodeInPath(Coord coord, Path* path);
void pop(Path* path, int indice);
void morph(Path* path, int rx, int ry, int indice, Map* atlas);
void randomAdd(Path* path, int rx, int ry, Map* atlas);

void mutate(Path* path, Map* atlas);
Path* generateNeighbor(Path* path, Map* atlas);
Path* chooseBestNeighbor(Path** neighbors, int size);

void simulatedAnnealing(Path* path, Map* atlas);

void LookForPath(Map* atlas, Path* path);

#endif