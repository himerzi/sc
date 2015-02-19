// LoadSC is based on CompileSC4
// CompileSC4 - fourth compiler for systemic computation programs, compatible with v7 of systemic computation
// (c) 2006-2013 Peter J Bentley
// version 2 supports overlapping scopes with the new #scope format and scope tables
// version 3 (HAoS C model) by Christos Sakellariou
// version 4 adds chaining of systems and allows for the chain array as well as the scope array
// version 5 is integrated into sc as the loading phase of the computer

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "loadsc.h"

BOOL foundfile(char *fname)
/* returns TRUE if it can find and open file fname, otherwise FALSE */
{   FILE *ptr;
    
    ptr = fopen(fname,"r");
    if (ptr == NULL) return FALSE;
    else
    { fclose(ptr);
        return TRUE;
    }
}

BOOL getnextscpoutfile()
// cerates the next outscpfilename, assumes scpoutfilename is big enough
{   char numstr[6]; // no more than a million output files please!
    
    do
    {   // construct the next name
        strcpy(scpoutfilename, scpfilename);
        scpoutfilename[strlen(scpoutfilename)-4] = '\0'; // take off the .scp
        sprintf(numstr,"%ld",scpoutfilenum);
        strcat(scpoutfilename,numstr);
        strcat(scpoutfilename,".scp");
        // does it already exist?
    } while (++scpoutfilenum < 99999 && foundfile(scpoutfilename));
    if (scpoutfilenum < 99999) return TRUE;
    else return FALSE;
}

BOOL getnextscoutfile()
// creates the next outscfilename, assumes scoutfilename is big enough
{   char numstr[6]; // no more than a million output files please!
    
    do
    {   // construct the next name
        strcpy(scoutfilename, scfilename);
        scoutfilename[strlen(scoutfilename)-3] = '\0'; // take off the .sc
        sprintf(numstr,"%ld",scoutfilenum);
        strcat(scoutfilename,numstr);
        strcat(scoutfilename,".sc");
        // does it already exist?
    } while (++scoutfilenum < 99999 && foundfile(scoutfilename));
    if (scoutfilenum < 99999) return TRUE;
    else return FALSE;
}

static inline void copyover(unsigned char *s1, unsigned char *s2, int copysize)
// copies s1 over s2; s1 remains unchanged
{    int n;

     for (n = 0; n < copysize; n++) s2[n] = s1[n];
}

void parseerror(int *e,FILE *fstrm, char *line, const char str[],int c,BOOL fatal)
// prints that lovely error message
{   int i;
        
    if (line[strlen(line)-1] == '\n') line[strlen(line)-1] = '\0';

    if (fatal)
    { printf("\n%s\n",line);
      sprintf(console+strlen(console),"\n%s\n",line);
        for (i = 0; i < c; i++) { printf(" "); strcat(console, " "); }
      printf("^ %d    FATAL PARSING ERROR: %s",c,str);
      sprintf(console+strlen(console),"^ %d    FATAL PARSING ERROR: %s",c,str);

      if (fstrm != NULL) fclose(fstrm);
      safe_exit(5);
    }
    else
    { printf("\n%s\n",line);
      sprintf(console+strlen(console),"\n%s\n",line);
    
      for (i = 0; i < c; i++) { printf(" "); strcat(console, " "); }
      printf("^ %d    PARSE ERROR: %s",c,str);
      sprintf(console+strlen(console),"^ %d    PARSE ERROR: %s",c,str);
        
      (*e)++;
    }
}

void error(int *e,FILE *fstrm, char *line, const char str[],int l,BOOL fatal)
// prints that lovely error message
{   char *lineptr;
    unsigned int i;
    
    for (i = 0; (line[i] == ' ' || line[i] == '\t' || line[i] == '\n') && i < strlen(line); i++);
    lineptr = line+i; if (lineptr[strlen(lineptr)-1] == '\n') lineptr[strlen(lineptr)-1] = '\0';

    if (fatal)
    { printf("\n%s",lineptr);
      sprintf(console+strlen(console),"\n%s",lineptr);

      printf("\nLine %d  FATAL ERROR: %s",l,str);
      sprintf(console+strlen(console),"\nLine %d  FATAL ERROR: %s",l,str);
        
      if (fstrm != NULL) fclose(fstrm);
      safe_exit(5);
    }
    else
    { printf("\n%s",lineptr);
      sprintf(console+strlen(console),"\n%s",lineptr);
      printf("\nLine %d  ERROR: %s",l,str);
      sprintf(console+strlen(console),"\nLine %d  ERROR: %s",l,str);
        
      (*e)++;
    }
}


// better merge all getval functions in one (adding one argument for length)
BOOL getval(unsigned char *schema,char *number,int l,int *e)
// sets schema to the number represented in number[] if valid, returns TRUE if successful
{   unsigned int i;
	int val,ret;

    ret = FALSE;
    if (number[0] != '%') parseerror(e,NULL,number,"missing number format specification",l,FALSE);
    if (number[1] == 'd')
    { if (sscanf(number+2,"%d",&val) != 1) parseerror(e,NULL,number,"unrecognised number format",l,FALSE);
      else { itoschema(val, schema); ret = TRUE; }
    }
    else if (number[1] == 'b')
    { if (strlen(number+2) != schemasize)
		{
        parseerror(e,NULL,number,"schema binary value contains incorrect number of digits",l,FALSE);
		printf("\n%s   %zd\n",number+2,strlen(number+2));
        sprintf(console+strlen(console),"\n%s   %zd\n",number+2,strlen(number+2));
		}
      else
      { for (i = 0; i < schemasize; i++)
        {  switch (number[i+2])
           {  case '0': schema[i] = ZERO; break;
              case '1': schema[i] = ONE; break;
              default: schema[i] = WILDCARD;
           }
        }
        ret = TRUE;
      }
    }
    else parseerror(e,NULL,number,"unrecognised number format specification",l,FALSE);

    return ret;
}

BOOL getfunctionval(unsigned char *function,char *number,int l,int *e)
// sets function to the number represented in number[] if valid, returns TRUE if successful
{   unsigned int i;
	int val,ret;

    ret = FALSE;
    if (number[0] != '%') parseerror(e,NULL,number,"missing number format specification",l,FALSE);
    if (number[1] == 'd')
    { 
		if (sscanf(number+2,"%d",&val) != 1) 
			parseerror(e,NULL,number,"unrecognised number format",l,FALSE);
		else 
		{ 
			itofunction(val, function); ret = TRUE;
		}
    }
    else if (number[1] == 'b')
    { if (strlen(number+2) != functionsize)
        parseerror(e,NULL,number,"function binary value contains incorrect number of digits",l,FALSE);
      else
      { for (i = 0; i < functionsize; i++)
        {  switch (number[i+2])
           {  case '0': function[i] = ZERO; break;
              case '1': function[i] = ONE; break;
              default: function[i] = WILDCARD;
           }
        }
        ret = TRUE;
      }
    }
    else parseerror(e,NULL,number,"unrecognised number format specification",l,FALSE);

    return ret;
}

