#ifndef COORDLIST_H
#define COORDLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
bool isAround(int current_line, int current_column, int line_to_check, int column_to_check); 
listNode** getAliveCells(char **frame, int lines, int columns, int *aliveCount); 

#endif // COORDLIST_H
