#ifndef GENSTACK_H
#define GENSTACK_H

#include "coordlist.h"

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

#endif // GENSTACK_H
