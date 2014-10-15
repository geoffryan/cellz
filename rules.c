#include "rules.h"

int rule000(int a, int b, int c)
{
    if(a==0 && b==0 && c==0) return 0;
    if(a==0 && b==0 && c==1) return 0;
    if(a==0 && b==1 && c==0) return 0;
    if(a==0 && b==1 && c==1) return 0;
    if(a==1 && b==0 && c==0) return 0;
    if(a==1 && b==0 && c==1) return 0;
    if(a==1 && b==1 && c==0) return 0;
    if(a==1 && b==1 && c==1) return 0;
    return 0;
}

int rule022(int a, int b, int c)
{
    if(a==0 && b==0 && c==0) return 0;
    if(a==0 && b==0 && c==1) return 1;
    if(a==0 && b==1 && c==0) return 1;
    if(a==0 && b==1 && c==1) return 0;
    if(a==1 && b==0 && c==0) return 1;
    if(a==1 && b==0 && c==1) return 0;
    if(a==1 && b==1 && c==0) return 0;
    if(a==1 && b==1 && c==1) return 0;
    return 0;
}
