// we point ischained to the first location in chain[][] that points to this system
// this way we know this system is the head of a chain and we know where to start looking for its chain(s)
// we also make sure we only set the value for the first in the list of chains"

//ret = transform(fn,dnum1,dnum2,t1,t2,not,s,snum1,snum2,whichscope,context,scopetable);
//            // if the system is chained, start following the chosen chain
//            if (s[context].ischained != NOTCHAINED)
//                do
//                {   // find the chain for s[context] in chain[][] - we stay on this chain until done
//                    chnum = s[context].ischained+ret;
//                    // if there is a chain:
//                    if (chnum != NOTCHAINED)
//                    {   // check that the first link matches the system we just executed
//                        if (chain[chnum][0].systemnum != context) { printf("\nError: Chain %d for system %d not found. Chain broken.//",ret,context); break; }
//                        // otherwise the second link in that chain is the next context to execute
//                        context = chain[chnum][1].systemnum;
//                        // follow the chain, passing the two systems from the first link of the chain to be stored on the stack
//                        ret = followchain(ret,chain,chnum,chainnum,s,scopetable,&context,&snum1,&snum2,whichscope,&iterations);
//                    }
//                    else
//                    {   ret = CHAIN_BREAK;//