BOOL getlabelval(unsigned char *schema,char *number,int l,int *e)
// sets label to the number represented in number[] if valid, returns TRUE if successful
{   unsigned int i;
	int val,ret;

    ret = FALSE;
    if (number[0] != '%') parseerror(e,NULL,number,"missing number format specification",l,FALSE);
    if (number[1] == 'd')
    { if (sscanf(number+2,"%d",&val) != 1) parseerror(e,NULL,number,"unrecognised number format",l,FALSE);
      else { itofunction(val, schema); ret = TRUE; } // needs further changes to support it
    }
    else if (number[1] == 'b')
	{ if (!((strlen(number+2) == schemasize) | (strlen(number+2) == functionsize) ))
		{
			parseerror(e,NULL,number,"schema binary value contains incorrect number of digits",l,FALSE);
            
		}
      else
      { for (i = 2; i < strlen(number); i++)
        {  switch (number[i])
           {  case '0': schema[i-2] = ZERO; break;
              case '1': schema[i-2] = ONE; break;
              default: schema[i-2] = WILDCARD;
           }
        }
        ret = TRUE;
      }
    }
    else parseerror(e,NULL,number,"unrecognised number format specification",l,FALSE);

    return ret;
}

void checkforduplicates(symbol *functions, int fn, symbol *labels, int ln, syssymbol *sys, int sn,int *e,int l)
// checks to see if any identifier has been accidentally repeated and reports an error if so
{   int a,b,c;

    // have we used the same name for more than one thing?
    for (a = 0; a < fn; a++)
    {  for (b = 0; b < ln; b++)
       {  if (strcmp(functions[a].name,labels[b].name) == 0)
          { error(e,NULL,functions[a].name,"cannot use function name as label",l,FALSE);
            // and prevent the same error again by changing the name:
            functions[a].name[0] = '2';
          }
       }
       for (c = 0; c < sn; c++)
       {  if (strcmp(functions[a].name,sys[c].name) == 0)
          { error(e,NULL,functions[a].name,"cannot use function name as system identifier",l,FALSE);
            // and prevent the same error again by changing the name:
            functions[a].name[0] = '2';
          }
       }
    }

    for (c = 0; c < sn; c++)
    {  for (b = 0; b < ln; b++)
      { if (strcmp(sys[c].name,labels[b].name) == 0)
        { error(e,NULL,sys[c].name,"cannot use label name as system identifier",l,FALSE);
          // and prevent the same error again by changing the name:
          sys[c].name[0] = '2';
        }
      }
    }

    // or do we have any duplicates of the same type?
    for (a = 0; a < fn; a++)
       for (b = 0; b < fn; b++)
          if (a != b && strcmp(functions[a].name,functions[b].name) == 0)
          {  error(e,NULL,functions[a].name,"duplicate function definition",l,FALSE);
             // and prevent the same error again by changing the name:
             functions[a].name[0] = '2';
          }
    for (a = 0; a < ln; a++)
       for (b = 0; b < ln; b++)
          if (a != b && strcmp(labels[a].name,labels[b].name) == 0)
          { error(e,NULL,labels[a].name,"duplicate label definition",l,FALSE);
            // and prevent the same error again by changing the name:
            labels[a].name[0] = '2';
          }
    for (a = 0; a < sn; a++)
       for (b = 0; b < sn; b++)
          if (a != b && strcmp(sys[a].name,sys[b].name) == 0)
          { error(e,NULL,sys[a].name,"duplicate system identifier",l,FALSE);
            // and prevent the same error again by changing the name:
            sys[a].name[0] = '2';
          }
}

BOOL isplus(char *in, int *l)
// updates *l to point to beginning of next chunk that starts with ( if it exists, or end of string otherwise, and returns TRUE if a + was found
{   BOOL foundplus = FALSE;
    
    for (; in[*l] != '(' && in[*l] != '\n' && in[*l] != '\0' && (unsigned int) *l < strlen(in); (*l)++)
    {  if (in[*l] == '+') foundplus = TRUE;
    }
    return foundplus;
}

void getchunk(char *chunk,char *in, int *l)
// places the next chunk (delineated by ']' or ')') from in[*l]; updating *l to point to the end of the chunk
{   int i;

    i = 0;
    for (; in[*l] != ']' && in[*l] != ']' && in[*l] != ')' && in[*l] != '(' &&
           in[*l] != '\n' && in[*l] != '\0' && (unsigned int) *l < strlen(in); (*l)++)
    {  if (in[*l] != '[' && in[*l] != ']' && in[*l] != '(' && in[*l] != ')') chunk[i++] = in[*l];
    }
    chunk[i] = '\0';
} 

void getword_nob(char *word,char *in, int *l)
// places the next word (separated by whitespace) from in[*l]; updating *l to point to the end of the word
// the nob means no bracket
{   int i;

    i = 0;
    for (; in[*l] != ' ' && in[*l] != '\n' && in[*l] != '\t' && in[*l] != '\0' && in[*l] != ')' && (unsigned int)*l < strlen(in); (*l)++)
//    for (; in[*l] != ' ' && in[*l] != '\n' && in[*l] != '\t' && in[*l] != '\0' && (unsigned int)*l < strlen(in); (*l)++)
    {  word[i++] = in[*l];
    }
    word[i] = '\0';
} 

BOOL getfunction(symbol *functions, int fn, unsigned char *schema,char *word,int l,int *e)
// extracts and codes the function from word into schema, returning TRUE if successful
{   char func[32];
    BOOL ret;
    int t1,t2,f,i;
    unsigned char t1schema[functionsize], t2schema[functionsize];

    ret = FALSE;
    // first extract the function name:
    i = 0;
    getchunk(func,word,&i);
    if (sscanf(word+i,"(%4d,%4d)",&t1,&t2) != 2) 
		parseerror(e,NULL,word,"function incorrectly specified",l,FALSE);
    else
    { // do we recognise this function?
      for (f = 0; (strcmp(func,functions[f].name) != 0) && f < fn; f++);
      if (f >= fn) parseerror(e,NULL,func,"unrecognised function",l,FALSE);
      else // code the function by combining four elements:
      { itoschema(t1,t1schema);
        itoschema(t2,t2schema);
        for (i = 0; i < functionsize-1; i++)
        { 
			if (i < iniFUNCTIONID) 
				schema[i] = functions[f].schema[i];
			else if	(i < iniFUNCTIONID+iniMATCHINGTHRES) 
				schema[i] = t1schema[i-iniFUNCTIONID];
			else if (i < iniFUNCTIONID+iniMATCHINGTHRES+iniMATCHINGTHRES) 
				schema[i] = t2schema[i-iniFUNCTIONID-iniMATCHINGTHRES];
            else if (i < functionsize)
                schema[i] = 0;
        }
        if (word[strlen(word)-1] == '!') 
			schema[functionsize-1] = 1;
		else
			schema[functionsize-1] = 0;
        ret = TRUE;
      }
    }
    return ret;
}

BOOL issystem(char *name, int *s, syssymbol *sys, int sn)
// returns TRUE if name is listed in sys[]
{  BOOL ret;
   int i;
    
   ret = FALSE;
   for (i = 0; i < sn; i++) if (strcmp(name,sys[i].name) == 0) { ret = TRUE; *s = i; break; } 
   
   return ret;
}

