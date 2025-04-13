#include "genstack.h"

int emptyStack(const stackNode *StackHead)
{
    return StackHead == NULL; 
}

void push(stackNode **StackHead, listNode *head)
{
    stackNode *newNode = (stackNode *)malloc(sizeof(stackNode)); 
    if (newNode == NULL)
    {
        printf("Error while trying to allocate memory for the new stack node!\n"); 
        exit(1); 
    }

    newNode -> item = head; 
    newNode -> next = NULL; 

    newNode -> next = *StackHead; 
    *StackHead = newNode;    
}

// Deleting items from the stack. 
listNode* pop(stackNode **StackHead)
{
    if (emptyStack(*StackHead)) return NULL; 

    stackNode *temp = *StackHead;
    listNode *listPopped = temp -> item;  
    
    *StackHead = (*StackHead) -> next; 
    free(temp); 
    return listPopped; 
}

void freeStackMem(stackNode **StackHead)
{ 
    if (StackHead == NULL)
    {
        return ; // Already empty. 
    }   

    stackNode *current = *StackHead;
    while (current != NULL)
    {
        stackNode *temporary = current; 
        current = current -> next; 
        freeList(&temporary->item); // Free each list first. 
        free(temporary); 
    }
    *StackHead = NULL;
}

//Since the first generation is all the way to the bottom of the stack, a temporary stack is needed to display generations in order. 
void displayStack(stackNode *StackHead, FILE *file)
{
    stackNode *tempStackHead = NULL; 
    
    // Moving the items the the temporary stack (last generation will be now the first in the new stack). 
    while (!emptyStack(StackHead))
    {
        listNode *poppedHeadList = pop(&StackHead);
        push(&tempStackHead, poppedHeadList);
    }

    // Displaying.
    int generation = 1; 
    stackNode *current = tempStackHead;
    while (current != NULL)
    {
        listNode* head = current->item; 
        fprintf(file, "%d ", generation); 
        displayList(head, file); 
        current = current->next; 
        generation++; 
    }

    freeStackMem(&tempStackHead); 
}
