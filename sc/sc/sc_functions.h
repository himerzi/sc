// sc_functions has the core transformation functions for sc and enables linking to user-defined libraries (based on SC_FUNCTIONS)
// to add your own functions just #include your header file below
// (c) 2013-2014 Peter J Bentley


#ifndef SC_FUNCTIONS
#define SC_FUNCTIONS

//////////////////////////////// PROBLEM-SPECIFIC CODE HERE ////////////////////////////////
//  #include your user function file here:
#include "example_user_functions.h"
////////////////////////////////////////////////////////////////////////////////////////////


// verbose or not?
#define PRINTFUNCTIONS 1

#if PRINTFUNCTIONS
#define PRINTFF(...) printf(__VA_ARGS__)
#else
#define PRINTFF(...)
#endif

// print to file or not?
#define PRINTTOFILE 1
#if PRINTTOFILE
FILE *resfile;
char resfilename[99];
#define PRINTR(...) fprintf(resfile, __VA_ARGS__)
#else
#define PRINTR(...) printf(__VA_ARGS__)
#endif


// standard function set
// each value must be the same as the binary value used in the .sc code or the wrong function will be called

#define NOPfn				0
#define ADDfn				1
#define ADDefn				2
#define SUBTRACTfn			3
#define SUBTRACTefn			4
#define MULTfn				5
#define MULTefn				6
#define DIVfn				7
#define DIVefn				8
#define MODfn				9
#define MODefn				10
#define ANDfn				11
#define ORfn				12
#define EORfn				13
#define ZEROfn              14
#define ESCAPEfn			15
#define CAPTUREfn			16
#define PRINTfn				17
#define COPYfn				18
#define ISZEROfn			19
#define ISEQUALfn           20
#define CAPTUREINHERITfn	21
#define RESERVED1fn         22
#define RESERVED2fn         23
#define RESERVED3fn         24
#define RESERVED4fn         25
#define RESERVED5fn         26
#define RESERVED6fn         27
#define RESERVED7fn         28
#define RESERVED8fn         29
#define RESERVED9fn         30
#define RESERVED10fn        31

#define NSYSTEMFUNCTIONS    32                                     // total number of system functions (first 32 are reserved)
#define TOTALSYSTEMFUNCTIONS  NSYSTEMFUNCTIONS+NUMUSERFUNCTIONS    // total number of functions altogether

extern int numberfunctionsused[512];                              // counter to store number of times each function is used

extern symbol systemfunctions[TOTALSYSTEMFUNCTIONS];                     // labels for all system transformation functions

void user_initialise(char *name1,char *name2,char *name3,char *name4,systemic *s,int **scopetable);
void user_wrapup(char *name1,char *name2,char *name3,char *name4,systemic *s,int **scopetable);

void initProblem(char *name1,char *name2,char *name3,char *name4,systemic *s,int **scopetable);
void endProblem(char *name1,char *name2,char *name3,char *name4,systemic *s,int **scopetable);

void printfunctionsusage();

int zero(unsigned char *s1, unsigned char *s2);
__inline BOOL isNOP(unsigned char *c);
BOOL iszero(unsigned char *s1, unsigned char *s2);
BOOL isequal(unsigned char *s1, unsigned char *s2);
int copy(unsigned char *s1, unsigned char *s2);
int and(unsigned char *s1, unsigned char *s2);
int or(unsigned char *s1, unsigned char *s2);
int eor(unsigned char *s1, unsigned char *s2);
int printsystem(systemic *s);
int print(systemic *s1, systemic *s2);
int escape(systemic *s, int i1, int i2, int parent,int **scopetable);
int capture(systemic *s, int i1, int i2, int parent, int **scopetable);
int captureInherit(systemic *s, int i1, int i2, int **scopetable);
int mult(unsigned char *s1, unsigned char *s2);
int mydiv(unsigned char *s1, unsigned char *s2);
int mod(unsigned char *s1, unsigned char *s2);
int subtract(unsigned char *s1, unsigned char *s2);
int add(unsigned char *s1, unsigned char *s2);

#endif