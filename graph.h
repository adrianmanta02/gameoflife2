#ifndef GRAPH_H
#define GRAPH_H

#include "coordlist.h"

typedef struct
{
    int V; 
    int E;
    int **adjacency;    
}
Graph;

typedef struct 
{
    int *compVertices; 
    int compLength; 
}
connection;

int* longestHamiltonianInGraph(Graph *g, connection *connectedComponents, int componentsNumber, int *pathLen);
int *findPathInComponent(Graph *g, connection component);
bool buildHamiltonianPath(Graph *g, int *path, int depth, int currentNode, int *visited, int targetLength);
connection* DFS (Graph *g, int *componentNumber);
Graph *createGraph(char **frame, int lines, int columns, listNode **array, int index);
void printHamiltonianPath(const int *path, int pathLength, listNode** aliveNodes, FILE *file);
void sortComponents(connection *components, int componentsNumber);

#endif // GRAPH_H
