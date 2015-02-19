// sc_functions has the core transformation functions for sc and enables linking to user-defined libraries (based on sc_user_plugins)
// (c) 2013-2014 Peter J Bentley


#include "systemic9.h"
#include "sc_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



void addfunction(char *fn_name)
{   static int fn = 0;
    
    strcpy(systemfunctions[fn++].name,fn_name);
    if (fn > TOTALSYSTEMFUNCTIONS)
    {
        printf("\nError - too many functions. Increase value of TOTALSYSTEMFUNCTIONS");
    }
}

void init()
// compulsory initialisation to fill the main data structures
{
    addfunction("NOP");
    addfunction("ADD");
    addfunction("ADDe");
    addfunction("SUBTRACT");
    addfunction("SUBTRACTe");
    addfunction("MULT");
    addfunction("MULTe");
    addfunction("DIV");
    addfunction("DIVe");
    addfunction("MOD");
    addfunction("MODe");
    addfunction("AND");
    addfunction("OR");
    addfunction("EOR");
    addfunction("ZERO");
    addfunction("ESCAPE");
    addfunction("CAPTURE");
    addfunction("PRINT");
    addfunction("COPY");
    addfunction("ISZERO");
    addfunction("ISEQUAL");
    addfunction("CAPTUREINHERIT");
    addfunction("RESERVED1");
    addfunction("RESERVED2");
    addfunction("RESERVED3");
    addfunction("RESERVED4");
    addfunction("RESERVED5");
    addfunction("RESERVED6");
    addfunction("RESERVED7");
    addfunction("RESERVED8");
    addfunction("RESERVED9");
    addfunction("RESERVED10");
    
    // your own user functions are added by this macro
    USERSYSTEMFUNCTIONS

}

void initProblem(char *name1,char *name2,char *name3,char *name4,systemic *s,int **scopetable)
// by #including your user-defined function file above, you can call your problem specific intialisation functions here,
// such as loading datasets, allocating memory.
{
    
    init(); // don't remove this one
    
    user_initialise(name1,name2,name3,name4,s,scopetable);
    
}

void endProblem(char *name1,char *name2,char *name3,char *name4,systemic *s,int **scopetable)

// by #including your user-defined function file above, you can call your problem-specific ending functions here,
// such as closing files, writing output, and deallocating memory
{
    user_wrapup(name1,name2,name3,name4,s,scopetable);
}


