// Systemic9 - the nineth experimental systemic computation program
// (c) 2005 - 2014 Peter J Bentley
// version 2 uses a 1 to 3 compression coding scheme in schema instead of the wildcard/wildlength version used previously
// version 3 adds scopes to systems
// version 4 adds variable function coding; reads and writes .scp files compatible with the compiler compilesc (compatible with compilesc)
// version 5 has a reimplementation of scopes, enabling overlapping scopes by the use of a global scope table (compatible with compilesc2)
// version 6 Hardware version compatibility (changed schema code, 32-bit function).
// version 7 adds chaining and removes queues
// version 8 integrates the compiler and computer together, removes functioncode[], corrects faulty function codes and memory handling, restores "scp.ini"
//           Note that "scp.ini" should be placed in same folder as executable. Xcode Preferences... Locations... Derived Data should be relative to Project or Workspace folder
// version 9 adds OpenCL and multi-threading, using twin valid triplets buffers; all memory management is improved; compact scopetable is used in SCP files;
//           compiler speed is optimised for larger SC programs

#include "systemic9.h"


////////////////////////////////////////////////////////// Open CL specific //////////////////////////////////////////////////////////

const char *KernelSource  =                                             "\n" \
"__kernel void compare(                                                 \n" \
"   __global unsigned char *input1,                                     \n" \
"   __global unsigned char *input2,                                     \n" \
"   __global unsigned int *output,                                      \n" \
"   const int ssize,                                                    \n" \
"   const int bsize,                                                    \n" \
"   __local unsigned char *cache2)                                      \n" \
"{                                                                      \n" \
"   int i = get_global_id(0);                                           \n" \
"   int j = i * ssize;                                                  \n" \
"   int n;                                                              \n" \
"   unsigned int ctr;                                                   \n" \
"                                                                       \n" \
"                                                                       \n" \
"   if (j < bsize)                                                      \n" \
"   {   for (n = 0; n < ssize; n++) cache2[n] = input2[n];              \n" \
"       ctr = 0;                                                        \n" \
"       for (n = 0; n < ssize; n++)                                     \n" \
"       {                                                               \n" \
"            if (cache2[n] < 2)                                         \n" \
"            {	if (input1[j+n] > 32)                                   \n" \
"               {                                                       \n" \
"                    ctr++;                                             \n" \
"               }                                                       \n" \
"            else if (input1[j+n] != cache2[n])                         \n" \
"               {                                                       \n" \
"                    ctr++;                                             \n" \
"               }                                                       \n" \
"            }                                                          \n" \
"       }                                                               \n" \
"       output[i] = ctr;                                                \n" \
"    }                                                                  \n" \
"}                                                                      \n" \
"\n";


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BOOL isvalidfunction(systemic *s, int i)
// check if functional region contains non-quaternary values; the system is invalid if so
{
    BOOL ret = TRUE;
    for (int n = 0; n < functionsize; n++)
    {
        if (s[i].function[n] != ZERO && s[i].function[n] != ONE && s[i].function[n] != WILDCARD)
        {
            printf("\n(%d)",s[i].function[n]);
            ret = FALSE;
            break;
        }
    }
    
    return ret;
}

void decode_to_ascii(unsigned char *sch, int len, char *s1, char *s2, char *s3)
// performs the opposite of encode - used for the decompiler
{
    char chunk[4];
    int n, l;
    
    for (n = l = 0; n < len; n++)
    {
        switch (sch[n])
        {
            case '!': chunk[0] = '0'; chunk[1] = '0'; chunk[2] = '0'; chunk[3] = '0'; break;
            case '#': chunk[0] = '0'; chunk[1] = '0'; chunk[2] = '0'; chunk[3] = '1'; break;
            case '%': chunk[0] = '0'; chunk[1] = '0'; chunk[2] = '0'; chunk[3] = '?'; break;
            case '&': chunk[0] = '0'; chunk[1] = '0'; chunk[2] = '1'; chunk[3] = '0'; break;
            case '*': chunk[0] = '0'; chunk[1] = '0'; chunk[2] = '1'; chunk[3] = '1'; break;
            case '+': chunk[0] = '0'; chunk[1] = '0'; chunk[2] = '1'; chunk[3] = '?'; break;
            case ',': chunk[0] = '0'; chunk[1] = '0'; chunk[2] = '?'; chunk[3] = '0'; break;
            case '-': chunk[0] = '0'; chunk[1] = '0'; chunk[2] = '?'; chunk[3] = '1'; break;
            case '.': chunk[0] = '0'; chunk[1] = '0'; chunk[2] = '?'; chunk[3] = '?'; break;
            case '/': chunk[0] = '0'; chunk[1] = '1'; chunk[2] = '0'; chunk[3] = '0'; break;
            case '2': chunk[0] = '0'; chunk[1] = '1'; chunk[2] = '0'; chunk[3] = '1'; break;
            case '3': chunk[0] = '0'; chunk[1] = '1'; chunk[2] = '0'; chunk[3] = '?'; break;
            case '4': chunk[0] = '0'; chunk[1] = '1'; chunk[2] = '1'; chunk[3] = '0'; break;
            case '5': chunk[0] = '0'; chunk[1] = '1'; chunk[2] = '1'; chunk[3] = '1'; break;
            case '6': chunk[0] = '0'; chunk[1] = '1'; chunk[2] = '1'; chunk[3] = '?'; break;
            case '7': chunk[0] = '0'; chunk[1] = '1'; chunk[2] = '?'; chunk[3] = '0'; break;
            case '8': chunk[0] = '0'; chunk[1] = '1'; chunk[2] = '?'; chunk[3] = '1'; break;
            case '9': chunk[0] = '0'; chunk[1] = '1'; chunk[2] = '?'; chunk[3] = '?'; break;
            case ':': chunk[0] = '0'; chunk[1] = '?'; chunk[2] = '0'; chunk[3] = '0'; break;
            case ';': chunk[0] = '0'; chunk[1] = '?'; chunk[2] = '0'; chunk[3] = '1'; break;
            case '<': chunk[0] = '0'; chunk[1] = '?'; chunk[2] = '0'; chunk[3] = '?'; break;
            case '=': chunk[0] = '0'; chunk[1] = '?'; chunk[2] = '1'; chunk[3] = '0'; break;
            case '>': chunk[0] = '0'; chunk[1] = '?'; chunk[2] = '1'; chunk[3] = '1'; break;
            case '[': chunk[0] = '0'; chunk[1] = '?'; chunk[2] = '1'; chunk[3] = '?'; break;
            case ']': chunk[0] = '0'; chunk[1] = '?'; chunk[2] = '?'; chunk[3] = '0'; break;
            case '^': chunk[0] = '0'; chunk[1] = '?'; chunk[2] = '?'; chunk[3] = '1'; break;
            case '_': chunk[0] = '0'; chunk[1] = '?'; chunk[2] = '?'; chunk[3] = '?'; break;
            case '@': chunk[0] = '1'; chunk[1] = '0'; chunk[2] = '0'; chunk[3] = '0'; break;
            case 'A': chunk[0] = '1'; chunk[1] = '0'; chunk[2] = '0'; chunk[3] = '1'; break;
            case 'B': chunk[0] = '1'; chunk[1] = '0'; chunk[2] = '0'; chunk[3] = '?'; break;
            case 'C': chunk[0] = '1'; chunk[1] = '0'; chunk[2] = '1'; chunk[3] = '0'; break;
            case 'D': chunk[0] = '1'; chunk[1] = '0'; chunk[2] = '1'; chunk[3] = '1'; break;
            case 'E': chunk[0] = '1'; chunk[1] = '0'; chunk[2] = '1'; chunk[3] = '?'; break;
            case 'F': chunk[0] = '1'; chunk[1] = '0'; chunk[2] = '?'; chunk[3] = '0'; break;
            case 'G': chunk[0] = '1'; chunk[1] = '0'; chunk[2] = '?'; chunk[3] = '1'; break;
            case 'H': chunk[0] = '1'; chunk[1] = '0'; chunk[2] = '?'; chunk[3] = '?'; break;
            case 'I': chunk[0] = '1'; chunk[1] = '1'; chunk[2] = '0'; chunk[3] = '0'; break;
            case 'J': chunk[0] = '1'; chunk[1] = '1'; chunk[2] = '0'; chunk[3] = '1'; break;
            case 'K': chunk[0] = '1'; chunk[1] = '1'; chunk[2] = '0'; chunk[3] = '?'; break;
            case 'L': chunk[0] = '1'; chunk[1] = '1'; chunk[2] = '1'; chunk[3] = '0'; break;
            case 'M': chunk[0] = '1'; chunk[1] = '1'; chunk[2] = '1'; chunk[3] = '1'; break;
            case 'N': chunk[0] = '1'; chunk[1] = '1'; chunk[2] = '1'; chunk[3] = '?'; break;
            case 'O': chunk[0] = '1'; chunk[1] = '1'; chunk[2] = '?'; chunk[3] = '0'; break;
            case 'P': chunk[0] = '1'; chunk[1] = '1'; chunk[2] = '?'; chunk[3] = '1'; break;
            case 'Q': chunk[0] = '1'; chunk[1] = '1'; chunk[2] = '?'; chunk[3] = '?'; break;
            case 'R': chunk[0] = '1'; chunk[1] = '?'; chunk[2] = '0'; chunk[3] = '0'; break;
            case 'S': chunk[0] = '1'; chunk[1] = '?'; chunk[2] = '0'; chunk[3] = '1'; break;
            case 'T': chunk[0] = '1'; chunk[1] = '?'; chunk[2] = '0'; chunk[3] = '?'; break;
            case 'U': chunk[0] = '1'; chunk[1] = '?'; chunk[2] = '1'; chunk[3] = '0'; break;
            case 'V': chunk[0] = '1'; chunk[1] = '?'; chunk[2] = '1'; chunk[3] = '1'; break;
            case 'W': chunk[0] = '1'; chunk[1] = '?'; chunk[2] = '1'; chunk[3] = '?'; break;
            case 'X': chunk[0] = '1'; chunk[1] = '?'; chunk[2] = '?'; chunk[3] = '0'; break;
            case 'Y': chunk[0] = '1'; chunk[1] = '?'; chunk[2] = '?'; chunk[3] = '1'; break;
            case 'Z': chunk[0] = '1'; chunk[1] = '?'; chunk[2] = '?'; chunk[3] = '?'; break;
            case '`': chunk[0] = '?'; chunk[1] = '0'; chunk[2] = '0'; chunk[3] = '0'; break;
            case 'a': chunk[0] = '?'; chunk[1] = '0'; chunk[2] = '0'; chunk[3] = '1'; break;
            case 'b': chunk[0] = '?'; chunk[1] = '0'; chunk[2] = '0'; chunk[3] = '?'; break;
            case 'c': chunk[0] = '?'; chunk[1] = '0'; chunk[2] = '1'; chunk[3] = '0'; break;
            case 'd': chunk[0] = '?'; chunk[1] = '0'; chunk[2] = '1'; chunk[3] = '1'; break;
            case 'e': chunk[0] = '?'; chunk[1] = '0'; chunk[2] = '1'; chunk[3] = '?'; break;
            case 'f': chunk[0] = '?'; chunk[1] = '0'; chunk[2] = '?'; chunk[3] = '0'; break;
            case 'g': chunk[0] = '?'; chunk[1] = '0'; chunk[2] = '?'; chunk[3] = '1'; break;
            case 'h': chunk[0] = '?'; chunk[1] = '0'; chunk[2] = '?'; chunk[3] = '?'; break;
            case 'i': chunk[0] = '?'; chunk[1] = '1'; chunk[2] = '0'; chunk[3] = '0'; break;
            case 'j': chunk[0] = '?'; chunk[1] = '1'; chunk[2] = '0'; chunk[3] = '1'; break;
            case 'k': chunk[0] = '?'; chunk[1] = '1'; chunk[2] = '0'; chunk[3] = '?'; break;
            case 'l': chunk[0] = '?'; chunk[1] = '1'; chunk[2] = '1'; chunk[3] = '0'; break;
            case 'm': chunk[0] = '?'; chunk[1] = '1'; chunk[2] = '1'; chunk[3] = '1'; break;
            case 'n': chunk[0] = '?'; chunk[1] = '1'; chunk[2] = '1'; chunk[3] = '?'; break;
            case 'o': chunk[0] = '?'; chunk[1] = '1'; chunk[2] = '?'; chunk[3] = '0'; break;
            case 'p': chunk[0] = '?'; chunk[1] = '1'; chunk[2] = '?'; chunk[3] = '1'; break;
            case 'q': chunk[0] = '?'; chunk[1] = '1'; chunk[2] = '?'; chunk[3] = '?'; break;
            case 'r': chunk[0] = '?'; chunk[1] = '?'; chunk[2] = '0'; chunk[3] = '0'; break;
            case 's': chunk[0] = '?'; chunk[1] = '?'; chunk[2] = '0'; chunk[3] = '1'; break;
            case 't': chunk[0] = '?'; chunk[1] = '?'; chunk[2] = '0'; chunk[3] = '?'; break;
            case 'u': chunk[0] = '?'; chunk[1] = '?'; chunk[2] = '1'; chunk[3] = '0'; break;
            case 'v': chunk[0] = '?'; chunk[1] = '?'; chunk[2] = '1'; chunk[3] = '1'; break;
            case 'w': chunk[0] = '?'; chunk[1] = '?'; chunk[2] = '1'; chunk[3] = '?'; break;
            case 'x': chunk[0] = '?'; chunk[1] = '?'; chunk[2] = '?'; chunk[3] = '0'; break;
            case 'y': chunk[0] = '?'; chunk[1] = '?'; chunk[2] = '?'; chunk[3] = '1'; break;
            default: chunk[0] = '?'; chunk[1] = '?'; chunk[2] = '?'; chunk[3] = '?';
        }
        
        if (l < schemasize) // we're working on schema1
        {
            s1[l++] = chunk[0];
            s1[l++] = chunk[1];
            s1[l++] = chunk[2];
            s1[l++] = chunk[3];
        }
        else if (l < schemasize+functionsize) // we're working on the function
        {
            s2[l-schemasize] = chunk[0]; l++;
            s2[l-schemasize] = chunk[1]; l++;
            s2[l-schemasize] = chunk[2]; l++;
            s2[l-schemasize] = chunk[3]; l++;
        }
        else // we're working on schema2
        {
            s3[l-schemasize-functionsize] = chunk[0]; l++;
            s3[l-schemasize-functionsize] = chunk[1]; l++;
            s3[l-schemasize-functionsize] = chunk[2]; l++;
            s3[l-schemasize-functionsize] = chunk[3]; l++;
        }

    }
    s1[schemasize] = s2[functionsize] = s3[schemasize] = '\0';
}