BOOL replacewithlabel(char *sch)
// replaces contents of sch[] with its equiv label if one can be found
{
    int n,m,foundnum;
    unsigned long len;
    BOOL matched = FALSE;
    
    len = strlen(sch);
    for (n = 0; n < iniMAXFUNCTIONS && !matched; n++)
    {   matched = TRUE;
        for (m = 0; m < len; m++)
        {
            if ((labels[n].schema[m] == 0 && sch[m] != '0') ||
                (labels[n].schema[m] == 1 && sch[m] != '1') ||
                (labels[n].schema[m] == '?' && sch[m] != '?')) { matched = FALSE; break; } // no match, give up with this label
        }
        if (matched) foundnum = n;
    }
    if (matched) strcpy(sch, labels[foundnum].name);
    return matched;
}

BOOL islabel(char *name, int *n, symbol *label, int ln)
// returns TRUE if name is listed in label[]
{  BOOL ret;
   int i;
    
   ret = FALSE;
   for (i = 0; i < ln; i++) if (strcmp(name,label[i].name) == 0) { ret = TRUE; *n = i; break; } 
   
   return ret;
}


BOOL code(systemic *s, unsigned char *schema,char *chunk,symbol *labels,int ln,syssymbol *sys,int sn,int l,int *e)
// figures out the three symbols in chunk, and codes them into a schema, returning TRUE if successful
{   BOOL ret;
    int r,snum,lnum;
    char one[32], two[32], three[32];
    unsigned char sch1[schemasize],sch2[schemasize],fn[functionsize];

    ret = FALSE;
    r = sscanf(chunk,"%s %s %s",one,two,three);
    if (r == 1) // only one label specified, so this must be listed as a system or there'll be trouble
    { if (!issystem(one,&snum,sys,sn)) parseerror(e,NULL,chunk,"missing schema / label is not a recognised system identifier",l,FALSE);
      else
      { // it's OK, it's system no. snum
        encode(schema,s[snum].schema1,s[snum].function,s[snum].schema2);
        ret = TRUE;
      }
    }
    else if (r == 3) // three labels specified, so these must be listed as #labels
    {   if (one[0] == '%') getval(sch1,one,0,e);
        else
        { if (!islabel(one,&lnum,labels,ln)) parseerror(e,NULL,one,"undefined label in system definition",0,FALSE);
        else copyover(labels[lnum].schema,sch1,schemasize);
        }
        if (two[0] == '%') getval(fn,two,0,e);
        else
        { if (!islabel(two,&lnum,labels,ln)) parseerror(e,NULL,two,"undefined label in function definition",0,FALSE);
          else copyover(labels[lnum].schema,fn,functionsize);
        }
        if (three[0] == '%') getval(sch2,three,0,e);
        else
        { if (!islabel(three,&lnum,labels,ln)) parseerror(e,NULL,three,"undefined label in system definition",0,FALSE);
          else copyover(labels[lnum].schema,sch2,schemasize);
        }
        encode(schema,sch1,fn,sch2);
        ret = TRUE;
    }  
    else // oh dear
    { parseerror(e,NULL,chunk,"incorrect number of schema specified",l,FALSE);
    }
    return ret;
}

void loadchain(FILE *fstrm,int sysnum, syssymbol *sys, int sn, chainlink **chain,int *cnum, int *l, int *errors)
// fills the chaintable with the details read between { and }
{    BOOL done,started,ending;
    char *line, *lineptr;
    char one[128],two[128], three[128];
    char chainlink[128];
    int i,snum,link;
    int chunknum;
    char chainstack[iniMAXSYSTEMS][128];
    int look, csnum;
    BOOL found,chaindone;
    
    line = malloc(MAXLLEN * sizeof(char)); _memcheck(line, "line");
    started = ending = done = FALSE;
    do
    {
        // we know the head of the chain already, fill it with default values:
        link = 0;
        chain[*cnum][link].systemnum = sysnum;
        chain[*cnum][link].sch1 = SCHLEFT;
        chain[*cnum][link].sch2 = SCHRIGHT;
        link++;
        csnum = 0; // reset our chainstack pointer for variable names
        chaindone = FALSE;     
        
        do
        { (*l)++;
          if (fgets(line,MAXLLEN,fstrm) == NULL) done = TRUE;
          // remove whitespace at start:
          for (i = 0; (line[i] == ' ' || line[i] == '\t') && (unsigned int)i < strlen(line); i++);
          lineptr = line+i;
        } while (!done && (lineptr[0] == '/' || lineptr[0] == ';' || lineptr[0] == '\n'));
        
        if (lineptr[0] == '}') ending = TRUE;
        if (started && !ending)
        {   // found { now looking for the first chain link
            do
            {
                if (lineptr[0]  != '(')
                {   error(errors,fstrm,lineptr,"syntax error in chain definition, missing: (",*l,FALSE);
                    chaindone = TRUE;
                }
                else
                {   // get the next link
                    lineptr++; chunknum = 0;
                    getchunk(chainlink,lineptr, &chunknum);
                    if (sscanf(chainlink,"%s %s %s",one,two,three) != 3) error(errors,fstrm,lineptr,"syntax error in chain definition, expected: ($L|$R|?name|name function $L|$R|?name|name)",*l,FALSE);
                    {   lineptr += chunknum; chunknum = 0;
                        // have we got a valid system in the link?
                        if (!issystem(two, &snum, sys, sn))
                        {   error(errors,fstrm,lineptr,"undefined system identifier in chain",*l,FALSE);
                            chaindone = TRUE;
                        }
                        else // found our chain link triplet, but which schema to use?
                        {   chain[*cnum][link].systemnum = snum;
                            // first the left schema
                            if (one[0] == '$') // programmer wants this link to use the schema from the head of the chain
                            {
                                if (one[1] == 'L') chain[*cnum][link].sch1 = SCHLEFT;
                                else if (one[1] == 'R') chain[*cnum][link].sch1 = SCHRIGHT;
                                else error(errors,fstrm,lineptr,"unrecognized symbol in chain definition, expected $L or $R",*l,FALSE);
                            }
                            else if (one[0] == '?') // programmer wants this link to find a new random schema
                            {
                                if (one[1] == ' ') error(errors,fstrm,lineptr,"missing variable name after ? symbol in chain definition",*l,FALSE);
                                // has the name arleady been used?
                                found = FALSE;
                                for (look = 0; look < csnum && !found; look++)
                                    if (strcmp(chainstack[look], one+1) == 0) found = TRUE;
                                if (found)
                                    error(errors,fstrm,lineptr,"duplicated variable in chain definition, to reuse previously declared variable do not add ? symbol",*l,FALSE);
                                else
                                {    // add this variable name to the stack and remember the variable number
                                    strcpy(chainstack[csnum],one+1);
                                    chain[*cnum][link].sch1 = csnum++;
                                }
                            }
                            else // programmer wants to use the schema found in an earlier link
                            {   found = FALSE;
                                // which value is it?
                                for (look = 0; look < csnum && !found; look++)
                                {
                                    if (strcmp(chainstack[look], one) == 0)
                                    {
                                        chain[*cnum][link].sch1 = look;
                                        found = TRUE;
                                    }
                                }
                                if (!found) error(errors,fstrm,lineptr,"undeclared variable in chain definition - new random schema must be declared using ?name",*l,FALSE);
                            }
                            // then the right schema, same deal:
                            if (three[0] == '$') // programmer wants this link to use the schema from the head of the chain
                            {
                                if (three[1] == 'L') chain[*cnum][link].sch2 = SCHLEFT;
                                else if (three[1] == 'R') chain[*cnum][link].sch2 = SCHRIGHT;
                                else error(errors,fstrm,lineptr,"unrecognized symbol in chain definition, expected $L or $R",*l,FALSE);
                            }
                            else if (three[0] == '?') // programmer wants this link to find a new random schema
                            {   if (three[1] == ' ') error(errors,fstrm,lineptr,"missing variable name after ? symbol in chain definition",*l,FALSE);
                                // has the name arleady been used?
                                found = FALSE;
                                for (look = 0; look < csnum && !found; look++)
                                    if (strcmp(chainstack[look], three+1) == 0) found = TRUE;
                                if (found)
                                    error(errors,fstrm,lineptr,"duplicated variable in chain definition, to reuse previously declared variable do not add ? symbol",*l,FALSE);
                                else
                                {    // add this variable name to the stack and remember the variable number
                                    strcpy(chainstack[csnum],three+1);
                                    chain[*cnum][link].sch2 = csnum++;
                                }
                            }
                            else // programmer wants to use the schema found in an earlier link
                            {   found = FALSE;
                                // which value is it?
                                for (look = 0; look < csnum && !found; look++)
                                {
                                    if (strcmp(chainstack[look], three) == 0)
                                    {
                                        chain[*cnum][link].sch2 = look;
                                        found = TRUE;
                                    }
                                }
                                if (!found) error(errors,fstrm,lineptr,"undeclared variable in chain definition - new random schema must be declared using ?name",*l,FALSE);
                            }
                        }
                    }
                }
                // now find the + to the next link, if it exists
                if (isplus(lineptr, &chunknum)) { link++; lineptr += chunknum; chunknum = 0; } // another link in the chain
                else chaindone = TRUE; // no more links in the chain

            } while (!chaindone);
        }
        if (lineptr[0] == '{') started = TRUE;
        else if (!ending && !done) (*cnum)++; // filled one chain
    } while (!ending && !done);
    free(line);
}

