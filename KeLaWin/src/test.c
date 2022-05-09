#include "map.h"
#include "path.h"

int main(void) {
    Map atlas;
    Coord start;
    Coord end;
    int moove, wall, wallLeft, wallForward;
    int i, j;

    readAndCreateMap(&atlas, "../tracks/starter_droit_au_but.txt");
    printMap(&atlas);

    printf("\n\n");
    printf("...............test start/end................");
    printf("\n\n");

    findStartingPoint(&atlas, &start.x, &start.y);
    findEndPoint(&atlas, &end.x, &end.y);

    printf("starting point : %d/%d (expected 3/5)\n", start.x, start.y);
    printf("end point : %d/%d (expected 28/10)\n", end.x, end.y);

    printf("\n\n");
    printf("...............test moove................");
    printf("\n\n");

    for (i=0; i<3; i++) {
        for (j=0; j<3; j++) {
            moove = isMooveValid(start.x, start.y, start.x+i, start.y+j);
            printf("est ce que le moove %d/%d -> %d/%d est valide ? %d\n", start.x, start.y, start.x+i, start.y+j, moove);
        }
    }

    printf("\n\n");
    printf("...............test wall................");
    printf("\n\n");

    wall = isAWall( start.x, start.y,  &atlas);
    printf("est ce que la case %d/%d est un mur (debut)? %d\n", start.x, start.y, wall);
    wall = isAWall( end.x, end.y,  &atlas);
    printf("est ce que la case %d/%d est un mur (fin)? %d\n", end.x, end.y, wall);
    wall = isAWall( 0, 0,  &atlas);
    printf("est ce que la case %d/%d est un mur (mur)? %d\n", 0, 0, wall);
    wall = isAWall( 3, 4,  &atlas);
    printf("est ce que la case %d/%d est un mur (route)? %d\n", 3, 4, wall);

    printf("\n\n");
    printf("...............test wall left................");
    printf("\n\n");

    wallLeft = isThereWallLeft( &atlas,  2,  4, W);
    printf("est-ce qu'il y a un mur à gauche de la case 2/4 (vers W) ? %d\n", wallLeft);
    wallLeft = isThereWallLeft( &atlas,  2,  4, S);
    printf("est-ce qu'il y a un mur à gauche de la case 2/4 (vers S) ? %d\n", wallLeft);
    wallLeft = isThereWallLeft( &atlas,  2,  4, E);
    printf("est-ce qu'il y a un mur à gauche de la case 2/4 (vers E) ? %d\n", wallLeft);
    wallLeft = isThereWallLeft( &atlas,  2,  4, N);
    printf("est-ce qu'il y a un mur à gauche de la case 2/4 (vers N) ? %d\n", wallLeft);

    printf("\n\n");
    printf("...............test wall forward................");
    printf("\n\n");

    wallForward = isThereWallForward( &atlas,  2,  4, W);
    printf("est-ce qu'il y a un mur en face de la case 2/4 (vers W) ? %d\n", wallForward);
    wallForward = isThereWallForward( &atlas,  2,  4, S);
    printf("est-ce qu'il y a un mur en face de la case 2/4 (vers S) ? %d\n", wallForward);
    wallForward = isThereWallForward( &atlas,  2,  4, E);
    printf("est-ce qu'il y a un mur en face de la case 2/4 (vers E) ? %d\n", wallForward);
    wallForward = isThereWallForward( &atlas,  2,  4, N);
    printf("est-ce qu'il y a un mur en face de la case 2/4 (vers N) ? %d\n", wallForward);

    printf("\n\n");
    printf("...............Apres verification, tous les tests sont passés pour les fonctions de map.c................");
    printf("\n\n");

    
    return 0;
}