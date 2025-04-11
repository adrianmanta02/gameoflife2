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
} list;

typedef struct TreeNode
{
    list *headList;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

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
    list *new_pair = (list *)malloc(sizeof(list)); // Memory allocation for a new node.
    if (new_pair == NULL)
    {
        printf("Error while trying to allocate memory to new entry in list!\n");
        exit(1);
    }

    new_pair->l = current_line;
    new_pair->c = current_column;
    new_pair->next = NULL;

    if (*head == NULL) // It is the first element in the list, therefore is the head of it.
    {
        *head = new_pair;
    }
    else
    {
        current = *head;
        while (current->next != NULL) // "Looping" through the list to find the ending.
        {
            current = current->next;
        }
        // Add the node at the end.
        current->next = new_pair;
    }
}

void display_list(list *head, FILE *file)
{
    list *current = head;

    while (current != NULL)
    {
        // Avoiding to print a space at the final of a row.
        if (current->next == NULL){
            fprintf(file, "%d %d", current->l, current->c);
            printf("%d %d", current-> l, current -> c);
        } 
        else{
            fprintf(file, "%d %d ", current->l, current->c);
            printf("%d %d", current-> l, current -> c);}
        current = current->next;
    }
    fprintf(file, "\n");
}

void free_list(list *head) 
{
    while (head != NULL) 
    {
        list *temp = head;
        head = head->next;
        free(temp);
    }
}
// Reading the frame from the input file.
void read_frame(FILE *file, char **frame, int lines, int columns, list **object)
{
    int counter = 0; // Count each element while looping in the file.
    char state;

    *object = NULL;

    while (counter < lines * columns)
    {
        int i = counter / columns;
        int j = counter % columns;

        fscanf(file, " %c", &state);
        if (state == ALIVE)
            add_list(object, i, j); // Adding the alive cells' coordinates into the initial object list.
        frame[i][j] = state;        // change the rows and columns when needed.
        counter++;
    }
}

void display_frame_in_file(char **frame, int lines, int columns, FILE *file)
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

// Takes each element in the frame to check info about neighbours.
void checkAroundOldRule(char **frame, int lines, int columns, int current_line, int current_column, list **head)
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
            add_list(head, current_line, current_column); // Changed state --> add to the list.
        }
    }

    else if (current_cell == DEAD && count_alive == 3)
    {
        // This dead cell should revive.
        add_list(head, current_line, current_column); // Changed state --> add to the list.
    }
}

void checkAroundNewRule(char **frame, int lines, int columns, int current_line, int current_column, list **head)
{
    int count_alive = 0, count_dead = 0;
    char current_cell = frame[current_line][current_column];

    if (current_cell == DEAD)
    {
        countNeigbours(frame, lines, columns, current_line, current_column, &count_alive, &count_dead);
        if (count_alive == 2)
        {
            add_list(head, current_line, current_column);
        }
    }
}

// Function for applying changes after checking around for each cell.
void make_changes(char **frame, list *head)
{
    list *current = head;

    while (current != NULL)
    {
        int line = current->l;
        int column = current->c; 

        // Change the state for each cell that was identified in the check_around function
        if (frame[line][column] == DEAD)
        {
            frame[line][column] = ALIVE;
        }
        else
        {
            frame[line][column] = DEAD;
        }
        current = current->next; // Go to the next cell in the list.
    }
}

TreeNode *createTreeNode(list *head)
{
    // Memory allocation for a tree node. 
    TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
    if (newNode == NULL)
    {
        printf("Error while trying to allocate memory for new node in the tree!\n");
        exit(1);
    }

    newNode->right = NULL;
    newNode->left = NULL;

    //Assigning information to node's field
    newNode->headList = head;
    return newNode;
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

int treeHeight(TreeNode *root)
{
    int heightLeft, heightRight;
    
    if (root == NULL)
        return -1;

    heightLeft = treeHeight(root->left);
    heightRight = treeHeight(root->right);
    return 1 + ((heightLeft > heightRight) ? heightLeft : heightRight);
}

void freeTree(TreeNode *root) 
{
    if (root == NULL) return;
    
    freeTree(root->left); 
    freeTree(root->right);
    
    if (root->headList != NULL) 
    {
        free_list(root->headList); 
        root->headList = NULL;
    }

    free(root);
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
void createFrameUsingCoord(char **frame, int lines, int columns, list *head)
{
    list *current = head;
 
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


void TreeBuilder(TreeNode *root, char **tempFrame, int lines, int columns, int level, int generations, FILE *file)
{
    if (root == NULL || level >= generations)
    {
        return; 
    } 

    // If not at the root level, apply the changes according to the coordinates in the list. 
    if (level != 0)
    {
        make_changes(tempFrame, root->headList); 
    }

    // Initialization for each tree node's usable information
    list *headNew = NULL; 
    list *headOld = NULL; 

    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            // Creating both lists, depending on the rules.
            checkAroundNewRule(tempFrame, lines, columns, i, j, &headNew); 
            checkAroundOldRule(tempFrame, lines, columns, i, j, &headOld); 
        }
    } 

    char **backupFrame = cloneMatrix(tempFrame, lines, columns);

    TreeNode *leftNode = createTreeNode(headNew);
    TreeNode *rightNode = createTreeNode(headOld);

    root->left = leftNode; 
    root->right = rightNode;

    TreeBuilder(root->left, tempFrame, lines, columns, level + 1, generations, file);
    restoreChanges(tempFrame, backupFrame, lines, columns); 
    TreeBuilder(root->right, tempFrame, lines, columns, level + 1, generations, file);

    freeFrameMem(&backupFrame, lines);
}

void preOrder(TreeNode *root, char **frame, int lines, int columns, int level, FILE *file)
{
    if (root == NULL)
    {
        return ; 
    }

    if (level != 0)
        make_changes(frame, root -> headList); 

    display_frame_in_file(frame, lines, columns, file); 
        
    char **backupFrame = cloneMatrix(frame, lines, columns); 
    
    preOrder(root -> left, frame, lines, columns, level + 1, file); 
    restoreChanges(frame, backupFrame, lines, columns); 
    preOrder(root -> right, frame, lines, columns, level + 1, file); 

    freeFrameMem(&backupFrame, lines);  
}

int main(int argc, char *argv[])
{
    char **frame;
    int task, lines, columns, generations; 

    // Reading from file.
    FILE *file = fopen(argv[1], "rt");
    verify_opening(file, argv[1]);
    fscanf(file, "%d%d%d%d", &task, &lines, &columns, &generations);

    if (task == 3)
    {
        // Memory allocation for the frame.
        frame = (char **)malloc(lines * sizeof(char *));
        if (frame == NULL)
        {
            printf("Error while trying to allocate memory for lines!\n");
            exit(1);
        }
        for (int i = 0; i < lines; i++)
        {
            *(frame + i) = (char *)malloc(columns * sizeof(char));
            if (frame[i] == NULL)
            {
                printf("Error while trying to allocate memory for columns!\n");
                exit(1);
            }
        }

        list *initialListHead = NULL;
        read_frame(file, frame, lines, columns, &initialListHead);
        fclose(file);

        file = fopen(argv[2], "wt"); // Opening the output file and verifing if the operation has been completed succesfully.
        verify_opening(file, argv[2]);
        
        TreeNode *root = createTreeNode(initialListHead);
        TreeBuilder(root, frame, lines, columns, 0, generations, file); 
        
        createFrameUsingCoord(frame, lines, columns, initialListHead); 

        preOrder(root, frame, lines, columns, 0, file); 
        freeTree(root); 
        
        fclose(file); 
        freeFrameMem(&frame, lines); 
    }
}