void encode(unsigned char *out, unsigned char *in1, unsigned char *in2, unsigned char *in3)
// encodes 64-bit in1 & in3 as 16-char schema and 32-char in2 function 方程都是给的32位的
{   int n,i;
    unsigned char sys[schemasize+functionsize+schemasize];

    i = 0;
    // fill sys with inputs
    for (n = 0; n < schemasize; n++) { sys[i] = in1[n]; i++; }
    for (n = 0; n < functionsize; n++) { sys[i] = in2[n]; i++; }
    for (n = 0; n < schemasize; n++) { sys[i] = in3[n]; i++; }

    // encode sys, 4 bits at a time 变成四位了？
    for (i = 0; i < schemasize+schemasize+functionsize; i += 4)
	{  
		switch(sys[i])
		{
			case 0:
				switch(sys[i+1])
				{ 
					case 0:
						switch(sys[i+2])
						{ 
							case 0:
								switch(sys[i+3])
								{ 
									case 0:  out[i/4] = '!'; break;
									case 1:  out[i/4] = '#'; break;
									default: out[i/4] = '%';
								}
								break;
							case 1:
								switch(sys[i+3])
								{ 
									case 0: out [i/4] = '&'; break;
									case 1: out [i/4] = '*'; break;
									default: out[i/4] = '+';
								}
								break;
							default:
								switch(sys[i+3])
								{ 
									case 0:  out[i/4] = ','; break;
									case 1:  out[i/4] = '-'; break;
									default: out[i/4] = '.';
								}
						}
						break;
					case 1:
						switch(sys[i+2])
						{ 
							case 0:
								switch(sys[i+3])
								{ 
									case 0:  out[i/4] = '/'; break;
									case 1:  out[i/4] = '2'; break;
									default: out[i/4] = '3';
								}
								break;
							case 1:
								switch(sys[i+3])
								{ 
									case 0:  out[i/4] = '4'; break;
									case 1:  out[i/4] = '5'; break;
									default: out[i/4] = '6';
								}
								break;
							default:
								switch(sys[i+3])
								{ 
									case 0:  out[i/4] = '7'; break;
									case 1:  out[i/4] = '8'; break;
									default: out[i/4] = '9';
								}
						}
						break;
					default:
						switch(sys[i+2])
						{ 
							case 0:
								switch(sys[i+3])
								{ 
									case 0:  out[i/4] = ':'; break;
									case 1:  out[i/4] = ';'; break;
									default: out[i/4] = '<';
								}
								break;
							case 1:
								switch(sys[i+3])
								{ 
									case 0:  out[i/4] = '='; break;
									case 1:  out[i/4] = '>'; break;
									default: out[i/4] = '[';
								}
								break;
							default:
								switch(sys[i+3])
								{ 
									case 0:  out[i/4] = ']'; break;
									case 1:  out[i/4] = '^'; break;
									default: out[i/4] = '_';
								}
						}
				}
				break;
			case 1:
				switch(sys[i+1])
				{ 
					case 0:
						switch(sys[i+2])
						{ 
							case 0:
								switch(sys[i+3])
								{ 
									case 0:  out[i/4] = '@'; break;
									case 1:  out[i/4] = 'A'; break;
									default: out[i/4] = 'B';
								}
								break;
							case 1:
								switch(sys[i+3])
								{ 
									case 0: out [i/4] = 'C'; break;
									case 1: out [i/4] = 'D'; break;
									default: out[i/4] = 'E';
								}
								break;
							default:
								switch(sys[i+3])
								{ 
									case 0:  out[i/4] = 'F'; break;
									case 1:  out[i/4] = 'G'; break;
									default: out[i/4] = 'H';
								}
						}
						break;
					case 1:
						switch(sys[i+2])
						{ 
							case 0:
								switch(sys[i+3])
								{ 
									case 0:  out[i/4] = 'I'; break;
									case 1:  out[i/4] = 'J'; break;
									default: out[i/4] = 'K';
								}
								break;
							case 1:
								switch(sys[i+3])
								{ 
									case 0:  out[i/4] = 'L'; break;
									case 1:  out[i/4] = 'M'; break;
									default: out[i/4] = 'N';
								}
								break;
							default:
								switch(sys[i+3])
								{ 
									case 0:  out[i/4] = 'O'; break;
									case 1:  out[i/4] = 'P'; break;
									default: out[i/4] = 'Q';
								}
						}
						break;
					default:
						switch(sys[i+2])
						{ 
							case 0:
								switch(sys[i+3])
								{ 
									case 0:  out[i/4] = 'R'; break;
									case 1:  out[i/4] = 'S'; break;
									default: out[i/4] = 'T';
								}
								break;
							case 1:
								switch(sys[i+3])
								{ 
									case 0:  out[i/4] = 'U'; break;
									case 1:  out[i/4] = 'V'; break;
									default: out[i/4] = 'W';
								}
								break;
							default:
								switch(sys[i+3])
								{ 
									case 0:  out[i/4] = 'X'; break;
									case 1:  out[i/4] = 'Y'; break;
									default: out[i/4] = 'Z';
								}
						}
				}
				break;
			default:
				switch(sys[i+1])
				{ 
					case 0:
						switch(sys[i+2])
						{ 
							case 0:
								switch(sys[i+3])
								{ 
									case 0:  out[i/4] = '`'; break;
									case 1:  out[i/4] = 'a'; break;
									default: out[i/4] = 'b';
								}
								break;
							case 1:
								switch(sys[i+3])
								{ 
									case 0: out [i/4] = 'c'; break;
									case 1: out [i/4] = 'd'; break;
									default: out[i/4] = 'e';
								}
								break;
							default:
								switch(sys[i+3])
								{ 
									case 0:  out[i/4] = 'f'; break;
									case 1:  out[i/4] = 'g'; break;
									default: out[i/4] = 'h';
								}
						}
						break;
					case 1:
						switch(sys[i+2])
						{ 
							case 0:
								switch(sys[i+3])
								{ 
									case 0:  out[i/4] = 'i'; break;
									case 1:  out[i/4] = 'j'; break;
									default: out[i/4] = 'k';
								}
								break;
							case 1:
								switch(sys[i+3])
								{ 
									case 0:  out[i/4] = 'l'; break;
									case 1:  out[i/4] = 'm'; break;
									default: out[i/4] = 'n';
								}
								break;
							default:
								switch(sys[i+3])
								{ 
									case 0:  out[i/4] = 'o'; break;
									case 1:  out[i/4] = 'p'; break;
									default: out[i/4] = 'q';
								}
						}
						break;
					default:
						switch(sys[i+2])
						{ 
							case 0:
								switch(sys[i+3])
								{ 
									case 0:  out[i/4] = 'r'; break;
									case 1:  out[i/4] = 's'; break;
									default: out[i/4] = 't';
								}
								break;
							case 1:
								switch(sys[i+3])
								{ 
									case 0:  out[i/4] = 'u'; break;
									case 1:  out[i/4] = 'v'; break;
									default: out[i/4] = 'w';
								}
								break;
							default:
								switch(sys[i+3])
								{ 
									case 0:  out[i/4] = 'x'; break;
									case 1:  out[i/4] = 'y'; break;
									default: out[i/4] = 'z';
								}
						}
				}
		}
	}
}

//__inline void sysswap(systemic *s, int i1, int i2)
//// swaps s[i1] with s[i2]
//{   int n;
//    unsigned char uctmp;
//
//    for (n = 0; n < schemasize; n++)
//    {  swap(s[i1].schema1[n],s[i2].schema1[n],uctmp);
//       swap(s[i1].schema2[n],s[i2].schema2[n],uctmp);       
//    }
//    for (n = 0; n < FUNCTIONSIZE; n++)
//    {  swap(s[i1].function[n],s[i2].function[n],uctmp);
//    }
//}

int syscopy(systemic *tmp, int i1, systemic *s, int i2)
// copies s[i1] to tmp[i2]
{   int n;

    for (n = 0; n < schemasize; n++)
    {  tmp[i1].schema1[n] = s[i2].schema1[n];
       tmp[i1].schema2[n] = s[i2].schema2[n];       
    }
    for (n = 0; n < FUNCTIONSIZE; n++)
    {  tmp[i1].function[n] = s[i2].function[n];
    }
    return 0;
}

void atoschema(char *str, int l, unsigned char *sc)
// converts the ascii string in str[] to schema format in sc[]
// uses new compression scheme introduced in v2
{   int n;

    for (n = 0; n < l; n++)
    {  
//	   if (str[n] >= '`' && str[n] <= 'z')  // codes go from ascii 96 to 122
	   if (str[n] >= '!' && str[n] <= 'z')  // changed (hardware) codes go from ascii 96 to 122
         sc[n] = str[n];
       if (str[n] == '0') sc[n] = ZERO;
       if (str[n] == '1') sc[n] = ONE;
    }
}

void schematoi(unsigned char *schema, int l, int *num)
// transforms the first l bits of binary schema[] to integer num; ignores non-binary chars and doesn't check overflows
{   int n,a;

    *num = 0; a = 1;
    for (n = 0; n < l; n++)
    {  if (schema[n] == ONE) *num += a;
       a = a << 1;
    }
}

void schemaparttoi(unsigned char *schema, int s, int l, int *num)
// transforms l bits of binary schema[] starting from bit s to integer num; ignores non-binary chars and doesn't check overflows
{   int n,a;
    
    *num = 0; a = 1;
    for (n = 0; n < l; n++)
    {  if (schema[n+s] == ONE) *num += a;
        a = a << 1;
    }
}

void itoschema(int num, unsigned char *schema)
// transforms num into binary and places in schema[]
{   
	int n,p;

	if (num < 0)
	{
		schema[schemasize-1] = ONE;
		num = (int)pow(2.0,schemasize-1)+num;
	}
	else
		schema[schemasize-1] = ZERO;

    p = (int)pow(2.0,schemasize-2);
    for (n = schemasize-2; n >= 0; n--)
    {  if (num >= p)
       { num -= p;
         schema[n] = ONE;
       }
       else
         schema[n] = ZERO;
       p = p >> 1;
    }
}

void itoschemapart(int num, int s, int l, unsigned char *schema)
// transforms positive num into binary with length l
// and places it in schema[] starting at bit s
{
	int n,p;
    
    p = (int)pow(2.0,l-1);
    for (n = s+l-1; n >= s; n--)
    {  if (num >= p)
    { num -= p;
        schema[n] = ONE;
    }
    else
        schema[n] = ZERO;
        p = p >> 1;
    }
}

void itofunction(int num, unsigned char *function)
// transforms num into binary and places in function[]
{   
	unsigned long int p;
	int n;

	if (num < 0)
	{
		printf("ERROR : Only non-negative numbers can be transformed to a function");
	}
	else
	{
		p = (unsigned long int)pow(2.0,iniFUNCTIONID-1);
		for (n = iniFUNCTIONID-1; n >= 0; n--)
		{  
			if ((unsigned long int) num >= p)
			{ 
				num -= p;
				function[n] = ONE;
			}
			else
				function[n] = ZERO;
			p = p >> 1;
		}
	}
}

