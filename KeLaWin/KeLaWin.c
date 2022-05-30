#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./include/map.h"
#include "./include/path.h"
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

  Path path;
  Coord start, end;
  Map atlas;                                           

  boosts = boosts;                       /* Prevent warning "unused variable" */  
  fgets(line_buffer, MAX_LINE_LENGTH, stdin);      /* Read gas level at Start */  
  sscanf(line_buffer, "%d %d %d", &width, &height, &gasLevel);                    
  fprintf(stderr, "=== >Map< ===\n");                                             
  fprintf(stderr, "Size %d x %d\n", width, height);                               
  fprintf(stderr, "Gas at start %d \n\n", gasLevel);                              
  for (row = 0; row < height; ++row) {        /* Read map data, line per line */  
    fgets(line_buffer, MAX_LINE_LENGTH, stdin);   /* on stock la carte dans line buffer */                                
    fputs(line_buffer, stderr);                   /* on la print dans la sortie d'erreur */                                
  }                                                                               
  fflush(stderr);                                                                 
  fprintf(stderr, "\n=== Race start ===\n");                                      

  readAndCreateMap(&atlas, "input");
  fprintf(stderr, "1");
  findStartingPoint(&atlas, &start.x, &start.y);
  fprintf(stderr, "1");
  findEndPoint(&atlas, &end.x, &end.y);
  fprintf(stderr, "1");
  path = *generateFirstPath(&atlas, start, end);
  fprintf(stderr, "1");
  fprintMap(&atlas, "err.txt");
  fprintf(stderr, "1");
  fprintPath(&path, "err.txt");
  fprintf(stderr, "1");
  
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
    /* Gas consumption cannot be accurate here. */                                
    gasLevel += gasConsumption(accelerationX, accelerationY, speedX, speedY, 0);  
    speedX += accelerationX;                                                      
    speedY += accelerationY;                                                      

    /*------------modifier cette partie pour le trajet quitte à la déplacer en haut------------*/
    if (speedX > 4) {
      accelerationX = 0;
    }
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
