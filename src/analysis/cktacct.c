/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/
/*
 */

    /*
     *  CKTacct
     *  get the specified accounting item into 'value' in the 
     *  given circuit 'ckt'.
     */

#include "ngspice.h"
#include "const.h"
#include "optdefs.h"
#include "ifsim.h"
#include <stdio.h>
#include "cktdefs.h"
#include "spmatrix.h"



/* ARGSUSED */
int
CKTacct(CKTcircuit *ckt, void *anal, int which, IFvalue *val)
{
    switch(which) {
        
    case OPT_EQNS:
        val->iValue = ckt->CKTmaxEqNum;
        break;
    case OPT_ORIGNZ:
	if ( ckt->CKTmatrix != NULL ) {
	    val->iValue = spOriginalCount((char *)ckt->CKTmatrix);
	} else {
	    val->iValue = 0;
	}
        break;
    case OPT_FILLNZ:
	if ( ckt->CKTmatrix != NULL ) {
	    val->iValue = spFillinCount((char *)ckt->CKTmatrix);
	} else {
	    val->iValue = 0;
	}
        break;
    case OPT_TOTALNZ:
	if ( ckt->CKTmatrix != NULL ) {
	    val->iValue = spElementCount((char *)ckt->CKTmatrix);
	} else {
	    val->iValue = 0;
	}
        break;
    case OPT_ITERS:
        val->iValue = ckt->CKTstat->STATnumIter;
        break;
    case OPT_TRANIT:
        val->iValue = ckt->CKTstat->STATtranIter;
        break;
    case OPT_TRANCURITER:
        val->iValue = ckt->CKTstat->STATnumIter - ckt->CKTstat->STAToldIter;
        break;
    case OPT_TRANPTS:
        val->iValue = ckt->CKTstat->STATtimePts;
        break;
    case OPT_TRANACCPT:
        val->iValue = ckt->CKTstat->STATaccepted;
        break;
    case OPT_TRANRJCT:
        val->iValue = ckt->CKTstat->STATrejected;
        break;
    case OPT_TOTANALTIME:
        val->rValue = ckt->CKTstat->STATtotAnalTime;
        break;
    case OPT_TRANTIME:
        val->rValue = ckt->CKTstat->STATtranTime;
        break;
    case OPT_ACTIME:
        val->rValue = ckt->CKTstat->STATacTime;
        break;
    case OPT_LOADTIME:
        val->rValue = ckt->CKTstat->STATloadTime;
        break;
    case OPT_SYNCTIME:
        val->rValue = ckt->CKTstat->STATsyncTime;
        break;
    case OPT_COMBTIME:
        val->rValue = ckt->CKTstat->STATcombineTime;
        break;
    case OPT_REORDTIME:
        val->rValue = ckt->CKTstat->STATreorderTime;
        break;
    case OPT_DECOMP:
        val->rValue = ckt->CKTstat->STATdecompTime;
        break;
    case OPT_SOLVE:
        val->rValue = ckt->CKTstat->STATsolveTime;
        break;
    case OPT_TRANLOAD:
        val->rValue = ckt->CKTstat->STATtranLoadTime;
        break;
    case OPT_TRANSYNC:
        val->rValue = ckt->CKTstat->STATtranSyncTime;
        break;
    case OPT_TRANCOMB:
        val->rValue = ckt->CKTstat->STATtranCombTime;
        break;
    case OPT_TRANDECOMP:
        val->rValue = ckt->CKTstat->STATtranDecompTime;
        break;
    case OPT_TRANSOLVE:
        val->rValue = ckt->CKTstat->STATtranSolveTime;
        break;
    case OPT_TRANTRUNC:
        val->rValue = ckt->CKTstat->STATtranTruncTime;
        break;
    case OPT_ACLOAD:
        val->rValue = ckt->CKTstat->STATacLoadTime;
        break;
    case OPT_ACSYNC:
        val->rValue = ckt->CKTstat->STATacSyncTime;
        break;
    case OPT_ACCOMB:
        val->rValue = ckt->CKTstat->STATacCombTime;
        break;
    case OPT_ACDECOMP:
        val->rValue = ckt->CKTstat->STATacDecompTime;
        break;
    case OPT_ACSOLVE:
        val->rValue = ckt->CKTstat->STATacSolveTime;
        break;
    case OPT_TEMP:
        val->rValue = ckt->CKTtemp - CONSTCtoK;
        break;
    case OPT_TNOM:
        val->rValue = ckt->CKTnomTemp - CONSTCtoK;
        break;
    default:
        return(-1);
    }
    return(0);
}