void decode_to_quaternary(unsigned char *c, unsigned char *sch)
// decodes schema into a single flat quaternary array ready for comparisons with other systems
{
    int n = 0, i = 0;
    do
    { switch (c[n])
        {
            case '!': sch[i] = 0;        sch[i+1] = 0;        sch[i+2] = 0;        sch[i+3] = 0;        i+=4; break;
            case '#': sch[i] = 0;        sch[i+1] = 0;        sch[i+2] = 0;        sch[i+3] = 1;        i+=4; break;
            case '%': sch[i] = 0;        sch[i+1] = 0;        sch[i+2] = 0;        sch[i+3] = WILDCARD; i+=4; break;
            case '&': sch[i] = 0;        sch[i+1] = 0;        sch[i+2] = 1;        sch[i+3] = 0;        i+=4; break;
            case '*': sch[i] = 0;        sch[i+1] = 0;        sch[i+2] = 1;        sch[i+3] = 1;        i+=4; break;
            case '+': sch[i] = 0;        sch[i+1] = 0;        sch[i+2] = 1;        sch[i+3] = WILDCARD; i+=4; break;
            case ',': sch[i] = 0;        sch[i+1] = 0;        sch[i+2] = WILDCARD; sch[i+3] = 0;        i+=4; break;
            case '-': sch[i] = 0;        sch[i+1] = 0;        sch[i+2] = WILDCARD; sch[i+3] = 1;        i+=4; break;
            case '.': sch[i] = 0;        sch[i+1] = 0;        sch[i+2] = WILDCARD; sch[i+3] = WILDCARD; i+=4; break;
            case '/': sch[i] = 0;        sch[i+1] = 1;        sch[i+2] = 0;        sch[i+3] = 0;        i+=4; break;
            case '2': sch[i] = 0;        sch[i+1] = 1;        sch[i+2] = 0;        sch[i+3] = 1;        i+=4; break;
            case '3': sch[i] = 0;        sch[i+1] = 1;        sch[i+2] = 0;        sch[i+3] = WILDCARD; i+=4; break;
            case '4': sch[i] = 0;        sch[i+1] = 1;        sch[i+2] = 1;        sch[i+3] = 0;        i+=4; break;
            case '5': sch[i] = 0;        sch[i+1] = 1;        sch[i+2] = 1;        sch[i+3] = 1;        i+=4; break;
            case '6': sch[i] = 0;        sch[i+1] = 1;        sch[i+2] = 1;        sch[i+3] = WILDCARD; i+=4; break;
            case '7': sch[i] = 0;        sch[i+1] = 1;        sch[i+2] = WILDCARD; sch[i+3] = 0;        i+=4; break;
            case '8': sch[i] = 0;        sch[i+1] = 1;        sch[i+2] = WILDCARD; sch[i+3] = 1;        i+=4; break;
            case '9': sch[i] = 0;        sch[i+1] = 1;        sch[i+2] = WILDCARD; sch[i+3] = WILDCARD; i+=4; break;
            case ':': sch[i] = 0;        sch[i+1] = WILDCARD; sch[i+2] = 0;        sch[i+3] = 0;        i+=4; break;
            case ';': sch[i] = 0;        sch[i+1] = WILDCARD; sch[i+2] = 0;        sch[i+3] = 1;        i+=4; break;
            case '<': sch[i] = 0;        sch[i+1] = WILDCARD; sch[i+2] = 0;        sch[i+3] = WILDCARD; i+=4; break;
            case '=': sch[i] = 0;        sch[i+1] = WILDCARD; sch[i+2] = 1;        sch[i+3] = 0;        i+=4; break;
            case '>': sch[i] = 0;        sch[i+1] = WILDCARD; sch[i+2] = 1;        sch[i+3] = 1;        i+=4; break;
            case '[': sch[i] = 0;        sch[i+1] = WILDCARD; sch[i+2] = 1;        sch[i+3] = WILDCARD; i+=4; break;
            case ']': sch[i] = 0;        sch[i+1] = WILDCARD; sch[i+2] = WILDCARD; sch[i+3] = 0;        i+=4; break;
            case '^': sch[i] = 0;        sch[i+1] = WILDCARD; sch[i+2] = WILDCARD; sch[i+3] = 1;        i+=4; break;
            case '_': sch[i] = 0;        sch[i+1] = WILDCARD; sch[i+2] = WILDCARD; sch[i+3] = WILDCARD; i+=4; break;
                
            case '@': sch[i] = 1;        sch[i+1] = 0;        sch[i+2] = 0;        sch[i+3] = 0;        i+=4; break;
            case 'A': sch[i] = 1;        sch[i+1] = 0;        sch[i+2] = 0;        sch[i+3] = 1;        i+=4; break;
            case 'B': sch[i] = 1;        sch[i+1] = 0;        sch[i+2] = 0;        sch[i+3] = WILDCARD; i+=4; break;
            case 'C': sch[i] = 1;        sch[i+1] = 0;        sch[i+2] = 1;        sch[i+3] = 0;        i+=4; break;
            case 'D': sch[i] = 1;        sch[i+1] = 0;        sch[i+2] = 1;        sch[i+3] = 1;        i+=4; break;
            case 'E': sch[i] = 1;        sch[i+1] = 0;        sch[i+2] = 1;        sch[i+3] = WILDCARD; i+=4; break;
            case 'F': sch[i] = 1;        sch[i+1] = 0;        sch[i+2] = WILDCARD; sch[i+3] = 0;        i+=4; break;
            case 'G': sch[i] = 1;        sch[i+1] = 0;        sch[i+2] = WILDCARD; sch[i+3] = 1;        i+=4; break;
            case 'H': sch[i] = 1;        sch[i+1] = 0;        sch[i+2] = WILDCARD; sch[i+3] = WILDCARD; i+=4; break;
            case 'I': sch[i] = 1;        sch[i+1] = 1;        sch[i+2] = 0;        sch[i+3] = 0;        i+=4; break;
            case 'J': sch[i] = 1;        sch[i+1] = 1;        sch[i+2] = 0;        sch[i+3] = 1;        i+=4; break;
            case 'K': sch[i] = 1;        sch[i+1] = 1;        sch[i+2] = 0;        sch[i+3] = WILDCARD; i+=4; break;
            case 'L': sch[i] = 1;        sch[i+1] = 1;        sch[i+2] = 1;        sch[i+3] = 0;        i+=4; break;
            case 'M': sch[i] = 1;        sch[i+1] = 1;        sch[i+2] = 1;        sch[i+3] = 1;        i+=4; break;
            case 'N': sch[i] = 1;        sch[i+1] = 1;        sch[i+2] = 1;        sch[i+3] = WILDCARD; i+=4; break;
            case 'O': sch[i] = 1;        sch[i+1] = 1;        sch[i+2] = WILDCARD; sch[i+3] = 0;        i+=4; break;
            case 'P': sch[i] = 1;        sch[i+1] = 1;        sch[i+2] = WILDCARD; sch[i+3] = 1;        i+=4; break;
            case 'Q': sch[i] = 1;        sch[i+1] = 1;        sch[i+2] = WILDCARD; sch[i+3] = WILDCARD; i+=4; break;
            case 'R': sch[i] = 1;        sch[i+1] = WILDCARD; sch[i+2] = 0;        sch[i+3] = 0;        i+=4; break;
            case 'S': sch[i] = 1;        sch[i+1] = WILDCARD; sch[i+2] = 0;        sch[i+3] = 1;        i+=4; break;
            case 'T': sch[i] = 1;        sch[i+1] = WILDCARD; sch[i+2] = 0;        sch[i+3] = WILDCARD; i+=4; break;
            case 'U': sch[i] = 1;        sch[i+1] = WILDCARD; sch[i+2] = 1;        sch[i+3] = 0;        i+=4; break;
            case 'V': sch[i] = 1;        sch[i+1] = WILDCARD; sch[i+2] = 1;        sch[i+3] = 1;        i+=4; break;
            case 'W': sch[i] = 1;        sch[i+1] = WILDCARD; sch[i+2] = 1;        sch[i+3] = WILDCARD; i+=4; break;
            case 'X': sch[i] = 1;        sch[i+1] = WILDCARD; sch[i+2] = WILDCARD; sch[i+3] = 0;        i+=4; break;
            case 'Y': sch[i] = 1;        sch[i+1] = WILDCARD; sch[i+2] = WILDCARD; sch[i+3] = 1;        i+=4; break;
            case 'Z': sch[i] = 1;        sch[i+1] = WILDCARD; sch[i+2] = WILDCARD; sch[i+3] = WILDCARD; i+=4; break;
                
            case '`': sch[i] = WILDCARD; sch[i+1] = 0;        sch[i+2] = 0;        sch[i+3] = 0;        i+=4; break;
            case 'a': sch[i] = WILDCARD; sch[i+1] = 0;        sch[i+2] = 0;        sch[i+3] = 1;        i+=4; break;
            case 'b': sch[i] = WILDCARD; sch[i+1] = 0;        sch[i+2] = 0;        sch[i+3] = WILDCARD; i+=4; break;
            case 'c': sch[i] = WILDCARD; sch[i+1] = 0;        sch[i+2] = 1;        sch[i+3] = 0;        i+=4; break;
            case 'd': sch[i] = WILDCARD; sch[i+1] = 0;        sch[i+2] = 1;        sch[i+3] = 1;        i+=4; break;
            case 'e': sch[i] = WILDCARD; sch[i+1] = 0;        sch[i+2] = 1;        sch[i+3] = WILDCARD; i+=4; break;
            case 'f': sch[i] = WILDCARD; sch[i+1] = 0;        sch[i+2] = WILDCARD; sch[i+3] = 0;        i+=4; break;
            case 'g': sch[i] = WILDCARD; sch[i+1] = 0;        sch[i+2] = WILDCARD; sch[i+3] = 1;        i+=4; break;
            case 'h': sch[i] = WILDCARD; sch[i+1] = 0;        sch[i+2] = WILDCARD; sch[i+3] = WILDCARD; i+=4; break;
            case 'i': sch[i] = WILDCARD; sch[i+1] = 1;        sch[i+2] = 0;        sch[i+3] = 0;        i+=4; break;
            case 'j': sch[i] = WILDCARD; sch[i+1] = 1;        sch[i+2] = 0;        sch[i+3] = 1;        i+=4; break;
            case 'k': sch[i] = WILDCARD; sch[i+1] = 1;        sch[i+2] = 0;        sch[i+3] = WILDCARD; i+=4; break;
            case 'l': sch[i] = WILDCARD; sch[i+1] = 1;        sch[i+2] = 1;        sch[i+3] = 0;        i+=4; break;
            case 'm': sch[i] = WILDCARD; sch[i+1] = 1;        sch[i+2] = 1;        sch[i+3] = 1;        i+=4; break;
            case 'n': sch[i] = WILDCARD; sch[i+1] = 1;        sch[i+2] = 1;        sch[i+3] = WILDCARD; i+=4; break;
            case 'o': sch[i] = WILDCARD; sch[i+1] = 1;        sch[i+2] = WILDCARD; sch[i+3] = 0;        i+=4; break;
            case 'p': sch[i] = WILDCARD; sch[i+1] = 1;        sch[i+2] = WILDCARD; sch[i+3] = 1;        i+=4; break;
            case 'q': sch[i] = WILDCARD; sch[i+1] = 1;        sch[i+2] = WILDCARD; sch[i+3] = WILDCARD; i+=4; break;
            case 'r': sch[i] = WILDCARD; sch[i+1] = WILDCARD; sch[i+2] = 0;        sch[i+3] = 0;        i+=4; break;
            case 's': sch[i] = WILDCARD; sch[i+1] = WILDCARD; sch[i+2] = 0;        sch[i+3] = 1;        i+=4; break;
            case 't': sch[i] = WILDCARD; sch[i+1] = WILDCARD; sch[i+2] = 0;        sch[i+3] = WILDCARD; i+=4; break;
            case 'u': sch[i] = WILDCARD; sch[i+1] = WILDCARD; sch[i+2] = 1;        sch[i+3] = 0;        i+=4; break;
            case 'v': sch[i] = WILDCARD; sch[i+1] = WILDCARD; sch[i+2] = 1;        sch[i+3] = 1;        i+=4; break;
            case 'w': sch[i] = WILDCARD; sch[i+1] = WILDCARD; sch[i+2] = 1;        sch[i+3] = WILDCARD; i+=4; break; 
            case 'x': sch[i] = WILDCARD; sch[i+1] = WILDCARD; sch[i+2] = WILDCARD; sch[i+3] = 0;        i+=4; break; 
            case 'y': sch[i] = WILDCARD; sch[i+1] = WILDCARD; sch[i+2] = WILDCARD; sch[i+3] = 1;        i+=4; break; 
            default : sch[i] = WILDCARD; sch[i+1] = WILDCARD; sch[i+2] = WILDCARD; sch[i+3] = WILDCARD; i+=4; break;
        }
        n++;
    } while (i < schemasize+functionsize+schemasize);
}

