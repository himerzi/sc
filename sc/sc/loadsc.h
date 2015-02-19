// LoadSC is based on CompileSC4
// CompileSC4 - fourth compiler for systemic computation programs, compatible with v7 of systemic computation
// (c) 2006, 2013 Peter J Bentley
// version 2 supports overlapping scopes with the new #scope format and scope tables
// version 3 (HAoS C model) by Christos Sakellariou
// version 4 adds chaining of systems
// version 5 is integrated into sc as the loading phase of the computer


#ifndef loadsc_h
#define loadsc_h

#include "systemic9.h"
#include "sc_functions.h"

symbol systemfunctions[TOTALSYSTEMFUNCTIONS];
int numberfunctionsused[512];

#define MAXCONSOLE   32768

char console[MAXCONSOLE];    // used to store output to console from assembler in case we want to pass error reports elsewhere

int parse(unsigned char *c, int* t1, int *t2, int *not);
BOOL foundfile(char *fname);
BOOL getnextscpoutfile();
static inline void copyover(unsigned char *s1, unsigned char *s2, int copysize);
void parseerror(int *e,FILE *fstrm, char *line, const char str[],int c,BOOL fatal);
void error(int *e,FILE *fstrm, char *line, const char str[],int l,BOOL fatal);
BOOL getval(unsigned char *schema,char *number,int l,int *e);
BOOL getfunctionval(unsigned char *schema,char *number,int l,int *e);
BOOL getlabelval(unsigned char *schema,char *number,int l,int *e);
void checkforduplicates(symbol *functions, int fn, symbol *labels, int ln, syssymbol *sys, int sn,int *e,int l);
BOOL isplus(char *in, int *l);
void getchunk(char *chunk,char *in, int *l);
void getword_nob(char *word,char *in, int *l);
BOOL getfunction(symbol *functions, int fn, unsigned char *schema,char *word,int l,int *e);
BOOL issystem(char *name, int *s, syssymbol *sys, int sn);
BOOL islabel(char *name, int *n, symbol *label, int ln);
BOOL code(systemic *s, unsigned char *schema,char *chunk,symbol *labels,int ln,syssymbol *sys,int sn,int l,int *e);
void loadchain(FILE *fstrm,int sysnum, syssymbol *sys, int sn, chainlink **chain,int *cnum, int *l, int *errors);
void loadscope(FILE *fstrm,int sysnum, syssymbol *sys, int sn, int **scopetable,int *l, int *errors,char *scopename);
BOOL assemble(systemic *s, int *numsystems, symbol *functions, int fn, symbol *labels, int ln, syssymbol *sys, int *e);
BOOL assembleprogram(char *fname, systemic *s, int *numsystems, symbol *functions, int *fn,int **scopetable, int *scnum, chainlink **chain, int *cnum);
void saveprogram(char *fname, char *fname2, systemic *s, int numsystems, symbol *functions, int fn,int **scopetable, chainlink **chain, int cnum);

#endif



