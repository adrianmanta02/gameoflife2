#ifndef GENSTACK_H
#define GENSTACK_H

#include "coordlist.h"
#include "frame.h"

// Structure for the stack; it keeps information about each list's head. 
typedef struct stackNode
{
    listNode *item; // Stack of pointers
    struct stackNode *next;  
}
stackNode; 

void push(stackNode **StackHead, listNode *head); 
listNode* pop(stackNode **StackHead); 
void displayStack(stackNode *StackHead, FILE *file); 
void freeStackMem(stackNode **StackHead); 
stackNode* getStackGenerations(FILE *input);
void generateInitialFrame(stackNode *stackHead, char **frame, int lines, int columns, FILE *output);

#endif // GENSTACK_H
