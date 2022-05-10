#include "map.h"
#include "path.h"

/*###########################################################################################*/
/*############################ Creation de la carte #########################################*/
/*###########################################################################################*/

int readAndCreateMap(Map* atlas, char* ficName) {
    char line[MAX_LINE_LENGTH];
    int i;
    int j;

    FILE* inputFile;

    j=0;
    inputFile = fopen(ficName, "r");

    if (inputFile == NULL) {
        printf("fichier non trouvé\n");
        return -1;
    } 

    fgets(line, MAX_LINE_LENGTH, inputFile);

    sscanf(line, "%d %d %d",&(atlas->width), &(atlas->heigth), &(atlas->startingGas));

    atlas->map = (char **) malloc( atlas->width*sizeof(char *));

    while (fgets(line, MAX_LINE_LENGTH, inputFile)) {
        atlas->map[j] = (char *) malloc( atlas->width*sizeof(char));
        for (i=0; i<atlas->width; i++) {
            atlas->map[j][i] = line[i];
        }
        j++;
    }
    return 0;
}

Map* copyMap(Map* atlas) {
    int i;
    int j;
    Map* copiedAtlas = (Map*) malloc(sizeof(Map));

    copiedAtlas->heigth = atlas->heigth;
    copiedAtlas->width = atlas->width;
    copiedAtlas->startingGas = copiedAtlas->startingGas;

    copiedAtlas->map = (char **) malloc( atlas->heigth*sizeof(char *));

    for(j=0; j<atlas->heigth; j++) {
        copiedAtlas->map[j] = (char *) malloc( atlas->width*sizeof(char));
        for (i=0; i<atlas->width; i++) {
            copiedAtlas->map[j][i] = atlas->map[j][i];
        }
    }
    return copiedAtlas;
}

/*###########################################################################################*/
/*#################################### print ################################################*/
/*###########################################################################################*/

void printMap(Map* atlas) {
    int i;
    int j;

    printf("heigth : %d, width : %d, gas : %d\n",atlas->heigth, atlas->width, atlas->startingGas);

    for (i=0; i<atlas->heigth; i++) {
        for (j=0; j<atlas->width; j++) {
            printf("%c", atlas->map[i][j]);
        }
        printf("\n");
    } 
}

void fprintMap(Map* atlas, char* file_txt) {
    int i;
    int j;

    FILE* file = fopen(file_txt,"a");

    if ( file == NULL ) {
        fprintf( stderr, "Cannot open file %s\n", file_txt);
        exit( 0 );
    }

    fprintf(file,"heigth : %d, width : %d, gas : %d\n",atlas->heigth, atlas->width, atlas->startingGas);

    for (i=0; i<atlas->heigth; i++) {
        for (j=0; j<atlas->width; j++) {
            fprintf(file,"%c", atlas->map[i][j]);
        }
        fprintf(file,"\n");
    }
    fclose(file);
}

/*###########################################################################################*/
/*###################################### Locate #############################################*/
/*###########################################################################################*/

void findStartingPoint(Map* atlas, int* i, int* j) {
    int k;
    int l;

    for (k=0; k<atlas->heigth; k++) {
        for (l=0; l<atlas->width; l++) {
            if (atlas->map[k][l]=='1') {
                *i = l;
                *j = k;
            }
        }
    }
}

void findEndPoint(Map* atlas, int* i, int* j) { /*s'arrete des qu'il trouve une arrivée!*/
    int y;
    int x;

    for (y=0; y<atlas->heigth; y++) {
        for (x=0; x<atlas->width; x++) {
            if (atlas->map[y][x]=='=') {
                *i = x;
                *j = y;
                break;
            }
        }
        if ( *i == x && *j == y) {
            break;
        }
    }
}

/*###########################################################################################*/
/*############################# to be or not to be ##########################################*/
/*###########################################################################################*/

int isMooveValid(int x1, int y1, int x2, int y2) {
    return distEucli(x1, y1, x2, y2) < sqrt(5) ? 1 : 0;
}

int isAWall(int x, int y, Map* atlas) {
    return atlas->map[y][x] == '.'? 1 : 0;
}

int isThereWallLeft(Map* atlas, int x, int y, enum DIRECTION dir) {
    if (dir == N) {
        if (x <= 0 || atlas->map[y][x-1] != '.') {
            return 0;
        }
        return 1;
    } else if (dir == E) {
        if (y <= 0 || atlas->map[y-1][x] != '.') {
            return 0;
        }
        return 1;
    } else if (dir == S) {
        if (x >= atlas->width || atlas->map[y][x+1] != '.') {
            return 0;
        }
        return 1;
    } else if (dir == W) {
        if (x >= atlas->heigth || atlas->map[y+1][x] != '.') {
            return 0;
        }
        return 1;
    }
    return 0;
}

int isThereWallForward(Map* atlas, int x, int y, enum DIRECTION dir) {
    if (dir == N) {
        if (y <= 0 || atlas->map[y-1][x] == '#') {
            return 0;
        }
        return 1;
    } else if (dir == E) {
        if (x >= atlas->width || atlas->map[y][x+1] != '.') {
            return 0;
        }
        return 1;
    } else if (dir == S) {
        if (y >= atlas->heigth || atlas->map[y+1][x] != '.') {
            return 0;
        }
        return 1;
    } else if (dir == W) {
        if (x <= 0 || atlas->map[y][x-1] != '.') {
            return 0;
        }
        return 1;
    }
    return 0;
}