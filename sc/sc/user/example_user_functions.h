#ifndef EXUSRFNS
#define EXUSRFNS


//////////////////////////////// SC_USER-PLUGIN-SPECIFIC CODE HERE ////////////////////////////////

// We need to define four key elements here:
// 1. the label names for the functions to be used by the programmer in .sc source files, e.g. "ABSORB" or "COMPARE"
// 2. the function codes - numeric values corresponding to each function
// 3. function-calling - the codes are used to call your own C code functions
// 4. declarations of your own C functions

// 1. user-defined system functions (label names to be used by programmer in code), should appear in same order as listed in SC code
#define USERSYSTEMFUNCTIONS     addfunction("POW");\
                                addfunction("ROOT");\
                                addfunction("ASINEB");\
                                addfunction("ACOSB");\
                                addfunction("ATANB");\
                                addfunction("ADDCHAINLINK");\
                                addfunction("REMOVECHAINLINK");\
                                addfunction("XCHAINS");\
                                addfunction("BITMARK");\
                                addfunction("INCREMENTOR");\
                                addfunction("MULT_V2");\
                                addfunction("BITMARKP1");\
                                addfunction("RECORDER");\
                                addfunction("RESETTER");\
                                addfunction("ESCAPE_BOTH");\
                                addfunction("TAG_FITNESS");\
                                addfunction("TAG_CHAIN");\
                                addfunction("BITMARK2");
                     


// 2. user-defined function codes (numbers less than 31 are reserved)
// each value must be the same as the binary value used in the .sc code or the wrong function will be called
#define POWfn                   32
#define ROOTfn                  33
#define ASINEBfn                34
#define ACOSBfn                 35
#define ATANBfn                 36
#define ADDCHAINLINKfn          37
#define REMOVECHAINLINKfn       38
#define XCHAINSfn               39
#define BITMARKfn               40
#define INCREMENTORfn           41
#define MULT_V2fn               42
#define BITMARKP1fn             43
#define RECORDERfn              44
#define RESETTERfn              45
#define ESCAPE_BOTHfn           46
#define TAG_FITNESSfn           47
#define TAG_CHAINfn             48
#define BITMARK2fn              49


// the total number of user-defined functions above
#define NUMUSERFUNCTIONS        18

