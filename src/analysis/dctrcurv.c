/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
Modified: 1999 Paolo Nenzi
**********/

#include "ngspice.h"
#include <stdio.h>

#include "vsrc/vsrcdefs.h"
#include "isrc/isrcdefs.h"
#include "res/resdefs.h"

#include "cktdefs.h"
#include "const.h"
#include "sperror.h"


int
DCtrCurv(CKTcircuit *ckt, int restart) 
                
                /* forced restart flag */
{
    register TRCV* cv = (TRCV*)ckt->CKTcurJob; /* Where we get the job to do */ 
    int i;
    double *temp;
    int converged;
    int rcode;     
    int vcode;
    int icode;
    int j;
    int error;
    IFuid varUid;
    IFuid *nameList;
    int numNames;
    int firstTime=1;
    static void *plot;

#ifdef WANT_SENSE2
#ifdef SENSDEBUG
    if(ckt->CKTsenInfo && (ckt->CKTsenInfo->SENmode&DCSEN) ){
        printf("\nDC Sensitivity Results\n\n");
        CKTsenPrint(ckt);
    }
#endif /* SENSDEBUG */
#endif


    rcode = CKTtypelook("Resistor");
    vcode = CKTtypelook("Vsource");
    icode = CKTtypelook("Isource");
    if(!restart && cv->TRCVnestState >= 0) {
        /* continuing */
        i = cv->TRCVnestState;
        goto resume;
    }
    ckt->CKTtime = 0;
    ckt->CKTdelta = cv->TRCVvStep[0];
    ckt->CKTmode = (ckt->CKTmode & MODEUIC) | MODEDCTRANCURVE | MODEINITJCT ;
    ckt->CKTorder=1;

    
    /* Save the state of the circuit */
    for(i=0;i<7;i++) {
        ckt->CKTdeltaOld[i]=ckt->CKTdelta;
    }
    
    for(i=0;i<=cv->TRCVnestLevel;i++) {
        if(rcode >= 0) {
            /* resistances are in this version, so use them */
            register RESinstance *here;
            register RESmodel *model;
            for(model = (RESmodel *)ckt->CKThead[rcode];model != NULL;
                    model=model->RESnextModel){
                for(here=model->RESinstances;here!=NULL;
                        here=here->RESnextInstance) {
                    if(here->RESname == cv->TRCVvName[i]) {
                        cv->TRCVvElt[i]   = (GENinstance *)here;
                        cv->TRCVvSave[i]  = here->RESresist;
			cv->TRCVgSave[i]  = here->RESresGiven;
                        cv->TRCVvType[i]  = rcode;
                        here->RESresist   = cv->TRCVvStart[i];
                        here->RESresGiven = 1;
                        printf("** Resistor sweep is highly alpha code\n**Results may not be accurate.\n");
                        goto found;
                    }
                }
            }
        }
        if(vcode >= 0) {
            /* voltage sources are in this version, so use them */
            register VSRCinstance *here;
            register VSRCmodel *model;
            for(model = (VSRCmodel *)ckt->CKThead[vcode];model != NULL;
                    model=model->VSRCnextModel){
                for(here=model->VSRCinstances;here!=NULL;
                        here=here->VSRCnextInstance) {
                    if(here->VSRCname == cv->TRCVvName[i]) {
                        cv->TRCVvElt[i]   = (GENinstance *)here;
                        cv->TRCVvSave[i]  = here->VSRCdcValue;
			cv->TRCVgSave[i]  = here->VSRCdcGiven;
                        cv->TRCVvType[i]  = vcode;
                        here->VSRCdcValue = cv->TRCVvStart[i];
                        here->VSRCdcGiven = 1;
                        goto found;
                    }
                }
            }
        }
        if(icode >= 0 ) {
            /* current sources are in this version, so use them */
            register ISRCinstance *here;
            register ISRCmodel *model;

            for(model= (ISRCmodel *)ckt->CKThead[icode];model != NULL;
                    model=model->ISRCnextModel){
                for(here=model->ISRCinstances;here!=NULL;
                        here=here->ISRCnextInstance) {
                    if(here->ISRCname == cv->TRCVvName[i]) {
                        cv->TRCVvElt[i]   = (GENinstance *)here;
                        cv->TRCVvSave[i]  = here->ISRCdcValue;
			cv->TRCVgSave[i]  = here->ISRCdcGiven;
                        cv->TRCVvType[i]  = icode;
                        here->ISRCdcValue = cv->TRCVvStart[i];
                        here->ISRCdcGiven = 1;
                        goto found;
                    }
                }
            }
        }
	
        if(!strcmp(cv->TRCVvName[i], "temp"))
        {
            cv->TRCVvSave[i]=ckt->CKTtemp; /* Saves the old circuit temperature */
            cv->TRCVvType[i]=TEMP_CODE;    /* Set the sweep type code */
            ckt->CKTtemp = cv->TRCVvStart[i] + CONSTCtoK; /* Set the new circuit temp */
            printf("Temperature sweep is alpha code\nresults may not be accurate\n");
            goto found;
        }
	
        (*(SPfrontEnd->IFerror))(ERR_FATAL, 
                "DCtrCurv: source / resistor %s not in circuit", &(cv->TRCVvName[i]));
        return(E_NODEV);

found:;
    }
    
    i--; /* PN: This seems to do nothing ??? */ 
    
    error = CKTnames(ckt,&numNames,&nameList);
    if(error) return(error);
    
        if (cv->TRCVvType[i]==vcode)
    	(*(SPfrontEnd->IFnewUid))((void *)ckt,&varUid,(IFuid )NULL,
            "v-sweep", UID_OTHER, (void **)NULL);
    else if (cv->TRCVvType[i]==icode)
    	(*(SPfrontEnd->IFnewUid))((void *)ckt,&varUid,(IFuid )NULL,
            "i-sweep", UID_OTHER, (void **)NULL);
    else if (cv->TRCVvType[i]==TEMP_CODE)
    	(*(SPfrontEnd->IFnewUid))((void *)ckt,&varUid,(IFuid )NULL,
            "temp-sweep", UID_OTHER, (void **)NULL);
    /* PN Resistance Sweep */	    
    else if (cv->TRCVvType[i]==rcode)
    	(*(SPfrontEnd->IFnewUid))((void *)ckt,&varUid,(IFuid )NULL,
            "res-sweep", UID_OTHER, (void **)NULL);
    else
    	(*(SPfrontEnd->IFnewUid))((void *)ckt,&varUid,(IFuid )NULL,
            "?-sweep", UID_OTHER, (void **)NULL);	    
    
    error = (*(SPfrontEnd->OUTpBeginPlot))((void *)ckt,
	(void*)ckt->CKTcurJob, ckt->CKTcurJob->JOBname,
	varUid,IF_REAL,numNames,nameList, IF_REAL,&plot);
    
    if(error) return(error);
    /* now have finished the initialization - can start doing hard part */
    
    i = 0;

resume:
    
    for(;;) {

        if(cv->TRCVvType[i]==vcode) { /* voltage source */
            if((((VSRCinstance*)(cv->TRCVvElt[i]))->VSRCdcValue)*
                    SIGN(1.,cv->TRCVvStep[i]) - 
                    SIGN(1.,cv->TRCVvStep[i]) * cv->TRCVvStop[i] >
		    0.5 * fabs(cv->TRCVvStep[i]))
                { 
                    i++ ; 
                    firstTime=1;
                    ckt->CKTmode = (ckt->CKTmode & MODEUIC) | 
                            MODEDCTRANCURVE | MODEINITJCT ;
                    if (i > cv->TRCVnestLevel ) break ; 
                    goto nextstep;
                }
        } else if(cv->TRCVvType[i]==icode) { /* current source */
            if((((ISRCinstance*)(cv->TRCVvElt[i]))->ISRCdcValue)*
                    SIGN(1.,cv->TRCVvStep[i]) -
                    SIGN(1.,cv->TRCVvStep[i]) * cv->TRCVvStop[i] >
		    0.5 * fabs(cv->TRCVvStep[i]))
                { 
                    i++ ; 
                    firstTime=1;
                    ckt->CKTmode = (ckt->CKTmode & MODEUIC) | 
                            MODEDCTRANCURVE | MODEINITJCT ;
                    if (i > cv->TRCVnestLevel ) break ; 
                    goto nextstep;
                } 
		
	} else if(cv->TRCVvType[i]==rcode) { /* resistance */
            if((((RESinstance*)(cv->TRCVvElt[i]))->RESresist)*
                    SIGN(1.,cv->TRCVvStep[i]) -
                    SIGN(1.,cv->TRCVvStep[i]) * cv->TRCVvStop[i] >
		    0.5 * fabs(cv->TRCVvStep[i]))
                { 
                    i++ ; 
                    firstTime=1;
                    ckt->CKTmode = (ckt->CKTmode & MODEUIC) | 
                            MODEDCTRANCURVE | MODEINITJCT ;
                    if (i > cv->TRCVnestLevel ) break ; 
                    goto nextstep;
                } 
        } else if(cv->TRCVvType[i]==TEMP_CODE) { /* temp sweep */
            if(((ckt->CKTtemp) - CONSTCtoK) * SIGN(1.,cv->TRCVvStep[i]) -
	            SIGN(1.,cv->TRCVvStep[i]) * cv->TRCVvStop[i] >
		    0.5 * fabs(cv->TRCVvStep[i]))
		  {
		     i++ ;
		     firstTime=1;
		     ckt->CKTmode = (ckt->CKTmode & MODEUIC) |
		            MODEDCTRANCURVE | MODEINITJCT ;
		     if (i > cv->TRCVnestLevel ) break ;
		     goto nextstep;
		  
		  }
	   
        } /* else  not possible */
        while (i > 0) { 
            /* init(i); */
            i--; 
            if(cv->TRCVvType[i]==vcode) { /* voltage source */
                ((VSRCinstance *)(cv->TRCVvElt[i]))->VSRCdcValue =
                        cv->TRCVvStart[i];
			
            } else if(cv->TRCVvType[i]==icode) { /* current source */
                ((ISRCinstance *)(cv->TRCVvElt[i]))->ISRCdcValue =
                        cv->TRCVvStart[i];
			
            } else if(cv->TRCVvType[i]==TEMP_CODE) { 
                ckt->CKTtemp = cv->TRCVvStart[i] + CONSTCtoK;
                CKTtemp(ckt); 
	    
	    } else if(cv->TRCVvType[i]==rcode) { 
                ((RESinstance *)(cv->TRCVvElt[i]))->RESresist =
                        cv->TRCVvStart[i];
		((RESinstance *)(cv->TRCVvElt[i]))->RESconduct =	
		  1/(((RESinstance *)(cv->TRCVvElt[i]))->RESresist); 
                                                     /* Note: changing the resistance does nothing */
		                                     /* changing the conductance 1/r instead */
		RESload((GENmodel*)(cv->TRCVvElt[i]->GENmodPtr),ckt); 
	     
	     
	     } /* else not possible */
        }

        /* Rotate state vectors. */
        temp = ckt->CKTstates[ckt->CKTmaxOrder+1];
        for(j=ckt->CKTmaxOrder;j>=0;j--) {
            ckt->CKTstates[j+1] = ckt->CKTstates[j];
        }
        ckt->CKTstate0 = temp;

        /* do operation */
        converged = NIiter(ckt,ckt->CKTdcTrcvMaxIter);
        if(converged != 0) {
            converged = CKTop(ckt,
                (ckt->CKTmode&MODEUIC)|MODEDCTRANCURVE | MODEINITJCT,
                (ckt->CKTmode&MODEUIC)|MODEDCTRANCURVE | MODEINITFLOAT,
                ckt->CKTdcMaxIter);
            if(converged != 0) {
                return(converged);
            }
        }
        ckt->CKTmode = (ckt->CKTmode&MODEUIC) | MODEDCTRANCURVE | MODEINITPRED ;
        if(cv->TRCVvType[0] == vcode) {
            ckt->CKTtime = ((VSRCinstance *)(cv->TRCVvElt[i]))
                    ->VSRCdcValue ;
        } else if(cv->TRCVvType[0] == icode) {
            ckt->CKTtime = ((ISRCinstance *)(cv->TRCVvElt[i]))
                    ->ISRCdcValue ;
        } else if(cv->TRCVvType[0] == rcode) {
            ckt->CKTtime = ((RESinstance *)(cv->TRCVvElt[i]))
                    ->RESresist;
        } 
        /* PN Temp sweep */
	else 
        {
	ckt->CKTtime = ckt->CKTtemp - CONSTCtoK ; 
        }
#ifdef WANT_SENSE2
/*
        if(!ckt->CKTsenInfo) printf("sensitivity structure does not exist\n");
    */
        if(ckt->CKTsenInfo && (ckt->CKTsenInfo->SENmode&DCSEN) ){
	    int senmode;

#ifdef SENSDEBUG
            if(cv->TRCVvType[i]==vcode) { /* voltage source */
                printf("Voltage Source Value : %.5e V\n",
                        ((VSRCinstance*) (cv->TRCVvElt[i]))->VSRCdcValue);
            }
            if(cv->TRCVvType[i]==icode) { /* current source */
                printf("Current Source Value : %.5e A\n",
                        ((ISRCinstance*)(cv->TRCVvElt[i]))->ISRCdcValue);
            }
	    if(cv->TRCVvType[i]==rcode) { /* resistance */
                printf("Current Resistance Value : %.5e Ohm\n",
                        ((RESinstance*)(cv->TRCVvElt[i]->GENmodPtr))->RESresist);
            }
	    if(cv->TRCVvType[i]==TEMP_CODE) { /* Temperature */
                printf("Current Circuit Temperature : %.5e C\n",
                        ckt-CKTtime - CONSTCtoK);
            }
	    
#endif /* SENSDEBUG */

            senmode = ckt->CKTsenInfo->SENmode;
            save = ckt->CKTmode;
            ckt->CKTsenInfo->SENmode = DCSEN;
            if(error = CKTsenDCtran(ckt)) return (error);
            ckt->CKTmode = save;
            ckt->CKTsenInfo->SENmode = senmode;

        }
#endif


        CKTdump(ckt,ckt->CKTtime,plot);
        if(firstTime) {
            firstTime=0;
            bcopy((char *)ckt->CKTstate0,(char *)ckt->CKTstate1,
                    ckt->CKTnumStates*sizeof(double));
        }

nextstep:;
        if(cv->TRCVvType[i]==vcode) { /* voltage source */
            ((VSRCinstance*)(cv->TRCVvElt[i]))->VSRCdcValue +=
                    cv->TRCVvStep[i];
        } else if(cv->TRCVvType[i]==icode) { /* current source */
            ((ISRCinstance*)(cv->TRCVvElt[i]))->ISRCdcValue +=
                    cv->TRCVvStep[i];
	} else if(cv->TRCVvType[i]==rcode) { /* resistance */
            ((RESinstance*)(cv->TRCVvElt[i]))->RESresist +=
                    cv->TRCVvStep[i];
	    /* This code should update resistance and conductance */    
	    ((RESinstance*)(cv->TRCVvElt[i]))->RESconduct =
	    1/(((RESinstance*)(cv->TRCVvElt[i]))->RESresist);
            RESload((GENmodel*)(cv->TRCVvElt[i]->GENmodPtr),ckt); 
	}
	/* PN Temp Sweep - serban */
        else if (cv->TRCVvType[i]==TEMP_CODE)
        {
    	    ckt->CKTtemp += cv->TRCVvStep[i];
            CKTtemp(ckt);	    
        } /* else not possible */
        
	if( (*(SPfrontEnd->IFpauseTest))() ) {
            /* user asked us to pause, so save state */
            cv->TRCVnestState = i;
            return(E_PAUSE);
        }
    }

    /* all done, lets put everything back */

    for(i=0;i<=cv->TRCVnestLevel;i++) {
        if(cv->TRCVvType[i] == vcode) {   /* voltage source */
            ((VSRCinstance*)(cv->TRCVvElt[i]))->VSRCdcValue = 
                    cv->TRCVvSave[i];
            ((VSRCinstance*)(cv->TRCVvElt[i]))->VSRCdcGiven = cv->TRCVgSave[i];
        } else  if(cv->TRCVvType[i] == icode) /*current source */ {
            ((ISRCinstance*)(cv->TRCVvElt[i]))->ISRCdcValue = 
                    cv->TRCVvSave[i];
            ((ISRCinstance*)(cv->TRCVvElt[i]))->ISRCdcGiven = cv->TRCVgSave[i];
        } else  if(cv->TRCVvType[i] == rcode) /* Resistance */ {
            ((RESinstance*)(cv->TRCVvElt[i]))->RESresist = 
                    cv->TRCVvSave[i];
	    /* We restore both resistance and conductance */
	    ((RESinstance*)(cv->TRCVvElt[i]))->RESconduct =
	    1/(((RESinstance*)(cv->TRCVvElt[i]))->RESresist);
	    
            ((RESinstance*)(cv->TRCVvElt[i]))->RESresGiven = cv->TRCVgSave[i];
	    RESload((GENmodel*)(cv->TRCVvElt[i]->GENmodPtr),ckt); 
        }
	 else if(cv->TRCVvType[i] == TEMP_CODE) {
            ckt->CKTtemp = cv->TRCVvSave[i];
	    CKTtemp(ckt);
	} /* else not possible */
    }
    (*(SPfrontEnd->OUTendPlot))(plot);

    return(OK);
}
