#ifndef FRAME_H
#define FRAME_H

#include "coordlist.h"
#include <string.h>
#include <stdbool.h>

//Structure used to store information about the cells that are changing their state. -- used at task1
typedef struct frame_change
{
    int line_to_change; 
    int column_to_change;
    char state; 
}
frame_change;

void verifyOpening(const FILE *file, const char *filename); 
void readFrame(FILE *file, char **frame, int lines, int columns, listNode **object, bool toggle);
void displayFrame(char **frame, int lines, int columns, FILE *file);
void countNeigbours(char **frame, int lines, int columns, int current_line, int current_column, int *count_alive, int *count_dead);
void checkAroundOldRule(char **frame, int lines, int columns, int current_line, int current_column, listNode **head); 
void checkAroundNewRule(char **frame, int lines, int columns, int current_line, int current_column, listNode **head);
void makeChanges(char **frame, listNode *head);
void freeFrameMem(char ***frame, int lines); 
char** cloneMatrix(char **mainFrame, int lines, int columns); 
void restoreChanges(char **frame, char **modifiedFrame, int lines, int columns); 
void resetFrame(char **frame, int lines, int columns); 
void createFrameUsingCoord(char **frame, int lines, int columns, listNode *head); 
void addToArray(frame_change **array, int *index, int current_column, int current_line, char current_cell); 
void makeChangesViaArray(char **frame, const frame_change *array, int index);
void checkAroundViaArray(char **frame, int lines, int columns, int current_line, int current_column, frame_change **array, int *index); 

#endif // FRAME_H
