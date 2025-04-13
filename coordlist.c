#include "coordlist.h"

// This function adds (line, column) for the cell that is chaniging state into a list. 
void addList(listNode **head, int current_line, int current_column)
{
    listNode *current;
    listNode *newPair = (listNode *)malloc(sizeof(listNode));  // Memory allocation for a new node.
    if (newPair == NULL)
    {
        printf("Error while trying to allocate memory to new entry in list!\n"); 
        exit(1); 
    }

    // Setting atributes to the new node. 
    newPair -> l = current_line; 
    newPair -> c = current_column;
    newPair -> next = NULL;

    if (*head == NULL) // It is the first element in the list, therefore is the head of it. 
    {
        *head = newPair; 
        return ; 
    }
    else // The list has items already. 
    {   
        current = *head;
        while (current -> next != NULL) // "Looping" through the list to find the ending.
        {
            current = current -> next; 
        }
        // Add the node at the end. 
        current -> next = newPair;
    }   
}

void displayList(listNode *head, FILE *file)
{
    listNode *current = head; 
 
    while (current != NULL)
    { 
        // Avoiding to print a space at the final of a row. 
        if (current -> next == NULL)
            fprintf(file, "%d %d", current -> l, current ->c);
        else
            fprintf(file, "%d %d ", current->l, current->c);  
        current = current -> next; 
    }
    fprintf(file, "\n"); 
} 

void freeList(listNode **head)
{
    listNode *current = *head; 
    while (current != NULL)
    {
        listNode *temporary = current; 
        current = current -> next; 
        free(temporary); 
    }
    *head = NULL;
}
