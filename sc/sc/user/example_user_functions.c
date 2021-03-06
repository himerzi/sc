#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "systemic9.h"
#include "example_user_functions.h"
#include "sc_functions.h"


void user_initialise(char *name1,char *name2,char *name3,char *name4,systemic *s,int **scopetable)
// automatically called at the start of execution;
// up to four input and/or output filenames may be specified in scp.ini and will be passed here
{
     srand(time(NULL)); // randomize seed
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

int Bitmark2(systemic *s, int i1, int i2, int parent, int **scopetable, chainlink **chain, int context)
// Adds a marker to  S11
{
    unsigned char *s1 = s[i1].schema1;
    unsigned char *s2 = s[i2].schema1;
    // transforms positive num into binary with length l
    // and places it in schema[] starting at bit s
    //(int num, int s, int l, unsigned char *schema)
    
    itoschemapart(3, 0, 2, s1);
    itoschemapart(3, 0, 2, s2);
 
    
    
    //TODO, ideally this should only run once for a given solution (because we only need to tag its chain once, I believe the chain id shouldn't change for a solution)
    if (s[context].ischained == NOTCHAINED)
    {
        printf("ERROR : This should be the head of a chain, but ischained == NOTCHAINED");
    }
    else
    {
        int chnum = s[context].ischained;
        itoschema(chnum, s[parent].schema2);
        // check that the first link matches the system we just executed
        if (chain[chnum][0].systemnum != context) { printf("\nError: Chain for system not found. Chain broken.//"); }
    }
    
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

int Bitmark_init(systemic *s, int i1, int i2, int parent, int **scopetable, chainlink **chain, int context)
// Adds a marker to  S11
{
    int chnum;
    unsigned char *s1 = s[i1].schema1;
    unsigned char *s2 = s[i2].schema1;
    // transforms positive num into binary with length l
    // and places it in schema[] starting at bit s
    //(int num, int s, int l, unsigned char *schema)
    
    itoschemapart(3, 0, 2, s1);
    itoschemapart(3, 0, 2, s2);
    
    
    chnum = s[context].ischained;
    //TODO, ideally this should only run once for a given solution (because we only need to tag its chain once, I believe the chain id shouldn't change for a solution)
    if (chnum == NOTCHAINED)
    {
        printf("ERROR : This should be the head of a chain, but ischained == NOTCHAINED");
    }
    else
    {
        
        itoschema(chnum, s[parent].schema2);
        // check that the first link matches the system we just executed
        if (chain[chnum][0].systemnum != context) {
            printf("\nError: Chain for system not found. Chain broken.//");
        }
    }
    //how do u convt to funciton format....
    //choose any funciton out of 42, 51, 52, 53, 54

    

    int funcOptions[5] = { 42, 51 ,52 ,53 ,54};
    int fno1, fno2, choice1, choice2, optionsSize = 5;
    
    choice1 = rand()%optionsSize;
    choice2 = rand()%optionsSize;
    int hollow1 = chain[chnum][1].systemnum;
    int hollow2 = chain[chnum][2].systemnum;
    schematoi(s[hollow1].function, schemasize, &fno1);
    schematoi(s[hollow2].function, schemasize, &fno2);
   if (fno1  == 0 && fno2 == 0) // might be 9...
    {
        //am i using the correct index?
        itofunction(funcOptions[choice1], s[hollow1].function);
        itofunction(funcOptions[choice2], s[hollow2].function);
    }

    return 0;
}

int Bitmarkp1(unsigned char *s11, unsigned char *s21, unsigned char *s22)
// Increases marker values of s11 and s21 by 1
//also checks if s22 (which is the current value of x) is equal to 10, in which case its time to break out of the chain, as this means
//the fitness has now been measured.
{
    int a, b, x;
    
    //read the value of the current x independent variable
    schematoi(s22,schemasize,&x);
    //if x is now 10, then were done with the cycle of evaluating fitness
    if (x > 10) {
        return 1;
    }
    
    //read the value of s11 and s21
    schematoi(s11,schemasize,&a);
    schematoi(s21,schemasize,&b);
    //printf("a b pre: %d %d\n", a, b);
    ++a;++b;
    //printf("a b post: %d %d\n", a, b);
    itoschema(a, s11);
    itoschema(b, s21);
    
    

    return 0;
}

int Incrementor(unsigned char *s12, unsigned char *s22)
// Reads the value of s22 and increments it by 1, copies it to s12 and s22
{
    int a;
    //read the value of s22
    schematoi(s22,schemasize,&a);

    ++a;
    itoschema(a, s12);
    itoschema(a, s22);

    return 0;
}

int Mult_v2(unsigned char *s11,unsigned char *s12, unsigned char *s22)
// multiplies binary values s1 * s2, placing results in s1 and, differently to normal mult, preserves original value of s22; under/overflow is ignored, other characters are ignored. Marks s11 with a special marking 
{    int a, b;
    
    // do it the lazy way
    schematoi(s12,schemasize,&a);//取变量地址
    schematoi(s22,schemasize,&b);
    //mark it with a 20
    //itoschema(20, s11);
    //store the answer
    itoschema(a*b,s12);
    
    return 0;
}

int Recorder(unsigned char *s12,unsigned char *s22)
// Records solution on 12, recalculates the overall fitness measure(The sum of absolute errors)
//, and records solution to 22.
{
    int a, b;
    schematoi(s12,schemasize,&a);
    schematoi(s22,schemasize,&b);
    //store the answer
    itoschema(a+b,s22);
    return 0;
}

int Resetter(unsigned char *s11,unsigned char *s21)
// Resetter, has a look at 21 to check it hasn't gone past 10, (we're only evaluating expressions in the range [0-10]. Then
//resets the type schemas ie. 11 and 21 are set to "num" type.
//In the example above, if the return value from the chain head "cond_calculate" is 0, then the "add" system will be executed next. If the return value is 1, then "subtract" will be executed next. If the return value is greater than 1 in this case then the chain will be broken and the SC will return to normal parallel interactions. A return value of -1 forces a chain break after the chain head.
//maybe should do that here...
{
    int NUM = 1;
    itoschema(NUM ,s11);
    itoschema(NUM, s21);
    return 0;
}

int Escape_both(systemic *s, int i1, int i2, int parent,int **scopetable)
// removes s[i1] from the scope of s[parent]; s[i1] and s[i2] are otherwise unchanged
// puts s[i1]  & s[i2] into all the scopes that the parent is in

{   int n;
    
    scopetable[parent][i1] = 0; scopetable[parent][i2] = 0; updatescopestate(s, scopetable, parent);
    for (n = 0; n < numsystems; n++)
        if (scopetable[n][parent] > 0)
        {
            scopetable[n][i1] = 1; scopetable[n][i2] = 1; updatescopestate(s, scopetable, n);
        }
    return 0;
}

int Tag_fitness(systemic *s, unsigned char *s12, int parent, int **scopetable, chainlink **chain)
//Places the fitness value on to the main systems left schema
//s12 is the fitness value
{
    //todo implement this in practice. do another one to encode chain on right schema
    int fitness, n;
    schematoi(s12,schemasize,&fitness);
    
    for (n = 0; n < numsystems; n++)
        if (scopetable[n][parent] > 0)
        {
            itoschema(fitness, s[n].schema1);
            //for testing purposes
            testing(s, fitness, s[n].schema2, chain);
            break;
        }

    //schema one of main stores the fitness value for this candidate solution.
    return 0;
}


int Tag_chain(systemic *s, int i1, int parent, int **scopetable, chainlink **chain)
// this doesnt get called anymore. bitmark_init does this now
//Places the head of the chain on to the main system's right schema
{
    printf("TAGS");
 
    if (s[i1].ischained == NOTCHAINED)
    {
        printf("ERROR : This should be the head of a chain, but ischained == NOTCHAINED");
    }
    else
    {
        int n, chnum = s[i1].ischained;
        for (n = 0; n < numsystems; n++)
            if (scopetable[n][parent] > 0)
            {
                itoschema(chnum, s[n].schema2);
                // check that the first link matches the system we just executed
                if (chain[chnum][0].systemnum != i1) { printf("\nError: Chain for system not found. Chain broken.//"); break; }

                break;
            }
    }
    return 0;
}

int Add_v2(unsigned char *s11,unsigned char *s12, unsigned char *s22)
// adds binary values s12 + s22, places result in s1(2), marks s11 with 20 to indicate s1 has the result
{    int a, b;
    
    // do it the lazy way
    schematoi(s12,schemasize,&a);//取变量地址
    schematoi(s22,schemasize,&b);
    //mark it with a 20, do denote it carries the "answer"
    //itoschema(20, s11);
    //store the answer
    itoschema(a+b,s12);
    
    return 0;
}

int Add1(unsigned char *s12)
// adds 1 to the value of s12
{    int a;
    schematoi(s12,schemasize,&a);
    //mark it with a 20, do denote it carries the "answer"
   // itoschema(20, s11);
    //store the answer
    itoschema(1+a,s12);
    
    return 0;
}

int Add2(unsigned char *s12)
// adds 2 to the value of s12
{    int a;
    schematoi(s12,schemasize,&a);
    //mark it with a 20, do denote it carries the "answer"
    //itoschema(20, s11);
    //store the answer
    itoschema(2+a,s12);
    
    return 0;
}
int Add3(unsigned char *s12)
// adds 3 to the value of s12
{    int a;
    schematoi(s12,schemasize,&a);
    //mark it with a 20, do denote it carries the "answer"
    //itoschema(20, s11);
    //store the answer
    itoschema(3+a,s12);
    
    return 0;
}

int Mark_answer(unsigned char *s11)
//mark s11 with 20 to denote an answer system
{
    itoschema(20, s11);
    return 0;
}

void testing(systemic *s, int fitness, unsigned char *schema_chnum, chainlink **chain)
{
    FILE *ofp;
    
    char outputFilename[] = "tests.txt";

    ofp = fopen(outputFilename, "a");
    
    if (ofp == NULL) {
        fprintf(stderr, "Can't open output file %s!\n",
                outputFilename);
        exit(1);
    }
    
    int chnum, sys1, sys2;
    unsigned char *fn1, *fn2;
    schematoi(schema_chnum, schemasize, &chnum);
    fn1 = s[chain[chnum][1].systemnum].function;
    fn2 = s[chain[chnum][2].systemnum].function;
    schematoi(fn1, schemasize, &sys1);
    schematoi(fn2, schemasize, &sys2);
    char functionname1[128], functionname2[128];
    // look up the function name for s[n].function
    strcpy(functionname1, systemfunctions[sys1-1].name);
    strcpy(functionname2, systemfunctions[sys2-1].name);
    fprintf(ofp, "%s + %s result: %d\n", functionname1, functionname2, fitness);
    fclose(ofp);
}

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

