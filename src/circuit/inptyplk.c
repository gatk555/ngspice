/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/

/*  look up the 'type' in the device description struct and return the
 *  appropriatestrchr for the device found, or -1 for not found 
 */

#include "ngspice.h"
#include "inpdefs.h"
#include "cpdefs.h"
#include "fteext.h"
#include "ifsim.h"
#include "inp.h"

int
INPtypelook(char *type)
{

    int i;
    for(i=0;i<ft_sim->numDevices;i++) {
        if(strcmp(type,(*(ft_sim->devices)[i]).name)==0) {
            /*found the device - return it */
            return(i);
        }
    }
    return(-1);
}