void loadscope(FILE *fstrm,int sysnum, syssymbol *sys, int sn, int **scopetable,int *l, int *errors,char *scopename)
// fills the scopetable with the details read between { and }
{    BOOL done,started,ending;
     char *line;
     char *lineptr;
     char one[128],arrsys[128];
     char syscnt[33];
     int arrl,arrh;
     int i,snum;

     line = malloc(MAXLLEN * sizeof(char)); _memcheck(line, "line");
     started = ending = done = FALSE;
     do
     { do
       { (*l)++; 
         if (fgets(line,MAXLLEN,fstrm) == NULL) done = TRUE;  
         // remove whitespace at start:
         for (i = 0; (line[i] == ' ' || line[i] == '\t') && (unsigned int)i < strlen(line); i++);
         lineptr = line+i; 
       } while (!done && (lineptr[0] == '/' || lineptr[0] == ';' || lineptr[0] == '\n'));

       if (lineptr[0] == '}') ending = TRUE;
       if (started && !ending)
       { if (sscanf(lineptr,"%s",one) != 1) error(errors,fstrm,lineptr,"unable to find system identifier in scope definition",*l,FALSE);
         else
         { 
            if(one[0] == '[')
            {
              if (sscanf(one,"[%d:%d]%s",&arrl,&arrh,one) != 3) error(errors,fstrm,lineptr,"syntax error in system array identifier in scope list",*l,FALSE);
              if (arrl < 0 || arrh < 0 || arrl > arrh) error(errors,fstrm,lineptr,"syntax error in system array identifier in scope list",*l,FALSE);
              for(i = arrl; i <= arrh; i++)
              {
                sprintf(syscnt,"%d",i);
                
                strcpy(arrsys,one);
                strcat(arrsys,syscnt);
                
                if (!issystem(arrsys, &snum, sys, sn)) error(errors,fstrm,lineptr,"undefined system identifier in scope list",*l,FALSE);              
                else scopetable[sysnum][snum] = 1; // we know the programmer wants system snum to be in the scope of system sysnum
              }
            }
            else
            {
              //check if this is the scope itself if so, pad it with the array index
              if (memcmp(scopename, one, strlen(one))==0) {
                    strcpy(one, scopename);
                }
              if (!issystem(one, &snum, sys, sn)) error(errors,fstrm,lineptr,"undefined system identifier in scope list",*l,FALSE);
              else scopetable[sysnum][snum] = 1; // we know the programmer wants system snum to be in the scope of system sysnum
            }
         }
       }
       if (lineptr[0] == '{') started = TRUE;


     } while (!ending && !done);
     if (!ending) error(errors,fstrm,lineptr,"premature ending of scope definition (missing }?)",*l,FALSE);
     free(line);
}

BOOL assemble(systemic *s, int *numsystems, symbol *functions, int fn, symbol *labels, int ln, syssymbol *sys, int *e)
// uses all the data in functions, labels and sys to create the encoded systems in s
// returns TRUE if successful, FALSE if errors were found (e.g. missing identifiers)
{   int i,j,l,lnum;
    size_t len;
    BOOL schem1OK,schem2OK,funcOK;
    char word[schemasize+functionsize+schemasize+32];

    // main loop - we need to create each system in turn
    for (i = 0; i < *numsystems; i++)
    {  // first reset all contents to zero:
       for (j = 0; j < schemasize; j++) s[i].schema1[j] = s[i].schema2[j] = 0;
       for (j = 0; j < functionsize; j++) s[i].function[j] = 0;

       len = strlen(sys[i].systembody);
       if (len < 13) parseerror(e,NULL,sys[i].systembody,"incomplete system",0,FALSE);
       else if (sys[i].systembody[0] != '(') parseerror(e,NULL,sys[i].systembody,"missing '(' from system definition",0,FALSE);
       else
       { schem1OK = schem2OK = funcOK = FALSE;
         for (l = 1; l < len && !(schem1OK && schem2OK && funcOK); l++)
         {  if (sys[i].systembody[l] == ')' && !(schem1OK && schem2OK && funcOK)) parseerror(e,NULL,sys[i].systembody,"premature ')' in system definition",l,FALSE);
            // so far so good; now we are expecting either a '%' or a '['
            if (sys[i].systembody[l] == '%') // programmer is defining the schema directly with a number
            { getword_nob(word,sys[i].systembody,&l);
              if (!schem1OK) 
				  schem1OK = getval(s[i].schema1,word,l,e);
              else if (!funcOK) 
				  funcOK = getfunctionval(s[i].function,word,l,e);
              else if (!schem2OK) 
				  schem2OK = getval(s[i].schema2,word,l,e);
            }
            if (sys[i].systembody[l] == '[') // programmer is using labels and system identifiers to define this system
            { getchunk(word,sys[i].systembody,&l); l++;
                if (!schem1OK) { schem1OK = code(s,s[i].schema1,word,labels,ln,sys,*numsystems,l,e); /*if (schem1OK) strcpy(agent[i].sch1label, word);*/  }
                else if (!schem2OK) { schem2OK = code(s,s[i].schema2,word,labels,ln,sys,*numsystems,l,e); /*if (schem2OK) strcpy(agent[i].sch2label, word);*/  }
            }
            if (sys[i].systembody[l] != '%' && sys[i].systembody[l] != '[' && sys[i].systembody[l] != ' ' && sys[i].systembody[l] != '\t')
            { 
              if (schem1OK && funcOK && !schem2OK)
              { // must be the second schema
                getword_nob(word,sys[i].systembody,&l);
                if (!(schem2OK=islabel(word,&lnum,labels,ln))) 
					parseerror(e,NULL,sys[i].systembody,"unrecognised label in definition",l,FALSE);
                else 
                  { copyover(labels[lnum].schema,s[i].schema2,schemasize); /*strcpy(agent[i].sch2label, word);*/  }
              }
              if (schem1OK && !funcOK && !schem2OK)
              { // must be the function
                getword_nob(word,sys[i].systembody,&l);
				if (islabel(word,&lnum,labels,ln))
				{
					copyover(labels[lnum].schema,s[i].function,functionsize);
					funcOK = TRUE;
				}
				else if(getfunction(functions,fn,s[i].function,word,l,e))
					funcOK = TRUE;
				else
					funcOK = FALSE;
                  //if (funcOK) { strcpy(agent[i].fnlabel, word); strcat(agent[i].fnlabel, ")"); }
              }              
              if (!schem1OK)
              { // must be the first schema
                getword_nob(word,sys[i].systembody,&l);
                if (!(schem1OK=islabel(word,&lnum,labels,ln))) 
					parseerror(e,NULL,sys[i].systembody,"unrecognised label in definition",l,FALSE);
                else 
				{	copyover(labels[lnum].schema,s[i].schema1,schemasize); /*strcpy(agent[i].sch1label, word);*/ }
              }
            }
         }
         if (!(schem1OK && schem2OK &&funcOK)) 
			 parseerror(e,NULL,sys[i].systembody,"incomplete system definition",l,FALSE);
       }
       if (!isvalidfunction(s,i))
       {
           printf("\nFATAL ERROR: corrupted functional part of system detected.\n");
           safe_exit(-43);
       }

    }
    
    if (*e > 0)
    { printf("\nAssembly failed with %d errors", *e);
      sprintf(console+strlen(console),"\nAssembly failed with %d errors", *e);
      return FALSE;
    }
    else
    { printf("\nAssembly succeeded!");
      strcat(console,"\nAssembly succeeded!");
      return TRUE;
    }
}