int transform(int f, int m1, int m2, int t1, int t2, int not, systemic *s, int i1, int i2, int scppos, int context, int **scopetable)
// transforms s[i1] and s[i2] using function f and probability determined by matching differences m1 and m2,
// and thresholds t1 and t2, in scope of s[scppos]
{
    int ret = 0;
    
    switch (f)
    {   case NOPfn:
            PRINTFF("[]");
            break;
        case ADDfn:
            if (_matchedok(m1,m2,t1,t2,not))
            {
                ret = add(s[i1].schema2,s[i2].schema2);
                PRINTFF("+");
            }
            break;
        case ADDefn:
            if (_matchedok(m1,m2,t1,t2,not))
            { ret = add(s[i1].schema2,s[i2].schema2);
                if (ret != CHAIN_BREAK) ret = escape(s,i1,i2,scppos,scopetable);
                PRINTFF("<{+}>");
            }
            break;
        case SUBTRACTfn:
            if (_matchedok(m1,m2,t1,t2,not))
            { ret = subtract(s[i1].schema2,s[i2].schema2);
                PRINTFF("-");
            }
            break;
        case SUBTRACTefn:
            if (_matchedok(m1,m2,t1,t2,not))
            { ret = subtract(s[i1].schema2,s[i2].schema2);
                if (ret != CHAIN_BREAK) ret = escape(s,i1,i2,scppos,scopetable);
                PRINTFF("<{-}>");
            }
            break;
        case MULTfn:
            if (_matchedok(m1,m2,t1,t2,not))
            { ret = mult(s[i1].schema2,s[i2].schema2);
                PRINTFF("*");
            }
            break;
        case MULTefn:
            if (_matchedok(m1,m2,t1,t2,not))
            { ret = mult(s[i1].schema2,s[i2].schema2);
                if (ret != CHAIN_BREAK) ret = escape(s,i1,i2,scppos,scopetable);
                PRINTFF("<{*}>");
            }
            break;
        case DIVfn:
            if (_matchedok(m1,m2,t1,t2,not))
            { ret = mydiv(s[i1].schema2,s[i2].schema2);
                PRINTFF("/");
            }
            break;
        case DIVefn:
            if (_matchedok(m1,m2,t1,t2,not))
            { ret = mydiv(s[i1].schema2,s[i2].schema2);
                if (ret != CHAIN_BREAK) ret = escape(s,i1,i2,scppos,scopetable);
                PRINTFF("<{/}>");
            }
            break;
        case MODfn:
            if (_matchedok(m1,m2,t1,t2,not))
            { ret = mod(s[i1].schema2,s[i2].schema2);
                PRINTFF("%%");
            }
            break;
        case MODefn:
            if (_matchedok(m1,m2,t1,t2,not))
            { ret = mod(s[i1].schema2,s[i2].schema2);
                if (ret != CHAIN_BREAK) ret = escape(s,i1,i2,scppos,scopetable);
                PRINTFF("<{%%}>");
            }
            break;
        case ZEROfn:
            if (_matchedok(m1,m2,t1,t2,not))
            { ret = zero(s[i1].schema1,s[i2].schema1);
                ret += zero(s[i1].schema2,s[i2].schema2);
                PRINTFF("z");
            }
            break;
        case ANDfn:
            if (_matchedok(m1,m2,t1,t2,not))
            { ret = and(s[i1].schema1,s[i2].schema1);
                ret += and(s[i1].schema2,s[i2].schema2);
                PRINTFF("&");
            }
            break;
        case ORfn:
            if (_matchedok(m1,m2,t1,t2,not))
            { ret = or(s[i1].schema1,s[i2].schema1);
                ret += or(s[i1].schema2,s[i2].schema2);
                PRINTFF("||");
            }
            break;
        case EORfn:
            if (_matchedok(m1,m2,t1,t2,not))
            { ret = eor(s[i1].schema1,s[i2].schema1);
                ret += eor(s[i1].schema2,s[i2].schema2);
                PRINTFF("e");
            }
            break;
        case PRINTfn:
            if (_matchedok(m1,m2,t1,t2,not))
            {
				ret = print(s+i1,s+i2);
            }
            break;
        case ESCAPEfn:
            if (/*scppos >= 0 && s[scppos].inscope > 0 &&*/ _matchedok(m1,m2,t1,t2,not))
            { ret = escape(s,i1,i2,scppos,scopetable);
                PRINTFF("<{}>");
            }
            break;
        case CAPTUREfn:
            if (_matchedok(m1,m2,t1,t2,not))
            { ret = capture(s,i1,i2,scppos,scopetable);
                PRINTFF(">{}<");
            }
            break;
        case CAPTUREINHERITfn:
            if (_matchedok(m1,m2,t1,t2,not))
            { ret = captureInherit(s,i1,i2,scopetable);
                PRINTFF(">{}<");
            }
            break;
        case COPYfn:
            if (_matchedok(m1,m2,t1,t2,not))
            { ret = syscopy(s,i2,s,i1);
                PRINTFF("c");
            }
            break;
        case ISZEROfn:
            if (_matchedok(m1,m2,t1,t2,not))
            { if (iszero(s[i1].schema2,s[i2].schema2))
            {  s[i1].schema1[schemasize-1] = ONE;
                ret = 1;
            }
            else
            {  s[i1].schema1[schemasize-1] = ZERO;
                ret = 0;
            }
                PRINTFF("?0");
            }
            break;
        case ISEQUALfn:
            if (_matchedok(m1,m2,t1,t2,not))
            {   if (isequal(s[i1].schema1,s[i2].schema1) && isequal(s[i1].schema2,s[i2].schema2))
            {  ret = 1;
            }
            else
            {  ret = 0;
            }
                PRINTFF("==");
            }
            break;

            // your own user defined functions are added with this macro:
            USERTRANSFORMFUNCTIONS;
    }
    
    if (_matchedok(m1,m2,t1,t2,not))
    {
		interaction_count++;
        if (f < TOTALSYSTEMFUNCTIONS) { /*printf("\n %d", numberfunctionsused[f]);*/ numberfunctionsused[f]++; }
        else { printf("\nFatal error: function %d has bad code",f); exit(-11); } // shou0ld not happen
	}
    return ret;
}

