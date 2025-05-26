#include "graph.h"
#include "frame.h"

// Creates a graph using the alive cells in the frame. 
Graph *createGraph(char **frame, int lines, int columns, listNode **array, int index)
{
    Graph *g = (Graph *)malloc(sizeof(Graph)); 
    if (g == NULL)
    {
        printf("Error while trying to allocate memory for the graph!\n"); 
        exit(1);
    }

    g->V = index; 
    
    g -> adjacency = (int **)malloc(g -> V * sizeof(int *)); 
    if (g->adjacency == NULL)
    {
        printf("Error while trying to allocate memory for the matrix!\n"); 
        exit(1);
    }   
    
    for (int i = 0; i < g -> V; i++)
        g->adjacency[i] = (int *)calloc(g -> V, sizeof(int));   

    g->E = 0;

    // for each alive cell found, mark the connection with its neighbours in the adjacency matrix. 
    for (int i = 0; i < g->V; i++) {
        for (int j = i + 1; j < g->V; j++) {
            if (isAround(array[i]->l, array[i]->c, array[j]->l, array[j]->c)) 
            {
                g->adjacency[i][j] = 1;
                g->adjacency[j][i] = 1;
                g->E++;
            }
        }
    }
    return g; 
}

// Creates a connected component in a graph. 
int* DFS_scan (Graph *g, int visited [] , int i, int *component, int *nodesNumber)
{
    int j; visited[i] = 1;
    component[*nodesNumber] = i; //add the current graph node in the component
    (*nodesNumber)++;

    for(j = 0; j < g -> V; j++)
        if(g -> adjacency[i][j]==1 && visited [j]==0) // the node is linked to others and was not visited before
            DFS_scan (g, visited ,j, component, nodesNumber);

    return component;  
}

// Returns an array of connected components. Each component is an array of nodes and has a certain length. 
connection* DFS (Graph *g, int *componentNumber)
{
    int i, nodesNumber, visited[g->V];
    *componentNumber = 0;

    connection *array = (connection *)malloc(g->V * sizeof(connection));
    if (array == NULL)
    {
        printf("Error while trying to allocate memory for connected components array!\n"); 
        exit(1); 
    }

    // initialization for the visited nodes. 
    for (i = 0; i < g->V; i++) 
        visited [i]=0;
    
    for (i = 0; i < g->V; i++)
        if( visited[i] == 0)
        { 
            // current node is not visited
            nodesNumber = 0; 
            int *connectedComponent = (int *)malloc(g->V * sizeof(int));  

            // create the connected component starting from the current graph node
            connectedComponent = DFS_scan(g, visited ,i, connectedComponent, &nodesNumber);

            // add the connected componend found in the array. 
            array[*componentNumber].compVertices = connectedComponent; 
            array[*componentNumber].compLength = nodesNumber; 
            (*componentNumber)++; 
        }
    return array; 
}

bool buildHamiltonianPath(Graph *g, int *path, int depth, int currentNode, int *visited, int targetLength) 
{
    // once the maximum depth of the connected component was reached, the path was found. 
    if (depth == targetLength) return true;

    for (int v = 0; v < g->V; v++) 
    {
        // check if the current element is marked with 1 in the adjacency matrix and it was not visited
        if (g->adjacency[currentNode][v] == 1 && visited[v] == 0) {
            visited[v] = 1; // mark it as visited
            path[depth] = v; // add to path

            if (buildHamiltonianPath(g, path, depth + 1, v, visited, targetLength))
                return true;

            visited[v] = 0; // use backtracking if the previous path was not good 
        }
    }
    return false;
}

int *findPathInComponent(Graph *g, connection component) 
{
    int *visited = calloc(g->V, sizeof(int));
    int *path = malloc(component.compLength * sizeof(int));

    // loop through each node in the current connected component. 
    for (int i = 0; i < component.compLength; i++) 
    {
        int start = component.compVertices[i];
        // reinitialising the visited status for each connected component
        for (int j = 0; j < g->V; j++)
            visited[j] = 0; 

        // place the first element in the current connected component as the first in the hamiltonian path
        path[0] = start;
        visited[start] = 1; // mark it as visited 

        // check if the path was found for the current node and return it if so. 
        if (buildHamiltonianPath(g, path, 1, start, visited, component.compLength)) 
        {
            free(visited);
            visited = NULL; 
            return path;
        }
    }

    // no Hamiltonian path found, exit. 
    free(path);  free(visited);
    visited = NULL;  path = NULL;  
    return NULL; 
}

int* longestHamiltonianInGraph(Graph *g, connection *connectedComponents, int componentsNumber, int *pathLen)
{
    int *path =  NULL;  
    *pathLen = 0;

    // loop through each (sorted) connected components
    for (int i = 0; i < componentsNumber; i++)
    { 
        path = findPathInComponent(g, connectedComponents[i]);  
        if (path != NULL) // there is a valid Hamiltonian path
        {
            *pathLen = connectedComponents[i].compLength; 
            break;
        }  
    }   
    return path; 
}

// Sorts the connected components for a graph in a descending order in terms of their length. 
void sortComponents(connection *components, int componentsNumber)
{
    int sorted = 0; 
    while (sorted == 0)
    {
        sorted = 1;
        for (int i = 0; i < componentsNumber - 1; i++)
        {
            if (components[i].compLength < components[i+1].compLength)
            {
                connection aux = components[i]; 
                components[i] = components[i + 1]; 
                components[i + 1] = aux;
                sorted = 0;  

            }
        }
    }
}

void printHamiltonianPath(const int *path, int pathLength, listNode** aliveNodes, FILE *file)
{
    if (path == NULL)
    {
        fprintf(file, "%d\n", -1);
    }
    else
    {
        fprintf(file, "%d\n", pathLength - 1);
        for (int i = 0; i < pathLength; i++)
        {
            if (i == 0)
                fprintf(file, "(%d,%d)", aliveNodes[path[i]]->l, aliveNodes[path[i]]->c);
            else
                fprintf(file, " (%d,%d)", aliveNodes[path[i]]->l, aliveNodes[path[i]]->c);
        }
        fprintf(file, "\n");
    }
}
