/**** BSIM4.0.0, Released by Weidong Liu 3/24/2000 ****/

/**********
 * Copyright 2000 Regents of the University of California. All rights reserved.
 * File: b4mdel.c of BSIM4.0.0.
 * Authors: Weidong Liu, Kanyu M. Cao, Xiaodong Jin, Chenming Hu.
 * Project Director: Prof. Chenming Hu.
 **********/

#include "ngspice.h"
#include <stdio.h>
#include "bsim4def.h"
#include "sperror.h"

int
BSIM4mDelete(inModel,modname,kill)
GENmodel **inModel;
IFuid modname;
GENmodel *kill;
{
BSIM4model **model = (BSIM4model**)inModel;
BSIM4model *modfast = (BSIM4model*)kill;
BSIM4instance *here;
BSIM4instance *prev = NULL;
BSIM4model **oldmod;

    oldmod = model;
    for (; *model ; model = &((*model)->BSIM4nextModel)) 
    {    if ((*model)->BSIM4modName == modname || 
             (modfast && *model == modfast))
	     goto delgot;
         oldmod = model;
    }
    return(E_NOMOD);

delgot:
    *oldmod = (*model)->BSIM4nextModel; /* cut deleted device out of list */
    for (here = (*model)->BSIM4instances; here; here = here->BSIM4nextInstance)
    {    if(prev) FREE(prev);
         prev = here;
    }
    if(prev) FREE(prev);
    FREE(*model);
    return(OK);
}
