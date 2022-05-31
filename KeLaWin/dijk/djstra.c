#include "dij.h"

Graph generate_graph(Map atlas){
    int e;
    int i, j, k;
    int x, y;
    char tile;
    Vertex vert;
    Pos2Dint position;
    float dist;
    int vertexcount = 0;
    int speed;
    int countgoal=0;

    Graph graph = init_graph(atlas.width*atlas.heigth, atlas.width, atlas.heigth);
    graph.goalvertices = malloc(atlas.goalTileCount* sizeof(Vertex));
    graph.goalTileCount = atlas.goalTileCount;
    /* Add vertices to the list */
    /* Use of an arraymap to allow the search of an vertex through coordinates alone*/
    for (j=0; j<map.height; j++){
        for (i=0; i<map.width; i++){
            tile = getTile(i, j, map);
            if (tile != OUT_CHAR){
                position.x = i;
                position.y = j;
                vert = init_vertex(position, tile, vertexcount);
                graph.vertices[vertexcount] = vert;
                graph.arraymap[j*map.width+i] = vertexcount;
                if (tile == GOAL_CHAR){
                    graph.goalvertices[countgoal]=vert;
                    countgoal++;
                }
                vertexcount++;
            }
        }
    }
    /* Add edges to the matrix */
    for (i=0; i<vertexcount; i++){
        speed = MAX_SPEED;
        if (graph.vertices[i].tile==SAND_CHAR){
            speed=MAX_SAND_SPEED;
        }
        x = graph.vertices[i].pos.x;
        y = graph.vertices[i].pos.y;
        for (j=y-speed; j<=y+speed; j++){
            for (k=x-speed; k<=x+speed; k++){
                if (getTile(k, j, map)!=OUT_CHAR && areVerticesConnected(x,y,k,j,map)){
                    e = graph.arraymap[j*map.width+k];
                    if (e!=NILL){
                        dist = getDistance(x, y, k, j);
                        dist = (dist<=speed) ? dist : INFINITY;
                        
                        setVerticesDistance(&graph, i, e, k-x, j-y, dist);
                    }
                }
            }
        }
    }
    return graph;
}

Path *dijkstraPath(Graph graph, int source, float speed, int speedX, int speedY, int dest){
/*  Function to find the shortest path */
/*  with minimum edges in a graph */
/*  Code inspired by : https://www.geeksforgeeks.org/dijkstras-shortest-path-with-minimum-edges */
    
    int i, u, v, count;
    float *dist = malloc(graph.size * sizeof(float));
    int *distPath = calloc(graph.size, sizeof(int));
    int *Blackened = calloc(graph.size, sizeof(int));
    int *pathlength=  calloc(graph.size, sizeof(int));
    int *parent = calloc(graph.size, sizeof(int));
    Edge *parentSpeed = calloc(graph.size , sizeof(Edge));
    Path *path = NULL;
    float max_accel = getDistance(0, 0, 1 ,1);
    max_accel = 1;
    /*  The parent Of the source vertex is always equal to nill */
    parent[source] = NILL;
    parentSpeed[source].distance = speed;
    parentSpeed[source].distX = speedX;
    parentSpeed[source].distY = speedY;
    /*  first, we initialize all distances to infinity. */
    for (i = 0; i < graph.size; i++){
        dist[i] = INFINITY;
        distPath[i] = INFINITY;
    }
    dist[source] = 0;
    distPath[source] = 0;
    for (count = 0; count < graph.size - 1; count++) {
        u = minDistance(dist, Blackened, graph.size);
        if (u == INFINITY){
            break;
        } 
        Blackened[u] = 1;
        for (v = 0; v < graph.size; v++) {            
            if (Blackened[v] == 0 && getVerticesDistance(graph, u, v).distance != 0
            && getVerticesDistance(graph, u, v).distance != INFINITY
            && distPath[u] + 1  < distPath[v]
            && parentSpeed[u].distX + max_accel >= getVerticesDistance(graph, u, v).distX && parentSpeed[u].distX - max_accel <= getVerticesDistance(graph, u, v).distX
            && parentSpeed[u].distY + max_accel >= getVerticesDistance(graph, u, v).distY && parentSpeed[u].distY - max_accel <= getVerticesDistance(graph, u, v).distY
            && getVerticesDistance(graph, u, v).distance <=5){
                parent[v] = u;
                pathlength[v] = pathlength[parent[v]] + 1;
                dist[v] = dist[u] + getVerticesDistance(graph, u, v).distance;
                distPath[v] = distPath[u] + 1;
                parentSpeed[v] = getVerticesDistance(graph, u, v);
                

            } else if (Blackened[v] == 0 && getVerticesDistance(graph, u, v).distance != 0 
            && getVerticesDistance(graph, u, v).distance != INFINITY
            && distPath[u] == distPath[v] && pathlength[u] + 1 <= pathlength[v]
            && parentSpeed[u].distX + max_accel >= getVerticesDistance(graph, u, v).distX && parentSpeed[u].distX - max_accel <= getVerticesDistance(graph, u, v).distX
            && parentSpeed[u].distY + max_accel >= getVerticesDistance(graph, u, v).distY && parentSpeed[u].distY - max_accel <= getVerticesDistance(graph, u, v).distY
            && getVerticesDistance(graph, u, v).distance <=5)
            {   
                parent[v] = u;
                pathlength[v] = pathlength[u] + 1;
                parentSpeed[v] = getVerticesDistance(graph, u, v);
            }            
        }
    }
    /* Printing the path */
    if (dist[dest] != INFINITY){
        getPath(parent, dest, &path);
        return path;
    } else{

        fprintf(stderr, "There is no path between vertex ");
        fprintf(stderr, "%d to vertex %d\n",  source ,dest );
        return NULL;
    }
}