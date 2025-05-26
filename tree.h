#ifndef TREE_H
#define TREE_H

#include "coordlist.h"
#include "frame.h"
#include "graph.h"

typedef struct TreeNode
{
    listNode *headList;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

TreeNode* createTreeNode(listNode *head); 
int treeHeight(TreeNode *root); 
void freeTree(TreeNode *root); 
void TreeBuilder(TreeNode *root, char **tempFrame, int lines, int columns, int level, int generations); 
void preOrder(TreeNode *root, char **frame, int lines, int columns, int level, FILE *file); 
void printLevel(TreeNode *root, int level, FILE *file); 
void Hamilton4Nodes(TreeNode *root, char **tempFrame, int lines, int columns, int level, FILE *file);

#endif // TREE_H
