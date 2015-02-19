const char *KernelSource1  =                                             "\n" \
"__kernel void compare(                                                 \n" \
"   __global unsigned char *input1,                                     \n" \
"   __global unsigned char *input2,                                     \n" \
"   __global unsigned int *output,                                      \n" \
"   const int ssize,                                                    \n" \
"   const int bsize)                                                    \n" \
"{                                                                      \n" \
"   int i = get_global_id(0);                                           \n" \
"   int j = i * ssize;                                                  \n" \
"   unsigned int ctr;                                                   \n" \
"                                                                       \n" \
"   if (j < bsize)                                                      \n" \
"   {   ctr = 0;                                                        \n" \
"       for (int n = 0; n < ssize; n++)                                 \n" \
"       {                                                               \n" \
"            if (input2[n] < 2)                                         \n" \
"            {	if (input1[j+n] > 32)                                   \n" \
"               {                                                       \n" \
"                    ctr++;                                             \n" \
"               }                                                       \n" \
"            else if (input1[j+n] != input2[n])                         \n" \
"               {                                                       \n" \
"                    ctr++;                                             \n" \
"               }                                                       \n" \
"            }                                                          \n" \
"       }                                                               \n" \
"       output[i] = ctr;                                                \n" \
"    }                                                                  \n" \
"}                                                                      \n" \
"\n";



const char *KernelSource2  =                                             "\n" \
"__kernel void compare(                                                 \n" \
"   __global unsigned char *input1,                                     \n" \
"   __global unsigned char *input2,                                     \n" \
"   __global unsigned int *output,                                      \n" \
"   __global int *nummatches,                                           \n" \
"   const int t,                                                        \n" \
"   const int not,                                                      \n" \
"   const int ssize)                                                    \n" \
"{                                                                      \n" \
"   int i = get_global_id(0);                                           \n" \
"   int j = i * ssize;                                                \n" \
"                                                                       \n" \
"                                                                       \n" \
"   output[i] = 0;                                                      \n" \
"   for (int n = 0; n < ssize; n++)                                     \n" \
"   {                                                                   \n" \
"        if (input2[n] < 2)                                             \n" \
"        {	if (input1[j+n] > 32)                                       \n" \
"            {                                                          \n" \
"                (output[i])++;                                         \n" \
"            }                                                          \n" \
"        else if (input1[j+n] != input2[n])                             \n" \
"            {                                                          \n" \
"                (output[i])++;                                         \n" \
"            }                                                          \n" \
"        }                                                              \n" \
"    }                                                                  \n" \
"    if ((not == 0) ? (output[i] <= t) : !(output[i] <= t)) (*nummatches)++; \n" \
"                                                                       \n" \
"}                                                                      \n" \
"\n";






BOOL schema_match(unsigned char *schbuffer, unsigned char *schemata1,int schsize, int *t, int *not, int *sd)
// returns TRUE if the first schsize bytes in schbuffer match schemata1, otherwise FALSE
{
    // compare schbuffer with schemata1 - we have a match if the right bit is present and the same
    *sd = 0;
    for (int n = 0; n < schsize; n++)
    {
        // no problem when sch[n] is a wildcard, but if it's a binary value then compare:
        if (schemata1[n] < 2) // it's a binary value
        {	if (schbuffer[n] > 32)
            {
                (*sd)++;// compared to a code - printable ascii character
            }
            else if (schbuffer[n] != schemata1[n])
            {
                (*sd)++; // compared to another binary value
            }
        }
        
    }

    return ((*not == 0) ? (*sd <= *t) : !(*sd <= *t));
}

