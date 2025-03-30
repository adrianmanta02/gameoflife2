#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

//---------------------------------------------------DEFINED---------------------------------------------------
#define ALIVE 'X'
#define DEAD '+'

// Structure for list
typedef struct list
{
    int l; // Line for the cell
    int c; // Column for the cell
    struct list *next; 
}
list; 

// Structure for the stack; it keeps information about each list's head. 
typedef struct stack
{
    list *item; // Stack of pointers
    struct stack *next;  
}
stack; 

//---------------------------------------------------FUNCTIONS---------------------------------------------------

// Funtion to verify if the text file opens correctly. 
void verify_opening(const FILE *file, const char *filename)
{
    if (file == NULL)
    {
        printf("Error while trying to open the file %s\n", filename);
        exit(1); 
    }
}

// This function adds (line, column) for the cell that is chaniging state into a list. 
void add_list(list **head, int current_line, int current_column)
{
    list *current;
    list *new_pair = (list *)malloc(sizeof(list));  // Memory allocation for a new node.
    if (new_pair == NULL)
    {
        printf("Error while trying to allocate memory to new entry in list!\n"); 
        exit(1); 
    }

    new_pair -> l = current_line; 
    new_pair -> c = current_column;
    new_pair -> next = NULL;

    if (*head == NULL) // It is the first element in the list, therefore is the head of it. 
    {
        *head = new_pair; 
    }
    else
    {   
        current = *head;
        while (current -> next != NULL) // "Looping" through the list to find the ending.
        {
            current = current -> next; 
        }
        // Add the node at the end. 
        current -> next = new_pair;
    }   
}

void display_list(list *head, FILE *file)
{
    list *current = head; 
 
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

void add_stack(stack **StackHead, list *head)
{ 
    stack *newStackNode = (stack *)malloc(sizeof(stack));
    if (newStackNode == NULL)
    {
        printf("Error while trying to allocate memory for a new stack node!\n"); 
        exit(1);
    }

    newStackNode -> item = head; // Adding the head of the list in the stack;
    newStackNode -> next = NULL; 

    // Adding the node to the list, taking into account that it could be the first element as well. 
    if (*StackHead == NULL)
    {
        *StackHead = newStackNode; 
        return ; 
    }

    // Stack has already list heads inside of it;
    stack *current = *StackHead; 
    while (current -> next != NULL)
    {
        current = current -> next; 
    }
    current -> next = newStackNode;
}

void display_stack(stack *StackHead, FILE *file)
{
    list *head = NULL;    
    stack *current = StackHead;
    int generation = 1; 
    while (current != NULL)
    {
        head = current -> item; 
        fprintf(file, "%d ", generation); 
        display_list(head, file); // The stack is made out of nodes. Displaying the stack means to display each list  
        current = current -> next; 
        generation++; 
    }
}

// Reading the frame from the input file. 
void read_frame(FILE *file, char **frame, int lines, int columns)
{
    int counter = 0; // Count each element while looping in the file. 
    char state; 

    while (counter < lines*columns)
    {
        fscanf(file, " %c", &state); 
        frame[counter / columns][counter % columns] = state; // change the rows and columns when needed. 
        counter++;
    }
}

// Takes each element in the frame to check info about neighbours. 
void check_around(char **frame, int lines, int columns, int current_line, int current_column, list **head)
{
    char current_cell; 
    int i,j, count_dead = 0, count_alive = 0; // counters for dead & alive cells around. 
    
    current_cell = frame[current_line][current_column]; 
    
    // Each current cell has a 'border' around (there are max. 8 neighbours). 
    for (i = current_line - 1; i <= current_line + 1; i++)
    {
        for (j = current_column - 1; j <= current_column + 1; j++)
        {
            if (i >= 0 && i < lines && j >= 0 && j < columns) // Check if the limits of the frame are not surpassed. 
            {
                // Count dead/alive cells around. 
                if (frame[i][j] == ALIVE) count_alive++; 
                else if (frame[i][j] == DEAD) count_dead++;
            }  
        }
    }

    if (current_cell == ALIVE)
    {
        count_alive --; // Substract one from alives, because it counted the current cell too. 
        if (count_alive < 2 || count_alive > 3) 
        {
            // This cell should die due to underpopulation (when it has less than 2 neighbours) or due to overpopulation (more than 3 neighbours).
            add_list(head, current_line, current_column); // Changed state --> add to the list.           
        } 
    }

    else if (current_cell == DEAD && count_alive == 3)
    { 
        // This dead cell should revive.
        add_list(head, current_line, current_column); // Changed state --> add to the list. 
    }
}

// Function for applying changes after checking around for each cell. 
void make_changes(char **frame, list **head)
{
    list *current = *head; 

    while (current != NULL)
    {
        int line = current -> l; 
        int column = current -> c; 
     
        // Change the state for each cell that was identified in the check_around function
        if(frame[line][column] == DEAD)
        {
            frame[line][column] = ALIVE;
        }
        else
        {
            frame[line][column] = DEAD; 
        }

        current = current -> next; // Go to the next cell in the list. 
    }
}

void free_list(list *head)
{
    list *current = head; 
    while (current != NULL)
    {
        list *temporary = current; 
        current = current -> next; 
        free(temporary); 
    }
    free(head); 
}

void free_stack_memory(stack **StackHead, list *head)
{ 
    if (StackHead == NULL)
    {
        return ; // Already empty. 
    }   

    stack *current = *StackHead;
    while (current != NULL)
    {
        stack *temporary = current; 
        current = current -> next; 
        free_list(temporary -> item);
        free(temporary); 
    }
    free(*StackHead); 
    *StackHead = NULL;
}

//---------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    char **frame;
    int task, lines, columns, generations, i, j, k;
    list *head = NULL; 
    stack *StackHead = NULL;   
    
    // Reading from file. 
    FILE *file = fopen(argv[1], "rt"); 
    verify_opening(file, argv[1]); 
    fscanf(file, "%d%d%d%d", &task, &lines, &columns, &generations);
    
    // Memory allocation for the frame. 
    frame = (char **)malloc(lines * sizeof(char*)); 
    if (frame == NULL)
    {
        printf("Error while trying to allocate memory for lines!\n"); 
        exit(1);
    }
    for (i = 0; i < lines; i++)
    {
        *(frame + i) = (char *)malloc(columns * sizeof(char)); 
        if (frame[i] == NULL)
        {
            printf("Error while trying to allocate memory for columns!\n"); 
            exit(1); 
        }
    }

    // Reading the frame from the file
    read_frame(file, frame, lines, columns); 
    fclose(file);
     
    for (k = 0; k < generations; k++)
    {
        head = NULL; // For each generation, there will be different lists. 
        // Checking around any cell
        for (i = 0 ; i < lines; i++)
        {
            for (j = 0; j < columns; j++)
            {  
                check_around(frame, lines, columns, i, j, &head);
            }
        }

        add_stack(&StackHead, head); // Place the list's head in the stack. 

        // Once storing data about cells, modify the frame (completing one generation)    
        make_changes(frame, &head);    
    }

    file = fopen(argv[2], "wt"); // Opening the output file and verifing if the operation has been completed succesfully. 
    verify_opening(file, argv[2]);
    display_stack(StackHead, file); 
    fclose(file); 

    // Free memory for the frame and the stack(as well as the nodes in the stack). 
    for (i = 0; i < lines; i++)
    {
        free(frame[i]); 
    }
    free(frame);

    free_stack_memory(&StackHead, head);
    free(head);  
}
