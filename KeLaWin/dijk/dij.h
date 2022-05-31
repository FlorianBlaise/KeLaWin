#include "map.h"
#include "path.h"

int isStop(int** visite, int x, int y);
Coord trouveMin(int** visite, int** distance, int x, int y);
Path* dijkstra(Map* atlas, Coord start);