// 3. calling the user-defined transformation functions when corresponding function code has been recognised in the .scp code
#define USERTRANSFORMFUNCTIONS \
        case POWfn:             if (_matchedok(m1,m2,t1,t2,not)) { ret = Pow(s[i1].schema2,s[i2].schema2); PRINTFF("\n>{Pow}<"); } break;\
        case ROOTfn:            if (_matchedok(m1,m2,t1,t2,not)) { ret = Root(s[i1].schema2,s[i2].schema2); PRINTFF("\n>{Root}<"); } break;\
        case ASINEBfn:          if (_matchedok(m1,m2,t1,t2,not)) { ret = ASineB(s[i1].schema2,s[i2].schema2); PRINTFF("\n>{ASineB}<"); } break;\
        case ACOSBfn:           if (_matchedok(m1,m2,t1,t2,not)) { ret = ACosB(s[i1].schema2,s[i2].schema2); PRINTFF("\n>{ACosB}<"); } break;\
        case ATANBfn:           if (_matchedok(m1,m2,t1,t2,not)) { ret = ATanB(s[i1].schema2,s[i2].schema2); PRINTFF("\n>{ATanB}<"); } break;\
        case ADDCHAINLINKfn:    if (_matchedok(m1,m2,t1,t2,not)) { ret = AddChainLink(s[i1].schema2,s[i2].schema2,chainnum,chain); PRINTFF("\n>{AddChainLink}<"); } break;\
        case REMOVECHAINLINKfn: if (_matchedok(m1,m2,t1,t2,not)) { ret = RemoveChainLink(s[i1].schema2,s[i2].schema2,chainnum,chain); PRINTFF("\n>{RemoveChainLink}<"); } break;\
        case XCHAINSfn:         if (_matchedok(m1,m2,t1,t2,not)) { ret = XChains(s[i1].schema2,s[i2].schema2,chainnum,chain); PRINTFF("\n>{XChains}<"); } break;\
        case BITMARKfn:         if (_matchedok(m1,m2,t1,t2,not)) { ret = Bitmark(s[i1].schema1, s[i2].schema1); PRINTFF("\n>{Bitmark}<"); } break;\
        case INCREMENTORfn:     if (_matchedok(m1,m2,t1,t2,not)) { ret = Incrementor(s[i1].schema2, s[i2].schema2); PRINTFF("\n>{Incrementor}<"); } break;\
        case MULT_V2fn:         if (_matchedok(m1,m2,t1,t2,not)) { ret = Mult_v2(s[i1].schema1, s[i1].schema2, s[i2].schema2); PRINTFF("\n>{MULT_V2}<"); } break;\
        case BITMARKP1fn:       if (_matchedok(m1,m2,t1,t2,not)) { ret = Bitmarkp1(s[i1].schema1, s[i2].schema1, s[i2].schema2); PRINTFF("\n>{BITMARKP1}<"); } break;\
        case RECORDERfn:        if (_matchedok(m1,m2,t1,t2,not)) { ret = Recorder(s[i1].schema2, s[i2].schema2); PRINTFF("\n>{RECORDER}<"); } break;\
        case RESETTERfn:        if (_matchedok(m1,m2,t1,t2,not)) { ret = Resetter(s[i1].schema1, s[i2].schema1); PRINTFF("\n>{RESETTER}<"); } break;\
        case ESCAPE_BOTHfn:     if (_matchedok(m1,m2,t1,t2,not)) { ret = Escape_both(s,i1,i2,scppos,scopetable); PRINTFF("\n>{ESC_BOTH}<"); } break;\
        case TAG_FITNESSfn:     if (_matchedok(m1,m2,t1,t2,not)) { ret = Tag_fitness(s,s[i1].schema2,scppos,scopetable); PRINTFF("\nTAG_FITNESS"); } break;\
        case TAG_CHAINfn:       if (_matchedok(m1,m2,t1,t2,not)) { ret = Tag_chain(s,i1,scppos,scopetable, chain); PRINTFF("\n TAG_CHAIN"); } break;\
        case BITMARK2fn:        if (_matchedok(m1,m2,t1,t2,not)) { ret = Bitmark2(s,i1,i2,scppos,scopetable, chain, context); PRINTFF("\n BMARK2"); } break;

// 4. user-defined transformation functions in C
int Pow(unsigned char *s1, unsigned char *s2);
int Root(unsigned char *s1, unsigned char *s2);
int ASineB(unsigned char *s1, unsigned char *s2);
int ACosB(unsigned char *s1, unsigned char *s2);
int ATanB(unsigned char *s1, unsigned char *s2);
int AddChainLink(unsigned char *s1, unsigned char *s2, int chainnum, chainlink **chain);
int RemoveChainLink(unsigned char *s1, unsigned char *s2, int chainnum, chainlink **chain);
int XChains(unsigned char *s1, unsigned char *s2, int chainnum, chainlink **chain);
int Bitmark(unsigned char *s1, unsigned char *s2);

int Incrementor(unsigned char *s1, unsigned char *s2);
int Mult_v2(unsigned char *s11,unsigned char *s12, unsigned char *s2);
int Bitmarkp1(unsigned char *s11, unsigned char *s21, unsigned char *s22);
int Recorder(unsigned char *s12, unsigned char *s22);
int Resetter(unsigned char *s11, unsigned char *s21);
int Escape_both(systemic *s, int i1, int i2, int parent,int **scopetable);
int Tag_fitness(systemic *s, unsigned char *s12, int parent, int **scopetable);
int Tag_chain(systemic *s, int i1, int parent, int **scopetable, chainlink **chain);
int Bitmark2(systemic *s, int i1, int i2, int parent, int **scopetable, chainlink **chain, int context);
////////////////////////////////// END OF SC_USER-PLUGIN-SPECIFIC CODE ////////////////////////////////

// functions definitions of standard functionset in case user functions want to call them:
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