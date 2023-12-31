/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/
/*
 */

/* loop through all the devices and 
 * allocate parameter #s to design parameters 
 */

#include "ngspice.h"
#include <stdio.h>
#include "cktdefs.h"
#include "capdefs.h"
#include "trandefs.h"
#include "sperror.h"
#include "suffix.h"


int
CAPsSetup(info,inModel)
    register SENstruct *info;
    GENmodel *inModel;

{
    register CAPmodel *model = (CAPmodel*)inModel;
    register CAPinstance *here;

    /*  loop through all the capacitor models */
    for( ; model != NULL; model = model->CAPnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->CAPinstances; here != NULL ;
                here=here->CAPnextInstance) {
	    if (here->CAPowner != ARCHme) continue;

            if(here->CAPsenParmNo){
                here->CAPsenParmNo = ++(info->SENparms);
            }
        }
    }
    return(OK);
}

