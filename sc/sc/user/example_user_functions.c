#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "systemic9.h"
#include "example_user_functions.h"


void user_initialise(char *name1,char *name2,char *name3,char *name4,systemic *s,int **scopetable)
// automatically called at the start of execution;
// up to four input and/or output filenames may be specified in scp.ini and will be passed here
{
    // declare memory
    // setup i/o files
}

void user_wrapup(char *name1,char *name2,char *name3,char *name4,systemic *s,int **scopetable)
// automatically called at the end of execution;
// up to four input and/or output filenames may be specified in scp.ini and will be passed here
{
    // free memory
    // finish writing and close files
}

int Pow(unsigned char *s1, unsigned char *s2)
// fills S12 with value of S12^S22, setting S22 to zero
// if value is too big, maximum possible value for S11 will be stored and 1 is returned, otherwise 0 is returned
{   int a, b, c;
    
    schematoi(s1,schemasize,&a);
    schematoi(s2,schemasize,&b);
    c = (int)pow((double)a, (double)b);
    itoschema(c,s1);
    zero(s2,s2);
    
    return (c <= (int)pow(2.0,schemasize-2) ? 0 : 1);
}

int Root(unsigned char *s1, unsigned char *s2)
// finds the nth root of S11 where n is specified by S21, setting S21 to 1
// if result is less than 1 then S11 = 0 and the function returns 1, otherwise 0 is returned
{   int a, b, c;

    schematoi(s1,schemasize,&a);
    schematoi(s2,schemasize,&b);
    c = (int)pow((double)a, 1.0/(double)b);
    itoschema(c,s1);
    itoschema(1,s2);
    
    return (c > 0 ? 0 : 1);
}

int ASineB(unsigned char *s1, unsigned char *s2)
// places result of S12*sine(S22) into S22, resetting S12 to 1; overflow is ignored
{   int a, b, c;
    
    schematoi(s1,schemasize,&a);
    schematoi(s2,schemasize,&b);
    c = (int)((double)a*(sin((double)b)));
    itoschema(c,s2);
    itoschema(1,s1);
    
    return 0;
}

int ACosB(unsigned char *s1, unsigned char *s2)
// places result of S12*cosine(S22) into S22, resetting S12 to 1; overflow is ignored
{   int a, b, c;
    
    schematoi(s1,schemasize,&a);
    schematoi(s2,schemasize,&b);
    c = (int)((double)a*(cos((double)b)));
    itoschema(c,s2);
    itoschema(1,s1);
    
    return 0;
}

int ATanB(unsigned char *s1, unsigned char *s2)
// places result of S12*tan(S22) into S22, resetting S12 to 1; overflow is ignored
{   int a, b, c;
    
    schematoi(s1,schemasize,&a);
    schematoi(s2,schemasize,&b);
    c = (int)((double)a*(tan((double)b)));
    itoschema(c,s2);
    itoschema(1,s1);
    
    return 0;
}

int AddChainLink(unsigned char *s1, unsigned char *s2, int chainnum, chainlink **chain)
{
    return 0;
}

int RemoveChainLink(unsigned char *s1, unsigned char *s2, int chainnum, chainlink **chain)
{
    return 0;
}

int XChains(unsigned char *s1, unsigned char *s2, int chainnum, chainlink **chain)
{
    return 0;
}

int Bitmark(unsigned char *s1, unsigned char *s2)
// Adds a marker to  S11
{
   
    // transforms positive num into binary with length l
    // and places it in schema[] starting at bit s
    //(int num, int s, int l, unsigned char *schema)
    
    itoschemapart(3, 0, 2, s1);
    itoschemapart(3, 0, 2, s2);
    
    return 0;
}


/// finally any general utility functions can be placed below:

void itofunctionpart(int num, int s, int l, unsigned char *function)
// transforms positive num into binary with length l
// and places it in function[] starting at bit s
{
    unsigned long int p;
    int n;
    
    if (num < 0)
    {
        printf("ERROR : Only non-negative numbers can be transformed to a function");
    }
    else
    {
        p = (unsigned long int)pow(2.0,l-1);
        for (n=s+l-1;n>=s; n--)
        {
            if ((unsigned long int) num >= p) {
                num -= p;
                function[n] = ONE;
                p = p >> 1;
            } else {
                function[n] = ZERO;
                p = p >> 1;
            }
            
        }
        
    }
}

