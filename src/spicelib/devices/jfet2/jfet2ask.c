/**********
Based on jfetask.c
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1987 Mathew Lew and Thomas L. Quarles

Modified to add PS model and new parameter definitions ( Anthony E. Parker )
   Copyright 1994  Macquarie University, Sydney Australia.
   10 Feb 1994:   JFET2vtrap and JFET2pave added
**********/

#include "ngspice.h"
#include <stdio.h>
#include "const.h"
#include "ifsim.h"
#include "cktdefs.h"
#include "devdefs.h"
#include "jfet2defs.h"
#include "sperror.h"
#include "suffix.h"


/*ARGSUSED*/
int
JFET2ask(ckt,inst,which,value,select)
    CKTcircuit *ckt;
    GENinstance *inst;
    int which;
    IFvalue *value;
    IFvalue *select;
{
    JFET2instance *here = (JFET2instance*)inst;
    static char *msg = "Current and power not available for ac analysis";
    switch(which) {
        case JFET2_TEMP:
            value->rValue = here->JFET2temp-CONSTCtoK;
            return(OK);
        case JFET2_AREA:
            value->rValue = here->JFET2area;
            return(OK);
        case JFET2_IC_VDS:
            value->rValue = here->JFET2icVDS;
            return(OK);
        case JFET2_IC_VGS:
            value->rValue = here->JFET2icVGS;
            return(OK);
        case JFET2_OFF:
            value->iValue = here->JFET2off;
            return(OK);
        case JFET2_DRAINNODE:
            value->iValue = here->JFET2drainNode;
            return(OK);
        case JFET2_GATENODE:
            value->iValue = here->JFET2gateNode;
            return(OK);
        case JFET2_SOURCENODE:
            value->iValue = here->JFET2sourceNode;
            return(OK);
        case JFET2_DRAINPRIMENODE:
            value->iValue = here->JFET2drainPrimeNode;
            return(OK);
        case JFET2_SOURCEPRIMENODE:
            value->iValue = here->JFET2sourcePrimeNode;
            return(OK);
        case JFET2_VGS:
            value->rValue = *(ckt->CKTstate0 + here->JFET2vgs);
            return(OK);
        case JFET2_VGD:
            value->rValue = *(ckt->CKTstate0 + here->JFET2vgd);
            return(OK);
        case JFET2_CG:
            value->rValue = *(ckt->CKTstate0 + here->JFET2cg);
            return(OK);
        case JFET2_CD:
            value->rValue = *(ckt->CKTstate0 + here->JFET2cd);
            return(OK);
        case JFET2_CGD:
            value->rValue = *(ckt->CKTstate0 + here->JFET2cgd);
            return(OK);
        case JFET2_GM:
            value->rValue = *(ckt->CKTstate0 + here->JFET2gm);
            return(OK);
        case JFET2_GDS:
            value->rValue = *(ckt->CKTstate0 + here->JFET2gds);
            return(OK);
        case JFET2_GGS:
            value->rValue = *(ckt->CKTstate0 + here->JFET2ggs);
            return(OK);
        case JFET2_GGD:
            value->rValue = *(ckt->CKTstate0 + here->JFET2ggd);
            return(OK);
        case JFET2_QGS:
            value->rValue = *(ckt->CKTstate0 + here->JFET2qgs);
            return(OK);
        case JFET2_CQGS:
            value->rValue = *(ckt->CKTstate0 + here->JFET2cqgs);
            return(OK);
        case JFET2_QGD:
            value->rValue = *(ckt->CKTstate0 + here->JFET2qgd);
            return(OK);
        case JFET2_CQGD:
            value->rValue = *(ckt->CKTstate0 + here->JFET2cqgd);
            return(OK);
        case JFET2_VTRAP:
            value->rValue = *(ckt->CKTstate0 + here->JFET2vtrap);
            return(OK);
        case JFET2_PAVE:
            value->rValue = *(ckt->CKTstate0 + here->JFET2pave);
            return(OK);
        case JFET2_CS :
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "JFET2ask";
                strcpy(errMsg,msg);
                return(E_ASKCURRENT);
            } else {
                value->rValue = -*(ckt->CKTstate0 + here->JFET2cd);
                value->rValue -= *(ckt->CKTstate0 + here->JFET2cg);
            }
            return(OK);
        case JFET2_POWER :
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "JFET2ask";
                strcpy(errMsg,msg);
                return(E_ASKPOWER);
            } else {
                value->rValue = *(ckt->CKTstate0 + here->JFET2cd) *
                        *(ckt->CKTrhsOld + here->JFET2drainNode);
                value->rValue += *(ckt->CKTstate0 + here->JFET2cg) * 
                        *(ckt->CKTrhsOld + here->JFET2gateNode);
                value->rValue -= (*(ckt->CKTstate0 + here->JFET2cd) +
                        *(ckt->CKTstate0 + here->JFET2cg)) *
                *(ckt->CKTrhsOld + here->JFET2sourceNode);
            }
            return(OK);
        default:
            return(E_BADPARM);
    }
    /* NOTREACHED */
}

