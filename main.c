/* ------------------------------------------
Program 1: Totalistic Cellular Automaton
    This program simulates a 1D totalistic cellular automaton.
    Rule (0-2186) is converted to a 7 digits in base 3 where each digit gives the next 
    generation status for a local sum in [0...6].
    The world starts with only the middle cell active with status 1 or 2.
    For each generation, every cell's next status is determined by its local sum 
    and the rule.
    It prints every evolution based on the user chosen generations and initial middle value.
    It also vertically shows the accumulated counts per cell at the end. 

Course: CS 211, Fall 2025, UIC
Author: Eldin Nokic
------------------------------------------- */

#include <stdio.h>
#include <stdbool.h>

const int WORLD_SIZE = 65;

typedef struct cell_struct{
    int localSum; // total sum of local cells, [left] + [me] + [right]: possible values are 0-6
    int status;   // this cell's current status: 0, 1, or 2
    int count;    // running accumulated count of this cell's active status for all generations
} cell;

/*
    Purpose: Converts a decimal rule (0...2186) into 7 base-3 digits.
             Index = local sum (0..6), value = next status (0,1,2).
    Params:  valArray[7] - receives the 7 digits.
             rule - integer rule in [0..2186].
    Returns: true if the rule is valid, otherwise false.
*/
bool setValArray(int valArray[7], int rule) {
        if (rule < 0 || rule > 2186) { // rule must be in valid range
            return false; 
        }
        for (int i = 0; i < 7; i++) {
            valArray[i] = rule % 3; // get base-3 digit
            rule = rule / 3;
        }
        return true;
}

/*
    Purpose: Computes local sums for each cell. Wrap around at edges.
    Params:  world[WORLD_SIZE] -  reads status, writes localSum.
    Returns: (none).
*/
void setSums(cell world[WORLD_SIZE]) {
    for (int i = 0; i < WORLD_SIZE; i++) {
        int left = (i == 0) ? WORLD_SIZE - 1 : i - 1;
        int right = (i == WORLD_SIZE - 1) ? 0 : i + 1;
        int addUp = world[left].status + world[i].status + world[right].status;
        world[i].localSum = addUp;
    }
    return;
}

/*
    Purpose:  Evolves the world by one generation using the rule array.
              Updates statuses, increments counts.
    Params:   world[WORLD_SIZE] - reads localSum, writes status and count.
              ruleValArray[7] - mapping from local sum to next status.
    Returns: The sum of all statuses in this generation.
*/
int evolveWorld(cell world[WORLD_SIZE], int ruleValArray[7]) {
    int total = 0;
    for (int i = 0; i < WORLD_SIZE; i++) {
        world[i].status = ruleValArray[world[i].localSum];
        world[i].count += world[i].status;
        total += world[i].status;
    }
    return total;
}

/*
    Purpose:  Converts the status value (0,1,2) to a character ('+', '-', ' ').
    Params:   s - status value (0,1 or 2).
    Returns:  '+' for 2, '-' for 1, ' ' for 0.
*/
char statusToChar(int s) {
    if (s == 2) {
        return '+'; // 2 is '+'
    }
    if (s == 1) {
        return '-'; // 1 is '-'
    }
    return ' '; // 0 is blank space
}

int main() {
    cell world[WORLD_SIZE];
    int rule;
    int ruleValArray[7];
    bool ok = false;

    printf("Welcome to the Totalistic Cellular Automaton!\n");

    // Input rule number and validate
    while (!ok) {
        printf("Enter the rule # (0-2186): ");
        scanf("%d", &rule);
        ok = setValArray(ruleValArray, rule);
    }
    
    // Show rule mapping
    printf("\nThe value array for rule #%d is ", rule);
    for (int i = 6; i >= 0; i--) {
        printf("%d", ruleValArray[i]);
    }

    printf("\n\n");
    printf("The evolution of all possible states are as follows:\n");
    printf("Local Sum:   6       5       4       3       2       1       0      \n");
    printf("States:    |+++|   |++-|   |++ |   |+- |   |-- |   |-  |   |   |    \n");
    printf("                   |+-+|   |+ +|   |+ -|   |- -|   | - |            \n");
    printf("                   |-++|   | ++|   |-+ |   | --|   |  -|            \n");
    printf("                           |+--|   | +-|   |+  |                    \n");
    printf("                           |-+-|   |- +|   | + |                    \n");
    printf("                           |--+|   | -+|   |  +|                    \n");
    printf("                                   |---|                            \n");
    printf("New Status: |%c|     |%c|     |%c|     |%c|     |%c|     |%c|     |%c|     \n\n",
        statusToChar(ruleValArray[6]), statusToChar(ruleValArray[5]), statusToChar(ruleValArray[4]),
        statusToChar(ruleValArray[3]), statusToChar(ruleValArray[2]), statusToChar(ruleValArray[1]), statusToChar(ruleValArray[0]));      
    
    int lifeSpan = 0;
    int initialCell = 0;

    // Input lifespan and number of generations
    while (lifeSpan < 1 || lifeSpan > 49) {
        printf("Enter the number of generations (1-49): ");     
        scanf("%d", &lifeSpan);
    }
    
    // Input initial active cell value
    while (initialCell != 1 && initialCell != 2) {
        printf("Enter the value (1 or 2) for the initial active cell: ");
        scanf("%d", &initialCell);
    }

    // Initialize world
    printf("\nInitializing world & evolving...\n");
    for (int i = 0; i < WORLD_SIZE; i++) {
        world[i].status = 0;
        world[i].count = 0;
        world[i].localSum = 0;
    }
    int middle = WORLD_SIZE / 2;
    world[middle].status = initialCell;
    world[middle].count = initialCell;

    setSums(world);
    
    int genTotal = 0;
    // Simulate generations
    for (int g = 0; g < lifeSpan; g++) {
        if (g == 0) {
            // Print initial state
            genTotal = 0;
            for (int i = 0; i < WORLD_SIZE; i++) {
                printf("%c", statusToChar(world[i].status));
                genTotal += world[i].status;
            }
            printf(" %d \n", genTotal);
        }
        else {
            // Evolve and print following states
            genTotal = evolveWorld(world, ruleValArray);
            setSums(world);
            for (int i = 0; i < WORLD_SIZE; i++) {
                printf("%c", statusToChar(world[i].status));
            }
            printf(" %d \n", genTotal);
        }
    }

    printf("_________________________________________________________________\n");
    // Print accumulated counts
    for (int i = 0; i < WORLD_SIZE; i++) {
        if (world[i].count < 10) {
            printf(" ");
        }
        else {
            printf("%d", world[i].count / 10);
        }
    }
    printf("\n");
    for (int i = 0; i < WORLD_SIZE; i++) {
        if (world[i].count < 10) {
            if (world[i].count == 0) {
                printf(" ");
            }
            else {
                printf("%d", world[i].count);
            }
        }
        else {
            printf("%d", world[i].count % 10);
        }
    }
    printf("\n"); 

    return 0;
}
