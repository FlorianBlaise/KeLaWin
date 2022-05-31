#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "path.h"

#define BOOSTS_AT_START 5

int main()
{
  int row;                                                                        
  int width, height;                                                              
  int gasLevel;                                                                   
  int boosts = BOOSTS_AT_START;                                                   
  int round = 0;                                                                  
  int accelerationX = 1, accelerationY = 0;                                       
  int speedX = 0, speedY = 0;                                                     
  char action[100];                                                               
  char line_buffer[MAX_LINE_LENGTH];

  Path* path;
  Coord start, end;
  Map atlas;                                           

  boosts = boosts;                       /* Prevent warning "unused variable" */  
  fgets(line_buffer, MAX_LINE_LENGTH, stdin);      /* Read gas level at Start */  
  sscanf(line_buffer, "%d %d %d", &width, &height, &gasLevel);   

  fprintf(stderr, "=== >Map< ===\n");                                             
  fprintf(stderr, "Size %d x %d\n", width, height);                               
  fprintf(stderr, "Gas at start %d \n\n", gasLevel);

  atlas.heigth = height;
  atlas.width = width;
  atlas.startingGas = gasLevel;

  atlas.map = (char **) malloc( atlas.heigth*sizeof(char *));
  for (row = 0; row < height; row++) {        /* Read map data, line per line */  
    atlas.map[row] = (char *) malloc( atlas.width*sizeof(char));
    fgets(atlas.map[row],MAX_LINE_LENGTH, stdin);   /* on stock la carte dans line buffer */     
    fputs(atlas.map[row], stderr);                   /* on la print dans la sortie d'erreur */                                
  }
                                                                           
  fflush(stderr);                                                                 
  fprintf(stderr, "\n=== Race start ===\n");                                      

  findEndPoint(&atlas, &end.x, &end.y);
  
  while (!feof(stdin)) {                                                          
    int myX, myY, secondX, secondY, thirdX, thirdY;                               
    round++;                                                                      
    fprintf(stderr, "=== ROUND %d\n", round);                                     
    fflush(stderr);                                                               
    fgets(line_buffer, MAX_LINE_LENGTH, stdin);   /* Read positions of pilots */  
    sscanf(line_buffer, "%d %d %d %d %d %d",                                      
           &myX, &myY, &secondX, &secondY, &thirdX, &thirdY);                     
    fprintf(stderr, "    Positions: Me(%d,%d)  A(%d,%d), B(%d,%d)\n",             
            myX, myY, secondX, secondY, thirdX, thirdY);                          
    fflush(stderr);                                                                                                                  

    /*------------modifier cette partie pour le trajet quitte à la déplacer en haut------------*/
    start.x = myX;
    start.y = myY;

    if(round == 1 ) {
      path = generateFirstPath(&atlas, start, end);
    }
    fprintPath(path,"out");
    fprintf(stderr, "pos = %d-%d/ next = %d-%d",start.x, start.y, path->next->coord.x, path->next->coord.y);

    accelerationX = (path->next->coord.x-start.x) - speedX;
    accelerationY = (path->next->coord.y-start.y) - speedY;
                              
    gasLevel += gasConsumption(accelerationX, accelerationY, speedX, speedY, 0);  
    speedX += accelerationX;                                                      
    speedY += accelerationY;

    path=path->next;

    /*------------fin de la partie à modifier-------------*/

    /* Write the acceleration request to the race manager (stdout). */
    sprintf(action, "%d %d", accelerationX, accelerationY);
    fprintf(stdout, "%s", action);
    fflush(stdout);                           /* CAUTION : This is necessary  */
    fprintf(stderr, "    Action: %s   Gas remaining: %d\n", action, gasLevel);
    fflush(stderr);
    if (0 && round > 4) { /* (DISABLED) Force a segfault for testing purpose */
      int * p = NULL;
      fprintf(stderr, "Good Bye!\n");
      fflush(stderr);
      *p = 0;
    }
  }

  return EXIT_SUCCESS;
}
