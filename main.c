#include "genstack.h"
#include "tree.h"

int main(int argc, const char *argv[])
{
    char **frame;
    int task, lines, columns, generations;
    listNode *initialListHead = NULL;

    // Reading from file.
    FILE *file = fopen(argv[1], "rt");
    verifyOpening(file, argv[1]);
    fscanf(file, "%d%d%d%d", &task, &lines, &columns, &generations);

    // Memory allocation for the frame.
    //
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
    //

    switch (task)
    {
        case 1:
        {
            // Reading the frame from the file
            readFrame(file, frame, lines, columns, &initialListHead, false);
            fclose(file);

            file = fopen(argv[2], "wt");
            verifyOpening(file, argv[2]);

            // memory allocation for the frame that helps building the next generation. 
            char **nextframe = (char **)malloc(lines * sizeof(char *));
            if (nextframe == NULL)
            {
                printf("Error while trying to allocate memory for lines!\n");
                exit(1);
            }

            for (int i = 0; i < lines; i++)
            {
                *(nextframe + i) = (char *)malloc(columns * sizeof(char));
                if (nextframe[i] == NULL)
                {
                    printf("Error while trying to allocate memory for columns!\n");
                    exit(1);
                }
            }
            //

            displayFrame(frame, lines, columns, file); // Initial display of the frame (gen 0).

            for (int k = 0; k < generations; k++)
            {
                createNextGen(frame, nextframe, lines, columns);
                displayFrame(nextframe, lines, columns, file); // Generation completed -> display the result.

                // switch the pointers for each frame used to generate the current generation
                char **temp = frame;
                frame = nextframe;
                nextframe = temp;

            }
            freeFrameMem(&nextframe, lines); 
            break;
        }

        case 2: 
        {
            listNode *head = NULL; 
            stackNode *StackHead = NULL;
                    
            // Reading the frame from the file
            readFrame(file, frame, lines, columns, &initialListHead, false);
            fclose(file);

            file = fopen(argv[2], "wt");
            verifyOpening(file, argv[2]);
            
            for (int k = 0; k < generations; k++)
            {
                head = NULL; // For each generation, there will be different lists. 
                // Checking around any cell and creating the coordinates list. 
                for (int i = 0 ; i < lines; i++)
                {
                    for (int j = 0; j < columns; j++)
                    {  
                        checkAroundOldRule(frame, lines, columns, i, j, &head);
                    }
                }

                push(&StackHead, head); // Place the list's head in the stack. 

                // Once storing data about cells, modify the frame (completing one generation)    
                makeChanges(frame, head);    
            }
            displayStack(StackHead, file);
            freeStackMem(&StackHead);
            fclose(file); 
            break; 
        }

        case 3: 
        {
            // Reading the frame from the file
            readFrame(file, frame, lines, columns, &initialListHead, true);
            fclose(file);

            file = fopen(argv[2], "wt");
            verifyOpening(file, argv[2]);
            
            TreeNode *root = createTreeNode(initialListHead); 
            TreeBuilder(root, frame, lines, columns, 0, generations); // 0 stands from the starting level. 
            
            // After building the tree, the frame lost its initial values. Must reset it and give the values from the initialList. 
            createFrameUsingCoord(frame, lines, columns, initialListHead); 

            preOrder(root, frame, lines, columns, 0, file); 
            freeTree(root); 
            break; 
        }

        case 4:
        {
            // Reading the frame from the file
            readFrame(file, frame, lines, columns, &initialListHead, true);
            fclose(file);

            file = fopen(argv[2], "wt");
            verifyOpening(file, argv[2]);
            
            TreeNode *root = createTreeNode(initialListHead);
            TreeBuilder(root, frame, lines, columns, 0, generations);
            
            createFrameUsingCoord(frame, lines, columns, initialListHead); 
            Hamilton4Nodes(root, frame, lines, columns, 0, file); 
            
            freeTree(root); 
            break;
        }
        case 5: 
        {
            readFrame(file, frame, lines, columns, &initialListHead, false); 
            stackNode *stackHead = getStackGenerations(file); 
            fclose(file); 

            file = fopen(argv[2], "wt"); 
            verifyOpening(file, argv[2]); 
        
            generateInitialFrame(stackHead, frame, lines, columns, file); 
            break; 
        }
        default:
        {
            freeFrameMem(&frame, lines);
            fclose(file);
            break; 
        }
    }
    freeFrameMem(&frame, lines);
}





// task 1 alternative
/*
case 1:
{
    frame_change *array = NULL;
    int index; 

    // Reading the frame from the file
    readFrame(file, frame, lines, columns, &initialListHead, false);
    fclose(file);

    file = fopen(argv[2], "wt");
    verifyOpening(file, argv[2]);

    char **nextframe = (char **)malloc(lines * sizeof(char *));
    if (nextframe == NULL)
    {
        printf("Error while trying to allocate memory for lines!\n");
        exit(1);
    }

    for (int i = 0; i < lines; i++)
    {
        *(nextframe + i) = (char *)malloc(columns * sizeof(char));
        if (nextframe[i] == NULL)
        {
            printf("Error while trying to allocate memory for columns!\n");
            exit(1);
        }
    }

    displayFrame(frame, lines, columns, file); // Initial display of the frame (gen 0).

    for (int k = 0; k < generations; k++)
    {
        createNextGen(frame, nextframe, lines, columns);
        displayFrame(nextframe, lines, columns, file); // Generation completed -> display the result.
    }
    break;
}
*/ 