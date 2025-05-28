#include "genstack.h"
#include <string.h>

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

void displayStack(stackNode *StackHead, FILE *file)
{
    // Since the first generation is all the way to the bottom of the stack, 
    // a temporary stack is needed to display generations in order. 
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

// Takes data from a file and converts it into a stack of linked lists. 
stackNode* getStackGenerations(FILE *input)
{
    int generation;  
    char charline[1024]; 

    stackNode *stackHead = NULL; 
       
    while (fscanf(input, "%d", &generation) == 1)
    {    
        listNode *listHead = NULL; 
        int counter = 0, line, column;
        
        // Reading each line which represents a stack frame and extract each number. Assign the numbers 
        // to line or column for the list nodes. Each list(head) is pushed into the stack. 
        fgets(charline, sizeof(charline), input); 
        const char *sequence = strtok(charline, " "); 
        
        while (sequence != NULL)
        { 
            int number = atoi(sequence);
            sequence = strtok(NULL, " ");
            if (counter % 2 == 0)   
                line = number; 
            else
            {
                column = number;
                addList(&listHead, line, column); 
            } 
            counter++; 
        }
        push(&stackHead, listHead); 
    } 
    return stackHead;  
}

// Changes the frame at the K generation into the initial one using a stack with information about cells 
// which are switching their state at each generation, until the Kth one. 
void generateInitialFrame(stackNode *stackHead, char **frame, int lines, int columns, FILE *output)
{      
    while (stackHead != NULL)
    { 
        listNode *listHead = pop(&stackHead);  
        makeChanges(frame, listHead);  
    } 
    displayFrame(frame, lines, columns, output);  
}
