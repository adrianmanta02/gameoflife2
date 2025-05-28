#include "tree.h"

// Creates a perfect binary tree. Each node has a linked list as usable information. 
TreeNode *createTreeNode(listNode *head)
{
    // Memory allocation for a tree node. 
    TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
    if (newNode == NULL)
    {
        printf("Error while trying to allocate memory for new node in the tree!\n");
        exit(1);
    }

    newNode->right = NULL;
    newNode->left = NULL;

    //Assigning information to node's field
    newNode->headList = head;
    return newNode;
}

int treeHeight(TreeNode *root)
{
    int heightLeft, heightRight;
    
    if (root == NULL)
        return -1;

    heightLeft = treeHeight(root->left);
    heightRight = treeHeight(root->right);
    return 1 + ((heightLeft > heightRight) ? heightLeft : heightRight);
}

void freeTree(TreeNode *root) 
{
    if (root == NULL) return;
    
    freeTree(root->left); 
    freeTree(root->right);
    
    if (root->headList != NULL) 
    {
        freeList(&root->headList); 
        root->headList = NULL;
    }

    free(root);
}

// Builds a tree according to 2 rules of generations creation. 
void TreeBuilder(TreeNode *root, char **tempFrame, int lines, int columns, int level, int generations)
{
    if (root == NULL || level >= generations)
    {
        return; 
    } 

    // If not at the root level, apply the changes according to the coordinates in the list. 
    if (level != 0)
    {
        makeChanges(tempFrame, root->headList); 
    }

    // Initialization for each tree node's usable information
    listNode *headNew = NULL; 
    listNode *headOld = NULL; 

    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            // Creating both lists, depending on the rules.
            checkAroundNewRule(tempFrame, lines, columns, i, j, &headNew); 
            checkAroundOldRule(tempFrame, lines, columns, i, j, &headOld); 
        }
    } 

    //Using a back-up frame in order to keep track of the before generation (parent's configuration). 
    char **backupFrame = cloneMatrix(tempFrame, lines, columns);

    TreeNode *leftNode = createTreeNode(headNew);
    TreeNode *rightNode = createTreeNode(headOld);

    root->left = leftNode; 
    root->right = rightNode;

    TreeBuilder(root->left, tempFrame, lines, columns, level + 1, generations); // New rule config. 
    restoreChanges(tempFrame, backupFrame, lines, columns); // Come back to the parent's configuration to generate the old-rule right son. 
    TreeBuilder(root->right, tempFrame, lines, columns, level + 1, generations); // Old rule config. 

    freeFrameMem(&backupFrame, lines);
}

// Displays the frame associated with every node traversed in preorder. 
void preOrder(TreeNode *root, char **frame, int lines, int columns, int level, FILE *file)
{
    if (root == NULL)
    {
        return ; 
    }

    if (level != 0)
        makeChanges(frame, root -> headList); 

    displayFrame(frame, lines, columns, file); 
        
    char **backupFrame = cloneMatrix(frame, lines, columns); 
    
    preOrder(root -> left, frame, lines, columns, level + 1, file); 
    restoreChanges(frame, backupFrame, lines, columns); 
    preOrder(root -> right, frame, lines, columns, level + 1, file); 

    freeFrameMem(&backupFrame, lines);  
}

void printLevel(TreeNode *root, int level, FILE *file)
{
    if (root == NULL) return; 
    if (level == 1) 
    {   
        displayList(root->headList, file);  
    }
    else if (level > 1)
    {
        printLevel(root -> left, level + 1, file); 
        printLevel(root -> right, level + 1, file); 
    }
}

// Displays the longest hamiltonian path for each node in the binary tree. 
void Hamilton4Nodes(TreeNode *root, char **tempFrame, int lines, int columns, int level, FILE *file) 
{
    // while advancing in the tree, build the current frame, get its alive cells
    // create the associated graph
    // search for its connected components a.k.a 'connection' structures
    // once all connected components are stored, sort them because the higher chance to find the longest 
    // hamiltonian path is in the longest component.
     
    if (root == NULL) return;

    // the root level doesn't need any changes
    if (level != 0)
        makeChanges(tempFrame, root->headList);

    // get alive cells for the graph
    int aliveCount = 0;  
    listNode **aliveNodes = getAliveCells(tempFrame, lines, columns, &aliveCount); 
    
    // build the graph 
    Graph *g = createGraph(tempFrame, lines, columns, aliveNodes, aliveCount);

    // get the connected components
    int componentCount = 0;
    connection *components = DFS(g, &componentCount);
    
    // sort the components from longest to smallest
    sortComponents(components, componentCount); 

    // get the hamiltonian path 
    int pathLen = 0; 
    int *path = longestHamiltonianInGraph(g, components, componentCount, &pathLen, aliveNodes);

    printHamiltonianPath(path, pathLen, aliveNodes, file); 
    
    // ---------------- free-ing zone -----------------------------
    free(path);
    path = NULL; 

    for (int i = 0; i < aliveCount; i++) 
    {
        free(aliveNodes[i]);
    }
    free(aliveNodes);
    aliveNodes = NULL; 

    for (int i = 0; i < componentCount; i++) 
    {
        free(components[i].compVertices);
    }
    free(components);
    components = NULL; 

    for (int i = 0; i < g->V; i++) 
    {
        free(g->adjacency[i]);
    }

    free(g->adjacency);
    g->adjacency = NULL;

    free(g);
    g = NULL; 
    // ------------------------------------------------------------

    // clone current frame for using it while coming back from the recursion
    char **backupFrame = cloneMatrix(tempFrame, lines, columns);
    
    Hamilton4Nodes(root->left, tempFrame, lines, columns, level + 1, file);
    // came back, restore the matrix at its previous configuration
    restoreChanges(tempFrame, backupFrame, lines, columns);

    // head into the right part
    Hamilton4Nodes(root->right, tempFrame, lines, columns, level + 1, file);
    
    freeFrameMem(&backupFrame, lines);
}
