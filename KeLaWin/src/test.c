#include "map.h"
#include "path.h"

int main(void) {
    Map atlas;
    Map atlasCopied;
    Path* way;
    Path* startWay;
    Coord start;
    Coord end;
    Coord act;
    int moove, wall, wallLeft, wallForward;
    int i, j;
    double dist;
    int cpt;

    readAndCreateMap(&atlas, "../tracks/starter_droit_au_but.txt");
    printMap(&atlas);
    
    printf("\n\n");
    printf("...............copy map................");
    printf("\n\n");

    atlasCopied = *copyMap(&atlas);
    printMap(&atlasCopied);

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

    printf("\n\n");
    printf("...............Test pour les fonctions de path.c................");
    printf("\n\n");

    printf("\n\n");
    printf("...............affichage du path en coord................");
    printf("\n\n");

    way = (Path*) malloc(sizeof(Path));
    act = start;
    way = createNode(atlas.map[act.y][act.x], act);
    act.x++;
    act.y++;
    startWay = way;
    way->next = createNode(atlas.map[act.y][act.x], act);
    way = way->next;
    act.x++;
    act.y++;
    way->next = createNode(atlas.map[act.y][act.x], act);
    way = way->next;
    act.x++;
    act.y++;
    way->next = createNode(atlas.map[act.y][act.x], act);
    way = way->next;
    act.x++;
    act.y++;
    way->next = createNode(atlas.map[act.y][act.x], act);
    way = way->next;
    act.x++;
    act.y++;
    way->next = createNode(atlas.map[act.y][act.x], act);

    printPath(startWay);

    printf("\n\n");
    printf("...............affichage du path en graphique................");
    printf("\n\n");

    displayPath(startWay, &atlas);

    printf("\nla carte de base ne doit pas etre modifié :\n");
    printMap(&atlas);

    printf("\n\n");
    printf("...............test distance euclidienne................");
    printf("\n\n");

    dist = distEucli(start.x, start.y, start.x+1, start.y);
    printf("distance entre la cas %d/%d et la case %d/%d (expected 1) ? %f\n",start.x, start.y, start.x+1, start.y, dist);
    dist = distEucli(start.x, start.y, start.x+1, start.y+1);
    printf("distance entre la cas %d/%d et la case %d/%d (expected sqtr(2)) ? %f\n",start.x, start.y, start.x+1, start.y+1, dist);
    dist = distEucli(start.x, start.y, start.x+5, start.y-1);
    printf("distance entre la cas %d/%d et la case %d/%d (expected sqrt(26)) ? %f\n",start.x, start.y, start.x+5, start.y-1, dist);

    printf("\n\n");
    printf("...............test nb node................");
    printf("\n\n");

    cpt = nbNode(startWay);
    printf("nombre de noeud du chemin (expected 5) ? %d\n", cpt);

    printf("\n\n");
    printf("...............test path length................");
    printf("\n\n");

    dist = pathLength(startWay);
    printf("longueur du chemin de test (expected 4*sqrt(2)) (par pathLength) ? %f\n",dist);
    dist = fitness(startWay);
    printf("longueur du chemin de test (expected 4*sqrt(2)) (par fitness) ? %f\n",dist);

/*
    void folowDir(Map* atlas, int* x, int* y, enum DIRECTION dir);
    enum DIRECTION validStartingDir(Map* atlas, int x, int y);
    void generateFirstPath(Map* atlas, int startX, int startY, int endX, int endY, Path* path);

    void mutate(Path* path, Map* atlas);
    Path* generateNeighbor(Path* path, Map* atlas);
    void simulatedAnnealing(Path* path, Map* atlas);

    void LookForPath(Map* atlas, Path* path);
*/

    return 0;
}