BOOL gotthreesystems3(systemic *s, int **scopetable, int *context, int *snum1, int *snum2, int *whichscope, int* t1, int *t2, int *not, int *fn, int *dnum1, int *dnum2)
// finds three random systems in a random scope; returns FALSE if unsuccessful (might not be enough systems in a scope)
// OpenCL version without valid triplet queue
{   int giveup,flr,tmp;
    int scp,randomsys,i;
    int schsize = schematasize+functionsize+schematasize;
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
        // parse the function to be used to transform the two systems
        *fn = parse(s[*context].function, t1, t2, not);

        // get the schema of the context
        unsigned char schemata1[schsize];
        unsigned char schemata2[schsize];
        decode_to_tertiary(s[*context].schemata1, schemata1);
        decode_to_tertiary(s[*context].schemata2, schemata2);
        // make randomised buffer of systems in the current scope ready for searching
        // Note that schsize should be a multiple of 16, 32, or 64 bytes for optimal performance
        int numtrials = scopesummary[scp];
        if ((numtrials % CLWKGRPSIZE) != 0)
        {   numtrials /= CLWKGRPSIZE; numtrials++; numtrials *= CLWKGRPSIZE; }
        int bufsize = numtrials*schsize;
        unsigned char schbuffer[bufsize];
        int bufindex[numtrials];
        
        randomizecycle(0);   // re-intialise randomcycle for decent randomness
        for (i = 0; i < bufsize; i += schsize)
        {   randomsys = scopelist[scp][randomcycle(0,scopesummary[scp])];
            if (randomsys != *context)
            {   bufindex[i/schsize] = randomsys;
                // fill schbuffer with s
                memccpy(schbuffer+i, s[randomsys].schemata1, schematasize, sizeof(unsigned char));
                memccpy(schbuffer+i+schematasize, s[randomsys].function, functionsize, sizeof(unsigned char));
                memccpy(schbuffer+i+schematasize+functionsize, s[randomsys].schemata2, schematasize, sizeof(unsigned char));
            }
            else i -= schsize; // we miss out the context in the buffer to prevent it matching itself
        }
        
        // find matching systems for the two schema
        unsigned int out1[numtrials];
        unsigned int out2[numtrials];
        
        cl_findmatches(0,schbuffer,schemata1,schsize,bufsize,out1);
        cl_findmatches(1,schbuffer,schemata2,schsize,bufsize,out2);
        //clFinish(cl[0].queue);
        //clFinish(cl[1].queue);
        // Read back the results from the devices to verify the output
        int err = clEnqueueReadBuffer(cl[0].queue, cl[0].output, CL_TRUE, 0, sizeof(unsigned int) * numtrials, out1, 0, NULL, NULL );
        err |= clEnqueueReadBuffer(cl[1].queue, cl[1].output, CL_TRUE, 0, sizeof(unsigned int) * numtrials, out2, 0, NULL, NULL );
        if (err != CL_SUCCESS)
        {
            printf("Fatal Error %d: Failed to read output array\n", err);
            exit(1);
        }

        // now check to find a mututally compatible pair, if they exist
        for (i = 0; i < numtrials; i++)
        {   if (_smatchedok(out1[i],*t1,*not))
            {   *snum1 = bufindex[i];
                *dnum1 = out1[i];
                snum1ok = TRUE;
                break;
            }
        }
        for (i = 0; i < numtrials; i++)
        {   if (_smatchedok(out2[i],*t2,*not) && i != *snum1)
            {   *snum2 = bufindex[i];
                *dnum2 = out2[i];
                snum2ok = TRUE;
                break;
            }
        }
        
        // optimisation - temporarily remove context that matches nothing from the list
        if (!snum1ok || !snum2ok)
        {   swap(functionlist[scp][flr],functionlist[scp][fnum[scp]],tmp);
            fnum[scp]--; flistchanged[scp] = TRUE;
        }
        else
        {
            // undo optimsation - restore all functions in this scope to the lists
            if (flistchanged[scp] && snum1ok && snum2ok) updatefunctionlist(s,scp);
        }
    }
    
    return (snum1ok && snum2ok);
}


