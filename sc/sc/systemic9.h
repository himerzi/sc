#ifndef SYSTEMIC9
#define SYSTEMIC9

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "timer.h"
#include "pthread.h"
#include <OpenCL/opencl.h>
//#include "loadsc.h"
//#include "sc_functions.h"

#define DEBUG 1
#ifdef DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif 

// macros 
#define randomize()                   srand((unsigned)time(NULL))
#define random(n)                     (rand() % (n))
#define frand()                       (((double)rand()) / ((double)RAND_MAX))
#define _matchedok(m1,m2,t1,t2,not)   ((not == 0) ? (m1 <= t1 && m2 <= t2) : !(m1 <= t1 && m2 <= t2))
#define _smatchedok(m,t,not)          ((not == 0) ? (m <= t) : !(m <= t))
#define swap(a,b,tmp)                 tmp = a; a = b; b = tmp
#define getnextline                   while (fgets(line,MAXLLEN,fstrm1) != NULL && (line[0] == '#' || line[0] == '\n'))
#define safe_exit(a)                  freesystems(); exit(a)
#define _memcheck(a,b)                if ((a) == NULL) { printf("\nERROR: unable to allocate memory for "); printf(b); exit (-99); }

// return a unique, non repeating "random" value between 0 and range; if called range+1 times, the cycle will repeat
// randomizecycle(a) should be called to reset cycle "a" and create a new (different) one
// index "a" allows different, independent randomcycles to be used simultaneously
#define randomcycle(a, range)         (rndcycl[(a)] = (rndcycl[(a)] + BIGPRIME) % (range))
#define randomizecycle(a)             rndcycl[(a)] = random(BIGPRIME)

#define FALSE               0
#define TRUE                1
#define BOOL                unsigned char
#define ZERO                0
#define ONE                 1

#define MAXLLEN             9000                // max line length in bytes when reading from all source files

#define SCHEMASIZE          16                  // schema size in bits (scaled by iniSCALEFACTOR)
#define FUNCTIONSIZE        32                  // function size in bits (scaled by iniSCALEFACTOR)

#define SCHLEFT             -2                  // used to define left schema in a chain
#define SCHRIGHT            -1                  // used to define right schema in a chain
#define NOTCHAINED          -1                  // system ischained == NOTCHAINED if not chained
#define CHAIN_BREAK         -1                  // return value from transformation function, meaning go no further in chain
#define NOTNUM              -99                 // return value from namenumber function if no number found
#define BIGPRIME            47777               // large prime number used as increment in randomcycle()

#define WILDCARD            123                 // ascii value bigger than any code, but closest to triple don't care: ???
#define MAXNAME             1024                // max name length in chars for symbols in user code

#define INIFILE             "/scp.ini"          // name of ini file auto-read at startup
#define DEFAULT_COMPILE     1                   // default compile value (overridden by ini file)
#define	DEFAULT_MAXSYSTEMS  512                 // default maximum systems (overridden by ini file)
#define DEFAULT_SCALEFACTOR 1                   // default scaling factor for word size (overridden by ini file)
#define MATCHINGTHRES       4                   // default matching threshold size in bits (overridden by ini file)
#define FUNCTIONIDSIZE      6                   // default functionid size in bits (overridden by ini file)
#define DEFAULT_LOOP_NUM    10000               // default max number of loops before termination (overridden by ini file)
#define DEFAULT_ITN_NUM     10000               // default max number of interactions before termination (overridden by ini file)
#define DEFAULT_FNAME1      "file1.txt"         // default user-defined filename 1 (overridden by ini file)
#define DEFAULT_FNAME2      "file2.txt"         // default user-defined filename 2 (overridden by ini file)
#define DEFAULT_FNAME3      "file3.txt"         // default user-defined filename 3 (overridden by ini file)
#define DEFAULT_FNAME4      "file4.txt"         // default user-defined filename 4 (overridden by ini file)
#define MAXRANDOMCYCLES     8                   // max number of randomcycles in the program

