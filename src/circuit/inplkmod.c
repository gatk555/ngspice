/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/
/*
 */

#include "ngspice.h"
#include <stdio.h>
#include "inpdefs.h"
#include "inp.h"


extern INPmodel *modtab;


int
INPlookMod(char *name)
{
    register INPmodel **i;

    for (i = &modtab;*i != (INPmodel *)NULL;i = &((*i)->INPnextModel)) {
        if (strcmp((*i)->INPmodName,name) == 0) {
            /* found the model in question - return TRUE */
            return(1);
        }
    }
    /* didn't find model - return FALSE */
    return(0);
}