int difference(unsigned char *c, systemic *s)
// returns a difference score for how different s is from c (higher is more different)
{   int diff,n,i;
    unsigned char sys[schemasize+functionsize+schemasize];
    unsigned char sch[schemasize+functionsize+schemasize];

	//PRINTF("\ndifference function");

    i = 0;
    // fill sys with s
    for (n = 0; n < schemasize; n++) { sys[i] = s->schema1[n]; i++; }
    for (n = 0; n < functionsize; n++) { sys[i] = s->function[n]; i++; }
    for (n = 0; n < schemasize; n++) { sys[i] = s->schema2[n]; i++; }
    i = 0;
    // fill sch with uncompressed c[]
    n = 0;
    do
    { switch (c[n])
		{    
			case '!': sch[i] = 0;        sch[i+1] = 0;        sch[i+2] = 0;        sch[i+3] = 0;        i+=4; break;
			case '#': sch[i] = 0;        sch[i+1] = 0;        sch[i+2] = 0;        sch[i+3] = 1;        i+=4; break; 
			case '%': sch[i] = 0;        sch[i+1] = 0;        sch[i+2] = 0;        sch[i+3] = WILDCARD; i+=4; break; 
			case '&': sch[i] = 0;        sch[i+1] = 0;        sch[i+2] = 1;        sch[i+3] = 0;        i+=4; break; 
			case '*': sch[i] = 0;        sch[i+1] = 0;        sch[i+2] = 1;        sch[i+3] = 1;        i+=4; break; 
			case '+': sch[i] = 0;        sch[i+1] = 0;        sch[i+2] = 1;        sch[i+3] = WILDCARD; i+=4; break; 
			case ',': sch[i] = 0;        sch[i+1] = 0;        sch[i+2] = WILDCARD; sch[i+3] = 0;        i+=4; break; 
			case '-': sch[i] = 0;        sch[i+1] = 0;        sch[i+2] = WILDCARD; sch[i+3] = 1;        i+=4; break; 
			case '.': sch[i] = 0;        sch[i+1] = 0;        sch[i+2] = WILDCARD; sch[i+3] = WILDCARD; i+=4; break; 
			case '/': sch[i] = 0;        sch[i+1] = 1;        sch[i+2] = 0;        sch[i+3] = 0;        i+=4; break; 
			case '2': sch[i] = 0;        sch[i+1] = 1;        sch[i+2] = 0;        sch[i+3] = 1;        i+=4; break; 
			case '3': sch[i] = 0;        sch[i+1] = 1;        sch[i+2] = 0;        sch[i+3] = WILDCARD; i+=4; break; 
			case '4': sch[i] = 0;        sch[i+1] = 1;        sch[i+2] = 1;        sch[i+3] = 0;        i+=4; break; 
			case '5': sch[i] = 0;        sch[i+1] = 1;        sch[i+2] = 1;        sch[i+3] = 1;        i+=4; break; 
			case '6': sch[i] = 0;        sch[i+1] = 1;        sch[i+2] = 1;        sch[i+3] = WILDCARD; i+=4; break; 
			case '7': sch[i] = 0;        sch[i+1] = 1;        sch[i+2] = WILDCARD; sch[i+3] = 0;        i+=4; break; 
			case '8': sch[i] = 0;        sch[i+1] = 1;        sch[i+2] = WILDCARD; sch[i+3] = 1;        i+=4; break; 
			case '9': sch[i] = 0;        sch[i+1] = 1;        sch[i+2] = WILDCARD; sch[i+3] = WILDCARD; i+=4; break; 
			case ':': sch[i] = 0;        sch[i+1] = WILDCARD; sch[i+2] = 0;        sch[i+3] = 0;        i+=4; break; 
			case ';': sch[i] = 0;        sch[i+1] = WILDCARD; sch[i+2] = 0;        sch[i+3] = 1;        i+=4; break; 
			case '<': sch[i] = 0;        sch[i+1] = WILDCARD; sch[i+2] = 0;        sch[i+3] = WILDCARD; i+=4; break; 
			case '=': sch[i] = 0;        sch[i+1] = WILDCARD; sch[i+2] = 1;        sch[i+3] = 0;        i+=4; break; 
			case '>': sch[i] = 0;        sch[i+1] = WILDCARD; sch[i+2] = 1;        sch[i+3] = 1;        i+=4; break; 
			case '[': sch[i] = 0;        sch[i+1] = WILDCARD; sch[i+2] = 1;        sch[i+3] = WILDCARD; i+=4; break; 
			case ']': sch[i] = 0;        sch[i+1] = WILDCARD; sch[i+2] = WILDCARD; sch[i+3] = 0;        i+=4; break; 
			case '^': sch[i] = 0;        sch[i+1] = WILDCARD; sch[i+2] = WILDCARD; sch[i+3] = 1;        i+=4; break; 
			case '_': sch[i] = 0;        sch[i+1] = WILDCARD; sch[i+2] = WILDCARD; sch[i+3] = WILDCARD; i+=4; break;
                                                                                                       
			case '@': sch[i] = 1;        sch[i+1] = 0;        sch[i+2] = 0;        sch[i+3] = 0;        i+=4; break;
			case 'A': sch[i] = 1;        sch[i+1] = 0;        sch[i+2] = 0;        sch[i+3] = 1;        i+=4; break; 
			case 'B': sch[i] = 1;        sch[i+1] = 0;        sch[i+2] = 0;        sch[i+3] = WILDCARD; i+=4; break; 
			case 'C': sch[i] = 1;        sch[i+1] = 0;        sch[i+2] = 1;        sch[i+3] = 0;        i+=4; break; 
			case 'D': sch[i] = 1;        sch[i+1] = 0;        sch[i+2] = 1;        sch[i+3] = 1;        i+=4; break; 
			case 'E': sch[i] = 1;        sch[i+1] = 0;        sch[i+2] = 1;        sch[i+3] = WILDCARD; i+=4; break; 
			case 'F': sch[i] = 1;        sch[i+1] = 0;        sch[i+2] = WILDCARD; sch[i+3] = 0;        i+=4; break; 
			case 'G': sch[i] = 1;        sch[i+1] = 0;        sch[i+2] = WILDCARD; sch[i+3] = 1;        i+=4; break; 
			case 'H': sch[i] = 1;        sch[i+1] = 0;        sch[i+2] = WILDCARD; sch[i+3] = WILDCARD; i+=4; break; 
			case 'I': sch[i] = 1;        sch[i+1] = 1;        sch[i+2] = 0;        sch[i+3] = 0;        i+=4; break; 
			case 'J': sch[i] = 1;        sch[i+1] = 1;        sch[i+2] = 0;        sch[i+3] = 1;        i+=4; break; 
			case 'K': sch[i] = 1;        sch[i+1] = 1;        sch[i+2] = 0;        sch[i+3] = WILDCARD; i+=4; break; 
			case 'L': sch[i] = 1;        sch[i+1] = 1;        sch[i+2] = 1;        sch[i+3] = 0;        i+=4; break; 
			case 'M': sch[i] = 1;        sch[i+1] = 1;        sch[i+2] = 1;        sch[i+3] = 1;        i+=4; break; 
			case 'N': sch[i] = 1;        sch[i+1] = 1;        sch[i+2] = 1;        sch[i+3] = WILDCARD; i+=4; break; 
			case 'O': sch[i] = 1;        sch[i+1] = 1;        sch[i+2] = WILDCARD; sch[i+3] = 0;        i+=4; break; 
			case 'P': sch[i] = 1;        sch[i+1] = 1;        sch[i+2] = WILDCARD; sch[i+3] = 1;        i+=4; break; 
			case 'Q': sch[i] = 1;        sch[i+1] = 1;        sch[i+2] = WILDCARD; sch[i+3] = WILDCARD; i+=4; break; 
			case 'R': sch[i] = 1;        sch[i+1] = WILDCARD; sch[i+2] = 0;        sch[i+3] = 0;        i+=4; break; 
			case 'S': sch[i] = 1;        sch[i+1] = WILDCARD; sch[i+2] = 0;        sch[i+3] = 1;        i+=4; break; 
			case 'T': sch[i] = 1;        sch[i+1] = WILDCARD; sch[i+2] = 0;        sch[i+3] = WILDCARD; i+=4; break; 
			case 'U': sch[i] = 1;        sch[i+1] = WILDCARD; sch[i+2] = 1;        sch[i+3] = 0;        i+=4; break; 
			case 'V': sch[i] = 1;        sch[i+1] = WILDCARD; sch[i+2] = 1;        sch[i+3] = 1;        i+=4; break; 
			case 'W': sch[i] = 1;        sch[i+1] = WILDCARD; sch[i+2] = 1;        sch[i+3] = WILDCARD; i+=4; break; 
			case 'X': sch[i] = 1;        sch[i+1] = WILDCARD; sch[i+2] = WILDCARD; sch[i+3] = 0;        i+=4; break; 
			case 'Y': sch[i] = 1;        sch[i+1] = WILDCARD; sch[i+2] = WILDCARD; sch[i+3] = 1;        i+=4; break; 
			case 'Z': sch[i] = 1;        sch[i+1] = WILDCARD; sch[i+2] = WILDCARD; sch[i+3] = WILDCARD; i+=4; break;
				                                                                                       
			case '`': sch[i] = WILDCARD; sch[i+1] = 0;        sch[i+2] = 0;        sch[i+3] = 0;        i+=4; break;
			case 'a': sch[i] = WILDCARD; sch[i+1] = 0;        sch[i+2] = 0;        sch[i+3] = 1;        i+=4; break; 
			case 'b': sch[i] = WILDCARD; sch[i+1] = 0;        sch[i+2] = 0;        sch[i+3] = WILDCARD; i+=4; break; 
			case 'c': sch[i] = WILDCARD; sch[i+1] = 0;        sch[i+2] = 1;        sch[i+3] = 0;        i+=4; break; 
			case 'd': sch[i] = WILDCARD; sch[i+1] = 0;        sch[i+2] = 1;        sch[i+3] = 1;        i+=4; break; 
			case 'e': sch[i] = WILDCARD; sch[i+1] = 0;        sch[i+2] = 1;        sch[i+3] = WILDCARD; i+=4; break; 
			case 'f': sch[i] = WILDCARD; sch[i+1] = 0;        sch[i+2] = WILDCARD; sch[i+3] = 0;        i+=4; break; 
			case 'g': sch[i] = WILDCARD; sch[i+1] = 0;        sch[i+2] = WILDCARD; sch[i+3] = 1;        i+=4; break; 
			case 'h': sch[i] = WILDCARD; sch[i+1] = 0;        sch[i+2] = WILDCARD; sch[i+3] = WILDCARD; i+=4; break; 
			case 'i': sch[i] = WILDCARD; sch[i+1] = 1;        sch[i+2] = 0;        sch[i+3] = 0;        i+=4; break; 
			case 'j': sch[i] = WILDCARD; sch[i+1] = 1;        sch[i+2] = 0;        sch[i+3] = 1;        i+=4; break; 
			case 'k': sch[i] = WILDCARD; sch[i+1] = 1;        sch[i+2] = 0;        sch[i+3] = WILDCARD; i+=4; break; 
			case 'l': sch[i] = WILDCARD; sch[i+1] = 1;        sch[i+2] = 1;        sch[i+3] = 0;        i+=4; break; 
			case 'm': sch[i] = WILDCARD; sch[i+1] = 1;        sch[i+2] = 1;        sch[i+3] = 1;        i+=4; break; 
			case 'n': sch[i] = WILDCARD; sch[i+1] = 1;        sch[i+2] = 1;        sch[i+3] = WILDCARD; i+=4; break; 
			case 'o': sch[i] = WILDCARD; sch[i+1] = 1;        sch[i+2] = WILDCARD; sch[i+3] = 0;        i+=4; break; 
			case 'p': sch[i] = WILDCARD; sch[i+1] = 1;        sch[i+2] = WILDCARD; sch[i+3] = 1;        i+=4; break; 
			case 'q': sch[i] = WILDCARD; sch[i+1] = 1;        sch[i+2] = WILDCARD; sch[i+3] = WILDCARD; i+=4; break; 
			case 'r': sch[i] = WILDCARD; sch[i+1] = WILDCARD; sch[i+2] = 0;        sch[i+3] = 0;        i+=4; break; 
			case 's': sch[i] = WILDCARD; sch[i+1] = WILDCARD; sch[i+2] = 0;        sch[i+3] = 1;        i+=4; break; 
			case 't': sch[i] = WILDCARD; sch[i+1] = WILDCARD; sch[i+2] = 0;        sch[i+3] = WILDCARD; i+=4; break; 
			case 'u': sch[i] = WILDCARD; sch[i+1] = WILDCARD; sch[i+2] = 1;        sch[i+3] = 0;        i+=4; break; 
			case 'v': sch[i] = WILDCARD; sch[i+1] = WILDCARD; sch[i+2] = 1;        sch[i+3] = 1;        i+=4; break; 
			case 'w': sch[i] = WILDCARD; sch[i+1] = WILDCARD; sch[i+2] = 1;        sch[i+3] = WILDCARD; i+=4; break; 
			case 'x': sch[i] = WILDCARD; sch[i+1] = WILDCARD; sch[i+2] = WILDCARD; sch[i+3] = 0;        i+=4; break; 
			case 'y': sch[i] = WILDCARD; sch[i+1] = WILDCARD; sch[i+2] = WILDCARD; sch[i+3] = 1;        i+=4; break; 
			default : sch[i] = WILDCARD; sch[i+1] = WILDCARD; sch[i+2] = WILDCARD; sch[i+3] = WILDCARD; i+=4; break;
		}
		n++;
    } while (i < schemasize+functionsize+schemasize);
    
    // compare sch with sys - we have a match if the right bit is present and the same
    diff = 0;
    for (n = 0; n < schemasize+functionsize+schemasize; n++)
    {   
		// no problem when sch[n] is a wildcard, but if it's a binary value then compare:
		if (sch[n] < 2) // it's a binary value
		{	if (sys[n] > 32) 
			{
				diff++; // compared to a code - printable ascii character
			}
			else if (sys[n] != sch[n])
			{
				diff++; // compared to another binary value
			}
		}
	 }
    return diff;
}

