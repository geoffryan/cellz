#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cellz.h"
#include "rules.h"

/*
 * A simulator for 1D Cellular Automata, in the style of Wolfram.  Parts of 
 * the code are deliberately inefficient (ie. step() malloc'ing every time 
 * its call) to easier demonstrate speed gains with parallelism.
 */

// Global function pointer.  Points to rule used in simulation.
int (*rule)(int, int, int);

// Advances *cell by a single iteration according to rule()
void step(int *cell, int len)
{
    int j;
    int *new_cell = (int *) malloc(len * sizeof(int));

    for(j=1; j<len-1; j++)
        new_cell[j] = rule(cell[j-1], cell[j], cell[j+1]);
    new_cell[0] = rule(cell[len-1], cell[0], cell[1]);
    new_cell[len-1] = rule(cell[len-2], cell[len-1], cell[0]);

    memcpy(cell, new_cell, len * sizeof(int));
    free(new_cell);
}

//Prints the current *cell to a file fname.
void output(int *cell, int len, char fname[], char mode[])
{
    int j;
    FILE *f = fopen(fname, mode);
    for(j=0; j<len; j++)
    {
        if(cell[j]==1)
            fprintf(f, "*");
        else
            fprintf(f, " ");
        //fprintf(f, "%d ", cell[j]);
    }
    fprintf(f, "\n");
    fclose(f);
}

//Evolves (initialized) cells N iterations, outputing each step.
void evolve(int *cells, int len, int N, char fname[])
{
    output(cells, len, fname, "w");
    
    int i,j;
    for(i=0; i<N; i++)
    {
        step(cells, len);
        output(cells, len, fname, "a");
    }
}

int main(int argc, char *argv)
{
    //Set the rule to use.
    rule = &rule030;

    int len = 100;  //Size of the world!
    int N = 200;    //Age of the universe.
    int *cells = (int *)malloc(len * sizeof(int));
    
    //Initialize
    int i;
    for(i=0; i<len; i++)
        cells[i] = 0;
    cells[len/2] = 1;

    //Evolve
    evolve(cells, len, N, "out.dat");

    //Clean
    free(cells);
    return 0;
}
