#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

//---------------------------------------------------DEFINED---------------------------------------------------
#define ALIVE 'X'
#define DEAD '+'

//Structure used to store information about the cells that are changing their state. 
typedef struct frame_change
{
    int line_to_change; 
    int column_to_change;
    char state; 
}
frame_change;

//---------------------------------------------------FUNCTIONS---------------------------------------------------

// Funtion to verify if the text file opens correctly. 
void verify_opening(const FILE *file, const char *filename)
{
    if(file == NULL)
    {
        printf("Error while trying to open the file %s\n", filename);
        exit(1); 
    }
}

// Reading the frame from the input file. 
void read_frame(FILE *file, char **frame, int lines, int columns)
{
    int counter = 0; // Count each element while looping in the file. 
    char state; 

    while(counter < lines*columns)
    {
        fscanf(file, " %c", &state); 
        frame[counter / columns][counter % columns] = state; // change the rows and columns when needed. 
        counter++;
    }
}

void display_frame_in_file(char **frame, int lines, int columns, const FILE *file)
{
    int i,j;

    for(i = 0; i < lines; i++)
    {
        for(j = 0; j < columns; j++)
        {
            fprintf((FILE *)file, "%c", *(*(frame+ i) + j)); 
        }
        fprintf((FILE *)file, "\n"); 
    }
    fprintf((FILE *)file, "\n");
}

// Takes each element in the frame to check info about neighbours. 
void check_around(char **frame, int lines, int columns, int current_line, int current_column, frame_change **array, int *index)
{
    char current_cell; 
    int i,j, count_dead = 0, count_alive = 0; // counters for dead & alive cells around. 
    
    current_cell = frame[current_line][current_column]; 
    
    // Each current cell has a 'border' around (there are max. 8 neighbours).
    for(i = current_line - 1; i <= current_line + 1; i++)
    {
        for(j = current_column - 1; j <= current_column + 1; j++) // Check if the limits of the frame are not surpassed. 
        {
            if (i >= 0 && i < lines && j >= 0 && j < columns) 
            {
                // Count dead/alive cells around.
                if(frame[i][j] == ALIVE) count_alive++;
                else if(frame[i][j] == DEAD) count_dead++;
            }  
        }
    }

    if(current_cell == ALIVE)
    {
        count_alive --; // Substract one from alives, because it counted the current cell too. 
        if(count_alive < 2 || count_alive > 3)
        {
            // This cell should die due to underpopulation (when it has less than 2 neighbours) or due to overpopulation (more than 3 neighbours).
            current_cell = DEAD;
            // Once the state of the current cell is switching, we store the line and column from the frame in the array. 
            
            *array = (frame_change *)realloc(*array, (*index + 1) * sizeof(frame_change)); 
            if(*array == NULL)
            {
                printf("Error while trying to reallocate memory to the array in function check_around!\n");
                exit(1);
            }
            
            // Add information in the array of structures to the cells that are changing the current state
            (*array + *index)->column_to_change = current_column; 
            (*array + *index)->line_to_change = current_line; 
            (*array + *index)->state = current_cell; 
            (*index)++;

        } 
    }

    else if(current_cell == DEAD && count_alive == 3)
    {  
        // This dead cell should revive.
        current_cell = ALIVE;

        *array = (frame_change *)realloc(*array, (*index + 1) * sizeof(frame_change)); 
        if(*array == NULL)
        {
            printf("Error while trying to reallocate memory to the array in function check_around!\n");
            exit(1);
        }

        // Add information in the array of structures to the cells that are changing the current state.
        (*array + *index)->column_to_change = current_column; 
        (*array + *index)->line_to_change = current_line; 
        (*array + *index)->state = current_cell; 
        (*index)++;
    }
}

// Function for applying changes after checking around for each cell. 
void make_changes(char **frame, const frame_change *array, int index)
{
    for(int i = 0; i<index; i++)
    {
        int line = array[i].line_to_change; 
        int column = array[i].column_to_change;
        char state = array[i].state;  
        frame[line][column] = state; 
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
int main(int argc, const char *argv[])
{
    char **frame;
    int task, lines, columns, generations, i, j, k, index;
    frame_change *array = NULL;

    // Reading from file. 
    FILE *file = fopen(argv[1], "rt"); 
    verify_opening(file, argv[1]);
    fscanf(file, "%d%d%d%d", &task, &lines, &columns, &generations);
    
    // Memory allocation for the frame. 
    frame = (char **)malloc(lines * sizeof(char*)); 
    if(frame == NULL)
    {
        printf("Error while trying to allocate memory for lines!\n"); 
        exit(1);
    }
    for(i = 0; i < lines; i++)
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

    file = fopen(argv[2], "wt"); 
    verify_opening(file, argv[2]);
    display_frame_in_file(frame, lines, columns, file); // Initial display of the frame (gen 0). 

    for(k = 0; k < generations; k++)
    {
        index = 0;
        //Memory allocation for the array (it is storing data for the cells that are changing their state)
        array = (frame_change *)malloc(sizeof(frame_change)); 
        if(array == NULL)
        {
            printf("Error while trying to allocate memory for array in main()!\n"); 
            exit(1); 
        }
    
        // Checking around any cell
        for(i = 0 ; i < lines; i++)
        {
            for(j = 0; j < columns; j++)
            { 
                check_around(frame, lines, columns, i, j, &array, &index);
            }
        }

        // Once storing data about cells, modify the frame (completing one generation)
        make_changes(frame, array, index); 
        free(array);
        display_frame_in_file(frame, lines, columns, file);
    }

    fclose(file); 
    for(i = 0; i < lines; i++)
    {
        free(frame[i]); 
    }
    free(frame); 
}
