#include "tree.h"
#include "coordlist.h"
#include "frame.h"

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