BOOL assembleprogram(char *fname, systemic *s, int *numsystems, symbol *functions, int *fn,int **scopetable, int *scnumptr, chainlink **chain, int *cnum)
// reads and parses the systemic computation assembly language program
{   
    char *line, *lineptr;
    BOOL done,startOK,functionsOK,labelsOK;
    int n,m,p,i,l,ln,sn,sysnum,errors;
	int arrl,arrh;
    FILE *fstrm;
    BOOL programassembled;
	char syscnt[33];
    char one[32], two[32], three[128], tmp[32];
    
    line = malloc(MAXLLEN * sizeof(char)); _memcheck(line, "line");
    console[0] = '\0';
    *cnum = *scnumptr = 0;
    
    done = startOK = functionsOK = labelsOK = FALSE;
    errors = *fn = ln = sn = l = 0;

    // reset our arrays:
    for (n = 0; n < iniMAXSYSTEMS; n++) for (m = 0; m < iniMAXSYSTEMS; m++) scopetable[n][m] = 0;
    for (n = 0; n < iniMAXFUNCTIONS; n++)
    {   for (p = 0; p < MAXNAME; p++) functions[n].name[p] = labels[n].name[p] = '\0';
        for (m = 0; m < functionsize; m++) functions[n].schema[m] = labels[n].schema[m] = 0;
    }
    for (n = 0; n < iniMAXSYSTEMS; n++)
    {   for (p = 0; p < MAXNAME; p++) sys[n].name[p] = '\0';
        for (m = 0; m < SCHEMASIZE+FUNCTIONSIZE+SCHEMASIZE+32; m++) sys[n].systembody[m] = '\0';
    }
    
    if ((fstrm = fopen(fname,"r")) == NULL) error(&errors,fstrm,fname,"unable to open input file",l,TRUE);

    sprintf(console+strlen(console), "%s\nschema size: %d\nFunction size: %d",fname,schemasize,functionsize);
    
    do
    {  // read file line by line
       do
       { l++; 
         if (fgets(line,MAXLLEN,fstrm) == NULL) done = TRUE;  
         // remove whitespace at start:
         for (i = 0; (line[i] == ' ' || line[i] == '\t') && (unsigned int)i < strlen(line); i++);
         lineptr = line+i; 
       } while (!done && (lineptr[0] == '/' || lineptr[0] == ';' || lineptr[0] == '\n' || lineptr[0] == '\r'));

      // can we find the functions?
      if (startOK && !functionsOK)
      { if (sscanf(lineptr,"%s %s %s",one,two,three) != 3)
        { if (strcmp("#function",one) == 0)
            error(&errors,fstrm,lineptr,"unexpected number of symbols after #function",l,FALSE);
          else
              error(&errors,fstrm,lineptr,"couldn't identify #function",l,FALSE);
        }
        else
        { if (strcmp("#function",one) == 0)
          { strcpy(functions[*fn].name,two);
            getfunctionval(functions[*fn].schema,three,l,&errors); 
            (*fn)++; if (*fn > iniMAXFUNCTIONS) error(&errors,fstrm,lineptr,"too many #function statements in program",l,FALSE);
            checkforduplicates(functions, *fn, labels, ln, sys, sn,&errors,l);
          }
          else
          if (strcmp("#label",one) == 0) { functionsOK = TRUE; printf("\n%d functions read",*fn); sprintf(console+strlen(console),"\n%d functions read",*fn); }
          else if (!functionsOK) error(&errors, fstrm,lineptr,"unexpected symbol - expecting #function or #label",l,FALSE);
        }
      } 

      // can we find the labels?
      if (functionsOK && !labelsOK)
      { if (*fn < 1) error(&errors,fstrm,lineptr,"program must contain at least one #function definition",l,FALSE);
        if (sscanf(lineptr,"%s %s %s",one,two,three) != 3)
        { if (strcmp("#label",one) == 0) error(&errors,fstrm,lineptr,"unexpected number of symbols after #label",l,FALSE);
          else error(&errors,fstrm,lineptr,"couldn't identify #label",l,FALSE);
        }
        else
        {  if (strcmp("#label",one) == 0)
           { strcpy(labels[ln].name,two);
             getlabelval(labels[ln].schema,three,l,&errors); // may need getfunctionval
             ln++; if (ln > iniMAXFUNCTIONS) error(&errors,fstrm,lineptr,"too many #label statements in program",l,FALSE);
             checkforduplicates(functions, *fn, labels, ln, sys, sn,&errors,l);
           }
           else
           { labelsOK = TRUE; printf("\n%d labels read",ln); sprintf(console+strlen(console),"\n%d labels read",ln); }
        }
      } 

      // can we find the systems?
      if (labelsOK)
      { if (strcmp("#systemic end",lineptr) == 0)
        { done = TRUE;  printf("\n%d systems read",sn); sprintf(console+strlen(console),"\n%d systems read",sn); }
        else
        { if (sscanf(lineptr,"%s (",one) != 1)
          { error(&errors,fstrm,lineptr,"syntax error in system definition",l,FALSE);
          }
          else
          {  // do we have a chain to deal with
              if (strcmp("#chain",one) == 0)
              { if (sscanf(lineptr,"%s %s",one,two) != 2) error(&errors,fstrm,lineptr,"syntax error in chain definition",l,FALSE);
                  sysnum = 0;
                  // allow for defining several identical chain at the same time in the form: [0:N]chainname . Each chain would be named and stored as chainameN.
                  if (two[0] == '[')
                  {
                      if (sscanf(two,"[%d:%d]%s",&arrl,&arrh,two) != 3) error(&errors,fstrm,lineptr,"syntax error in chain array definition:incomplete definition.",l,FALSE);
                      if (arrl < 0 || arrh < 0 || arrl > arrh) error(&errors,fstrm,lineptr,"syntax error in chain array definition: illegal definition of array number.",l,FALSE);
                      long flen = ftell(fstrm);
                      int tmpl = l;
                      for(n = arrl; n <= arrh; n++)
                      {
                          fseek(fstrm,flen,SEEK_SET);
                          l = tmpl;
                          strcpy(tmp,two);
                          sprintf(syscnt,"%d",n);
                          strcat(tmp,syscnt);
                          if (!issystem(tmp, &sysnum, sys, sn)) error(&errors,fstrm,lineptr,"undefined system identifier in chain array definition",l,FALSE);
                          loadchain(fstrm,sysnum,sys,sn,chain,cnum,&l,&errors);
                      }
                  }
                  else
                  {
                      if (!issystem(two, &sysnum, sys, sn)) error(&errors,fstrm,lineptr,"undefined system identifier in chain definition",l,FALSE);
                      loadchain(fstrm,sysnum,sys,sn,chain,cnum,&l,&errors);
                  }
                  
                  
                  //if (!issystem(two, &sysnum, sys, sn)) error(&errors,fstrm,lineptr,"undefined system identifier in chain definition",l,FALSE);
                  //loadchain(fstrm,sysnum,sys,sn,chain,cnum,&l,&errors);
              }
              else 
              {  if (strcmp("#scope",one) == 0) // do we have a scope to deal with
                 {
                     if (sscanf(lineptr,"%s %s",one,two) != 2) error(&errors,fstrm,lineptr,"syntax error in scope definition",l,FALSE);
                     sysnum = 0;
                     if (two[0] == '[')
                     // do we have a scope array to deal with
                     {
                         if (sscanf(two,"[%d:%d]%s",&arrl,&arrh,two) != 3) error(&errors,fstrm,lineptr,"syntax error in scope array definition",l,FALSE);
                         if (arrl < 0 || arrh < 0 || arrl > arrh) error(&errors,fstrm,lineptr,"syntax error in scope array definition",l,FALSE);
                         long flen = ftell(fstrm);//locate the curent position of fstrm
                         for (n = arrl; n <= arrh; n++)
                         {
                             fseek(fstrm,flen,SEEK_SET); //relocate to the starting point
                             sprintf(syscnt,"%d",n);
                             strcpy(three,two);
                             strcat(three,syscnt);
                             if (!issystem(three, &sysnum, sys, sn)) error(&errors,fstrm,lineptr,"undefined system identifier in scope definition",l,FALSE);
                             loadscope(fstrm,sysnum,sys,sn,scopetable,&l,&errors,three);
                             (*scnumptr)++;
                         }
                     }
                     else
                     {
                         if (!issystem(two, &sysnum, sys, sn)) error(&errors,fstrm,lineptr,"undefined system identifier in scope definition",l,FALSE);
                         loadscope(fstrm,sysnum,sys,sn,scopetable,&l,&errors,two);
                         (*scnumptr)++;
                     }
                    
                 }
                 else // must be a system
                 {
                     if (one[0] == '[')
                     {
                       if (sscanf(one,"[%d:%d]%s",&arrl,&arrh,one) != 3) error(&errors,fstrm,lineptr,"syntax error in system array definition",l,FALSE);
                       if (arrl < 0 || arrh < 0 || arrl > arrh) error(&errors,fstrm,lineptr,"syntax error in system array definition",l,FALSE);
                       for (i = 0; lineptr[i] != '(' && (unsigned int) i < strlen(lineptr); i++);
                       for (n = arrl; n <= arrh; n++)
                       {
                         sprintf(syscnt,"%d",n);
                         strcpy(sys[sn].name,one);
                         strcat(sys[sn].name,syscnt);
                           
                         strcpy(sys[sn].systembody,lineptr+i);
                         sn++; if (sn >= iniMAXSYSTEMS) error(&errors,fstrm,lineptr,"too many systems in program",l,FALSE);
                       }
                       checkforduplicates(functions, *fn, labels, ln, sys, sn,&errors,l); // check outside the loop to improve compilation speed
                     }
                     else
                     {
                       strcpy(sys[sn].name,one);
                       for (i = 0; lineptr[i] != '(' && (unsigned int) i < strlen(lineptr); i++);
                       strcpy(sys[sn].systembody,lineptr+i);
                       sn++; if (sn >= iniMAXSYSTEMS) error(&errors,fstrm,lineptr,"too many systems in program",l,FALSE);
                       checkforduplicates(functions, *fn, labels, ln, sys, sn,&errors,l);
                     }
                 }
              }
          }
        }
      }

      // can we find the start marker?
      if (!startOK && (strcmp(lineptr,"#systemic start\r\n") == 0 || strcmp(lineptr,"#systemic start\n") == 0)) startOK = TRUE;

    } while(!done);
    
    printf("\n%d chains read",*cnum);
    sprintf(console+strlen(console),"\n%d chains read",*cnum);
    printf("\n%d scopes read",*scnumptr);
    sprintf(console+strlen(console),"\n%d scopes read",*scnumptr);
    
    if (!done) error(&errors,fstrm,lineptr,"unexpected end of program.",l,FALSE);
    else { printf("\nprogram successfully read"); strcat(console, "\nprogram successfully read"); }

    fclose(fstrm);
    
    if (errors > 0)
    { printf("\n\nAssembly terminated with %d error",errors); if (errors > 1) printf("s");
      sprintf(console+strlen(console),"\n\nAssembly terminated with %d error",errors); if (errors > 1) strcat(console, "s");
        
      programassembled = FALSE;
      *numsystems = 0;
    }
    else // parse the input
    { *numsystems = sn;
      programassembled = assemble(s, numsystems, functions, *fn, labels, ln, sys, &errors);
    }
    free(line);
    return programassembled;
}

