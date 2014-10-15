#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"
#include "cellz.h"
#include "rules.h"

/*
 * A simulator for 1D Cellular Automata, in the style of Wolfram.  Parts of 
 * the code are deliberately inefficient (ie. step() malloc'ing every time 
 * its call) to easier demonstrate speed gains with parallelism.
 */

// Global function pointer.  Points to rule used in simulation.
int (*rule)(int, int, int);

int rank;
int size;

// Advances *cell by a single iteration according to rule()
void step(int *cell, int len)
{
    int j;
    int *new_cell = (int *) malloc(len * sizeof(int));

    for(j=1; j<len-1; j++)
        new_cell[j] = rule(cell[j-1], cell[j], cell[j+1]);

    memcpy(cell, new_cell, len * sizeof(int));
    free(new_cell);
}

void synchronize(int *cell, int len)
{
    int L, R;

    L = rank-1;
    if(L == -1)
        L = size-1;
    R = (rank+1)%size;

    MPI_Sendrecv(&(cell[1]),     1, MPI_INT, L, 0,
                 &(cell[len-1]), 1, MPI_INT, R, 0, MPI_COMM_WORLD, NULL);
    MPI_Sendrecv(&(cell[len-2]), 1, MPI_INT, R, 0,
                 &(cell[0]),     1, MPI_INT, L, 0, MPI_COMM_WORLD, NULL);
}

//Prints the current *cell to a file fname.
void output(int *cell, int len, char fname[], char mode[])
{
    int j;
    FILE *f = fopen(fname, mode);
    for(j=1; j<len-1; j++)
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
        synchronize(cells, len);
        step(cells, len);
        output(cells, len, fname, "a");
    }
}

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(argc != 3)
    {
        if(rank == 0)
            printf("\nNeed to give a rule and a global size!\n\n");
        MPI_Finalize();
        return 0;
    }

    int choice, len_global;
    sscanf(argv[1], "%d", &choice);
    sscanf(argv[2], "%d", &len_global);

    //Set the rule to use.
    if(choice == 22)
        rule = &rule022;
    else if (choice == 30)
        rule = &rule030;
    else
        rule = &rule000;

    char fname[48];
    sprintf(fname, "out.dat.%03d", rank);

    len_global = size*(len_global/size);    //Size of the world!
    int len_local = len_global/size+2;      //Size of my world!
    int N = len_global;                     //Age of the universe.
    int *cells = (int *)malloc(len_local * sizeof(int));
    
    //Initialize
    int i;
    for(i=0; i<len_local; i++)
        cells[i] = 0;
    i = len_global/2;
    if(i >= rank*(len_local-2) && i < (rank+1)*(len_local-2))
        cells[i%(len_local-2) + 1] = 1;
    //cells[len_local/2] = 1;

    //Evolve
    evolve(cells, len_local, N, fname);

    //Clean
    free(cells);
    MPI_Finalize();

    return 0;
}
