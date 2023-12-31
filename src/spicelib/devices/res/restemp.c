/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
Modified Apr 2000 - Paolo Nenzi 
**********/

#include "ngspice.h"
#include <stdio.h>
#include "cktdefs.h"
#include "resdefs.h"
#include "sperror.h"


int
REStemp(GENmodel *inModel, CKTcircuit *ckt)
        /* perform the temperature update to the resistors
         * calculate the conductance as a function of the
         * given nominal and current temperatures - the
         * resistance given in the struct is the nominal
         * temperature resistance
         */
{
    RESmodel *model =  (RESmodel *)inModel;
    RESinstance *here;
    double factor;
    double difference;


    /*  loop through all the resistor models */
    for( ; model != NULL; model = model->RESnextModel ) {

        /* Default Value Processing for Resistor Models */
        if(!model->REStnomGiven) model->REStnom = ckt->CKTnomTemp;
        if(!model->RESsheetResGiven) model->RESsheetRes = 0;
        if(!model->RESdefWidthGiven) model->RESdefWidth = 10.e-6; /*M*/
        if(!model->REStc1Given) model->REStempCoeff1 = 0;
        if(!model->REStc2Given) model->REStempCoeff2 = 0;
        if(!model->RESnarrowGiven) model->RESnarrow = 0;

        /* loop through all the instances of the model */
        for (here = model->RESinstances; here != NULL ;
                here=here->RESnextInstance) {
	    if (here->RESowner != ARCHme) continue;
            
            /* Default Value Processing for Resistor Instance */
            if(!here->REStempGiven) here->REStemp = ckt->CKTtemp;
            if(!here->RESwidthGiven) here->RESwidth = model->RESdefWidth;
            if(!here->RESlengthGiven)  here->RESlength = 0 ;
            if(!here->RESresGiven)  {
                if(model->RESsheetResGiven && (model->RESsheetRes != 0) &&
                        (here->RESlength != 0)) {
                    here->RESresist = model->RESsheetRes * (here->RESlength -
                        model->RESnarrow) / (here->RESwidth - model->RESnarrow);
                } else {
                    (*(SPfrontEnd->IFerror))(ERR_WARNING,
                            "%s: resistance=0, set to 1000",&(here->RESname));
                    here->RESresist=1000;
                }
            }

            difference = here->REStemp - model->REStnom;
            factor = 1.0 + (model->REStempCoeff1)*difference + 
                    (model->REStempCoeff2)*difference*difference;

            here->RESconduct = 1.0/(here->RESresist * factor);
	    
	    /* Paolo Nenzi: Temperature effects for AC value */
	    if(here->RESacresGiven) 
	       here->RESacConduct = 1.0/(here->RESacResist * factor);
        }
    }
    return(OK);
}