int parse(unsigned char *c, int* t1, int *t2, int *not)
// extracts the separate info in c[]: function id, thresholds 1 and 2, and the not operator
{   int f;

    schematoi(c, iniFUNCTIONID, &f);
    if (c[functionsize-1]) { *not = TRUE;}
    else *not = FALSE;
    //schematoi(c+iniFUNCTIONID,iniMATCHINGTHRES,t1);
    //schematoi(c+iniFUNCTIONID+iniMATCHINGTHRES,iniMATCHINGTHRES,t2);
	*t1 = *t2 = 0; // thresholds not supported in this version
    return f;
}

BOOL isData(unsigned char *c)
// returns TRUE if c is 0 - system is data
{   int f;

    schematoi(c, iniFUNCTIONID, &f);
    return (f == 0);
}

void initscopestate(systemic *s, int **scopetable)//双星号表示二维数组
// initialize the various scope lists
{
	int n,m;

    // first preprocess the scopetable to make a summary or two
    for (n = 0; n < numsystems; n++)
    {  
		scopesummary[n] = 0;
		for (m = 0; m < numsystems; m++) 
			if (scopetable[n][m]) 
				scopelist[n][scopesummary[n]++] = m;
    }

      // make a list of valid scopes 
      vscp = 0;
      for (n = 0; n < numsystems; n++)
         if (scopesummary[n] >= 3) vscopelist[vscp++] = n;

	// prepare context lists
    for (n = 0; n < numsystems; n++)
    {  
		fnum[n] = 0; flistchanged[n] = FALSE;
		for (m = 0; m < scopesummary[n]; m++) 
			if (!isData(s[scopelist[n][m]].function)) 
				functionlist[n][fnum[n]++] = scopelist[n][m];
    }
    
}

void updatefunctionlist(systemic *s, int whichscope)
// update the functionlist and corresponding number of functions fnum for specific scope
{
	int m;

	fnum[whichscope] = 0; flistchanged[whichscope] = FALSE;
	for (m = 0; m < scopesummary[whichscope]; m++) 
		if (!isData(s[scopelist[whichscope][m]].function)) 
			functionlist[whichscope][fnum[whichscope]++] = scopelist[whichscope][m];

}


void updatescopestate(systemic *s, int **scopetable, int whichscope)
// update the various scope lists for whichscope
{
	int m;

	scopesummary[whichscope] = 0;
	for (m = 0; m < numsystems; m++) 
		if (scopetable[whichscope][m]) 
			scopelist[whichscope][scopesummary[whichscope]++] = m;

    // make a list of valid scopes
    vscp = 0;
    for (m = 0; m < numsystems; m++)
		if (scopesummary[m] >= 3) vscopelist[vscp++] = m;

	updatefunctionlist(s,whichscope);

}

void updatecontextstate(systemic *s, int **scopetable, int whichsystem)
// update the function lists in case of a change in the function part of a system
{
	int m,n;

	for (m = 0; m < vscp; m++) 
		for (n = 0; n < scopesummary[vscopelist[m]]; n++)
			if (scopelist[vscopelist[m]][n] == whichsystem)
			{
				updatefunctionlist(s,vscopelist[m]);
				break;
			}
}

void cl_findmatches(int d,unsigned char *schbuffer,unsigned char *schema,int schsize,int bufsize, unsigned int *out1)
// push the problem of finding matching systems to our OpenCL device d and solve in parallel
{
    int err;
    size_t global[2];      // global domain size for our calculation
    size_t local[2];       // local domain size for our calculation

    // Write our data set into the input arrays in device memory
    err = clEnqueueWriteBuffer(cl[d].queue, cl[d].input1, CL_FALSE, 0, sizeof(unsigned char) * bufsize, schbuffer, 0, NULL, NULL);
    err |= clEnqueueWriteBuffer(cl[d].queue, cl[d].input2, CL_FALSE, 0, sizeof(unsigned char) * schsize, schema, 0, NULL, NULL);
    if (err != CL_SUCCESS) { printf("Error: Failed to write to source array!\n"); exit(1); }
    // Execute the kernel over the entire range of our 1d input data set
    // using (ideally) the maximum number of work group items for this device
    local[0] = CLWKGRPSIZE;
    global[0] = bufsize/schsize;
    err = clEnqueueNDRangeKernel(cl[d].queue, cl[d].kernel[0], 1, NULL, global, local, 0, NULL, NULL);
    if (err) { printf("Fatal Error: Failed to execute kernel %i!\n",d); exit(-11); }
}

void *refillqueue()
// thread to refill the emptybuffer queue in parallel to the reading of the validtriplets queue; calls OpenCL methods to exploit multiple cores
// note *emptybuffer is swapped with *validtriplets as soon as the former is full and the latter is empty
// this function reads many data structures shared by other functions, some of which may change during the parallel execution of this function
// this is not a big problem as even if garbage is written into emptybuffer, each triplet is checked before execution later
// in theory the following variables should be passed to this function for well-behaved threading: (in practise it makes little difference so we don't bother)
//     int schemasize, functionsize  -> constant values
//     struct OCL *cl         -> constant value
//     systemic *s            -> may change during execution
//     int vscp               -> may change during execution
//     int *vscopelist        -> may change during execution
//     int *fnum              -> may change during execution
//     int **functionlist     -> may change during execution
//     int *scopesummary      -> may change during execution
//     int **scopelist        -> may change during execution
{
    int giveup,flr;
    int scp,randomsys,i,j,k;
    int schsize = schemasize+functionsize+schemasize;
    int fn,context,t1,t2,not;
    int qptr, antifreeze;
    
    qptr = 0;
    giveup = 1; antifreeze = 2*MAXQUEUE;
    randomizecycle(2);   // re-intialise randomcycle for decent randomness
    while (qptr < MAXQUEUE-3 && --antifreeze > 0) // keep refilling until it's full enough or until we can't do any more
    {
        // We always do a random computation
        // randomly cycle through list of valid scopes:
        if (vscp > 0) { scp = vscopelist[randomcycle(2,vscp)]; giveup = 1; }
        else { scp = vscp = 0; giveup = -1; } // failed - no valid scopes
    
        // now pick random context with a non-zero function (can't randomly cycle as scope is changing):
        if (giveup > 0)
        {   if (fnum[scp] > 0) { context = functionlist[scp][flr=random(fnum[scp])]; giveup = 1; }
            else { context = 0; giveup = -1; } // failed - no valid contexts in this scope
        }
        
        if (giveup > 0)
        {
            // parse the function to be used to transform the two systems
            fn = parse(s[context].function, &t1, &t2, &not);
        
            // get the schemata of the context
            unsigned char schema1[schsize];
            unsigned char schema2[schsize];
            decode_to_quaternary(s[context].schema1, schema1);
            decode_to_quaternary(s[context].schema2, schema2);
            
            // make randomised buffer of systems in the current scope ready for searching
            // Note that schsize should be a multiple of 16, 32, or 64 bytes for optimal performance
            int numtrials = scopesummary[scp];
            if ((numtrials % CLWKGRPSIZE) != 0) { numtrials /= CLWKGRPSIZE; numtrials++; numtrials *= CLWKGRPSIZE; }
            int bufsize = numtrials*schsize;
            unsigned char *schbuffer;
            int *bufindex;
        
            // use heap to enable scaling to larger buffer sizes
            schbuffer = malloc(bufsize*sizeof(unsigned char)); _memcheck(schbuffer, "schbuffer");
            bufindex = malloc(numtrials*sizeof(int)); _memcheck(bufindex, "bufindex");

            randomizecycle(0);   // re-intialise randomcycle for decent randomness
            for (i = 0; i < bufsize; )
            {   randomsys = scopelist[scp][randomcycle(0,scopesummary[scp])];
                if (randomsys != context) // we miss out the context in the buffer to prevent it matching itself
                {   bufindex[i/schsize] = randomsys;
                    // fill schbuffer with s (memcopy unreliable so do it manually)
                    for (k = 0; k < schemasize; k++) schbuffer[i+k] = s[randomsys].schema1[k];
                    i += schemasize;
                    for (k = 0; k < functionsize; k++) schbuffer[i+k] = s[randomsys].function[k];
                    i += functionsize;
                    for (k = 0; k < schemasize; k++) schbuffer[i+k] = s[randomsys].schema2[k];
                    i += schemasize;
                }
            }
            
            // find matching systems for the two schema using OpenCL
            unsigned int out1[numtrials];
            unsigned int out2[numtrials];
        
            cl_findmatches(0,schbuffer,schema1,schsize,bufsize,out1);
            cl_findmatches(1,schbuffer,schema2,schsize,bufsize,out2);
            
            // Read back the results from the OpenCL devices to verify the output
            int err = clEnqueueReadBuffer(cl[0].queue, cl[0].output, CL_TRUE, 0, sizeof(unsigned int) * numtrials, out1, 0, NULL, NULL );
            err |= clEnqueueReadBuffer(cl[1].queue, cl[1].output, CL_TRUE, 0, sizeof(unsigned int) * numtrials, out2, 0, NULL, NULL );
            if (err != CL_SUCCESS) { printf("Fatal Error %d: Failed to read output array\n", err); exit(1); }
        
            // convert out1 and out2 into shortened lists of decoded indexes to matching schema
            int sm1 = 0, sm2 = 0;
            for (i = 0; i < numtrials; i++)
            {   if (_smatchedok(out1[i],t1,not)) out1[sm1++] = bufindex[i];
                if (_smatchedok(out2[i],t2,not)) out2[sm2++] = bufindex[i];
            }
            
            // now fill emptybuffer queue with all mututally compatible pairs, if they exist
            for (i = 0; i < sm1 && qptr < MAXQUEUE-3; i++)
            {   for (j = 0; j < sm2 && qptr < MAXQUEUE-3; j++)
                {   if (out1[i] != out2[j])
                    {   emptybuffer[qptr+QSCOPE] = scp;
                        emptybuffer[qptr+QCONTEXT] = context;
                        emptybuffer[qptr+QSYS1] = out1[i];
                        emptybuffer[qptr+QSYS2] = out2[j];
                        qptr += 4;
                    }
                }
            }
            free(schbuffer);
            free(bufindex);
        }
    }
    //printf("f:%ui ",iterations);
    pthread_exit(NULL);
}

BOOL gotthreesystems(systemic *s, int **scopetable, int *context, int *snum1, int *snum2, int *whichscope, int* t1, int *t2, int *not, int *fn, int *dnum1, int *dnum2)
// finds three random systems in a random scope; returns FALSE if unsuccessful (might not be enough systems in a scope)
// sequential version using valid triplet queue
{   int giveup,rc, *tmp;
    BOOL snum1ok, snum2ok;
    
    *fn = -1;
    snum1ok = snum2ok = FALSE;
    
    if (queue_front >= MAXQUEUE) _startrefillqueuethread(); // Finished current queue, time to swap to next one

    giveup = 2*MAXQUEUE;
    // get the next valid system from the queue, making sure it is still valid
    do
    {   *whichscope = validtriplets[queue_front+QSCOPE];
        *context = validtriplets[queue_front+QCONTEXT];
        *snum1 = validtriplets[queue_front+QSYS1];
        *snum2 = validtriplets[queue_front+QSYS2];
        queue_front += 4; if (queue_front >= MAXQUEUE) _startrefillqueuethread();
        
        // parse the function to be used to transform the two systems
        *fn = parse(s[*context].function, t1, t2, not);
        // check that this is still a valid triplet all in the original scope
        *dnum1 = difference(s[*context].schema1,s+*snum1);
        *dnum2 = difference(s[*context].schema2,s+*snum2);
        if (scopetable[*whichscope][*context] == 1 && scopetable[*whichscope][*snum1] == 1 && scopetable[*whichscope][*snum2] == 1 &&
        _smatchedok(*dnum1, *t1, *not) && _smatchedok(*dnum2, *t2, *not))  { snum1ok = snum2ok = TRUE; break; }
    } while (!(snum1ok && snum2ok) && --giveup > 0);

    if (!(snum1ok && snum2ok))
    {
        printf("\nUnable to find any matching systems.");
    }
    return (snum1ok && snum2ok);
}

BOOL gotthreesystems0(systemic *s, int **scopetable, int *context, int *snum1, int *snum2, int *whichscope, int* t1, int *t2, int *not, int *fn, int *dnum1, int *dnum2)
// finds three random systems in a random scope; returns FALSE if unsuccessful (might not be enough systems in a scope)
// original optimised sequential version
{   int giveup,flr,tmp;
    int scp,randomsys;
    BOOL snum1ok, snum2ok;
    
    *fn = -1;
    snum1ok = snum2ok = FALSE;
    
    // We always do a random computation
    // choose randomly from list of valid scopes
    
    if (vscp > 0) { scp = vscopelist[random(vscp)]; giveup = 1; }
    else { scp = vscp = 0; giveup = -1; } // failed
    *whichscope = scp;
    
    
    // now pick random context with a non-zero function:
    if (giveup > 0)
    {
        //printf(fnum[scp]);
        if (fnum[scp] > 0) { *context = functionlist[scp][flr=random(fnum[scp])]; giveup = 1; }
        else { *context = 0; giveup = -1; } // failed
    }
    
    if (giveup > 0)
    {
        // parse the function and apply it to transform the two systems
        *fn = parse(s[*context].function, t1, t2, not);
        
        randomizecycle(0);   // re-intialise randomcycle for decent randomness
        // random collisions in this context:
        giveup = scopesummary[scp]+1;   //2*scopesummary[scp]-1; need fewer trials when using randomcycle()
        *snum1 = *context;
        do
        {
            randomsys = scopelist[scp][randomcycle(0,scopesummary[scp])];
            *dnum1 = difference(s[*context].schema1,s+randomsys);
            if (randomsys != *context && _smatchedok(*dnum1, *t1, *not))
            {
                *snum1 = randomsys;
                snum1ok = TRUE;
                break;
            }
            giveup--;
        } while (giveup > 0);
    }
    
    if (giveup > 0)
    {  randomizecycle(0);   // re-intialise randomcycle for decent randomness
        giveup = scopesummary[scp]+1;   //2*scopesummary[scp]-1; need fewer trials when using randomcycle()
        *snum2 = *context;
        do
        {
            randomsys = scopelist[scp][randomcycle(0,scopesummary[scp])];
            *dnum2 = difference(s[*context].schema2,s+randomsys);
            if (randomsys != *snum1 && randomsys != *context && _smatchedok(*dnum2, *t2, *not))
            {
                *snum2 = randomsys;
                snum2ok = TRUE;
                break;
            }
            giveup--;
        } while (giveup > 0);
    }
    // optimisation - temporarily remove context that matches nothing from the list
    if (giveup == 0)
    {   swap(functionlist[scp][flr],functionlist[scp][fnum[scp]],tmp);
        fnum[scp]--; flistchanged[scp] = TRUE;
    }
    // undo optimsation - restore all functions in this scope to the lists
    if (giveup >= 0 && flistchanged[scp] && snum1ok && snum2ok) updatefunctionlist(s,scp);
    
    return (snum1ok && snum2ok);
}

