#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cellz.h"
#include "rules.h"

int (*rule)(int, int, int);

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

void output(int *dat, int len, char fname[], char mode[])
{
    int j;
    FILE *f = fopen(fname, mode);
    for(j=0; j<len; j++)
    {
        if(dat[j]==1)
            fprintf(f, "*");
        else
            fprintf(f, " ");
        //fprintf(f, "%d ", dat[j]);
    }
    fprintf(f, "\n");
    fclose(f);
}

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
    rule = &rule022;

    int len = 100;
    int N = 200;
    int *cells = (int *)malloc(len * sizeof(int));
    
    int i;
    for(i=0; i<len; i++)
        cells[i] = 0;
    cells[len/2] = 1;

    evolve(cells, len, N, "out.dat");

    free(cells);
    return 0;
}
