#include "frame.h"
#include "coordlist.h"

#define ALIVE 'X'
#define DEAD '+'

// Funtion to verify if the text file opens correctly. 
void verifyOpening(const FILE *file, const char *filename)
{
    if (file == NULL)
    {
        printf("Error while trying to open the file %s\n", filename);
        exit(1); 
    }
}

// Reading the frame from the input file.
void readFrame(FILE *file, char **frame, int lines, int columns, listNode **object, bool toggle) //boolean variable used depending on the task number
{
    int counter = 0; // Count each element while looping in the file.
    char state;

    while (counter < lines * columns)
    {
        int i = counter / columns;
        int j = counter % columns;

        fscanf(file, " %c", &state);
        if (state == ALIVE && toggle == true)
            addList(object, i, j); // Adding the alive cells' coordinates into the initial object list, useful for task3.
        frame[i][j] = state;        // change the rows and columns when needed.
        counter++;
    }
}

void displayFrame(char **frame, int lines, int columns, FILE *file)
{
    int i, j;

    for (i = 0; i < lines; i++)
    {
        for (j = 0; j < columns; j++)
        {
            fprintf(file, "%c", *(*(frame + i) + j)); // frame[i][j]
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\n");
}

// Takes each element in the frame to check info about its neighbours.
void countNeigbours(char **frame, int lines, int columns, int current_line, int current_column, int *count_alive, int *count_dead)
{
    *count_alive = 0;
    *count_dead = 0;

    char current_cell = frame[current_line][current_column];

    // Each current cell has a 'border' around (there are max. 8 neighbours).
    for (int i = current_line - 1; i <= current_line + 1; i++)
    {
        for (int j = current_column - 1; j <= current_column + 1; j++)
        {
            if (i >= 0 && i < lines && j >= 0 && j < columns) // Check if the limits of the frame are not surpassed.
            {
                // Count dead/alive cells around.
                if (frame[i][j] == ALIVE)
                    (*count_alive)++;
                else
                    (*count_dead)++;
            }
        }
    }

    // Avoid counting the current cell.
    if (current_cell == ALIVE)
        (*count_alive)--;
    else
        (*count_dead)--;
}

// Takes each element in the frame to check info about neighbours - following Standard Rule. Also creates a list of coordinates. 
void checkAroundOldRule(char **frame, int lines, int columns, int current_line, int current_column, listNode **head)
{
    char current_cell;
    int count_alive, count_dead;

    current_cell = frame[current_line][current_column];
    countNeigbours(frame, lines, columns, current_line, current_column, &count_alive, &count_dead);

    if (current_cell == ALIVE)
    {
        if (count_alive < 2 || count_alive > 3)
        {
            // This cell should die due to underpopulation (when it has less than 2 neighbours) or due to overpopulation (more than 3 neighbours).
            addList(head, current_line, current_column); // Changed state --> add to the list.
        }
    }

    else if (current_cell == DEAD && count_alive == 3)
    {
        // This dead cell should revive.
        addList(head, current_line, current_column); // Changed state --> add to the list.
    }
}

// Takes each element in the frame to check info about neighbours - following B Rule, the newer one.
void checkAroundNewRule(char **frame, int lines, int columns, int current_line, int current_column, listNode **head)
{
    int count_alive = 0, count_dead = 0;
    char current_cell = frame[current_line][current_column];

    if (current_cell == DEAD)
    {
        countNeigbours(frame, lines, columns, current_line, current_column, &count_alive, &count_dead);
        if (count_alive == 2)
        {
            addList(head, current_line, current_column);
        }
    }
}

// Function for applying changes after checking around for each cell. 
void makeChanges(char **frame, listNode *head)
{
    listNode *current = head; 

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

void freeFrameMem(char ***frame, int lines)
{
    for (int i = 0; i < lines; i++)
    {
        free((*frame)[i]);
    }
    free(*frame);
    *frame = NULL;
}

char** cloneMatrix(char **mainFrame, int lines, int columns) 
{
    char **copiedFrame = (char **)malloc(lines * sizeof(char*));
    if (copiedFrame == NULL)
    {
        printf("Error while trying to allocate memory for the copiedFrame!\n"); 
        exit(1); 
    }

    for (int i = 0; i < lines; i++) 
    {
        copiedFrame[i] = (char *)malloc(columns * sizeof(char));
        memcpy(copiedFrame[i], mainFrame[i], columns * sizeof(char));
    }
    return copiedFrame;
}

void restoreChanges(char **frame, char **modifiedFrame, int lines, int columns)
{
    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            frame[i][j] = modifiedFrame[i][j]; 
        }
    }
}

void resetFrame(char **frame, int lines, int columns)
{
    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            frame[i][j] = DEAD;
        }
    }
}

// Function to create a frame using the coordinates stored in a list. 
void createFrameUsingCoord(char **frame, int lines, int columns, listNode *head)
{
    listNode *current = head;
 
    resetFrame(frame, lines, columns); 
    while (current != NULL)
    {
        int listLine = current->l;
        int listColumn = current->c;
        if (listLine >= 0 && listLine < lines && listColumn >= 0 && listColumn < columns)
            frame[listLine][listColumn] = ALIVE;
        current = current->next;
    }
}

// Useful for task1 
void addToArray(frame_change **array, int *index, int current_column, int current_line, char current_cell)
{
    *array = (frame_change *)realloc(*array, (*index + 1) * sizeof(frame_change)); 
    if (*array == NULL)
    {
        printf("Error while trying to allocate memory to the array in function addToArray!\n");
        exit(1); 
    }

    (*array + *index)->line_to_change = current_line; 
    (*array + *index)->column_to_change = current_column; 
    (*array + *index)->state = current_cell; 
    (*index)++; 
}

void makeChangesViaArray(char **frame, const frame_change *array, int index)
{
    for (int i = 0; i<index; i++)
    {
        int line = array[i].line_to_change; 
        int column = array[i].column_to_change;
        char state = array[i].state;  
        frame[line][column] = state; 
    }
}

void checkAroundViaArray(char **frame, int lines, int columns, int current_line, int current_column, frame_change **array, int *index)
{
    char current_cell; 
    int count_dead = 0, count_alive = 0; // counters for dead & alive cells around. 
    
    current_cell = frame[current_line][current_column]; 
    countNeigbours(frame, lines, columns, current_line, current_column, &count_alive, &count_dead); 

    if (current_cell == ALIVE)
    { 
        if (count_alive < 2 || count_alive > 3)
        {
            // This cell should die due to underpopulation (when it has less than 2 neighbours) or due to overpopulation (more than 3 neighbours).
            current_cell = DEAD;
            
            // Once the state of the current cell is switching, we store the line and column from the frame in the array. 
            addToArray(array, index, current_column, current_line, current_cell); 
        } 
    }

    else if (current_cell == DEAD && count_alive == 3)
    {  
        // This dead cell should revive.
        current_cell = ALIVE;
        addToArray(array, index, current_column, current_line, current_cell); 
    }
}