BOOL onstack(int schptr, int *snum)
// returns TRUE and sets snum to the stack location if schptr+2 is listed in stack, FALSE otherwise
// (note we use schptr+2 as the pointers range from -2 for $L, -1 for $R, and upwards for new variables)
{
    int ret = FALSE;
    
    if ((ret = (schptr+2 < stackptr))) *snum = schptr+2;
    else *snum = -1;

    return ret;
}

int followchain(int ret, chainlink **chain, int chnum, int chainnum, systemic *s,int **scopetable,int *context,int *snum1,int *snum2,int whichscope, unsigned int *iterations)
// the head of the chain has just been executed and ret contains the result of the transformation function
// snum1 and snum2 contain the systems which interacted with that first system; now we follow the chain...
{
    int link = 1;
    int newret;
    int s1,s2,t1,t2,d1,d2,c,f,nt,randomsys,giveup;
    BOOL s1OK,s2OK,chaindone = FALSE;
    
    // initialize stack with pointers to the two systems of the head of the chain
    stackptr = 0;
    stack[stackptr++] = *snum1;
    stack[stackptr++] = *snum2;
    newret = ret;
    c = *context;
    
    // loop while we are not breaking the chain
    while (newret > CHAIN_BREAK && !chaindone)
    {
        // parse the function and apply it to transform the two systems
        f = parse(s[c].function, &t1, &t2, &nt);
        
        // is it a valid chained system?
        if (chnum < chainnum && c >= 0)
        {
            // the chained context needs systems; if they are already on the stack (and they match the context) we can use them
            s1OK = s2OK = FALSE; s1 = s2 = c;
            if (onstack(chain[chnum][link].sch1,&s1))
            {   // look in the stack and use the index stored there as the schema
                s1 = stack[s1];
                // is it still in the right scope?
                if (scopetable[whichscope][s1] == 0) // note this only checks the current scope; it does not look in other scopes
                {   PRINTF("\nChain broken: schema1 (from stack) out of scope.");
                    chaindone = TRUE; newret = CHAIN_BREAK;
                    break;
                }
                // does the system from the stack match the context?
                d1 = difference(s[c].schema1,s+s1);
                if (_smatchedok(d1, t1, nt))
                    s1OK = TRUE;
                else
                {   PRINTF("\nChain broken: schema1 (from stack) does not match chained context.");
                    chaindone = TRUE; newret = CHAIN_BREAK; break;
                }
            }
            // try the same for the second system
            if (onstack(chain[chnum][link].sch2,&s2))
            {   // look in the stack and use the index stored there as the schema
                s2 = stack[s2];
                // is it still in the right scope?
                if (scopetable[whichscope][s2] == 0) // note this only checks the current scope; it does not look in other scopes
                {   PRINTF("\nChain broken: schema2 out of scope.");
                    chaindone = TRUE; newret = CHAIN_BREAK;
                    break;
                }
                // does the system from the stack match the context?
                d2 = difference(s[c].schema2,s+s2);
                if (_smatchedok(d2, t2, nt))
                    s2OK = TRUE;
                else
                {   PRINTF("\nChain broken: schema2 (from stack) does not match chained context.");
                    printf("\n context %d %d %d",c,s1,s2);
                    chaindone = TRUE; newret = CHAIN_BREAK; break;
                }
            }
            // now if we couldn't find either system on the stack we need to find a new random system
            if (!s1OK)
            {   // if they are not yet on the stack we need to find a new random system and add to the stack, only try the current scope
                giveup = scopesummary[whichscope]+1;
                randomizecycle(1);   // re-intialise randomcycle for decent randomness
                do
                {   randomsys = scopelist[whichscope][randomcycle(1,scopesummary[whichscope])];
                    d1 = difference(s[c].schema1,s+randomsys);
                    if (_smatchedok(d1, t1, nt) && randomsys != s2 && randomsys != c)
                    {   s1 = randomsys;
                        s1OK = TRUE;
                        break;
                    }
                    giveup--; 
                } while (giveup > 0);
                
                if (s1OK) // found one ok
                {   stack[stackptr] = s1;
                    if (++stackptr > iniMAXSYSTEMS)
                    {   chaindone = TRUE; newret = CHAIN_BREAK;
                        printf("\nStack overflow: chain with s1 in current scope broken.\n");
                    }
                }
               // if still not successful, report failure.
                else
                {   PRINTF("\nChain broken: cannot find matching random schema1.\n");
                    chaindone = TRUE; newret = CHAIN_BREAK;
                    break;
                }
            
            }
            // now find the second random system if necessary
            if (!s2OK)
            {   // if it is not yet on the stack we need to find a new random system and add to the stack, only try the current scope
                giveup = scopesummary[whichscope]+1;
                randomizecycle(1);   // re-intialise randomcycle for decent randomness
                do
                {   randomsys = scopelist[whichscope][randomcycle(1,scopesummary[whichscope])];
                    d2 = difference(s[c].schema2,s+randomsys);
                    if (_smatchedok(d2, t2, nt) && randomsys != s1 && randomsys != c)
                    {   s2 = randomsys;
                        s2OK = TRUE;
                        break;
                    }
                    giveup--;
                } while (giveup > 0);
                
                if (s2OK) // found one ok
                {   stack[stackptr] = s2;
                    if (++stackptr > iniMAXSYSTEMS)
                    {   chaindone = TRUE; newret = CHAIN_BREAK;
                        printf("\nStack overflow: chain with s2 in current scope broken.");
                    }
                }
                else
                {   PRINTF("\nChain broken: cannot find matching random schema2.\n");
                    chaindone = TRUE; newret = CHAIN_BREAK;
                    break;
                }
            }
            // now we transform the two new systems with the chained function
            newret = transform(f,d1,d2,t1,t2,nt,s,s1,s2,whichscope,c,scopetable);
            *context = c; *snum1 = s1; *snum2 = s2; (*iterations)++;
            if (*iterations >= iniMAXLOOPS || interaction_count >= iniMAXINTRCTNS) { chaindone = TRUE; newret = CHAIN_BREAK; }// time to end computation
            // get the next link in the chain (if it exists)
            link++;
            c = chain[chnum][link].systemnum;
            if (c < 0)
            {   chaindone = TRUE; // no more links in this chain
                if (s[*context].ischained == NOTCHAINED) newret = CHAIN_BREAK; // last link is not the head of a new chain
            }
        }
        else // ran out of chain
            chaindone = TRUE;
    }
    return newret;
}

void compute(systemic *s, int **scopetable, chainlink **chain, int chainnum)
// computes s; assumes initialised s and definition of: systemic s[iniMAXSYSTEMS] and scopetable[][]
{   int whichscope;
    int context,snum1,snum2;
    int dnum1,dnum2, fn, not, t1,t2, ret,chnum;
    int *tmp;

    iterations = 0;
    ret = 0;
    
    _finishrefillqueuethread(); // fill queue, wait for thread to complete, swap valid triplet buffers
    
    do
    {   if (gotthreesystems(s,scopetable,&context,&snum1,&snum2,&whichscope, &t1, &t2, &not, &fn, &dnum1, &dnum2))
        {
            ret = transform(fn,dnum1,dnum2,t1,t2,not,s,snum1,snum2,whichscope,context,scopetable);
            // if the system is chained, start following the chosen chain
            if (s[context].ischained != NOTCHAINED)
                do
                {   // find the chain for s[context] in chain[][] - we stay on this chain until done
                    chnum = s[context].ischained+ret;
                    // if there is a chain:
                    if (chnum != NOTCHAINED)
                    {   // check that the first link matches the system we just executed
                        if (chain[chnum][0].systemnum != context) { printf("\nError: Chain %d for system %d not found. Chain broken.",ret,context); break; }
                        // otherwise the second link in that chain is the next context to execute
                        context = chain[chnum][1].systemnum;
                        // follow the chain, passing the two systems from the first link of the chain to be stored on the stack
                        ret = followchain(ret,chain,chnum,chainnum,s,scopetable,&context,&snum1,&snum2,whichscope,&iterations);
                    }
                    else
                    {   ret = CHAIN_BREAK;
                    }
                } while (ret != CHAIN_BREAK);
        }
        else
        {
            //PRINTF("failed to find the system matching context.\n");
            break;
        }
        iterations++;
    } while (iterations < iniMAXLOOPS && interaction_count < iniMAXINTRCTNS && simfinished == FALSE);
    pthread_join(qthread, NULL); // wait for other process to end
	printf("\niterations:%d, actual interactions:%d, successful interaction ratio:%.3f%%",iterations,interaction_count,100.0*(float)interaction_count/(float)iterations);
}

void forcezero(unsigned char *s1, unsigned char *s2)
// applies zeroing function to s1 and s2, placing results in both; zeroes everything
{    int n;

     for (n = 0; n < schemasize; n++) s1[n] = s2[n] = ZERO;
}

void loadchains(FILE *fstrm1,chainlink **chain,int *chainnum, systemic *s, int sn)
{   int n,c,l;
    BOOL done = FALSE;
	char *line;
    char chainchunk[32];
    BOOL chainheadfound;
    
    line = malloc(MAXLLEN * sizeof(char)); _memcheck(line, "line");
    n = 0;
    *chainnum = 0;
    while (!done)
    {   l = 0;
		if (fgets(line,MAXLLEN,fstrm1) == NULL) break;
        if (line[0] != '#' && line[0] != '/' && line[0] != ';' && line[0] != '\n') // three different ways to mark a comment
        {   chainheadfound = FALSE;
            for (n = 0; line[n] != '\n' && line[n] != '\0' && n < strlen(line); n++)
            {
                // get each chainlink in turn:
                c = 0;
                while (line[n] != '\n' && line[n] != '\0' && line[n] != '+') { chainchunk[c++] = line[n++]; }
                chainchunk[c] = '\0';
                sscanf(chainchunk,"%d}%d{%d",&(chain[*chainnum][l].sch1),&(chain[*chainnum][l].systemnum),&(chain[*chainnum][l].sch2));
                if (!chainheadfound)
                {   if (chain[*chainnum][l].systemnum < sn)
                    {   // we point ischained to the first location in chain[][] that points to this system
                        // this way we know this system is the head of a chain and we know where to start looking for its chain(s)
                        // we also make sure we only set the value for the first in the list of chains
                        if (s[chain[*chainnum][l].systemnum].ischained == NOTCHAINED) s[chain[*chainnum][l].systemnum].ischained = *chainnum;
                        // found the head
                        chainheadfound = TRUE;
                    }
                    else // chain refers to a system that we don't have!
                    {    // this should not happen unless the programmer is very confused
                        printf("\nERROR in chain: system number out of range. Check number of systems is > %d.",chain[*chainnum][l].systemnum);
                        safe_exit(-3);
                    }
                }
                l++; // next link
            }
            (*chainnum)++; // next chain
            if (*chainnum >= sn)
            {
                printf("\nFATAL ERROR: Chain overflow.");
                safe_exit(4);
            }
        }
    }
    free(line);
}

void loadsystems(FILE *fstrm, systemic *s, int l)
{   char sch1[schemasize+1], fn[functionsize+1],sch2[schemasize+1]; 
    int n;
	char *line;
    
    line = malloc(MAXLLEN * sizeof(char)); _memcheck(line, "line");
    n = 0; 
    while (n < l)
    {    
		if (fgets(line,MAXLLEN,fstrm) == NULL) break;
			if (line[0] != '#' && line[0] != '/' && line[0] != ';' && line[0] != '\n') // three different ways to mark a comment
			{  sscanf(line,"%s %s %s/n",sch1,fn,sch2);
				atoschema(sch1,schemasize,s[n].schema1);
				atoschema(fn,functionsize,s[n].function);
				atoschema(sch2,schemasize,s[n].schema2);
                s[n].ischained = NOTCHAINED;
				n++;
			}
    }
    for (; n < l; n++) // if the program is not big enough to fill the available space, pad with zeroed systems
    {  forcezero(s[n].schema1,s[n].schema2);
       forcezero(s[n].function,s[n].schema2);
       s[n].ischained = NOTCHAINED;
    }
    free(line);
}