void saveprogram(char *fname, char *fname2, systemic *s, int numsystems, symbol *functions, int fn,int **scopetable, chainlink **chain, int cnum)
// saves the current state of the program as a compiled scp file
{   int n,m,i;
    BOOL chainended;
    FILE *fptr;

    if ((fptr = fopen(fname,"w+")) == NULL) error(&n,fptr,fname,"unable to open output file",0,TRUE);
    fprintf(fptr,"// systemic computation code autogenerated from %s\n\n",fname2);
    fprintf(fptr,"// number of functions\n%d\n\n",fn);
    fprintf(fptr,"// number of systems\n%d\n\n",numsystems);
    fprintf(fptr,"// scope table\n");

    // write the scopes (compact scope table)
    for (n = 0; n < numsystems; n++)
       for (m = 0; m < numsystems; m++)
            if (scopetable[n][m]) fprintf(fptr,"%d(%d\n",n,m);
    fprintf(fptr,")\n");
    
    fprintf(fptr,"\n// function definitions");

    // write the functions
    for (i = 0; i < fn; i++)
    {   fprintf(fptr,"\n%s ",functions[i].name);
        for (n = 0; n < functionsize; n++)
        { if (functions[i].schema[n] < 2) fprintf(fptr,"%d",functions[i].schema[n]);
          else fprintf(fptr,"%c",functions[i].schema[n]);
        }
    }    

    fprintf(fptr,"\n\n// system definitions");
    // now write the systems:
    for (i = 0; i < numsystems; i++)
    {   fprintf(fptr,"\n");
        for (n = 0; n < schemasize; n++)
        { if (s[i].schema1[n] < 2) fprintf(fptr,"%d",s[i].schema1[n]);
          else fprintf(fptr,"%c",s[i].schema1[n]);
        }
        fprintf(fptr," ");
        for (n = 0; n < functionsize; n++)
        {   if (s[i].function[n] < 2) fprintf(fptr,"%d",s[i].function[n]);
            else fprintf(fptr,"%c",s[i].function[n]);
        }
        fprintf(fptr," ");
        for (n = 0; n < schemasize; n++)
        { if (s[i].schema2[n] < 2) fprintf(fptr,"%d",s[i].schema2[n]);
          else fprintf(fptr,"%c",s[i].schema2[n]);
        } 
    }
    fprintf(fptr,"\n\n// chain definitions");
    // now write the chains:
    for (i = 0; i < cnum; i++)
    {   fprintf(fptr,"\n");
        chainended = FALSE;
        for (n = 0; !chainended && n < iniMAXSYSTEMS; n++)
        {   fprintf(fptr,"%d}%d{%d",chain[i][n].sch1,chain[i][n].systemnum,chain[i][n].sch2);
            if (n+1 < iniMAXSYSTEMS && chain[i][n+1].systemnum > 0) fprintf(fptr,"+");
            else chainended = TRUE;
        }
    }
    
    fclose(fptr);
    printf("\nSCP Code saved.");
}

