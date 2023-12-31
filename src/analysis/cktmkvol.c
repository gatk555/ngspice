/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/

/* CKTmkVolt
 *  make the given name a 'node' of type current in the 
 * specified circuit
 */

#include "ngspice.h"
#include <stdio.h>
#include "ifsim.h"
#include "sperror.h"
#include "cktdefs.h"



/* ARGSUSED */
int
CKTmkVolt(CKTcircuit *ckt, CKTnode **node, IFuid basename, char *suffix)
{
    IFuid uid;
    int error;
    CKTnode *mynode;
    CKTnode *checknode;

    error = CKTmkNode(ckt,&mynode);
    if(error) return(error);
    checknode = mynode;
    error = (*(SPfrontEnd->IFnewUid))((void *)ckt,&uid,basename,
            suffix,UID_SIGNAL,(void**)&checknode);
    if(error) {
        FREE(mynode);
        if(node) *node = checknode;
        return(error);
    }
    mynode->name = uid;
    mynode->type = SP_VOLTAGE;
    if(node) *node = mynode;
    error = CKTlinkEq(ckt,mynode);
    return(error);
}
