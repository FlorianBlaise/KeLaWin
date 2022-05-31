#include "map.h"
#include "path.h"
/*
int main(void) {
    Map atlas, atlasCopied;
    Path* path;
    
    Path* way;
    Path* copiedWay1;
    Path* copiedWay2;
    Path* startWay;
    Path* neighbors[10];
    Path* bestNeighbor; 
    Coord start, end, act, zero, random;
    enum DIRECTION dir;
    int wall, wallLeft, wallForward, inPath;
    int i;
    double dist;
    int cpt; 
    int gas;
    

    srand(time(NULL));

    readAndCreateMap(&atlas, "../tracks/starter_serpent.txt");
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

    printf("starting point : %d/%d \n", start.x, start.y);
    printf("end point : %d/%d \n", end.x, end.y);
    
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
    copiedWay1 = copy(startWay);
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
    copiedWay2 = copy(startWay);

    printPath(startWay);
    printPath(copiedWay1);
    printPath(copiedWay2);

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
    printf("distance entre la cas %d/%d et la case %d/%d  ? %f\n",start.x, start.y, start.x+1, start.y, dist);
    dist = distEucli(start.x, start.y, start.x+1, start.y+1);
    printf("distance entre la cas %d/%d et la case %d/%d  ? %f\n",start.x, start.y, start.x+1, start.y+1, dist);
    dist = distEucli(start.x, start.y, start.x+5, start.y-1);
    printf("distance entre la cas %d/%d et la case %d/%d  ? %f\n",start.x, start.y, start.x+5, start.y-1, dist);

    printf("\n\n");
    printf("...............test nb node................");
    printf("\n\n");

    cpt = nbNode(startWay);
    printf("nombre de noeud du chemin (expected 6) ? %d\n", cpt);

    printf("\n\n");
    printf("...............test path length................");
    printf("\n\n");

    dist = pathLength(startWay);
    printf("longueur du chemin de test (par pathLength) ? %f\n",dist);
    dist = fitness(startWay, &atlas);
    printf("longueur du chemin de test (par fitness) ? %f\n",dist);

    printf("\n\n");
    printf("...............test followDir................");
    printf("\n\n");

    act = start;
    printf("coord au départ : %d/%d\n", start.x, start.y);
    folowDir(&atlas, &act.x, &act.y, N);
    printf("coord apres followdir N ? %d/%d \n",act.x, act.y);
    act = start;
    folowDir(&atlas, &act.x, &act.y, E);
    printf("coord apres followdir E ? %d/%d \n",act.x, act.y);
    act = start;
    folowDir(&atlas, &act.x, &act.y, S);
    printf("coord apres followdir S ? %d/%d \n",act.x, act.y);
    act = start;
    folowDir(&atlas, &act.x, &act.y, W);
    printf("coord apres followdir W ? %d/%d \n",act.x, act.y);

    printf("\n\n");
    printf("...............test validStartingdir................");
    printf("\n\n");

    dir = validStartingDir(&atlas, start.x, start.y);
    printf("direction de départ pour la recherche en suivant le mur depuis la case %d/%d ? %d\n", start.x, start.y, dir);
    dir = validStartingDir(&atlas, start.x-1, start.y-1);
    printf("direction de départ pour la recherche en suivant le mur depuis la case %d/%d ? %d\n", start.x-1, start.y-1, dir);
    dir = validStartingDir(&atlas, 28, 4);
    printf("direction de départ pour la recherche en suivant le mur depuis la case %d/%d ? %d\n", 28, 4, dir);

    printf("\n\n");
    printf("...............test suivons le mur................");
    printf("\n\n");

    path = generateFirstPath(&atlas, start, end);
    displayPath(path, &atlas);
    printf("la carte ne doit pas etre modifié :\n");
    printMap(&atlas);

    printf("\n\n");
    printf("...............On arrive à suivre le mur youpi................");
    printf("\n\n");

    printf("\n\n");
    printf("...............test recuit simulé................");
    printf("\n\n");

    printf("\n\n");
    printf("...............test savoir si un noeaud est dans un chemin................");
    printf("\n\n");


    zero.x = 0;
    zero.y = 0;

    act = start;
    act.x++;
    act.y--;

    inPath = isNodeInPath(start, path);
    printf("est ce que la case %d/%d est dans le chemin (start) ? %d\n",start.x, start.y, inPath);
    inPath = isNodeInPath(end, path);
    printf("est ce que la case %d/%d est dans le chemin (end) ? %d\n",end.x, end.y, inPath);
    inPath = isNodeInPath(zero, path);
    printf("est ce que la case %d/%d est dans le chemin (zero) ? %d\n",zero.x, zero.y, inPath);
    inPath = isNodeInPath(act, path);
    printf("est ce que la case %d/%d est dans le chemin (start+1) ? %d\n",act.x, act.y, inPath);

    printf("\n\n");
    printf("...............test savoir si pop est possible................");
    printf("\n\n");

    printPath(path);
    pop(path, 0);
    printPath(path);
    pop(path, 1);
    printPath(path);
    pop(path, 5);
    printPath(path);

    printf("\n\n");
    printf("...............test savoir si morphe possible................");
    printf("\n\n");

    printPath(path);
    morph(path, 1, 1, 0, &atlas);
    printPath(path);
    morph(path, 1, 1, 2, &atlas);
    printPath(path);
    morph(path, 1, -1, 5, &atlas);
    printPath(path);

    printf("\n\n");
    printf("...............test savoir si add possible................");
    printf("\n\n");

    printPath(path);
    random.x = 0;
    random.y = 0;
    add(path, random.x, random.y, 2, &atlas);
    printPath(path);
    random.x = 1;
    random.y = 1;
    add(path, random.x, random.y, 2, &atlas);
    printPath(path);
    random.x = 2;
    random.y = 2;
    add(path, random.x, random.y, 2, &atlas);    
    printPath(path);

    printf("\n\n");
    printf("...............test mutate................");
    printf("\n\n");

    path = generateFirstPath(&atlas, start, end);
    printf("chemin sans mutation : \n");
    printPath(path);
    printf("chemin apres 10 mutations aléatoires : \n");
    for(i=0; i<10; i++) {
        mutate(path, &atlas);
    }
    printf("\n");
    printPath(path);

    printf("\n\n");
    printf("...............test generation de voisin................");
    printf("\n\n");

    path = generateFirstPath(&atlas, start, end);
    printf("chemin de départ : \n");
    printPath(path);
    displayPath(path, &atlas);
    for(i=0; i<10; i++) {
        neighbors[i] = generateNeighbor(path, &atlas);
        printf("voisin n°%d :\n",i);
        printPath(neighbors[i]);
        displayPath(neighbors[i], &atlas);
    }

    printf("\n\n");
    printf("...............test choose best neighbor................");
    printf("\n\n");

    bestNeighbor = chooseBestNeighbor(neighbors, 10, &atlas);
    printf("fitness bestNeighbor = %f\n", fitness(bestNeighbor, &atlas));
    for(i=0; i<10; i++) {
        printf("fitness voisin n°%d = %f\n", i, fitness(neighbors[i], &atlas));
    }
    
    printf("\n\n");
    printf("...............test simulated annealing................");
    printf("\n\n");

    path = generateFirstPath(&atlas, start, end);
    printf("add path = %p\n",path);
    printPath(path);
    printf("\n\n");
    path = simulatedAnnealing(path, &atlas);
    printPath(path);

    printf("\n\n");
    printf("...............test gas consumption................");
    printf("\n\n");

    path = generateFirstPath(&atlas, start, end);
    gas = gasConsumptionForPath(path);
    printf("gas consommé pour le chemin de base? %d\n", gas);
    path = simulatedAnnealing(path, &atlas);
    gas = gasConsumptionForPath(path);
    printf("gas consommé pour le chemin modifié par SA? %d\n", gas);

    return 0;
}*/