void namenumber(char *input, char *name, int *number)
// extracts name and number from input where input is of the form "name12"; number is NOTNUM if it cannot be found;
// assumes name[] is big enough
{
    int n;
    
    // get name
    for (n = 0; input[n] > '9' && input[n] != '\0'; n++)
        name[n] = input[n];
    name[n] = '\0';
    // get number
    *number = NOTNUM;
    sscanf(input+n, "%d", number);
}

void getdecodedsystem(systemic *s, char *sysname, char *sysstring)
// fills sysstring with decoded and human-readable system
{
    int m, t1, t2, nt, f, val;
    char functionname[128], chunk[128];
    char one[schemasize*2], two[functionsize*2], three[schemasize*2];
    char s11[schemasize*2],s12[functionsize*2],s13[schemasize*2];
    char s21[schemasize*2],s22[functionsize*2],s23[schemasize*2];
    BOOL isnumber;
    
    // look up the function name for s[n].function
    f = parse(s->function, &t1, &t2, &nt);
    if (f >= 0 && f < TOTALSYSTEMFUNCTIONS)
    {   strcpy(functionname, systemfunctions[f].name);
        sprintf(two,"%s(%d,%d)",functionname,t1,t2);
        if (nt) strcat(two,"!");
    }
    else
    {   schematoi(s->function,schemasize,&val);
        sprintf(two, "%%d%d",val);
    }
    //    strcpy(functionname, "?Function");
    
    // how to interpret the schema? look to see if any character is not 1 or 0
    isnumber = TRUE;
    for (m = 0; m < schemasize; m++)
    {   if (s->schema1[m] != 0 && s->schema1[m] != 1) { isnumber = FALSE; break; }
        if (s->schema2[m] != 0 && s->schema2[m] != 1) { isnumber = FALSE; break; }
    }
    
    if (isnumber) // the schema of this system can be decoded as numbers
    {
        schematoi(s->schema1,schemasize,&val);
        sprintf(one, "%%d%d",val);
        
        schematoi(s->schema2,schemasize,&val);
        sprintf(three, "%%d%d",val);
        
        sprintf(sysstring,"%s (%s %s %s)", sysname, one, two, three);
    }
    else // otherwise this was being used for pattern matching, so we'll decode into three sections
    {
        decode_to_ascii(s->schema1,schemasize,s11,s12,s13);
        decode_to_ascii(s->schema2,schemasize,s21,s22,s23);
        
        // once decoded, can we use any labels to make it read better? If not, we'll output as raw codes
        sprintf(sysstring,"%s ([", sysname);
        if (!replacewithlabel(s11)) strcat(sysstring,"%%b");
        sprintf(chunk,"%s ",s11); strcat(sysstring, chunk);
        if (!replacewithlabel(s12)) strcat(sysstring,"%%b");
        sprintf(chunk,"%s ",s12); strcat(sysstring, chunk);
        if (!replacewithlabel(s13)) strcat(sysstring,"%%b");
        sprintf(chunk,"%s] %s [",s13,two); strcat(sysstring, chunk);
        
        if (!replacewithlabel(s21)) strcat(sysstring,"%%b");
        sprintf(chunk,"%s ",s21); strcat(sysstring, chunk);
        if (!replacewithlabel(s22)) strcat(sysstring,"%%b");
        sprintf(chunk,"%s ",s22); strcat(sysstring, chunk);
        if (!replacewithlabel(s23)) strcat(sysstring,"%%b");
        sprintf(chunk,"%s])",s23); strcat(sysstring, chunk);
    }
}