void printfunctionsusage()
// prints utilization of functions
{
	int i,j,fn;
    int fmap[512];
	printf("\nFUNCTIONS UTILIZATION:\n");
    
    // first make a map from numberfunctionsused[] to functions[].name as they may not be listed in same order
    for (j = 0; j < TOTALSYSTEMFUNCTIONS; j++)
    {   schematoi(functions[j].schema,iniFUNCTIONID,&fn);
        fmap[fn] = j;
    }
	// now we can display correct function names corresponding to each count
    for (i = 0; i < 512; i++)
        if (numberfunctionsused[i])
        {
			printf("%s : %d\n",functions[fmap[i]].name,numberfunctionsused[i]);
        }
}

/////////////////////////////////////// Default function set starts here ///////////////////////////////////////


int zero(unsigned char *s1, unsigned char *s2)
// applies zeroing function to s1 and s2, placing results in both;
// zeroes all ones in schema sections but ignores everything else (to protect codes if used)
{    int n;

     for (n = 0; n < schemasize; n++)
     { if (s1[n] == ONE) s1[n] = ZERO;
       if (s2[n] == ONE) s2[n] = ZERO; 
     }
    return 0;
}

__inline BOOL isNOP(unsigned char *c)
// returns TRUE if c is a NOP
{   int f;

    schematoi(c, iniFUNCTIONID, &f);
    return (f == NOPfn);
}

BOOL iszero(unsigned char *s1, unsigned char *s2)
// returns TRUE if s1 is zero; s1 remains unchanged
{    int n = 0;

     schematoi(s1,schemasize,&n);
     return (n == 0);
}

BOOL isequal(unsigned char *s1, unsigned char *s2)
// returns TRUE is s1 is identical to s2
{   int n,m;
    
    n = m = 0;
    
    schematoi(s1,schemasize,&n);
    schematoi(s2,schemasize,&m);

    return (n == m);
}

int copy(unsigned char *s1, unsigned char *s2)
// copies s1 over s2; s1 remains unchanged
{    int n;

     for (n = 0; n < schemasize; n++) s2[n] = s1[n];
     return 0;
}

int and(unsigned char *s1, unsigned char *s2)
// applies bitwise and to s1 and s2, placing results in both; ignores non-binary characters
{    int n;

     for (n = 0; n < schemasize; n++)  if (s1[n] == ZERO || s2[n] == ZERO) s1[n] = s2[n] = ZERO; // otherwise they already both equal ONE
     return 0;
}

int or(unsigned char *s1, unsigned char *s2)
// applies bitwise or to s1 and s2, placing results in both; ignores non-binary characters
{    int n;

     for (n = 0; n < schemasize; n++)  if (s1[n] == ONE || s2[n] == ONE) s1[n] = s2[n] = ONE;
     return 0;
}

int eor(unsigned char *s1, unsigned char *s2)
// applies exclusive-or to s1 and s2, placing results in both; ignores non-binary characters
{    int n;

     for (n = 0; n < schemasize; n++)
     {  if ((s1[n] == ZERO && s2[n] == ONE) || (s2[n] == ZERO && s1[n] == ONE))
        {  if (s1[n] != s2[n]) { s1[n] = s2[n] = ONE; }
           else { s1[n] = s2[n] = ZERO; }
        }
     }
     return 0;
}

int printsystem(systemic *s)
// prints the contents of s
{   int n;

    PRINTF("\n");
    for (n = 0; n < schemasize; n++) { if (s->schema1[n] < 2) PRINTF("%d",s->schema1[n]); else PRINTF("%c",s->schema1[n]); }
    PRINTF(" ");
    for (n = 0; n < functionsize; n++) { PRINTF("%d",s->function[n]); }
    PRINTF(" ");
    for (n = 0; n < schemasize; n++) { if (s->schema2[n] < 2) PRINTF("%d",s->schema2[n]); else PRINTF("%c",s->schema2[n]); }
    return 0;
}

int print(systemic *s1, systemic *s2)
// printfs s1 and s2 to the screen
{   int ret;
    
    ret = printsystem(s1);
    ret += printsystem(s2);
    return ret;
}