void getword(char *word,char *in, int *l)
// places the next word (separated by whitespace) from in[*l] into word[]; updating *l to point to the end of the word
{   int i;

    i = 0;
    for (; in[*l] != ' ' && in[*l] != '\n' && in[*l] != '\t' && in[*l] != '\0' && (size_t) *l < strlen(in); (*l)++)
    {  word[i++] = in[*l];
    }
    word[i] = '\0';
} 

void loadscopes(FILE *fstrm, systemic *s,int **scopetable)
// reads the scope table
{   int ret,n,m;
	char *line;

    line = malloc(MAXLLEN * sizeof(char)); _memcheck(line, "line");
    
    // three different ways to mark a comment
    do { if (fgets(line,MAXLLEN,fstrm) == NULL) break; }
    while (line[0] == '#' || line[0] == '/' || line[0] == ';' || line[0] == '\n');
    // now read the scopes
    do
    {   if (fgets(line,MAXLLEN,fstrm) == NULL || line[0] == ')') break;
        if ((ret = sscanf(line, "%d(%d", &n, &m)) == 2) scopetable[n][m] = 1;
        
    } while (line[0] != ')' && ret == 2);
    
    free(line);
}

void loadfunctions(FILE *fstrm, symbol *f, int fn)
// reads functions - doesn't error check (assumes .scp file is correctly written using the compiler)
{
    char *line;
    char sch[functionsize+1]; // in order to fit the function string plus the new line char
    int n, maxid;

    line = malloc(MAXLLEN * sizeof(char)); _memcheck(line, "line");
    n = maxid = 0;
    while (n < fn)
    {    
		if (fgets(line,MAXLLEN,fstrm) == NULL) break;
		if (line[0] != '#' && line[0] != '/' && line[0] != ';' && line[0] != '\n') // three different ways to mark a comment
        {  
			sscanf(line,"%s %s/n",f[n].name,sch);
			atoschema(sch,functionsize,f[n].schema);
			schematoi(f[n].schema, iniFUNCTIONID, &maxid);
			if (maxid > maxfunctionid) maxfunctionid = maxid;
			n++;
        }
    }
    free(line);
}

void loadsystemicprogramdimensions(char *fname, int *l, int *fn, long *filepos)
// loads number of systems and number of functions and stores the resulting file position
{   
    FILE *fstrm1;
	char *line;

    line = malloc(MAXLLEN * sizeof(char)); _memcheck(line, "line");
    PRINTF("\nOpening: %s",fname);
    if ((fstrm1 = fopen(fname,"r")) == NULL)
    { PRINTF("\nFatal error: unable to open input file.");
      safe_exit(2);
    }
    // get number of functions
    *fn = 0;
    while (*fn == 0 && fgets(line,MAXLLEN,fstrm1) != NULL)
    {    if (line[0] != '#' && line[0] != '/' && line[0] != ';' && line[0] != '\n') // three different ways to mark a comment
           sscanf(line,"%d/n",fn);
    }
    if (*fn < 1 || *fn > iniMAXFUNCTIONS)
    { PRINTF("\nFATAL ERROR: invalid number of functions specified.");
      fclose(fstrm1);
      safe_exit(4);
    }
    
    PRINTF("\nNumber of functions: %d",*fn);
    
    // get number of systems
    *l = 0;
    while (*l == 0 && fgets(line,MAXLLEN,fstrm1) != NULL)
    {    if (line[0] != '#' && line[0] != '/' && line[0] != ';' && line[0] != '\n') // three different ways to mark a comment
           sscanf(line,"%d/n",l);
    }
    if (*l < 3 || *l > iniMAXSYSTEMS)
    { PRINTF("\nFATAL ERROR: invalid number of systems specified.");
      fclose(fstrm1);
      safe_exit(4);
    }
    
    PRINTF("\nNumber of systems: %d",*l);
    *filepos = ftell(fstrm1);
    free(line);
    fclose(fstrm1);
}

void loadsystemicprogram(systemic *s, char *fname, long filepos, int *l, symbol *f, int *fn, int **scopetable, chainlink **chain, int *chainnum)
// loads functions into f[] and program into s; fills only the space available so a partial program may be loaded
{   
    FILE *fstrm1;

    if ((fstrm1 = fopen(fname,"r")) == NULL)
    { PRINTF("\nFatal error: unable to open input file.");
      safe_exit(2);
    }
    
    // skip number of functions and systems
    fseek(fstrm1, filepos, SEEK_SET);

    loadscopes(fstrm1,s,scopetable);
    loadfunctions(fstrm1,f,*fn);
    loadsystems(fstrm1,s,*l);
    loadchains(fstrm1,chain,chainnum,s,*l);
    PRINTF("\nNumber of chains: %d\n",*chainnum);
    fclose(fstrm1);
}

BOOL isfunction(char *name, int *n, symbol *function, int fn)
// returns TRUE if name is listed in function[], setting n to the id 
{  BOOL ret;
   int i;
    
   ret = FALSE;
   for (i = 0; i < fn; i++) 
	   if (strcmp(name,function[i].name) == 0) 
	   { ret = TRUE; *n = i; break; } 
   
   return ret;
}

void initsystems()
// memory allocation, initialization, and compiling of code
{
	int i, n, cnum, snum;
    long filepos;    
    
    // For the compiler:
    s = (systemic *) malloc(sizeof(systemic) * iniMAXSYSTEMS); _memcheck(s,"systems");
    for (i = 0; i < iniMAXSYSTEMS; i++)
    {   s[i].schema1 = (unsigned char *)malloc(sizeof(unsigned char) * schemasize); _memcheck(s[i].schema1,"system schema 1");
        s[i].function = (unsigned char *)malloc(sizeof(unsigned char) * functionsize); _memcheck(s[i].function,"system function");
        s[i].schema2 = (unsigned char *)malloc(sizeof(unsigned char) * schemasize); _memcheck(s[i].schema2,"system schema 2");
        s[i].ischained = NOTCHAINED;
    }
    
    scopetable =(int **) malloc(iniMAXSYSTEMS * sizeof(int *)); _memcheck(scopetable,"scope table");
    for (i = 0; i < iniMAXSYSTEMS; i++) { scopetable[i] = (int *) malloc(iniMAXSYSTEMS * sizeof(int)); _memcheck(scopetable[i],"scopetable row"); }

    
    chain =(chainlink **) malloc(iniMAXSYSTEMS * sizeof(chainlink *)); _memcheck(chain,"chains");
    for (i = 0; i < iniMAXSYSTEMS; i++)
    {  chain[i] = (chainlink *) malloc(iniMAXSYSTEMS * sizeof(chainlink)); _memcheck(chain[i],"chain links");
        for (n = 0; n < iniMAXSYSTEMS; n++) chain[i][n].systemnum = -1;
    }
    stack =(int *) malloc(iniMAXSYSTEMS * sizeof(int)); _memcheck(stack,"chain stack");
    
    functions = (symbol *) malloc(sizeof(symbol) * iniMAXFUNCTIONS); _memcheck(functions,"functions");
    for (i = 0; i < iniMAXFUNCTIONS; i++)
    {    functions[i].schema = (unsigned char *)malloc(sizeof(unsigned char) * functionsize); _memcheck(functions[i].schema,"function schema"); }
    
    labels = (symbol *)malloc(iniMAXFUNCTIONS * sizeof(symbol)); _memcheck(labels,"labels");
    for (i = 0; i < iniMAXFUNCTIONS; i++)
    {   labels[i].schema = (unsigned char *)malloc(sizeof(unsigned char) * functionsize); _memcheck(labels[i].schema,"label schema"); }
    
	sys = (syssymbol *)malloc(iniMAXSYSTEMS * sizeof(syssymbol)); _memcheck(sys,"sys");
    
    if (iniCOMPILESC)
    {   // Do the compiling
        printf("\n\nProgram to be compiled: %s",scfilename);
        printf("\nCompiled code to be saved as: %s",scpfilename);
        
        printf("\nCompiling...\n");
        assembleprogram(scfilename,s,&numsystems,functions,&numfunctions,scopetable,&snum,chain,&cnum);

        printf("\nSaving Compiled Program...\n");
        saveprogram(scpfilename,scfilename,s,numsystems,functions,numfunctions,scopetable,chain,cnum);
    }
    // For the simulator:
    
	maxfunctionid = 0;
    loadsystemicprogramdimensions(scpfilename,&numsystems,&numfunctions,&filepos);

    loadsystemicprogram(s,scpfilename,filepos,&numsystems,functions,&numfunctions,scopetable,chain,&chainnum);
    
	scopesummary = (int *)malloc(numsystems * sizeof(int)); _memcheck(scopesummary,"scope summary");
    scopelist = (int **)malloc(numsystems * sizeof(int *)); _memcheck(scopelist,"scope list");
    for (i = 0; i < numsystems; i++) { scopelist[i] = (int *)malloc(numsystems * sizeof(int)); _memcheck(scopelist[i],"scope list row"); }
    vscopelist = (int *)malloc(numsystems * sizeof(int)); _memcheck(vscopelist,"vscopelist");
	functionlist = (int **)malloc(numsystems * sizeof(int *)); _memcheck(functionlist,"functionlist");
    for (i = 0; i < numsystems; i++) { functionlist[i] = (int *)malloc(numsystems * sizeof(int)); _memcheck(functionlist[i],"functionlist row"); }
	fnum = (int *)malloc(numsystems * sizeof(int)); _memcheck(fnum,"fnum");
    flistchanged = (BOOL *)malloc(numsystems * sizeof(BOOL)); _memcheck(flistchanged,"flistchanged");
	initscopestate(s,scopetable);
    
    scpoutfilenum = scoutfilenum = 0;
}

void freesystems()
// free all memory, with checks to ensure NULL pointers are not freed, enabling this function to be called at any time when exiting
{
    int i;
    
    if (s)
    {   for (i = 0; i < iniMAXSYSTEMS; i++)
        {   if (s[i].schema1) free(s[i].schema1);
            if (s[i].function) free(s[i].function);
            if (s[i].schema2) free(s[i].schema2);
        }
        free(s);
    }

    if (scopetable)
    {   for (i = 0; i < iniMAXSYSTEMS; i++) if (scopetable[i]) free(scopetable[i]);
        free(scopetable);
    }

    if (chain)
    {   for (i = 0; i < iniMAXSYSTEMS; i++) if (chain[i]) free(chain[i]);
        free(chain);
    }

	if (stack) free(stack);
    
    if (functions)
    {   for (i = 0; i < iniMAXFUNCTIONS; i++) if (functions[i].schema) free(functions[i].schema);
        free(functions);
    }
    
    if (labels)
    {   for (i = 0; i < iniMAXFUNCTIONS; i++) if (labels[i].schema) free(labels[i].schema);
        free(labels);
    }

    if (sys) free(sys);
    //if (numberfunctionsused) free(numberfunctionsused);
    if (scopesummary) free(scopesummary);
    
    if (scopelist)
    {   for (i = 0; i < numsystems; i++) if (scopelist[i]) free(scopelist[i]);
        free(scopelist);
    }
    
    if (vscopelist) free(vscopelist);
    
    if (functionlist)
    {   for (i = 0; i < numsystems; i++) if(functionlist[i]) free(functionlist[i]);
        free(functionlist);
    }
    
	if (fnum) free(fnum);
	if (flistchanged) free(flistchanged);
    
    numsystems = 0;
}

void getcurrentdir(char *name,char *currentdir,char *currentfile)
// fills currentdir and currentfile with details extracted from name
{
     int l,n,i;
    
     l = n = (int)strlen(name)-1;
     while (n > 0 && name[n] != '\\' && name[n] != '/') n--;
 
     for (i = 0; i <= l; i++)
     { if (i < n) currentdir[i] = name[i];
       if (i == n) currentdir[i] = '\0';
       if (i > n) currentfile[i-n-1] = name[i];
     }
     currentfile[i-n-1] = '\0';
}