int rndcycl[MAXRANDOMCYCLES];                   // used in randomcycle() - can have up to MAXRANDOMCYCLES concurrently in any one program
int iniCOMPILESC;                               // are we compiling the sc or not? (as defined by ini file)
int iniFUNCTIONID;                              // variable storing actual functionid size (as defined by ini file)
int iniMAXSYSTEMS;                              // variable storing actual max number of systems (as defined by ini file)
int iniSCALEFACTOR;                             // variable storing actual scalefactor size (as defined by ini file)
int iniMATCHINGTHRES;                           // variable storing actual matching threshold size (as defined by ini file)
int iniMAXFUNCTIONS;                            // variable storing actual number of maximum functions (as defined by ini file)
int schemasize;                               // size of schema (as scaled by iniSCALEFACTOR)
int functionsize;                               // size of function (as scaled by iniSCALEFACTOR)
unsigned int iniMAXLOOPS;                       // maximum number of iterations (as defined by ini file)
unsigned int iniMAXINTRCTNS;                    // maximum number of interactions (as defined by ini file)
unsigned int iterations;                        // number of iterations (loops) so far
unsigned int interaction_count;                 // number of interactions so far
BOOL simfinished;                               // TRUE if done


typedef struct { char name[MAXNAME]; unsigned char *schema; } symbol;     // type for general symbols
typedef struct { char name[MAXNAME]; char systembody[SCHEMASIZE+FUNCTIONSIZE+SCHEMASIZE+32]; } syssymbol;     // type for system symbols
typedef struct { int sch1; int systemnum; int sch2; } chainlink;            // type for chains

typedef struct { unsigned char *schema1;
                 unsigned char *function;
                 unsigned char *schema2;
                 int ischained; } systemic;                                 // main SC object

char scfilename[299], scoutfilename[299], scpfilename[299], scpoutfilename[299];
long scpoutfilenum,scoutfilenum; // used to index scpotfilenames and scoutfilenames and create new outfiles

int maxfunctionid;
//int scopenum;                   //  number of scopes
int chainnum;                   //  number of chains 
int numsystems;                 //  number of systems 
int numfunctions;               //  number of functions 

int *scopesummary;              // how many systems in each scope
int **scopelist;                // which systems in each scope
int *vscopelist;                // which are the valid scopes
int **functionlist;             // list of context systems in each scope
int *fnum;                      // number of contexts in each scope
int vscp;                       // number of valid scopes
BOOL *flistchanged;             // flag for each scope marking whether we have changed functionlist and fnum or not

int *stack;                     // stack for chaining, global for speed
int stackptr;                   // stack pointer to end of current stack

symbol *labels;                 //  list of labels
syssymbol *sys;                 //  list of symbols
symbol *functions;              //  list of functions
systemic *s;                    //  list of systems
int **scopetable;               //  scopetable
chainlink **chain;              //  chain table

#define MAXQUEUE            1024     // max number of valid triplets in the queue (divide by 4 for actual value: need four integers per system)
#define QSCOPE              0        // index offset for scope in validtriplet queue
#define QCONTEXT            1        // index offset for context in validtriplet queue
#define QSYS1               2        // index offset for matching system 1 in validtriplet queue
#define QSYS2               3        // index offset for matching system 2 in validtriplet queue

int tripletsbuffer1[MAXQUEUE];  // buffer 1 to hold valid triplets
int tripletsbuffer2[MAXQUEUE];  // buffer 2 to hold valid triplets
int *validtriplets;             // pointer to the current buffer
int *emptybuffer;               // pointer to the background buffer
int queue_front;                // pointer to front of valid triplets queue
BOOL refilling;                 // flag set when refilling thread is running

///// pthread specific ////

#define _startrefillqueuethread()   { pthread_join(qthread, NULL); swap(emptybuffer,validtriplets,tmp); queue_front = 0; rc = pthread_create(&qthread, &qattr, refillqueue, NULL); if (rc) {  printf("\nFATAL ERROR %d: Unable to create refillqueue thread\n", rc); exit(rc);  } }
#define _finishrefillqueuethread()  { int rc = pthread_create(&qthread, &qattr, refillqueue, NULL); if (rc) {  printf("\nFATAL ERROR %d: Unable to create refillqueue thread\n", rc); exit(rc);  } pthread_join(qthread, NULL); swap(emptybuffer,validtriplets,tmp); queue_front = 0; }


pthread_t qthread;              // pointer to thread used for refilling the valid triplets queue
pthread_attr_t qattr;           // thread attributes


////////////////////////////////////////////////////////// Open CL specific //////////////////////////////////////////////////////////

BOOL OpenCLAvailable;           // TRUE if current hardware contains at least one OpenCL device
#define MAXDEVICES 10           // MAXDEVICES OpenCL devices supported
#define MAXKERNELS 10           // MAXKERNELS kernels per device supported
#define CLWKGRPSIZE 16          // OpenCL work group size. Tune this to optimise for different architectures.
#define MAXBUFSIZE 1048576*(schemasize+functionsize+schemasize)   // OpenCL maximum memory buffer size in bytes, always in multiples of systems; allow around a million in each scope

