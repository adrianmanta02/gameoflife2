#ifndef COORDLIST_H
#define COORDLIST_H

#include <stdio.h>
#include <stdlib.h>

// Structure for list
typedef struct listNode
{
    int l; // Line for the cell
    int c; // Column for the cell
    struct listNode *next; 
}
listNode; 

void addList(listNode **head, int current_line, int current_column); 
void displayList(listNode *head, FILE *file); 
void freeList(listNode **head); 

#endif // COORDLIST_H