void loadini(char name[], char userfname[4][MAXNAME])
// loads the default ini file and fills global constants
{   FILE *fstrm1;

    char *line, fname[MAXNAME], currentfile[MAXNAME];

    line = malloc(MAXLLEN * sizeof(char)); _memcheck(line, "line");
    getcurrentdir(name,fname,currentfile);
    strcat(fname,INIFILE);

    if ((fstrm1 = fopen(fname,"r")) == NULL)
    { PRINTF("\nFatal error: unable to open ini file.");
      exit(3);
    }

    getnextline;
    strcpy(scfilename,line); scfilename[strlen(scfilename)-1] = '\0';
    PRINTF("\nSC source: %s",scfilename);
    getnextline;
    strcpy(scpfilename,line); scpfilename[strlen(scpfilename)-1] = '\0';
    PRINTF("\nCompiled SCP: %s",scpfilename);
    
    getnextline;
    iniCOMPILESC = DEFAULT_COMPILE;
    sscanf(line,"%d\n",&iniCOMPILESC);
    if (iniCOMPILESC) PRINTF("\nCompiler activated.");
    else
    {   if (foundfile(scpfilename)) PRINTF("\nCompiler deactivated - using previously compiled program: %s",scpfilename);
        else
        {   // user wants to use previously compiled source but it's not there - turn on compiler
            PRINTF("\nCompiled program not found: %s\nCompiler activated.",scpfilename);
            iniCOMPILESC = 1;
        }
    }
    
    getnextline;
    iniSCALEFACTOR = DEFAULT_SCALEFACTOR;
    sscanf(line,"%d\n",&iniSCALEFACTOR);
    PRINTF("\nWord scaling factor: %d",iniSCALEFACTOR);
    schemasize = SCHEMASIZE*iniSCALEFACTOR;
    functionsize = FUNCTIONSIZE*iniSCALEFACTOR;
    PRINTF("\nschema size (bits): %d  Function size (bits): %d",schemasize,functionsize);

    iniFUNCTIONID = FUNCTIONIDSIZE;
    getnextline;
    sscanf(line,"%d\n",&iniFUNCTIONID);
    PRINTF("\nFunction ID size (bits): %d",iniFUNCTIONID);

    iniMATCHINGTHRES = MATCHINGTHRES;
    getnextline;
    sscanf(line,"%d\n",&iniMATCHINGTHRES);
    PRINTF("\nMatching threshold size (bits): %d",iniMATCHINGTHRES);

    iniMAXSYSTEMS = DEFAULT_MAXSYSTEMS;
    getnextline;
    sscanf(line,"%d\n",&iniMAXSYSTEMS);
    PRINTF("\nMaximum systems: %d",iniMAXSYSTEMS);
    if (MAXLLEN < 2*iniMAXSYSTEMS)
    {
        printf("\nFATAL ERROR: MAXLLEN (%d) < 2*MAXSYSTEMS (%d). Increase value of MAXLLEN to continue.",MAXLLEN,2*iniMAXSYSTEMS); safe_exit(10);
    }
    
	iniMAXLOOPS = DEFAULT_LOOP_NUM;
    getnextline;
    sscanf(line,"%d\n",&iniMAXLOOPS);
    PRINTF("\nMaximum loops: %d",iniMAXLOOPS);
    
	iniMAXINTRCTNS = DEFAULT_ITN_NUM;
    getnextline;
    sscanf(line,"%d\n",&iniMAXINTRCTNS);
    PRINTF("\nMaximum interactions: %d",iniMAXINTRCTNS);
    
    iniMAXFUNCTIONS = pow(2,iniFUNCTIONID);

    strcpy(userfname[0],DEFAULT_FNAME1);
    getnextline;
    sscanf(line,"%s\n",userfname[0]);
    PRINTF("\nUser defined filename1: %s",userfname[0]);
    strcpy(userfname[1],DEFAULT_FNAME2);
    getnextline;
    sscanf(line,"%s\n",userfname[1]);
    PRINTF("\nUser defined filename2: %s",userfname[1]);
    strcpy(userfname[2],DEFAULT_FNAME3);
    getnextline;
    sscanf(line,"%s\n",userfname[2]);
    PRINTF("\nUser defined filename3: %s",userfname[2]);
    strcpy(userfname[3],DEFAULT_FNAME4);
    getnextline;
    sscanf(line,"%s\n",userfname[3]);
    PRINTF("\nUser defined filename4: %s",userfname[3]);

    free(line);
    fclose(fstrm1);
}

BOOL clsetup(int bufsize, int schsize)
// Locate openCL devices available in hardware and setup contexts, build programs, make kernels
{
    BOOL bPassed = TRUE;
    char cBuffer[1024];
    cl_platform_id clSelectedPlatformID = NULL;
    cl_int ciErrNum;
    
    // Get OpenCL platform ID
    printf(" OpenCL SW Info:\n\n");
    
    // Get OpenCL platform name and version
    ciErrNum = clGetPlatformInfo (clSelectedPlatformID, CL_PLATFORM_NAME, sizeof(cBuffer), cBuffer, NULL);
    if (ciErrNum == CL_SUCCESS)
    {
        printf(" CL_PLATFORM_NAME: %s\n", cBuffer);
    }
    else
    {
        printf(" Error %i in clGetPlatformInfo Call\n\n", ciErrNum);
        bPassed = FALSE;
    }
    
    ciErrNum = clGetPlatformInfo (clSelectedPlatformID, CL_PLATFORM_VERSION, sizeof(cBuffer), cBuffer, NULL);
    if (ciErrNum == CL_SUCCESS)
    {
        printf(" CL_PLATFORM_VERSION: %s\n\n", cBuffer);
    }
    else
    {
        printf(" Error %i in clGetPlatformInfo Call\n\n", ciErrNum);
        bPassed = FALSE;
    }
    
    // Get and log OpenCL device info
    ciErrNum = clGetDeviceIDs (clSelectedPlatformID, CL_DEVICE_TYPE_ALL, 0, NULL, &clDeviceCount);
    
    // check for 0 devices found or errors...
    if (clDeviceCount == 0)
    {
        printf(" No devices found supporting OpenCL (return code %i)\n\n", ciErrNum);
        bPassed = FALSE;
    }
    else if (ciErrNum != CL_SUCCESS)
    {
        printf(" Error %i in clGetDeviceIDs call\n\n", ciErrNum);
        bPassed = FALSE;
    }
    else
    {
        // Get and log the OpenCL device ID's
        printf(" %u devices found supporting OpenCL:\n\n", clDeviceCount);
        ciErrNum = clGetDeviceIDs (clSelectedPlatformID, CL_DEVICE_TYPE_ALL, clDeviceCount, cldevices, &clDeviceCount);
        if (ciErrNum == CL_SUCCESS)
        {   // Connect to the compute devices
            for (unsigned int i = 0; i < clDeviceCount; ++i )
            {
                // show info for each device in the context
                printf(" --------------------------------------------------\n");
                clGetDeviceInfo(cldevices[i], CL_DEVICE_NAME, sizeof(cBuffer), &cBuffer, NULL);
                printf(" Device %s\n", cBuffer);
                printf(" --------------------------------------------------\n");
                
                // Create a compute context
                cl[i].context = clCreateContext(0, 1, cldevices+i, NULL, NULL, &ciErrNum);
                if (!cl[i].context)
                {
                    printf("Error %i: Failed to create a compute context!\n",ciErrNum);
                    bPassed = FALSE;
                    break;
                }
                // Create a command queue
                cl[i].queue = clCreateCommandQueue(cl[i].context, cldevices[i], 0, &ciErrNum);
                if (!cl[i].queue)
                {
                    printf("Error %i: Failed to create a command queue!\n",ciErrNum);
                    bPassed = FALSE;
                    break;
                }
                // Create the compute program from the source buffer
                cl[i].program = clCreateProgramWithSource(cl[i].context, 1, (const char **) & KernelSource, NULL, &ciErrNum);
                if (!cl[i].program)
                {
                    printf("Error %i: Failed to create compute program!\n",ciErrNum);
                    bPassed = FALSE;
                    break;
                }
                // Build the program executable
                ciErrNum = clBuildProgram(cl[i].program, 0, NULL, NULL, NULL, NULL);
                if (ciErrNum != CL_SUCCESS)
                {
                    size_t len;
                    char buffer[2048];
                    
                    printf("Error: Failed to build program executable!\n");
                    clGetProgramBuildInfo(cl[i].program, cldevices[i], CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
                    printf("%s\n", buffer);
                    bPassed = FALSE;
                    break;
                }
                else
                    printf("OpenCL program executable successfully built for this device.\n");

                // Create the compute kernel in the program we wish to run
                cl[i].kernel[0] = clCreateKernel(cl[i].program, "compare", &ciErrNum);
                
                if (!cl[i].kernel[0] || ciErrNum != CL_SUCCESS)
                {
                    printf("Error %i: Failed to create compute kernel!\n",ciErrNum);
                    bPassed = FALSE;
                    break;
                }
                else
                    printf("Compute kernel built successfully.\n");
                
                
                // Create the input and output arrays in device memory for our calculation
                cl[i].input1 = clCreateBuffer(cl[i].context,  CL_MEM_READ_ONLY,  sizeof(unsigned char) * bufsize, NULL, NULL);
                cl[i].input2 = clCreateBuffer(cl[i].context,  CL_MEM_READ_ONLY,  sizeof(unsigned char) * schsize, NULL, NULL);
                cl[i].output = clCreateBuffer(cl[i].context, CL_MEM_WRITE_ONLY, sizeof(unsigned int) * bufsize/schsize, NULL, NULL);
                if (!cl[i].input1 || !cl[i].input2 || !cl[i].output)
                {
                    printf("Fatal Error: Failed to allocate OpenCL device memory!\n");
                    exit(1);
                }
                ciErrNum  = clSetKernelArg(cl[i].kernel[0], 0, sizeof(cl_mem), &cl[i].input1);
                ciErrNum |= clSetKernelArg(cl[i].kernel[0], 1, sizeof(cl_mem), &cl[i].input2);
                ciErrNum |= clSetKernelArg(cl[i].kernel[0], 2, sizeof(cl_mem), &cl[i].output);
                ciErrNum |= clSetKernelArg(cl[i].kernel[0], 3, sizeof(int), &schsize);
                ciErrNum |= clSetKernelArg(cl[i].kernel[0], 4, sizeof(int), &bufsize);
                //ciErrNum |= clSetKernelArg(cl[i].kernel[0], 5, sizeof(unsigned char)*schsize, 0);
                ciErrNum |= clSetKernelArg(cl[i].kernel[0], 5, sizeof(unsigned char)*schsize, 0);
                if (ciErrNum != CL_SUCCESS)
                {
                    printf("Error: Failed to set kernel arguments! %d\n", ciErrNum);
                    exit(1);
                }
                // Get the maximum work group size for executing kernel on device
                size_t wkgpsize;
                ciErrNum = clGetKernelWorkGroupInfo(cl[i].kernel[0], cldevices[i], CL_KERNEL_WORK_GROUP_SIZE, sizeof(wkgpsize), &wkgpsize, NULL);
                if (ciErrNum != CL_SUCCESS)
                {
                    printf("Fatal Error: Failed to retrieve kernel %i work group info! %d\n",i, ciErrNum);
                    exit(1);
                }
                printf(" Device %i local workgroup size: %zu\n",i,wkgpsize);
                if (wkgpsize != CLWKGRPSIZE) printf(" Warning OpenCL device may not be used optimally: value of CLWKGRPSIZE is not equal to device %i local workgroup size\n",i);
                else printf(" Using CLWKGRPSIZE equal to device local workgroup size.\n");
            }
        }
        else
        {
            printf(" Error %i in clGetDeviceIDs call\n\n", ciErrNum);
            bPassed = FALSE;
        }
    }
    return bPassed;
}

void clshutdown()
// Shutdown and cleanup
{
    
    for (int i = 0; i < clDeviceCount; i++)
    {
        if (cl[i].input1) clReleaseMemObject(cl[i].input1);
        if (cl[i].input2) clReleaseMemObject(cl[i].input2);
        if (cl[i].output) clReleaseMemObject(cl[i].output);
        if (cl[i].program) clReleaseProgram(cl[i].program);
        for (int j = 0; j < MAXKERNELS; j++)
        {
            if (cl[i].kernel[j]) clReleaseKernel(cl[i].kernel[j]);
        }
        if (cl[i].queue) clReleaseCommandQueue(cl[i].queue);
        if (cl[i].context) clReleaseContext(cl[i].context);
    }
}

void initpthread()
// initialise POSIX thread variables
{
    validtriplets = tripletsbuffer1;
    emptybuffer = tripletsbuffer2;
    
    pthread_attr_init(&qattr);
    pthread_attr_setdetachstate(&qattr, PTHREAD_CREATE_JOINABLE);
}

void shutdownthread()
// close down thread variables
{
    pthread_attr_destroy(&qattr);
    //pthread_exit(NULL);  // not needed as we join and halt this thread before compute() ends
}

int main (int argc, const char * argv[])
{
    char fname[4][MAXNAME];
    
    printf("\nSystemic Computer v8\n\n");
    
    // set up systemic computer
    loadini((char *)argv[0],fname);
    randomize();

    // set up OpenCL (if available) and POSIX threads
    OpenCLAvailable = clsetup(MAXBUFSIZE,schemasize+functionsize+schemasize);
    initpthread();
    
    initsystems();
    
	interaction_count = 0;
	simfinished = FALSE;

	initProblem(fname[0],fname[1],fname[2],fname[3],s,scopetable);

    printf("\nBeginning excecution.");
	start_timer();

    // start the systemic computation:
    compute(s,scopetable,chain,chainnum);

	stop_timer();

	print_timer(MS);

	printfunctionsusage();

	endProblem(fname[0],fname[1],fname[2],fname[3],s,scopetable);
    
    // save the new state of the systems (de-compiled code)
    getnextscoutfile();
    printf("\nSaving decompiled program system state in %s",scoutfilename);
    savescprogram(scoutfilename,scfilename,s,numsystems,functions,numfunctions,scopetable,chain,chainnum);

    // save the new state of the systems (compiled code)
    getnextscpoutfile();
    printf("\nSaving end of program system state in %s",scpoutfilename);
    saveprogram(scpoutfilename,scfilename,s,numsystems,functions,numfunctions,scopetable,chain,chainnum);
    
    // free memory
    freesystems();
    
    // shutdown OpenCL and POSIX threads
    if (OpenCLAvailable) clshutdown();
    shutdownthread();
}