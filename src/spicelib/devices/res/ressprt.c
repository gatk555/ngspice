/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
Modified: Apr 2000 - Paolo Nenzi
**********/

/* Pretty print the sensitivity info for all 
 * the resistors in the circuit.
 */

#include "ngspice.h"
#include <stdio.h>
#include "cktdefs.h"
#include "resdefs.h"
#include "sperror.h"


void
RESsPrint(GENmodel *inModel, CKTcircuit *ckt)
{
    RESmodel *model = (RESmodel *)inModel;
    RESinstance *here;
    printf("RESISTORS-----------------\n");

    /*  loop through all the resistor models */
    for( ; model != NULL; model = model->RESnextModel ) {

        printf("Model name:%s\n",model->RESmodName);

        /* loop through all the instances of the model */
        for (here = model->RESinstances; here != NULL ;
                here=here->RESnextInstance) {
	    if (here->RESowner != ARCHme) continue;

            printf("    Instance name:%s\n",here->RESname);
            printf("      Positive, negative nodes: %s, %s\n",
            CKTnodName(ckt,here->RESposNode),CKTnodName(ckt,here->RESnegNode));
            printf("      Resistance: %f ",here->RESresist);
            printf(here->RESresGiven ? "(specified)\n" : "(default)\n");
            printf("    RESsenParmNo:%d\n",here->RESsenParmNo);

        }
    }
}
