# Game of Life – C Simulation Project

##  What is it?

The **Game of Life** is a cellular automaton devised by John Conway that demonstrates how simple rules can lead to complex behaviors. This project simulates the game using **C** and various data structures (linked lists, stacks, trees, graphs).

Each cell in a square grid can be either alive (X) or dead (+), and it interacts with up to 8 neighboring cells. Rules that govern the life/death status of a cell are described [here](https://playgameoflife.com/), in the **Explanation** tab.


##  Dependencies

- OS: Linux / WSL / macOS
- Compiler: GCC (>= 9.3)
- Tools: `git`, `valgrind`, `cppcheck`, `make`
- Required headers:
  - `coordlist.h`
  - `genstack.h`
  - `graph.h`
  - `tree.h`


##  Installation & Checker Setup

1. Download the checker binary from [GitLab - Checker Releases](https://gitlab.cs.pub.ro/paaa/checker/-/releases) based on your OS (e.g., WSL users should choose "Linux AMD64 Binary").

The checker has an interface that is giving a grade based on style, refs to output matching, memory management and git commits. More details can be found at: [Checker Documentation](https://gitlab.cs.pub.ro/paaa/checker/-/blob/master/README.md?ref_type=heads). 


2. Install required packages:
   ```bash
   sudo apt update # Update the package repositories
   sudo apt install valgrind # Install valgrind
   sudo apt install cppcheck # Install cppcheck
   sudo apt install git # Install git (if not already installed)

3. Building the checker

    ```bash
    make build-linux # ELF executable
    make build-windows # Win32 executable
    make build-macos # OSX executable


## Project Structure
  
    • main.c – Entry point of the simulation.
    • frame.c – Handles frame generation and evolution.
    • coordlist.c, genstack.c, graph.c, tree.c – Data structure implementations.
    • *.h – Corresponding header files for modular structure.


## How to use

Inside the project directory:   
    
    ```bash
    make # Compiles the project
    make run # Run the checker with input from .in files
    make bonus # Run custom bonus input (without checker validation). Results are found in bonus_out 
    make clear # Clears compiled files
    
To enable guided tutorial steps, set "tutorial": true in config.json. 



## Overview

This project aims to simulate generations for the Game of life using data structures and algorithms which are divided into multiple tasks.



## Task 1 - Basic Rules Simulation 
    • Uses a 2D matrix to represent the frame. 
    • Two implementation approaches: 
        1. Helper frame with pointer switching (createNextGeneration). 
        2. Structure array (frame_change) using "viaArray" functions. (placed at the bottom of the frame.c as commented lines. Using this alternative requires changing the main.c file’s case 1 with the final comment in the same file.)
    Each cell is being "watched around"; the number of dead and alive neigbours decide the state of the selected cell.


## Task 2 -  Efficient Difference Storage

    • Store only coordinates of changed cells per generation. 
    • A stack of linked lists is used. 
    • Key function: checkAroundOldRule() - does a count-up for a cell's neighbours, adds the changed cells into linked lists. 


## Bonus - Reverse to Generation 0

    • Use the stack and the last frame at the Kth generation to reconstruct the initial state frame. 
    • Important functions: getStackGenerations() - takes input from custom files in bonus_in directory; 
        generateInitialFrame() - makes pop operations and updates the frame. 



## Task 3 - Rule Switching via Binary Tree
    • Simulate both old and new rules: 
        • Old: Classic Game of Life rules; 
        • New: A cell with exactly 2 alive neighbors becomes ALIVE
    • Tree Node stores changed cell coordinates
    • Tree built recursively via TreeBuilder(), using a helper frame that is being restored between recursive calls. 



## Task 4 - Longest Hamiltonian Path
    • Alive cells = graph vertices - obtained via getAliveNodes()
    • Use DFS to find connected components 
    • Search for the longest Hamiltonian path via backtracking - buildHamiltonianPath()
    • Only the largest components are used for performance


## Contributing 
Pull requests are welcome. Please follow the code style used and ensure changes are tested before submitting. :D