int escape(systemic *s, int i1, int i2, int parent,int **scopetable)
// removes s[i1] from the scope of s[parent]; s[i1] and s[i2] are otherwise unchanged
// puts s[i1] into all the scopes that the parent is in
// assumes s[parent] exists (i.e., that s[i1] is not a root system) and that it actually has systems in its scope
{   int n;
    
    scopetable[parent][i1] = 0;	updatescopestate(s, scopetable, parent);
    for (n = 0; n < numsystems; n++) 
		if (scopetable[n][parent] > 0)
		{ 
			scopetable[n][i1] = 1; updatescopestate(s, scopetable, n);
		}
    return 0;
}

int capture(systemic *s, int i1, int i2, int parent, int **scopetable)
// adds s[i1] to the scope of s[i2]; s[i1] and s[i2] are otherwise unchanged
// removes s[i1] from the scope of parent
{   
     scopetable[i2][i1] = 1; updatescopestate(s, scopetable, i2);
     scopetable[parent][i1] = 0;  updatescopestate(s, scopetable, parent);
     return 0;
}

int captureInherit(systemic *s, int i1, int i2, int **scopetable)
{
    //put all the system within i2 into i1
    for (int n = 0; n < numsystems; n++)
    {
        if (scopetable[i2][n] > 0)
        {
            scopetable[i1][n] = 1;
        }
    }
    // put i2 into i1 as well
    scopetable[i1][i2] = 1;
    updatescopestate(s, scopetable, i1);
    return 0;
}

int mult(unsigned char *s1, unsigned char *s2)
// multiplies binary values s1 * s2, placing results in s1 and setting s2 to 1; under/overflow is ignored, other characters are ignored
{    int a, b;

     // do it the lazy way
     schematoi(s1,schemasize,&a);//取变量地址
     schematoi(s2,schemasize,&b);
     itoschema(a*b,s1);
     zero(s2,s2);
     //itoschema(1,s2);
     return 0;
}

int mydiv(unsigned char *s1, unsigned char *s2)
// divides binary values of s1 / s2, placing results in s1 and setting s2 to 1; under/overflow is ignored, other characters are ignored
{    int a, b, d;

     // do it the lazy way
     schematoi(s1,schemasize,&a);
     schematoi(s2,schemasize,&b);
     if (b != 0) d = a/b;
     else d = 0;
     itoschema(d,s1);
     //zero(s2,s2);
     itoschema(1,s2);
     return 0;
}

int mod(unsigned char *s1, unsigned char *s2)
// finds the remainder of s1 / s2, placing results in s1 and setting s2 to 1; under/overflow is ignored, other characters are ignored
{    int a, b, d;

     // do it the lazy way
     schematoi(s1,schemasize,&a);
     schematoi(s2,schemasize,&b);
     if (b != 0) d = a % b;
     else d = a;
     itoschema(d,s1);
     //zero(s2,s2);
     itoschema(1,s2);
     return 0;
}

int subtract(unsigned char *s1, unsigned char *s2)
// subtracts binary values of s1 - s2, placing results in s1 and zeroing s2; underflow is ignored, other characters are ignored
{    int a, b;

     // do it the lazy way
     schematoi(s1,schemasize,&a);
     schematoi(s2,schemasize,&b);
     itoschema(a-b,s1);
     zero(s2,s2);
     return 0;
}

int add(unsigned char *s1, unsigned char *s2)
// adds binary values of s1 and s2, placing results in s1 and zeroing s2; overflow is ignored, other characters are ignored
{    int n, carry;

     carry = ZERO;
     for (n = 0; n < schemasize; n++)
     {  if ((s1[n] == ZERO && s2[n] == ONE) || (s2[n] == ZERO && s1[n] == ONE))
        { if (carry == ZERO) { s1[n] = ONE; s2[n] = ZERO; }
          else { s1[n] = ZERO; s2[n] = ZERO; } // and leave carry = ONE
        }
        else
        { if (s1[n] == ZERO && s2[n] == ZERO)
          { if (carry == ZERO) { s1[n] = ZERO; s2[n] = ZERO; }
            else { s1[n] = ONE; s2[n] = ZERO; carry = ZERO; }
          }
          else if (s1[n] == ONE && s2[n] == ONE)
          { if (carry == ZERO) { s1[n] = ZERO; s2[n] = ZERO; carry = ONE; }
            else { s1[n] = ONE; s2[n] = ZERO; } // and leave carry = ONE
          }
        }
     }
     return 0;
}