BOOL gotthreesystems1(systemic *s, int **scopetable, int *context, int *snum1, int *snum2, int *whichscope, int* t1, int *t2, int *not, int *fn, int *dnum1, int *dnum2)
// finds three random systems in a random scope; returns FALSE if unsuccessful (might not be enough systems in a scope)
// sequential version using OpenCL compatible buffers
{   int giveup,flr,tmp;
    int sd,scp,randomsys,i;
    int schsize = schematasize+functionsize+schematasize;
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
        // parse the function to be used to transform the two systems
        *fn = parse(s[*context].function, t1, t2, not);
        
        // get the schema of the context
        unsigned char schemata1[schsize];
        unsigned char schemata2[schsize];
        decode_to_tertiary(s[*context].schemata1, schemata1);
        decode_to_tertiary(s[*context].schemata2, schemata2);
        // make randomised buffer of systems in the current scope ready for searching
        // Note that schsize should be a multiple of 16, 32, or 64 bytes for optimal performance
        long bufsize = scopesummary[scp]*schsize;
        unsigned char schbuffer[bufsize];
        int bufindex[scopesummary[scp]];
        
        randomizecycle(0);   // re-intialise randomcycle for decent randomness
        for (i = 0; i < bufsize; i += schsize)
        {   randomsys = scopelist[scp][randomcycle(0,scopesummary[scp])];
            if (randomsys != *context)
            {   bufindex[i/schsize] = randomsys;
                // fill schbuffer with s
                memccpy(schbuffer+i, s[randomsys].schemata1, schematasize, sizeof(unsigned char));
                memccpy(schbuffer+i+schematasize, s[randomsys].function, functionsize, sizeof(unsigned char));
                memccpy(schbuffer+i+schematasize+functionsize, s[randomsys].schemata2, schematasize, sizeof(unsigned char));
            }
            else i -= schsize; // we miss out the context in the buffer to prevent it matching itself
        }
        
        // find matching systems for the two schema
        unsigned char out1[scopesummary[scp]];
        unsigned char out2[scopesummary[scp]];
        int nummatches1,nummatches2;
        
        nummatches1 = nummatches2 = 0;
        // loop through schbuffer to find match for first schemata
        for (i = 0; i < bufsize; i += schsize)
        {
            if (schema_match(schbuffer+i,schemata1,schsize,t1,not,&sd)) nummatches1++;
            out1[i/schsize] = sd;
        }
        // loop through schbuffer to find match for second schemata
        for (i = 0; i < bufsize; i += schsize)
        {
            if (schema_match(schbuffer+i,schemata2,schsize,t2,not,&sd)) nummatches2++;
            out2[i/schsize] = sd;
        }
        // now check to find a mututally compatible pair, if they exist
        if (nummatches1 > 0 && nummatches2 > 0)
        {   if (nummatches1 >= nummatches2) // we have more matches for schemata1 so we choose one for schemata2 first
        {
            for (i = 0; i < scopesummary[scp]; i++)
            {   if (_smatchedok(out2[i],*t2,*not))
            {   *snum2 = bufindex[i];
                *dnum2 = out2[i];
                snum2ok = TRUE;
                break;
            }
            }
            for (i = 0; i < scopesummary[scp]; i++)
            {   if (_smatchedok(out1[i],*t1,*not) && i != *snum2)
            {   *snum1 = bufindex[i];
                *dnum1 = out1[i];
                snum1ok = TRUE;
                break;
            }
            }
        }
        else // we have more matches for schemata2 so we choose one for schemata1 first
        {
            for (i = 0; i < scopesummary[scp]; i++)
            {   if (_smatchedok(out1[i],*t1,*not))
            {   *snum1 = bufindex[i];
                *dnum1 = out1[i];
                snum1ok = TRUE;
                break;
            }
            }
            for (i = 0; i < scopesummary[scp]; i++)
            {   if (_smatchedok(out2[i],*t2,*not) && i != *snum1)
            {   *snum2 = bufindex[i];
                *dnum2 = out2[i];
                snum2ok = TRUE;
                break;
            }
            }
        }
        }
        
        // optimisation - temporarily remove context that matches nothing from the list
        if (!snum1ok || !snum2ok)
        {   swap(functionlist[scp][flr],functionlist[scp][fnum[scp]],tmp);
            fnum[scp]--; flistchanged[scp] = TRUE;
        }
        else
        {
            // undo optimsation - restore all functions in this scope to the lists
            if (flistchanged[scp] && snum1ok && snum2ok) updatefunctionlist(s,scp);
        }
    }
    
    return (snum1ok && snum2ok);
}