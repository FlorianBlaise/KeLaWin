#include "map.h"
#include "path.h"

int main(void) {
    Map atlas;
    Path path;
    char* name;
    name = "../tracks/starter_deux_virages.txt";

    readAndCreateMap(&atlas, name);
    printMap(&atlas);

    LookForPath(&atlas, &path);
    /*
    printMap(&atlas);
    printPath(&path);
    printf("\n");
    printf("path length : %f\n", pathLength(&path));
    */

    return 0;
}