struct OCL {    cl_mem input1, input2, output;                  // pointers to memory buffers for device
    cl_context context;                    // context for device
    cl_command_queue queue;                // command queue for this device
    cl_program program;                    // raw program to be built for the device
    cl_kernel kernel[MAXKERNELS];          // array of ptrs to kernels to be executed on this device
    size_t global[3];                      // global domain size for our calculation, assume no more than 3 dimensions
    size_t local[3];                       // local domain size for our calculation, assume no more than 3 dimensions
    size_t wkgpsize;                       // workgroup size for this device
} cl[MAXDEVICES];

cl_uint clDeviceCount;                                  // number of OpenCL devices available
cl_device_id cldevices[MAXDEVICES];                     // pointers to devices

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// from sc_functions.c
int transform(int f, int m1, int m2, int t1, int t2, int not, systemic *s, int i1, int i2, int scppos, int context, int **scopetable);
void initProblem(char *name1,char *name2,char *name3,char *name4,systemic *s,int **scopetable);
void endProblem(char *name1,char *name2,char *name3,char *name4,systemic *s,int **scopetable);
void printfunctionsusage();

// from loadsc.c
BOOL foundfile(char *fname);
BOOL getnextscpoutfile();
BOOL getnextscoutfile();
BOOL assembleprogram(char *fname, systemic *s, int *numsystems, symbol *functions, int *fn,int **scopetable, int *scnum, chainlink **chain, int *cnum);
void saveprogram(char *fname, char *fname2, systemic *s, int numsystems, symbol *functions, int fn,int **scopetable, chainlink **chain, int cnum);
void savescprogram(char *fname, char *fname2, systemic *s, int numsystems, symbol *functions, int fn,int **scopetable, chainlink **chain, int cnum);

// from systemic8.c
BOOL isvalidfunction(systemic *s, int i);
void decode_to_ascii(unsigned char *sch, int len, char *s1, char *s2, char *s3);
void encode(unsigned char *out, unsigned char *in1, unsigned char *in2, unsigned char *in3);
int syscopy(systemic *tmp, int i1, systemic *s, int i2);
void atoschema(char *str, int l, unsigned char *sc);
void schematoi(unsigned char *schema, int l, int *num);
void schemaparttoi(unsigned char *schema, int s, int l, int *num);
void itoschema(int num, unsigned char *schema);
void itoschemapart(int num, int s, int l, unsigned char *schema);
void itofunction(int num, unsigned char *function);
int difference(unsigned char *c, systemic *s);
int parse(unsigned char *c, int* t1, int *t2, int *not);
BOOL isData(unsigned char *c);
void initscopestate(systemic *s, int **scopetable);
void updatefunctionlist(systemic *s, int whichscope);
void updatescopestate(systemic *s, int **scopetable, int whichscope);
void updatecontextstate(systemic *s, int **scopetable, int whichsystem);
BOOL gotthreesystems(systemic *s, int **scopetable, int *context, int *snum1, int *snum2, int *whichscope, int* t1, int *t2, int *not, int *fn, int *dnum1, int *dnum2);
BOOL onstack(int schptr, int *snum);
int followchain(int ret, chainlink **chain, int chnum, int chainnum, systemic *s,int **scopetable,int *context,int *snum1,int *snum2,int whichscope, unsigned int *iterations);
void compute(systemic *s, int **scopetable, chainlink **chain, int chainnum);
void forcezero(unsigned char *s1, unsigned char *s2);
void loadchains(FILE *fstrm1,chainlink **chain,int *chainnum, systemic *s, int sn);
void loadsystems(FILE *fstrm, systemic *s, int l);
void getword(char *word,char *in, int *l);
void loadscopes(FILE *fstrm, systemic *s,int **scopetable);
void loadfunctions(FILE *fstrm, symbol *f, int fn);
void loadsystemicprogramdimensions(char *fname, int *l, int *fn, long *filepos);
void loadsystemicprogram(systemic *s, char *fname, long filepos, int *l, symbol *f, int *fn, int **scopetable, chainlink **chain, int *chainnum);
BOOL isfunction(char *name, int *n, symbol *function, int fn);
void printfunctionsusage();
void initsystems();
void freesystems();
void loadini(char name[], char fname[4][MAXNAME]);
#endif