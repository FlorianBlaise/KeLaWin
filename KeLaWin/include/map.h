#ifndef MAP_H
#define MAP_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_LINE_LENGTH 512

enum DIRECTION {S, E, N, W};

typedef struct _Map {
    int heigth;
    int width;
    int startingGas;
    char** map;
} Map;

int readAndCreateMap(Map* atlas, char* ficName);
Map* copyMap(Map* atlas);

void printMap(Map* atlas);
void fprintMap(Map* atlas, char* file_txt);

void findStartingPoint(Map* atlas, int* i, int* j);
void findEndPoint(Map* atlas, int* i, int* j);

int isMooveValid(int x1, int y1, int x2, int y2);
int isAWall(int x, int y, Map* atlas);
int isThereWallLeft(Map* atlas, int x, int y, enum DIRECTION dir);
int isThereWallForward(Map* atlas, int x, int y, enum DIRECTION dir);

#endif