void savescprogram(char *outfname, char *scfname, systemic *s, int numsystems, symbol *functions, int fn,int **scopetable, chainlink **chain, int cnum)
// decompiler: saves the current state of the program as a human-readable decompiled .sc
{   int n,m, err, l;
    char *line;
    char one[schemasize*2];
    BOOL fnlabldone, chaindone, chaining, isscope, isarray;
    char **arrayelement;
    char **arrayelementbody;
    int arraycount,*arrayindex;
    FILE *fptr1,*fptr2;
    
    // make some memory available and reset contents (use heap as this may be too much for the stack)
    line = malloc(MAXLLEN * sizeof(char)); _memcheck(line, "line");
    arrayindex = malloc(iniMAXSYSTEMS*sizeof(int)); _memcheck(arrayindex,"arrayindex"); memset(arrayindex, 0, iniMAXSYSTEMS*sizeof(int));
    arrayelement = malloc(iniMAXSYSTEMS*sizeof(char *)); _memcheck(arrayelement,"arrayelement");
    for (n = 0; n < iniMAXSYSTEMS; n++) { arrayelement[n] = malloc(MAXNAME*sizeof(char)); _memcheck(arrayelement[n],"arrayelement row"); memset(arrayelement[n], 0, MAXNAME); }
    arrayelementbody = malloc(iniMAXSYSTEMS*sizeof(char *)); _memcheck(arrayelementbody,"arrayelementbody");
    for (n = 0; n < iniMAXSYSTEMS; n++) { arrayelementbody[n] = malloc(MAXNAME*sizeof(char)); _memcheck(arrayelementbody[n],"arrayelementbody row"); memset(arrayelementbody[n], 0, MAXNAME); }
    
    if (!iniCOMPILESC) printf("\nUnable to produce decompiled output because code was not compiled before running. Change iniCOMPILESC setting to 1 in ini file.\n");
    else
    {
        err = 0;
        if ((fptr1 = fopen(scfname,"r")) == NULL) error(&err,fptr1,scfname,"unable to open input file",0,TRUE);
        if ((fptr2 = fopen(outfname,"w+")) == NULL) error(&err,fptr2,outfname,"unable to open output file",0,TRUE);
        
        // the decompiling begins - first, the function and label definitions can be copied unchanged from the original source
        fnlabldone = FALSE;
        
        l = 0;
        fprintf(fptr2,"// Auto-generated output from decompiler.\n// Original source: %s\n\n", scfname);
        while (!fnlabldone)
        {
            if (fgets(line,MAXLLEN,fptr1) == NULL) error(&err,fptr1,scfname,"premature end of sc source file found during decompilation",l,TRUE);
            l++;
            sscanf(line,"%s",one);
            if (one[0] != '#' && one[0] != '/' && one[0] != ' ' && one[0] != '\n')
            {   // found the first system definition
                fnlabldone = TRUE;
            }
            else
            {
                fprintf(fptr2,"%s",line);
            }
        }
        // make list of system names
        arraycount = 0;
        for (m = 0; m < numsystems; m++)
        {   getdecodedsystem(s+m,"\0"/*sys[m].name*/,arrayelementbody[m]);
            
            namenumber(sys[m].name, arrayelement[arraycount],arrayindex+arraycount);
            //printf("--> %s %d\n", arrayelement[arraycount].name,arrayindex[arraycount]);
            arraycount++;
            if (arraycount > iniMAXSYSTEMS) { PRINTF("\nError - too many systems. Output file will be incorrect."); arraycount = iniMAXSYSTEMS-1; }
        }
        // we've made a list of all the systems so now let's write them, creating arrays on the fly
        isarray = FALSE;
        for (m = 1; m < arraycount; m++)
        {   if (strcmp(arrayelement[m-1], arrayelement[m]) == 0 &&
                strcmp(arrayelementbody[m-1], arrayelementbody[m]) == 0 &&
                arrayindex[m-1]+1 == arrayindex[m]) // same name, system body, and next index so we can make an array
            {   if (!isarray) { fprintf(fptr2,"[%d", arrayindex[m-1]); isarray = TRUE; } // only write out the start of the array once
            }
            else
            {   if (isarray) { fprintf(fptr2,":%d]%s%s\n", arrayindex[m-1],arrayelement[m-1],arrayelementbody[m-1]); isarray = FALSE; } // write the end of the array and the name
                else if (arrayindex[m-1] == NOTNUM) fprintf(fptr2,"%s%s\n", arrayelement[m-1],arrayelementbody[m-1]); // never was an array so just display the previous system
                else fprintf(fptr2,"%s%d%s\n", arrayelement[m-1],arrayindex[m-1],arrayelementbody[m-1]); // is an isolated single system from an array
            }
        }
        // now for the final system
        if (isarray) { fprintf(fptr2,":%d]%s%s\n", arrayindex[m-1],arrayelement[m-1],arrayelementbody[m-1]); isarray = FALSE; }  // write the end of the array and the name
        else if (arrayindex[m-1] == NOTNUM) fprintf(fptr2,"%s%s\n", arrayelement[m-1],arrayelementbody[m-1]); // never was an array so just display the previous system
        else fprintf(fptr2,"%s%d%s\n", arrayelement[m-1],arrayindex[m-1],arrayelementbody[m-1]); // is an isolated single system from an array

        // reset storage:
        for (n = 0; n < arraycount; n++) for (m = 0; m < MAXNAME; m++) arrayelement[n][m] = '\0';
                
        // output human readable chains
        // chains are immutable so we can copy the original chains straight from the source file and preserve variable names
        fprintf(fptr2,"\n// set up any chains\n");
        chaindone = chaining = FALSE;
        while (!chaindone)
        {
            if (fgets(line,MAXLLEN,fptr1) == NULL) { chaindone = TRUE; break; }
            l++;
            sscanf(line,"%s",one);
            if (strcmp(one, "#chain") == 0) chaining = TRUE;
            if (chaining)
            {
                fprintf(fptr2, "%s", line);
            }
            if (chaining && strcmp(one, "}") == 0) { fprintf(fptr2, "\n"); chaining = FALSE; }
        }
        
        // output human readable scopes:
        fprintf(fptr2,"\n// set up the scopes\n");
        for (n = 0; n < numsystems; n++)
        {   isscope = FALSE;
            arraycount = 0;
            for (m = 0; m < numsystems; m++) if (scopetable[n][m]) { isscope = TRUE; break; }

            if (isscope)
            {   fprintf(fptr2,"#scope %s\n{\n", sys[n].name);
                // make list of systems in this scope
                for (m = 0; m < numsystems; m++)
                    if (scopetable[n][m])
                    {   namenumber(sys[m].name, arrayelement[arraycount],arrayindex+arraycount);
                        //printf("--> %s %d\n", arrayelement[arraycount].name,arrayindex[arraycount]);
                        arraycount++;
                        if (arraycount > iniMAXSYSTEMS) { PRINTF("\nError - too many scopes. Output file will be incorrect."); arraycount = iniMAXSYSTEMS-1; }
                    }
                // we've made a list of all the systems in scope n so now let's write them, creating arrays on the fly
                isarray = FALSE;
                for (m = 1; m < arraycount; m++)
                {   if (strcmp(arrayelement[m-1], arrayelement[m]) == 0 && arrayindex[m-1]+1 == arrayindex[m]) // same name and next index so we can make an array
                    {   if (!isarray) { fprintf(fptr2,"      [%d", arrayindex[m-1]); isarray = TRUE; } // only write out the start of the array once
                    }
                    else
                    {   if (isarray) { fprintf(fptr2,":%d]%s\n", arrayindex[m-1],arrayelement[m-1]); isarray = FALSE; } // write the end of the array and the name
                        else if (arrayindex[m-1] == NOTNUM) fprintf(fptr2,"      %s\n", arrayelement[m-1]); // never was an array so just display the previous system
                        else fprintf(fptr2,"      %s%d\n", arrayelement[m-1],arrayindex[m-1]); // is an isolated single system from an array
                    }
                }
                // now for the final system in the scope
                if (isarray) { fprintf(fptr2,":%d]%s\n", arrayindex[m-1],arrayelement[m-1]); isarray = FALSE; }  // write the end of the array and the name
                else if (arrayindex[m-1] == NOTNUM) fprintf(fptr2,"      %s\n", arrayelement[m-1]); // never was an array so just display the previous system
                else fprintf(fptr2,"      %s%d\n", arrayelement[m-1],arrayindex[m-1]); // is an isolated single system from an array

                // end of this scope
                fprintf(fptr2,"}\n\n");
            }
        }
        
        // finish the sc file
        fprintf(fptr2,"#systemic end");
        
        fclose(fptr1);
        fclose(fptr2);
 
        // free the memory
        free(line);
        free(arrayindex);
        if (arrayelement)
        {   for (n = 0; n < iniMAXSYSTEMS; n++) if (arrayelement[n]) free(arrayelement[n]);
            free(arrayelement);
        }
        if (arrayelementbody)
        {   for (n = 0; n < iniMAXSYSTEMS; n++) if (arrayelementbody[n]) free(arrayelementbody[n]);
            free(arrayelementbody);
        }

        printf("\nSC Code saved.");
    }
}