/**** BSIM4.0.0, Released by Weidong Liu 3/24/2000 ****/

/**********
 * Copyright 2000 Regents of the University of California. All rights reserved.
 * File: b4set.c of BSIM4.0.0.
 * Authors: Weidong Liu, Xiaodong Jin, Kanyu M. Cao, Chenming Hu.
 * Project Director: Prof. Chenming Hu.
 **********/

#include "ngspice.h"
#include <stdio.h>
#include <math.h>
#include "smpdefs.h"
#include "cktdefs.h"
#include "bsim4def.h"
#include "const.h"
#include "sperror.h"


#define MAX_EXP 5.834617425e14
#define MIN_EXP 1.713908431e-15
#define EXP_THRESHOLD 34.0
#define EPS0 8.85418e-12
#define EPSSI 1.03594e-10
#define PI 3.141592654
#define Charge_q 1.60219e-19

int
BSIM4setup(matrix,inModel,ckt,states)
register SMPmatrix *matrix;
register GENmodel *inModel;
register CKTcircuit *ckt;
int *states;
{
register BSIM4model *model = (BSIM4model*)inModel;
register BSIM4instance *here;
int error;
CKTnode *tmp;

double tmp1, tmp2;

    /*  loop through all the BSIM4 device models */
    for( ; model != NULL; model = model->BSIM4nextModel )
    {   /* process defaults of model parameters */
        if (!model->BSIM4typeGiven)
            model->BSIM4type = NMOS;     

        if (!model->BSIM4mobModGiven) 
            model->BSIM4mobMod = 0;
	else if ((model->BSIM4mobMod != 0) && (model->BSIM4mobMod != 1)
	         && (model->BSIM4mobMod != 2))
	{   model->BSIM4mobMod = 0;
            printf("Warning: mobMod has been set to its default value: 0.\n");
	}

        if (!model->BSIM4binUnitGiven) 
            model->BSIM4binUnit = 1;
        if (!model->BSIM4paramChkGiven) 
            model->BSIM4paramChk = 1;

        if (!model->BSIM4dioModGiven)
            model->BSIM4dioMod = 1;
        else if ((model->BSIM4dioMod != 0) && (model->BSIM4dioMod != 1)
            && (model->BSIM4dioMod != 2))
        {   model->BSIM4dioMod = 1;
            printf("Warning: dioMod has been set to its default value: 1.\n");
        }

        if (!model->BSIM4capModGiven) 
            model->BSIM4capMod = 2;
        else if ((model->BSIM4capMod != 0) && (model->BSIM4capMod != 1)
            && (model->BSIM4capMod != 2))
        {   model->BSIM4capMod = 2;
            printf("Warning: capMod has been set to its default value: 2.\n");
        }

        if (!model->BSIM4rdsModGiven)
            model->BSIM4rdsMod = 0;
	else if ((model->BSIM4rdsMod != 0) && (model->BSIM4rdsMod != 1))
        {   model->BSIM4rdsMod = 0;
	    printf("Warning: rdsMod has been set to its default value: 0.\n");
	}
        if (!model->BSIM4rbodyModGiven)
            model->BSIM4rbodyMod = 0;
        else if ((model->BSIM4rbodyMod != 0) && (model->BSIM4rbodyMod != 1))
        {   model->BSIM4rbodyMod = 0;
            printf("Warning: rbodyMod has been set to its default value: 0.\n");
        }

        if (!model->BSIM4rgateModGiven)
            model->BSIM4rgateMod = 0;
        else if ((model->BSIM4rgateMod != 0) && (model->BSIM4rgateMod != 1)
            && (model->BSIM4rgateMod != 2) && (model->BSIM4rgateMod != 3))
        {   model->BSIM4rgateMod = 0;
            printf("Warning: rgateMod has been set to its default value: 0.\n");
        }

        if (!model->BSIM4perModGiven)
            model->BSIM4perMod = 1;
        else if ((model->BSIM4perMod != 0) && (model->BSIM4perMod != 1))
        {   model->BSIM4perMod = 1;
            printf("Warning: perMod has been set to its default value: 1.\n");
        }

        if (!model->BSIM4geoModGiven)
            model->BSIM4geoMod = 0;

        if (!model->BSIM4fnoiModGiven) 
            model->BSIM4fnoiMod = 1;
        else if ((model->BSIM4fnoiMod != 0) && (model->BSIM4fnoiMod != 1))
        {   model->BSIM4fnoiMod = 1;
            printf("Warning: fnoiMod has been set to its default value: 1.\n");
        }
        if (!model->BSIM4tnoiModGiven)
            model->BSIM4tnoiMod = 0; /* WDLiu: tnoiMod=1 needs to set internal S/D nodes */
        else if ((model->BSIM4tnoiMod != 0) && (model->BSIM4tnoiMod != 1))
        {   model->BSIM4tnoiMod = 0;
            printf("Warning: tnoiMod has been set to its default value: 0.\n");
        }

        if (!model->BSIM4trnqsModGiven)
            model->BSIM4trnqsMod = 0; 
        else if ((model->BSIM4trnqsMod != 0) && (model->BSIM4trnqsMod != 1))
        {   model->BSIM4trnqsMod = 0;
            printf("Warning: trnqsMod has been set to its default value: 0.\n");
        }
        if (!model->BSIM4acnqsModGiven)
            model->BSIM4acnqsMod = 0;
        else if ((model->BSIM4acnqsMod != 0) && (model->BSIM4acnqsMod != 1))
        {   model->BSIM4acnqsMod = 0;
            printf("Warning: acnqsMod has been set to its default value: 0.\n");
        }

        if (!model->BSIM4igcModGiven)
            model->BSIM4igcMod = 0;
        else if ((model->BSIM4igcMod != 0) && (model->BSIM4igcMod != 1))
        {   model->BSIM4igcMod = 0;
            printf("Warning: igcMod has been set to its default value: 0.\n");
        }
        if (!model->BSIM4igbModGiven)
            model->BSIM4igbMod = 0;
        else if ((model->BSIM4igbMod != 0) && (model->BSIM4igbMod != 1))
        {   model->BSIM4igbMod = 0;
            printf("Warning: igbMod has been set to its default value: 0.\n");
        }

        if (!model->BSIM4versionGiven) 
            model->BSIM4version = "4.0.0";
        if (!model->BSIM4toxrefGiven)
            model->BSIM4toxref = 30.0e-10;
        if (!model->BSIM4toxeGiven)
            model->BSIM4toxe = 30.0e-10;
        if (!model->BSIM4toxpGiven)
            model->BSIM4toxp = model->BSIM4toxe;
        if (!model->BSIM4toxmGiven)
            model->BSIM4toxm = model->BSIM4toxe;
        if (!model->BSIM4dtoxGiven)
            model->BSIM4dtox = 0.0;
        if (!model->BSIM4epsroxGiven)
            model->BSIM4epsrox = 3.9;

        if (!model->BSIM4cdscGiven)
	    model->BSIM4cdsc = 2.4e-4;   /* unit Q/V/m^2  */
        if (!model->BSIM4cdscbGiven)
	    model->BSIM4cdscb = 0.0;   /* unit Q/V/m^2  */    
	    if (!model->BSIM4cdscdGiven)
	    model->BSIM4cdscd = 0.0;   /* unit Q/V/m^2  */
        if (!model->BSIM4citGiven)
	    model->BSIM4cit = 0.0;   /* unit Q/V/m^2  */
        if (!model->BSIM4nfactorGiven)
	    model->BSIM4nfactor = 1.0;
        if (!model->BSIM4xjGiven)
            model->BSIM4xj = .15e-6;
        if (!model->BSIM4vsatGiven)
            model->BSIM4vsat = 8.0e4;    /* unit m/s */ 
        if (!model->BSIM4atGiven)
            model->BSIM4at = 3.3e4;    /* unit m/s */ 
        if (!model->BSIM4a0Given)
            model->BSIM4a0 = 1.0;  
        if (!model->BSIM4agsGiven)
            model->BSIM4ags = 0.0;
        if (!model->BSIM4a1Given)
            model->BSIM4a1 = 0.0;
        if (!model->BSIM4a2Given)
            model->BSIM4a2 = 1.0;
        if (!model->BSIM4ketaGiven)
            model->BSIM4keta = -0.047;    /* unit  / V */
        if (!model->BSIM4nsubGiven)
            model->BSIM4nsub = 6.0e16;   /* unit 1/cm3 */
        if (!model->BSIM4ndepGiven)
            model->BSIM4ndep = 1.7e17;   /* unit 1/cm3 */
        if (!model->BSIM4nsdGiven)
            model->BSIM4nsd = 1.0e20;   /* unit 1/cm3 */
        if (!model->BSIM4phinGiven)
            model->BSIM4phin = 0.0; /* unit V */
        if (!model->BSIM4ngateGiven)
            model->BSIM4ngate = 0;   /* unit 1/cm3 */
        if (!model->BSIM4vbmGiven)
	    model->BSIM4vbm = -3.0;
        if (!model->BSIM4xtGiven)
	    model->BSIM4xt = 1.55e-7;
        if (!model->BSIM4kt1Given)
            model->BSIM4kt1 = -0.11;      /* unit V */
        if (!model->BSIM4kt1lGiven)
            model->BSIM4kt1l = 0.0;      /* unit V*m */
        if (!model->BSIM4kt2Given)
            model->BSIM4kt2 = 0.022;      /* No unit */
        if (!model->BSIM4k3Given)
            model->BSIM4k3 = 80.0;      
        if (!model->BSIM4k3bGiven)
            model->BSIM4k3b = 0.0;      
        if (!model->BSIM4w0Given)
            model->BSIM4w0 = 2.5e-6;    
        if (!model->BSIM4lpe0Given)
            model->BSIM4lpe0 = 1.74e-7;     
        if (!model->BSIM4lpebGiven)
            model->BSIM4lpeb = 0.0;
        if (!model->BSIM4dvtp0Given)
            model->BSIM4dvtp0 = 0.0;
        if (!model->BSIM4dvtp1Given)
            model->BSIM4dvtp1 = 0.0;
        if (!model->BSIM4dvt0Given)
            model->BSIM4dvt0 = 2.2;    
        if (!model->BSIM4dvt1Given)
            model->BSIM4dvt1 = 0.53;      
        if (!model->BSIM4dvt2Given)
            model->BSIM4dvt2 = -0.032;   /* unit 1 / V */     

        if (!model->BSIM4dvt0wGiven)
            model->BSIM4dvt0w = 0.0;    
        if (!model->BSIM4dvt1wGiven)
            model->BSIM4dvt1w = 5.3e6;    
        if (!model->BSIM4dvt2wGiven)
            model->BSIM4dvt2w = -0.032;   

        if (!model->BSIM4droutGiven)
            model->BSIM4drout = 0.56;     
        if (!model->BSIM4dsubGiven)
            model->BSIM4dsub = model->BSIM4drout;     
        if (!model->BSIM4vth0Given)
            model->BSIM4vth0 = (model->BSIM4type == NMOS) ? 0.7 : -0.7;
        if (!model->BSIM4euGiven)
            model->BSIM4eu = (model->BSIM4type == NMOS) ? 1.67 : 1.0;;
        if (!model->BSIM4uaGiven)
            model->BSIM4ua = (model->BSIM4mobMod == 2) ? 1.0e-15 : 1.0e-9; /* unit m/V */
        if (!model->BSIM4ua1Given)
            model->BSIM4ua1 = 1.0e-9;      /* unit m/V */
        if (!model->BSIM4ubGiven)
            model->BSIM4ub = 1.0e-19;     /* unit (m/V)**2 */
        if (!model->BSIM4ub1Given)
            model->BSIM4ub1 = -1.0e-18;     /* unit (m/V)**2 */
        if (!model->BSIM4ucGiven)
            model->BSIM4uc = (model->BSIM4mobMod == 1) ? -0.0465 : -0.0465e-9;   
        if (!model->BSIM4uc1Given)
            model->BSIM4uc1 = (model->BSIM4mobMod == 1) ? -0.056 : -0.056e-9;   
        if (!model->BSIM4u0Given)
            model->BSIM4u0 = (model->BSIM4type == NMOS) ? 0.067 : 0.025;
        if (!model->BSIM4uteGiven)
	    model->BSIM4ute = -1.5;    
        if (!model->BSIM4voffGiven)
	    model->BSIM4voff = -0.08;
        if (!model->BSIM4vofflGiven)
            model->BSIM4voffl = 0.0;
        if (!model->BSIM4minvGiven)
            model->BSIM4minv = 0.0;
        if (!model->BSIM4fproutGiven)
            model->BSIM4fprout = 0.0;
        if (!model->BSIM4pditsGiven)
            model->BSIM4pdits = 0.0;
        if (!model->BSIM4pditsdGiven)
            model->BSIM4pditsd = 0.0;
        if (!model->BSIM4pditslGiven)
            model->BSIM4pditsl = 0.0;
        if (!model->BSIM4deltaGiven)  
           model->BSIM4delta = 0.01;
        if (!model->BSIM4rdswminGiven)
            model->BSIM4rdswmin = 0.0;
        if (!model->BSIM4rdwminGiven)
            model->BSIM4rdwmin = 0.0;
        if (!model->BSIM4rswminGiven)
            model->BSIM4rswmin = 0.0;
        if (!model->BSIM4rdswGiven)
	    model->BSIM4rdsw = 200.0; /* in ohm*um */     
        if (!model->BSIM4rdwGiven)
            model->BSIM4rdw = 100.0;
        if (!model->BSIM4rswGiven)
            model->BSIM4rsw = 100.0;
        if (!model->BSIM4prwgGiven)
            model->BSIM4prwg = 1.0; /* in 1/V */
        if (!model->BSIM4prwbGiven)
            model->BSIM4prwb = 0.0;      
        if (!model->BSIM4prtGiven)
        if (!model->BSIM4prtGiven)
            model->BSIM4prt = 0.0;      
        if (!model->BSIM4eta0Given)
            model->BSIM4eta0 = 0.08;      /* no unit  */ 
        if (!model->BSIM4etabGiven)
            model->BSIM4etab = -0.07;      /* unit  1/V */ 
        if (!model->BSIM4pclmGiven)
            model->BSIM4pclm = 1.3;      /* no unit  */ 
        if (!model->BSIM4pdibl1Given)
            model->BSIM4pdibl1 = 0.39;    /* no unit  */
        if (!model->BSIM4pdibl2Given)
            model->BSIM4pdibl2 = 0.0086;    /* no unit  */ 
        if (!model->BSIM4pdiblbGiven)
            model->BSIM4pdiblb = 0.0;    /* 1/V  */ 
        if (!model->BSIM4pscbe1Given)
            model->BSIM4pscbe1 = 4.24e8;     
        if (!model->BSIM4pscbe2Given)
            model->BSIM4pscbe2 = 1.0e-5;    
        if (!model->BSIM4pvagGiven)
            model->BSIM4pvag = 0.0;     
        if (!model->BSIM4wrGiven)  
            model->BSIM4wr = 1.0;
        if (!model->BSIM4dwgGiven)  
            model->BSIM4dwg = 0.0;
        if (!model->BSIM4dwbGiven)  
            model->BSIM4dwb = 0.0;
        if (!model->BSIM4b0Given)
            model->BSIM4b0 = 0.0;
        if (!model->BSIM4b1Given)  
            model->BSIM4b1 = 0.0;
        if (!model->BSIM4alpha0Given)  
            model->BSIM4alpha0 = 0.0;
        if (!model->BSIM4alpha1Given)
            model->BSIM4alpha1 = 0.0;
        if (!model->BSIM4beta0Given)  
            model->BSIM4beta0 = 30.0;
        if (!model->BSIM4agidlGiven)
            model->BSIM4agidl = 0.0;
        if (!model->BSIM4bgidlGiven)
            model->BSIM4bgidl = 2.3e9; /* V/m */
        if (!model->BSIM4cgidlGiven)
            model->BSIM4cgidl = 0.5; /* V^3 */
        if (!model->BSIM4egidlGiven)
            model->BSIM4egidl = 0.8; /* V */
        if (!model->BSIM4aigcGiven)
            model->BSIM4aigc = (model->BSIM4type == NMOS) ? 0.43 : 0.31;
        if (!model->BSIM4bigcGiven)
            model->BSIM4bigc = (model->BSIM4type == NMOS) ? 0.054 : 0.024;
        if (!model->BSIM4cigcGiven)
            model->BSIM4cigc = (model->BSIM4type == NMOS) ? 0.075 : 0.03;
        if (!model->BSIM4aigsdGiven)
            model->BSIM4aigsd = (model->BSIM4type == NMOS) ? 0.43 : 0.31;
        if (!model->BSIM4bigsdGiven)
            model->BSIM4bigsd = (model->BSIM4type == NMOS) ? 0.054 : 0.024;
        if (!model->BSIM4cigsdGiven)
            model->BSIM4cigsd = (model->BSIM4type == NMOS) ? 0.075 : 0.03;
        if (!model->BSIM4aigbaccGiven)
            model->BSIM4aigbacc = 0.43;
        if (!model->BSIM4bigbaccGiven)
            model->BSIM4bigbacc = 0.054;
        if (!model->BSIM4cigbaccGiven)
            model->BSIM4cigbacc = 0.075;
        if (!model->BSIM4aigbinvGiven)
            model->BSIM4aigbinv = 0.35;
        if (!model->BSIM4bigbinvGiven)
            model->BSIM4bigbinv = 0.03;
        if (!model->BSIM4cigbinvGiven)
            model->BSIM4cigbinv = 0.006;
        if (!model->BSIM4nigcGiven)
            model->BSIM4nigc = 1.0;
        if (!model->BSIM4nigbinvGiven)
            model->BSIM4nigbinv = 3.0;
        if (!model->BSIM4nigbaccGiven)
            model->BSIM4nigbacc = 1.0;
        if (!model->BSIM4ntoxGiven)
            model->BSIM4ntox = 1.0;
        if (!model->BSIM4eigbinvGiven)
            model->BSIM4eigbinv = 1.1;
        if (!model->BSIM4pigcdGiven)
            model->BSIM4pigcd = 1.0;
        if (!model->BSIM4poxedgeGiven)
            model->BSIM4poxedge = 1.0;
        if (!model->BSIM4xrcrg1Given)
            model->BSIM4xrcrg1 = 12.0;
        if (!model->BSIM4xrcrg2Given)
            model->BSIM4xrcrg2 = 1.0;
        if (!model->BSIM4ijthsfwdGiven)
            model->BSIM4ijthsfwd = 0.1; /* unit A */
        if (!model->BSIM4ijthdfwdGiven)
            model->BSIM4ijthdfwd = model->BSIM4ijthsfwd;
        if (!model->BSIM4ijthsrevGiven)
            model->BSIM4ijthsrev = 0.1; /* unit A */
        if (!model->BSIM4ijthdrevGiven)
            model->BSIM4ijthdrev = model->BSIM4ijthsrev;
        if (!model->BSIM4tnoiaGiven)
            model->BSIM4tnoia = 1.5;
        if (!model->BSIM4tnoibGiven)
            model->BSIM4tnoib = 3.5;
        if (!model->BSIM4ntnoiGiven)
            model->BSIM4ntnoi = 1.0;

        if (!model->BSIM4xjbvsGiven)
            model->BSIM4xjbvs = 1.0; /* no unit */
        if (!model->BSIM4xjbvdGiven)
            model->BSIM4xjbvd = model->BSIM4xjbvs;
        if (!model->BSIM4bvsGiven)
            model->BSIM4bvs = 10.0; /* V */
        if (!model->BSIM4bvdGiven)
            model->BSIM4bvd = model->BSIM4bvs;
        if (!model->BSIM4gbminGiven)
            model->BSIM4gbmin = 1.0e-12; /* in mho */
        if (!model->BSIM4rbdbGiven)
            model->BSIM4rbdb = 50.0; /* in ohm */
        if (!model->BSIM4rbpbGiven)
            model->BSIM4rbpb = 50.0;
        if (!model->BSIM4rbsbGiven)
            model->BSIM4rbsb = 50.0;
        if (!model->BSIM4rbpsGiven)
            model->BSIM4rbps = 50.0;
        if (!model->BSIM4rbpdGiven)
            model->BSIM4rbpd = 50.0;

        if (!model->BSIM4cgslGiven)  
            model->BSIM4cgsl = 0.0;
        if (!model->BSIM4cgdlGiven)  
            model->BSIM4cgdl = 0.0;
        if (!model->BSIM4ckappasGiven)  
            model->BSIM4ckappas = 0.6;
        if (!model->BSIM4ckappadGiven)
            model->BSIM4ckappad = model->BSIM4ckappas;
        if (!model->BSIM4clcGiven)  
            model->BSIM4clc = 0.1e-6;
        if (!model->BSIM4cleGiven)  
            model->BSIM4cle = 0.6;
        if (!model->BSIM4vfbcvGiven)  
            model->BSIM4vfbcv = -1.0;
        if (!model->BSIM4acdeGiven)
            model->BSIM4acde = 1.0;
        if (!model->BSIM4moinGiven)
            model->BSIM4moin = 15.0;
        if (!model->BSIM4noffGiven)
            model->BSIM4noff = 1.0;
        if (!model->BSIM4voffcvGiven)
            model->BSIM4voffcv = 0.0;
        if (!model->BSIM4dmcgGiven)
            model->BSIM4dmcg = 0.0;
        if (!model->BSIM4dmciGiven)
            model->BSIM4dmci = model->BSIM4dmcg;
        if (!model->BSIM4dmdgGiven)
            model->BSIM4dmdg = 0.0;
        if (!model->BSIM4dmcgtGiven)
            model->BSIM4dmcgt = 0.0;
        if (!model->BSIM4xgwGiven)
            model->BSIM4xgw = 0.0;
        if (!model->BSIM4xglGiven)
            model->BSIM4xgl = 0.0;
        if (!model->BSIM4rshgGiven)
            model->BSIM4rshg = 0.1;
        if (!model->BSIM4ngconGiven)
            model->BSIM4ngcon = 1.0;
        if (!model->BSIM4tcjGiven)
            model->BSIM4tcj = 0.0;
        if (!model->BSIM4tpbGiven)
            model->BSIM4tpb = 0.0;
        if (!model->BSIM4tcjswGiven)
            model->BSIM4tcjsw = 0.0;
        if (!model->BSIM4tpbswGiven)
            model->BSIM4tpbsw = 0.0;
        if (!model->BSIM4tcjswgGiven)
            model->BSIM4tcjswg = 0.0;
        if (!model->BSIM4tpbswgGiven)
            model->BSIM4tpbswg = 0.0;

	/* Length dependence */
        if (!model->BSIM4lcdscGiven)
	    model->BSIM4lcdsc = 0.0;
        if (!model->BSIM4lcdscbGiven)
	    model->BSIM4lcdscb = 0.0;
	    if (!model->BSIM4lcdscdGiven) 
	    model->BSIM4lcdscd = 0.0;
        if (!model->BSIM4lcitGiven)
	    model->BSIM4lcit = 0.0;
        if (!model->BSIM4lnfactorGiven)
	    model->BSIM4lnfactor = 0.0;
        if (!model->BSIM4lxjGiven)
            model->BSIM4lxj = 0.0;
        if (!model->BSIM4lvsatGiven)
            model->BSIM4lvsat = 0.0;
        if (!model->BSIM4latGiven)
            model->BSIM4lat = 0.0;
        if (!model->BSIM4la0Given)
            model->BSIM4la0 = 0.0; 
        if (!model->BSIM4lagsGiven)
            model->BSIM4lags = 0.0;
        if (!model->BSIM4la1Given)
            model->BSIM4la1 = 0.0;
        if (!model->BSIM4la2Given)
            model->BSIM4la2 = 0.0;
        if (!model->BSIM4lketaGiven)
            model->BSIM4lketa = 0.0;
        if (!model->BSIM4lnsubGiven)
            model->BSIM4lnsub = 0.0;
        if (!model->BSIM4lndepGiven)
            model->BSIM4lndep = 0.0;
        if (!model->BSIM4lnsdGiven)
            model->BSIM4lnsd = 0.0;
        if (!model->BSIM4lphinGiven)
            model->BSIM4lphin = 0.0;
        if (!model->BSIM4lngateGiven)
            model->BSIM4lngate = 0.0;
        if (!model->BSIM4lvbmGiven)
	    model->BSIM4lvbm = 0.0;
        if (!model->BSIM4lxtGiven)
	    model->BSIM4lxt = 0.0;
        if (!model->BSIM4lkt1Given)
            model->BSIM4lkt1 = 0.0; 
        if (!model->BSIM4lkt1lGiven)
            model->BSIM4lkt1l = 0.0;
        if (!model->BSIM4lkt2Given)
            model->BSIM4lkt2 = 0.0;
        if (!model->BSIM4lk3Given)
            model->BSIM4lk3 = 0.0;      
        if (!model->BSIM4lk3bGiven)
            model->BSIM4lk3b = 0.0;      
        if (!model->BSIM4lw0Given)
            model->BSIM4lw0 = 0.0;    
        if (!model->BSIM4llpe0Given)
            model->BSIM4llpe0 = 0.0;
        if (!model->BSIM4llpebGiven)
            model->BSIM4llpeb = model->BSIM4llpe0;
        if (!model->BSIM4ldvtp0Given)
            model->BSIM4ldvtp0 = 0.0;
        if (!model->BSIM4ldvtp1Given)
            model->BSIM4ldvtp1 = 0.0;
        if (!model->BSIM4ldvt0Given)
            model->BSIM4ldvt0 = 0.0;    
        if (!model->BSIM4ldvt1Given)
            model->BSIM4ldvt1 = 0.0;      
        if (!model->BSIM4ldvt2Given)
            model->BSIM4ldvt2 = 0.0;
        if (!model->BSIM4ldvt0wGiven)
            model->BSIM4ldvt0w = 0.0;    
        if (!model->BSIM4ldvt1wGiven)
            model->BSIM4ldvt1w = 0.0;      
        if (!model->BSIM4ldvt2wGiven)
            model->BSIM4ldvt2w = 0.0;
        if (!model->BSIM4ldroutGiven)
            model->BSIM4ldrout = 0.0;     
        if (!model->BSIM4ldsubGiven)
            model->BSIM4ldsub = 0.0;
        if (!model->BSIM4lvth0Given)
           model->BSIM4lvth0 = 0.0;
        if (!model->BSIM4luaGiven)
            model->BSIM4lua = 0.0;
        if (!model->BSIM4lua1Given)
            model->BSIM4lua1 = 0.0;
        if (!model->BSIM4lubGiven)
            model->BSIM4lub = 0.0;
        if (!model->BSIM4lub1Given)
            model->BSIM4lub1 = 0.0;
        if (!model->BSIM4lucGiven)
            model->BSIM4luc = 0.0;
        if (!model->BSIM4luc1Given)
            model->BSIM4luc1 = 0.0;
        if (!model->BSIM4lu0Given)
            model->BSIM4lu0 = 0.0;
        if (!model->BSIM4luteGiven)
	    model->BSIM4lute = 0.0;    
        if (!model->BSIM4lvoffGiven)
	    model->BSIM4lvoff = 0.0;
        if (!model->BSIM4lminvGiven)
            model->BSIM4lminv = 0.0;
        if (!model->BSIM4lfproutGiven)
            model->BSIM4lfprout = 0.0;
        if (!model->BSIM4lpditsGiven)
            model->BSIM4lpdits = 0.0;
        if (!model->BSIM4lpditsdGiven)
            model->BSIM4lpditsd = 0.0;
        if (!model->BSIM4ldeltaGiven)  
            model->BSIM4ldelta = 0.0;
        if (!model->BSIM4lrdswGiven)
            model->BSIM4lrdsw = 0.0;
        if (!model->BSIM4lrdwGiven)
            model->BSIM4lrdw = 0.0;
        if (!model->BSIM4lrswGiven)
            model->BSIM4lrsw = 0.0;
        if (!model->BSIM4lprwbGiven)
            model->BSIM4lprwb = 0.0;
        if (!model->BSIM4lprwgGiven)
            model->BSIM4lprwg = 0.0;
        if (!model->BSIM4lprtGiven)
            model->BSIM4lprt = 0.0;
        if (!model->BSIM4leta0Given)
            model->BSIM4leta0 = 0.0;
        if (!model->BSIM4letabGiven)
            model->BSIM4letab = -0.0;
        if (!model->BSIM4lpclmGiven)
            model->BSIM4lpclm = 0.0; 
        if (!model->BSIM4lpdibl1Given)
            model->BSIM4lpdibl1 = 0.0;
        if (!model->BSIM4lpdibl2Given)
            model->BSIM4lpdibl2 = 0.0;
        if (!model->BSIM4lpdiblbGiven)
            model->BSIM4lpdiblb = 0.0;
        if (!model->BSIM4lpscbe1Given)
            model->BSIM4lpscbe1 = 0.0;
        if (!model->BSIM4lpscbe2Given)
            model->BSIM4lpscbe2 = 0.0;
        if (!model->BSIM4lpvagGiven)
            model->BSIM4lpvag = 0.0;     
        if (!model->BSIM4lwrGiven)  
            model->BSIM4lwr = 0.0;
        if (!model->BSIM4ldwgGiven)  
            model->BSIM4ldwg = 0.0;
        if (!model->BSIM4ldwbGiven)  
            model->BSIM4ldwb = 0.0;
        if (!model->BSIM4lb0Given)
            model->BSIM4lb0 = 0.0;
        if (!model->BSIM4lb1Given)  
            model->BSIM4lb1 = 0.0;
        if (!model->BSIM4lalpha0Given)  
            model->BSIM4lalpha0 = 0.0;
        if (!model->BSIM4lalpha1Given)
            model->BSIM4lalpha1 = 0.0;
        if (!model->BSIM4lbeta0Given)  
            model->BSIM4lbeta0 = 0.0;
        if (!model->BSIM4lagidlGiven)
            model->BSIM4lagidl = 0.0;
        if (!model->BSIM4lbgidlGiven)
            model->BSIM4lbgidl = 0.0;
        if (!model->BSIM4lcgidlGiven)
            model->BSIM4lcgidl = 0.0;
        if (!model->BSIM4legidlGiven)
            model->BSIM4legidl = 0.0;
        if (!model->BSIM4laigcGiven)
            model->BSIM4laigc = 0.0;
        if (!model->BSIM4lbigcGiven)
            model->BSIM4lbigc = 0.0;
        if (!model->BSIM4lcigcGiven)
            model->BSIM4lcigc = 0.0;
        if (!model->BSIM4laigsdGiven)
            model->BSIM4laigsd = 0.0;
        if (!model->BSIM4lbigsdGiven)
            model->BSIM4lbigsd = 0.0;
        if (!model->BSIM4lcigsdGiven)
            model->BSIM4lcigsd = 0.0;
        if (!model->BSIM4laigbaccGiven)
            model->BSIM4laigbacc = 0.0;
        if (!model->BSIM4lbigbaccGiven)
            model->BSIM4lbigbacc = 0.0;
        if (!model->BSIM4lcigbaccGiven)
            model->BSIM4lcigbacc = 0.0;
        if (!model->BSIM4laigbinvGiven)
            model->BSIM4laigbinv = 0.0;
        if (!model->BSIM4lbigbinvGiven)
            model->BSIM4lbigbinv = 0.0;
        if (!model->BSIM4lcigbinvGiven)
            model->BSIM4lcigbinv = 0.0;
        if (!model->BSIM4lnigcGiven)
            model->BSIM4lnigc = 0.0;
        if (!model->BSIM4lnigbinvGiven)
            model->BSIM4lnigbinv = 0.0;
        if (!model->BSIM4lnigbaccGiven)
            model->BSIM4lnigbacc = 0.0;
        if (!model->BSIM4lntoxGiven)
            model->BSIM4lntox = 0.0;
        if (!model->BSIM4leigbinvGiven)
            model->BSIM4leigbinv = 0.0;
        if (!model->BSIM4lpigcdGiven)
            model->BSIM4lpigcd = 0.0;
        if (!model->BSIM4lpoxedgeGiven)
            model->BSIM4lpoxedge = 0.0;
        if (!model->BSIM4lxrcrg1Given)
            model->BSIM4lxrcrg1 = 0.0;
        if (!model->BSIM4lxrcrg2Given)
            model->BSIM4lxrcrg2 = 0.0;
        if (!model->BSIM4leuGiven)
            model->BSIM4leu = 0.0;
        if (!model->BSIM4lvfbGiven)
            model->BSIM4lvfb = 0.0;

        if (!model->BSIM4lcgslGiven)  
            model->BSIM4lcgsl = 0.0;
        if (!model->BSIM4lcgdlGiven)  
            model->BSIM4lcgdl = 0.0;
        if (!model->BSIM4lckappasGiven)  
            model->BSIM4lckappas = 0.0;
        if (!model->BSIM4lckappadGiven)
            model->BSIM4lckappad = 0.0;
        if (!model->BSIM4lclcGiven)  
            model->BSIM4lclc = 0.0;
        if (!model->BSIM4lcleGiven)  
            model->BSIM4lcle = 0.0;
        if (!model->BSIM4lcfGiven)  
            model->BSIM4lcf = 0.0;
        if (!model->BSIM4lvfbcvGiven)  
            model->BSIM4lvfbcv = 0.0;
        if (!model->BSIM4lacdeGiven)
            model->BSIM4lacde = 0.0;
        if (!model->BSIM4lmoinGiven)
            model->BSIM4lmoin = 0.0;
        if (!model->BSIM4lnoffGiven)
            model->BSIM4lnoff = 0.0;
        if (!model->BSIM4lvoffcvGiven)
            model->BSIM4lvoffcv = 0.0;

	/* Width dependence */
        if (!model->BSIM4wcdscGiven)
	    model->BSIM4wcdsc = 0.0;
        if (!model->BSIM4wcdscbGiven)
	    model->BSIM4wcdscb = 0.0;  
	    if (!model->BSIM4wcdscdGiven)
	    model->BSIM4wcdscd = 0.0;
        if (!model->BSIM4wcitGiven)
	    model->BSIM4wcit = 0.0;
        if (!model->BSIM4wnfactorGiven)
	    model->BSIM4wnfactor = 0.0;
        if (!model->BSIM4wxjGiven)
            model->BSIM4wxj = 0.0;
        if (!model->BSIM4wvsatGiven)
            model->BSIM4wvsat = 0.0;
        if (!model->BSIM4watGiven)
            model->BSIM4wat = 0.0;
        if (!model->BSIM4wa0Given)
            model->BSIM4wa0 = 0.0; 
        if (!model->BSIM4wagsGiven)
            model->BSIM4wags = 0.0;
        if (!model->BSIM4wa1Given)
            model->BSIM4wa1 = 0.0;
        if (!model->BSIM4wa2Given)
            model->BSIM4wa2 = 0.0;
        if (!model->BSIM4wketaGiven)
            model->BSIM4wketa = 0.0;
        if (!model->BSIM4wnsubGiven)
            model->BSIM4wnsub = 0.0;
        if (!model->BSIM4wndepGiven)
            model->BSIM4wndep = 0.0;
        if (!model->BSIM4wnsdGiven)
            model->BSIM4wnsd = 0.0;
        if (!model->BSIM4wphinGiven)
            model->BSIM4wphin = 0.0;
        if (!model->BSIM4wngateGiven)
            model->BSIM4wngate = 0.0;
        if (!model->BSIM4wvbmGiven)
	    model->BSIM4wvbm = 0.0;
        if (!model->BSIM4wxtGiven)
	    model->BSIM4wxt = 0.0;
        if (!model->BSIM4wkt1Given)
            model->BSIM4wkt1 = 0.0; 
        if (!model->BSIM4wkt1lGiven)
            model->BSIM4wkt1l = 0.0;
        if (!model->BSIM4wkt2Given)
            model->BSIM4wkt2 = 0.0;
        if (!model->BSIM4wk3Given)
            model->BSIM4wk3 = 0.0;      
        if (!model->BSIM4wk3bGiven)
            model->BSIM4wk3b = 0.0;      
        if (!model->BSIM4ww0Given)
            model->BSIM4ww0 = 0.0;    
        if (!model->BSIM4wlpe0Given)
            model->BSIM4wlpe0 = 0.0;
        if (!model->BSIM4wlpebGiven)
            model->BSIM4wlpeb = model->BSIM4wlpe0;
        if (!model->BSIM4wdvtp0Given)
            model->BSIM4wdvtp0 = 0.0;
        if (!model->BSIM4wdvtp1Given)
            model->BSIM4wdvtp1 = 0.0;
        if (!model->BSIM4wdvt0Given)
            model->BSIM4wdvt0 = 0.0;    
        if (!model->BSIM4wdvt1Given)
            model->BSIM4wdvt1 = 0.0;      
        if (!model->BSIM4wdvt2Given)
            model->BSIM4wdvt2 = 0.0;
        if (!model->BSIM4wdvt0wGiven)
            model->BSIM4wdvt0w = 0.0;    
        if (!model->BSIM4wdvt1wGiven)
            model->BSIM4wdvt1w = 0.0;      
        if (!model->BSIM4wdvt2wGiven)
            model->BSIM4wdvt2w = 0.0;
        if (!model->BSIM4wdroutGiven)
            model->BSIM4wdrout = 0.0;     
        if (!model->BSIM4wdsubGiven)
            model->BSIM4wdsub = 0.0;
        if (!model->BSIM4wvth0Given)
           model->BSIM4wvth0 = 0.0;
        if (!model->BSIM4wuaGiven)
            model->BSIM4wua = 0.0;
        if (!model->BSIM4wua1Given)
            model->BSIM4wua1 = 0.0;
        if (!model->BSIM4wubGiven)
            model->BSIM4wub = 0.0;
        if (!model->BSIM4wub1Given)
            model->BSIM4wub1 = 0.0;
        if (!model->BSIM4wucGiven)
            model->BSIM4wuc = 0.0;
        if (!model->BSIM4wuc1Given)
            model->BSIM4wuc1 = 0.0;
        if (!model->BSIM4wu0Given)
            model->BSIM4wu0 = 0.0;
        if (!model->BSIM4wuteGiven)
	    model->BSIM4wute = 0.0;    
        if (!model->BSIM4wvoffGiven)
	    model->BSIM4wvoff = 0.0;
        if (!model->BSIM4wminvGiven)
            model->BSIM4wminv = 0.0;
        if (!model->BSIM4wfproutGiven)
            model->BSIM4wfprout = 0.0;
        if (!model->BSIM4wpditsGiven)
            model->BSIM4wpdits = 0.0;
        if (!model->BSIM4wpditsdGiven)
            model->BSIM4wpditsd = 0.0;
        if (!model->BSIM4wdeltaGiven)  
            model->BSIM4wdelta = 0.0;
        if (!model->BSIM4wrdswGiven)
            model->BSIM4wrdsw = 0.0;
        if (!model->BSIM4wrdwGiven)
            model->BSIM4wrdw = 0.0;
        if (!model->BSIM4wrswGiven)
            model->BSIM4wrsw = 0.0;
        if (!model->BSIM4wprwbGiven)
            model->BSIM4wprwb = 0.0;
        if (!model->BSIM4wprwgGiven)
            model->BSIM4wprwg = 0.0;
        if (!model->BSIM4wprtGiven)
            model->BSIM4wprt = 0.0;
        if (!model->BSIM4weta0Given)
            model->BSIM4weta0 = 0.0;
        if (!model->BSIM4wetabGiven)
            model->BSIM4wetab = 0.0;
        if (!model->BSIM4wpclmGiven)
            model->BSIM4wpclm = 0.0; 
        if (!model->BSIM4wpdibl1Given)
            model->BSIM4wpdibl1 = 0.0;
        if (!model->BSIM4wpdibl2Given)
            model->BSIM4wpdibl2 = 0.0;
        if (!model->BSIM4wpdiblbGiven)
            model->BSIM4wpdiblb = 0.0;
        if (!model->BSIM4wpscbe1Given)
            model->BSIM4wpscbe1 = 0.0;
        if (!model->BSIM4wpscbe2Given)
            model->BSIM4wpscbe2 = 0.0;
        if (!model->BSIM4wpvagGiven)
            model->BSIM4wpvag = 0.0;     
        if (!model->BSIM4wwrGiven)  
            model->BSIM4wwr = 0.0;
        if (!model->BSIM4wdwgGiven)  
            model->BSIM4wdwg = 0.0;
        if (!model->BSIM4wdwbGiven)  
            model->BSIM4wdwb = 0.0;
        if (!model->BSIM4wb0Given)
            model->BSIM4wb0 = 0.0;
        if (!model->BSIM4wb1Given)  
            model->BSIM4wb1 = 0.0;
        if (!model->BSIM4walpha0Given)  
            model->BSIM4walpha0 = 0.0;
        if (!model->BSIM4walpha1Given)
            model->BSIM4walpha1 = 0.0;
        if (!model->BSIM4wbeta0Given)  
            model->BSIM4wbeta0 = 0.0;
        if (!model->BSIM4wagidlGiven)
            model->BSIM4wagidl = 0.0;
        if (!model->BSIM4wbgidlGiven)
            model->BSIM4wbgidl = 0.0;
        if (!model->BSIM4wcgidlGiven)
            model->BSIM4wcgidl = 0.0;
        if (!model->BSIM4wegidlGiven)
            model->BSIM4wegidl = 0.0;
        if (!model->BSIM4waigcGiven)
            model->BSIM4waigc = 0.0;
        if (!model->BSIM4wbigcGiven)
            model->BSIM4wbigc = 0.0;
        if (!model->BSIM4wcigcGiven)
            model->BSIM4wcigc = 0.0;
        if (!model->BSIM4waigsdGiven)
            model->BSIM4waigsd = 0.0;
        if (!model->BSIM4wbigsdGiven)
            model->BSIM4wbigsd = 0.0;
        if (!model->BSIM4wcigsdGiven)
            model->BSIM4wcigsd = 0.0;
        if (!model->BSIM4waigbaccGiven)
            model->BSIM4waigbacc = 0.0;
        if (!model->BSIM4wbigbaccGiven)
            model->BSIM4wbigbacc = 0.0;
        if (!model->BSIM4wcigbaccGiven)
            model->BSIM4wcigbacc = 0.0;
        if (!model->BSIM4waigbinvGiven)
            model->BSIM4waigbinv = 0.0;
        if (!model->BSIM4wbigbinvGiven)
            model->BSIM4wbigbinv = 0.0;
        if (!model->BSIM4wcigbinvGiven)
            model->BSIM4wcigbinv = 0.0;
        if (!model->BSIM4wnigcGiven)
            model->BSIM4wnigc = 0.0;
        if (!model->BSIM4wnigbinvGiven)
            model->BSIM4wnigbinv = 0.0;
        if (!model->BSIM4wnigbaccGiven)
            model->BSIM4wnigbacc = 0.0;
        if (!model->BSIM4wntoxGiven)
            model->BSIM4wntox = 0.0;
        if (!model->BSIM4weigbinvGiven)
            model->BSIM4weigbinv = 0.0;
        if (!model->BSIM4wpigcdGiven)
            model->BSIM4wpigcd = 0.0;
        if (!model->BSIM4wpoxedgeGiven)
            model->BSIM4wpoxedge = 0.0;
        if (!model->BSIM4wxrcrg1Given)
            model->BSIM4wxrcrg1 = 0.0;
        if (!model->BSIM4wxrcrg2Given)
            model->BSIM4wxrcrg2 = 0.0;
        if (!model->BSIM4weuGiven)
            model->BSIM4weu = 0.0;
        if (!model->BSIM4wvfbGiven)
            model->BSIM4wvfb = 0.0;

        if (!model->BSIM4wcgslGiven)  
            model->BSIM4wcgsl = 0.0;
        if (!model->BSIM4wcgdlGiven)  
            model->BSIM4wcgdl = 0.0;
        if (!model->BSIM4wckappasGiven)  
            model->BSIM4wckappas = 0.0;
        if (!model->BSIM4wckappadGiven)
            model->BSIM4wckappad = 0.0;
        if (!model->BSIM4wcfGiven)  
            model->BSIM4wcf = 0.0;
        if (!model->BSIM4wclcGiven)  
            model->BSIM4wclc = 0.0;
        if (!model->BSIM4wcleGiven)  
            model->BSIM4wcle = 0.0;
        if (!model->BSIM4wvfbcvGiven)  
            model->BSIM4wvfbcv = 0.0;
        if (!model->BSIM4wacdeGiven)
            model->BSIM4wacde = 0.0;
        if (!model->BSIM4wmoinGiven)
            model->BSIM4wmoin = 0.0;
        if (!model->BSIM4wnoffGiven)
            model->BSIM4wnoff = 0.0;
        if (!model->BSIM4wvoffcvGiven)
            model->BSIM4wvoffcv = 0.0;

	/* Cross-term dependence */
        if (!model->BSIM4pcdscGiven)
	    model->BSIM4pcdsc = 0.0;
        if (!model->BSIM4pcdscbGiven)
	    model->BSIM4pcdscb = 0.0;   
	    if (!model->BSIM4pcdscdGiven)
	    model->BSIM4pcdscd = 0.0;
        if (!model->BSIM4pcitGiven)
	    model->BSIM4pcit = 0.0;
        if (!model->BSIM4pnfactorGiven)
	    model->BSIM4pnfactor = 0.0;
        if (!model->BSIM4pxjGiven)
            model->BSIM4pxj = 0.0;
        if (!model->BSIM4pvsatGiven)
            model->BSIM4pvsat = 0.0;
        if (!model->BSIM4patGiven)
            model->BSIM4pat = 0.0;
        if (!model->BSIM4pa0Given)
            model->BSIM4pa0 = 0.0; 
            
        if (!model->BSIM4pagsGiven)
            model->BSIM4pags = 0.0;
        if (!model->BSIM4pa1Given)
            model->BSIM4pa1 = 0.0;
        if (!model->BSIM4pa2Given)
            model->BSIM4pa2 = 0.0;
        if (!model->BSIM4pketaGiven)
            model->BSIM4pketa = 0.0;
        if (!model->BSIM4pnsubGiven)
            model->BSIM4pnsub = 0.0;
        if (!model->BSIM4pndepGiven)
            model->BSIM4pndep = 0.0;
        if (!model->BSIM4pnsdGiven)
            model->BSIM4pnsd = 0.0;
        if (!model->BSIM4pphinGiven)
            model->BSIM4pphin = 0.0;
        if (!model->BSIM4pngateGiven)
            model->BSIM4pngate = 0.0;
        if (!model->BSIM4pvbmGiven)
	    model->BSIM4pvbm = 0.0;
        if (!model->BSIM4pxtGiven)
	    model->BSIM4pxt = 0.0;
        if (!model->BSIM4pkt1Given)
            model->BSIM4pkt1 = 0.0; 
        if (!model->BSIM4pkt1lGiven)
            model->BSIM4pkt1l = 0.0;
        if (!model->BSIM4pkt2Given)
            model->BSIM4pkt2 = 0.0;
        if (!model->BSIM4pk3Given)
            model->BSIM4pk3 = 0.0;      
        if (!model->BSIM4pk3bGiven)
            model->BSIM4pk3b = 0.0;      
        if (!model->BSIM4pw0Given)
            model->BSIM4pw0 = 0.0;    
        if (!model->BSIM4plpe0Given)
            model->BSIM4plpe0 = 0.0;
        if (!model->BSIM4plpebGiven)
            model->BSIM4plpeb = model->BSIM4plpe0;
        if (!model->BSIM4pdvtp0Given)
            model->BSIM4pdvtp0 = 0.0;
        if (!model->BSIM4pdvtp1Given)
            model->BSIM4pdvtp1 = 0.0;
        if (!model->BSIM4pdvt0Given)
            model->BSIM4pdvt0 = 0.0;    
        if (!model->BSIM4pdvt1Given)
            model->BSIM4pdvt1 = 0.0;      
        if (!model->BSIM4pdvt2Given)
            model->BSIM4pdvt2 = 0.0;
        if (!model->BSIM4pdvt0wGiven)
            model->BSIM4pdvt0w = 0.0;    
        if (!model->BSIM4pdvt1wGiven)
            model->BSIM4pdvt1w = 0.0;      
        if (!model->BSIM4pdvt2wGiven)
            model->BSIM4pdvt2w = 0.0;
        if (!model->BSIM4pdroutGiven)
            model->BSIM4pdrout = 0.0;     
        if (!model->BSIM4pdsubGiven)
            model->BSIM4pdsub = 0.0;
        if (!model->BSIM4pvth0Given)
           model->BSIM4pvth0 = 0.0;
        if (!model->BSIM4puaGiven)
            model->BSIM4pua = 0.0;
        if (!model->BSIM4pua1Given)
            model->BSIM4pua1 = 0.0;
        if (!model->BSIM4pubGiven)
            model->BSIM4pub = 0.0;
        if (!model->BSIM4pub1Given)
            model->BSIM4pub1 = 0.0;
        if (!model->BSIM4pucGiven)
            model->BSIM4puc = 0.0;
        if (!model->BSIM4puc1Given)
            model->BSIM4puc1 = 0.0;
        if (!model->BSIM4pu0Given)
            model->BSIM4pu0 = 0.0;
        if (!model->BSIM4puteGiven)
	    model->BSIM4pute = 0.0;    
        if (!model->BSIM4pvoffGiven)
	    model->BSIM4pvoff = 0.0;
        if (!model->BSIM4pminvGiven)
            model->BSIM4pminv = 0.0;
        if (!model->BSIM4pfproutGiven)
            model->BSIM4pfprout = 0.0;
        if (!model->BSIM4ppditsGiven)
            model->BSIM4ppdits = 0.0;
        if (!model->BSIM4ppditsdGiven)
            model->BSIM4ppditsd = 0.0;
        if (!model->BSIM4pdeltaGiven)  
            model->BSIM4pdelta = 0.0;
        if (!model->BSIM4prdswGiven)
            model->BSIM4prdsw = 0.0;
        if (!model->BSIM4prdwGiven)
            model->BSIM4prdw = 0.0;
        if (!model->BSIM4prswGiven)
            model->BSIM4prsw = 0.0;
        if (!model->BSIM4pprwbGiven)
            model->BSIM4pprwb = 0.0;
        if (!model->BSIM4pprwgGiven)
            model->BSIM4pprwg = 0.0;
        if (!model->BSIM4pprtGiven)
            model->BSIM4pprt = 0.0;
        if (!model->BSIM4peta0Given)
            model->BSIM4peta0 = 0.0;
        if (!model->BSIM4petabGiven)
            model->BSIM4petab = 0.0;
        if (!model->BSIM4ppclmGiven)
            model->BSIM4ppclm = 0.0; 
        if (!model->BSIM4ppdibl1Given)
            model->BSIM4ppdibl1 = 0.0;
        if (!model->BSIM4ppdibl2Given)
            model->BSIM4ppdibl2 = 0.0;
        if (!model->BSIM4ppdiblbGiven)
            model->BSIM4ppdiblb = 0.0;
        if (!model->BSIM4ppscbe1Given)
            model->BSIM4ppscbe1 = 0.0;
        if (!model->BSIM4ppscbe2Given)
            model->BSIM4ppscbe2 = 0.0;
        if (!model->BSIM4ppvagGiven)
            model->BSIM4ppvag = 0.0;     
        if (!model->BSIM4pwrGiven)  
            model->BSIM4pwr = 0.0;
        if (!model->BSIM4pdwgGiven)  
            model->BSIM4pdwg = 0.0;
        if (!model->BSIM4pdwbGiven)  
            model->BSIM4pdwb = 0.0;
        if (!model->BSIM4pb0Given)
            model->BSIM4pb0 = 0.0;
        if (!model->BSIM4pb1Given)  
            model->BSIM4pb1 = 0.0;
        if (!model->BSIM4palpha0Given)  
            model->BSIM4palpha0 = 0.0;
        if (!model->BSIM4palpha1Given)
            model->BSIM4palpha1 = 0.0;
        if (!model->BSIM4pbeta0Given)  
            model->BSIM4pbeta0 = 0.0;
        if (!model->BSIM4pagidlGiven)
            model->BSIM4pagidl = 0.0;
        if (!model->BSIM4pbgidlGiven)
            model->BSIM4pbgidl = 0.0;
        if (!model->BSIM4pcgidlGiven)
            model->BSIM4pcgidl = 0.0;
        if (!model->BSIM4pegidlGiven)
            model->BSIM4pegidl = 0.0;
        if (!model->BSIM4paigcGiven)
            model->BSIM4paigc = 0.0;
        if (!model->BSIM4pbigcGiven)
            model->BSIM4pbigc = 0.0;
        if (!model->BSIM4pcigcGiven)
            model->BSIM4pcigc = 0.0;
        if (!model->BSIM4paigsdGiven)
            model->BSIM4paigsd = 0.0;
        if (!model->BSIM4pbigsdGiven)
            model->BSIM4pbigsd = 0.0;
        if (!model->BSIM4pcigsdGiven)
            model->BSIM4pcigsd = 0.0;
        if (!model->BSIM4paigbaccGiven)
            model->BSIM4paigbacc = 0.0;
        if (!model->BSIM4pbigbaccGiven)
            model->BSIM4pbigbacc = 0.0;
        if (!model->BSIM4pcigbaccGiven)
            model->BSIM4pcigbacc = 0.0;
        if (!model->BSIM4paigbinvGiven)
            model->BSIM4paigbinv = 0.0;
        if (!model->BSIM4pbigbinvGiven)
            model->BSIM4pbigbinv = 0.0;
        if (!model->BSIM4pcigbinvGiven)
            model->BSIM4pcigbinv = 0.0;
        if (!model->BSIM4pnigcGiven)
            model->BSIM4pnigc = 0.0;
        if (!model->BSIM4pnigbinvGiven)
            model->BSIM4pnigbinv = 0.0;
        if (!model->BSIM4pnigbaccGiven)
            model->BSIM4pnigbacc = 0.0;
        if (!model->BSIM4pntoxGiven)
            model->BSIM4pntox = 0.0;
        if (!model->BSIM4peigbinvGiven)
            model->BSIM4peigbinv = 0.0;
        if (!model->BSIM4ppigcdGiven)
            model->BSIM4ppigcd = 0.0;
        if (!model->BSIM4ppoxedgeGiven)
            model->BSIM4ppoxedge = 0.0;
        if (!model->BSIM4pxrcrg1Given)
            model->BSIM4pxrcrg1 = 0.0;
        if (!model->BSIM4pxrcrg2Given)
            model->BSIM4pxrcrg2 = 0.0;
        if (!model->BSIM4peuGiven)
            model->BSIM4peu = 0.0;
        if (!model->BSIM4pvfbGiven)
            model->BSIM4pvfb = 0.0;

        if (!model->BSIM4pcgslGiven)  
            model->BSIM4pcgsl = 0.0;
        if (!model->BSIM4pcgdlGiven)  
            model->BSIM4pcgdl = 0.0;
        if (!model->BSIM4pckappasGiven)  
            model->BSIM4pckappas = 0.0;
        if (!model->BSIM4pckappadGiven)
            model->BSIM4pckappad = 0.0;
        if (!model->BSIM4pcfGiven)  
            model->BSIM4pcf = 0.0;
        if (!model->BSIM4pclcGiven)  
            model->BSIM4pclc = 0.0;
        if (!model->BSIM4pcleGiven)  
            model->BSIM4pcle = 0.0;
        if (!model->BSIM4pvfbcvGiven)  
            model->BSIM4pvfbcv = 0.0;
        if (!model->BSIM4pacdeGiven)
            model->BSIM4pacde = 0.0;
        if (!model->BSIM4pmoinGiven)
            model->BSIM4pmoin = 0.0;
        if (!model->BSIM4pnoffGiven)
            model->BSIM4pnoff = 0.0;
        if (!model->BSIM4pvoffcvGiven)
            model->BSIM4pvoffcv = 0.0;

        /* unit degree celcius */
        if (!model->BSIM4tnomGiven)  
	    model->BSIM4tnom = ckt->CKTnomTemp; 
        if (!model->BSIM4LintGiven)  
           model->BSIM4Lint = 0.0;
        if (!model->BSIM4LlGiven)  
           model->BSIM4Ll = 0.0;
        if (!model->BSIM4LlcGiven)
           model->BSIM4Llc = model->BSIM4Ll;
        if (!model->BSIM4LlnGiven)  
           model->BSIM4Lln = 1.0;
        if (!model->BSIM4LwGiven)  
           model->BSIM4Lw = 0.0;
        if (!model->BSIM4LwcGiven)
           model->BSIM4Lwc = model->BSIM4Lw;
        if (!model->BSIM4LwnGiven)  
           model->BSIM4Lwn = 1.0;
        if (!model->BSIM4LwlGiven)  
           model->BSIM4Lwl = 0.0;
        if (!model->BSIM4LwlcGiven)
           model->BSIM4Lwlc = model->BSIM4Lwl;
        if (!model->BSIM4LminGiven)  
           model->BSIM4Lmin = 0.0;
        if (!model->BSIM4LmaxGiven)  
           model->BSIM4Lmax = 1.0;
        if (!model->BSIM4WintGiven)  
           model->BSIM4Wint = 0.0;
        if (!model->BSIM4WlGiven)  
           model->BSIM4Wl = 0.0;
        if (!model->BSIM4WlcGiven)
           model->BSIM4Wlc = model->BSIM4Wl;
        if (!model->BSIM4WlnGiven)  
           model->BSIM4Wln = 1.0;
        if (!model->BSIM4WwGiven)  
           model->BSIM4Ww = 0.0;
        if (!model->BSIM4WwcGiven)
           model->BSIM4Wwc = model->BSIM4Ww;
        if (!model->BSIM4WwnGiven)  
           model->BSIM4Wwn = 1.0;
        if (!model->BSIM4WwlGiven)  
           model->BSIM4Wwl = 0.0;
        if (!model->BSIM4WwlcGiven)
           model->BSIM4Wwlc = model->BSIM4Wwl;
        if (!model->BSIM4WminGiven)  
           model->BSIM4Wmin = 0.0;
        if (!model->BSIM4WmaxGiven)  
           model->BSIM4Wmax = 1.0;
        if (!model->BSIM4dwcGiven)  
           model->BSIM4dwc = model->BSIM4Wint;
        if (!model->BSIM4dlcGiven)  
           model->BSIM4dlc = model->BSIM4Lint;
        if (!model->BSIM4dlcigGiven)
           model->BSIM4dlcig = model->BSIM4Lint;
        if (!model->BSIM4dwjGiven)
           model->BSIM4dwj = model->BSIM4dwc;
	if (!model->BSIM4cfGiven)
           model->BSIM4cf = 2.0 * model->BSIM4epsrox * EPS0 / PI
		          * log(1.0 + 0.4e-6 / model->BSIM4toxe);

        if (!model->BSIM4xpartGiven)
            model->BSIM4xpart = 0.0;
        if (!model->BSIM4sheetResistanceGiven)
            model->BSIM4sheetResistance = 0.0;

        if (!model->BSIM4SunitAreaJctCapGiven)
            model->BSIM4SunitAreaJctCap = 5.0E-4;
        if (!model->BSIM4DunitAreaJctCapGiven)
            model->BSIM4DunitAreaJctCap = model->BSIM4SunitAreaJctCap;
        if (!model->BSIM4SunitLengthSidewallJctCapGiven)
            model->BSIM4SunitLengthSidewallJctCap = 5.0E-10;
        if (!model->BSIM4DunitLengthSidewallJctCapGiven)
            model->BSIM4DunitLengthSidewallJctCap = model->BSIM4SunitLengthSidewallJctCap;
        if (!model->BSIM4SunitLengthGateSidewallJctCapGiven)
            model->BSIM4SunitLengthGateSidewallJctCap = model->BSIM4SunitLengthSidewallJctCap ;
        if (!model->BSIM4DunitLengthGateSidewallJctCapGiven)
            model->BSIM4DunitLengthGateSidewallJctCap = model->BSIM4SunitLengthGateSidewallJctCap;
        if (!model->BSIM4SjctSatCurDensityGiven)
            model->BSIM4SjctSatCurDensity = 1.0E-4;
        if (!model->BSIM4DjctSatCurDensityGiven)
            model->BSIM4DjctSatCurDensity = model->BSIM4SjctSatCurDensity;
        if (!model->BSIM4SjctSidewallSatCurDensityGiven)
            model->BSIM4SjctSidewallSatCurDensity = 0.0;
        if (!model->BSIM4DjctSidewallSatCurDensityGiven)
            model->BSIM4DjctSidewallSatCurDensity = model->BSIM4SjctSidewallSatCurDensity;
        if (!model->BSIM4SjctGateSidewallSatCurDensityGiven)
            model->BSIM4SjctGateSidewallSatCurDensity = 0.0;
        if (!model->BSIM4DjctGateSidewallSatCurDensityGiven)
            model->BSIM4DjctGateSidewallSatCurDensity = model->BSIM4SjctGateSidewallSatCurDensity;
        if (!model->BSIM4SbulkJctPotentialGiven)
            model->BSIM4SbulkJctPotential = 1.0;
        if (!model->BSIM4DbulkJctPotentialGiven)
            model->BSIM4DbulkJctPotential = model->BSIM4SbulkJctPotential;
        if (!model->BSIM4SsidewallJctPotentialGiven)
            model->BSIM4SsidewallJctPotential = 1.0;
        if (!model->BSIM4DsidewallJctPotentialGiven)
            model->BSIM4DsidewallJctPotential = model->BSIM4SsidewallJctPotential;
        if (!model->BSIM4SGatesidewallJctPotentialGiven)
            model->BSIM4SGatesidewallJctPotential = model->BSIM4SsidewallJctPotential;
        if (!model->BSIM4DGatesidewallJctPotentialGiven)
            model->BSIM4DGatesidewallJctPotential = model->BSIM4SGatesidewallJctPotential;
        if (!model->BSIM4SbulkJctBotGradingCoeffGiven)
            model->BSIM4SbulkJctBotGradingCoeff = 0.5;
        if (!model->BSIM4DbulkJctBotGradingCoeffGiven)
            model->BSIM4DbulkJctBotGradingCoeff = model->BSIM4SbulkJctBotGradingCoeff;
        if (!model->BSIM4SbulkJctSideGradingCoeffGiven)
            model->BSIM4SbulkJctSideGradingCoeff = 0.33;
        if (!model->BSIM4DbulkJctSideGradingCoeffGiven)
            model->BSIM4DbulkJctSideGradingCoeff = model->BSIM4SbulkJctSideGradingCoeff;
        if (!model->BSIM4SbulkJctGateSideGradingCoeffGiven)
            model->BSIM4SbulkJctGateSideGradingCoeff = model->BSIM4SbulkJctSideGradingCoeff;
        if (!model->BSIM4DbulkJctGateSideGradingCoeffGiven)
            model->BSIM4DbulkJctGateSideGradingCoeff = model->BSIM4SbulkJctGateSideGradingCoeff;
        if (!model->BSIM4SjctEmissionCoeffGiven)
            model->BSIM4SjctEmissionCoeff = 1.0;
        if (!model->BSIM4DjctEmissionCoeffGiven)
            model->BSIM4DjctEmissionCoeff = model->BSIM4SjctEmissionCoeff;
        if (!model->BSIM4SjctTempExponentGiven)
            model->BSIM4SjctTempExponent = 3.0;
        if (!model->BSIM4DjctTempExponentGiven)
            model->BSIM4DjctTempExponent = model->BSIM4SjctTempExponent;

        if (!model->BSIM4oxideTrapDensityAGiven)
	{   if (model->BSIM4type == NMOS)
                model->BSIM4oxideTrapDensityA = 6.25e41;
            else
                model->BSIM4oxideTrapDensityA= 6.188e40;
	}
        if (!model->BSIM4oxideTrapDensityBGiven)
	{   if (model->BSIM4type == NMOS)
                model->BSIM4oxideTrapDensityB = 3.125e26;
            else
                model->BSIM4oxideTrapDensityB = 1.5e25;
	}
        if (!model->BSIM4oxideTrapDensityCGiven)
            model->BSIM4oxideTrapDensityC = 8.75e9;
        if (!model->BSIM4emGiven)
            model->BSIM4em = 4.1e7; /* V/m */
        if (!model->BSIM4efGiven)
            model->BSIM4ef = 1.0;
        if (!model->BSIM4afGiven)
            model->BSIM4af = 1.0;
        if (!model->BSIM4kfGiven)
            model->BSIM4kf = 0.0;

	/*
         * End processing models and begin to loop
         * through all the instances of the model
         */

        for (here = model->BSIM4instances; here != NULL ;
             here=here->BSIM4nextInstance) 
	{   
	    if (here->BSIM4owner == ARCHme) {
	    /* allocate a chunk of the state vector */
            here->BSIM4states = *states;
            *states += BSIM4numStates;
            }
	    /* perform the parameter defaulting */
            if (!here->BSIM4lGiven)
                here->BSIM4l = 5.0e-6;
            if (!here->BSIM4wGiven)
                here->BSIM4w = 5.0e-6;
            if (!here->BSIM4nfGiven)
                here->BSIM4nf = 1.0;
            if (!here->BSIM4minGiven)
                here->BSIM4min = 0; /* integer */
            if (!here->BSIM4icVDSGiven)
                here->BSIM4icVDS = 0.0;
            if (!here->BSIM4icVGSGiven)
                here->BSIM4icVGS = 0.0;
            if (!here->BSIM4icVBSGiven)
                here->BSIM4icVBS = 0.0;
            if (!here->BSIM4drainAreaGiven)
                here->BSIM4drainArea = 0.0;
            if (!here->BSIM4drainPerimeterGiven)
                here->BSIM4drainPerimeter = 0.0;
            if (!here->BSIM4drainSquaresGiven)
                here->BSIM4drainSquares = 1.0;
            if (!here->BSIM4sourceAreaGiven)
                here->BSIM4sourceArea = 0.0;
            if (!here->BSIM4sourcePerimeterGiven)
                here->BSIM4sourcePerimeter = 0.0;
            if (!here->BSIM4sourceSquaresGiven)
                here->BSIM4sourceSquares = 1.0;

            if (!here->BSIM4rbdbGiven)
                here->BSIM4rbdb = model->BSIM4rbdb; /* in ohm */
            if (!here->BSIM4rbsbGiven)
                here->BSIM4rbsb = model->BSIM4rbsb;
            if (!here->BSIM4rbpbGiven)
                here->BSIM4rbpb = model->BSIM4rbpb;
            if (!here->BSIM4rbpsGiven)
                here->BSIM4rbps = model->BSIM4rbps;
            if (!here->BSIM4rbpdGiven)
                here->BSIM4rbpd = model->BSIM4rbpd;

                    
            /* Process instance model selectors, some
             * may override their global counterparts
	     */
            if (!here->BSIM4rbodyModGiven)
                here->BSIM4rbodyMod = model->BSIM4rbodyMod;
            else if ((here->BSIM4rbodyMod != 0) && (here->BSIM4rbodyMod != 1))
            {   here->BSIM4rbodyMod = model->BSIM4rbodyMod;
                printf("Warning: rbodyMod has been set to its global value %d.\n",
	        model->BSIM4rbodyMod);
            }

            if (!here->BSIM4rgateModGiven)
                here->BSIM4rgateMod = model->BSIM4rgateMod;
            else if ((here->BSIM4rgateMod != 0) && (here->BSIM4rgateMod != 1)
	        && (here->BSIM4rgateMod != 2) && (here->BSIM4rgateMod != 3))
            {   here->BSIM4rgateMod = model->BSIM4rgateMod;
                printf("Warning: rgateMod has been set to its global value %d.\n",
                model->BSIM4rgateMod);
            }

            if (!here->BSIM4geoModGiven)
                here->BSIM4geoMod = model->BSIM4geoMod;
            if (!here->BSIM4rgeoModGiven)
                here->BSIM4rgeoMod = 0.0;
            if (!here->BSIM4trnqsModGiven)
                here->BSIM4trnqsMod = model->BSIM4trnqsMod;
            else if ((here->BSIM4trnqsMod != 0) && (here->BSIM4trnqsMod != 1))
            {   here->BSIM4trnqsMod = model->BSIM4trnqsMod;
                printf("Warning: trnqsMod has been set to its global value %d.\n",
                model->BSIM4trnqsMod);
            }

            if (!here->BSIM4acnqsModGiven)
                here->BSIM4acnqsMod = model->BSIM4acnqsMod;
            else if ((here->BSIM4acnqsMod != 0) && (here->BSIM4acnqsMod != 1))
            {   here->BSIM4acnqsMod = model->BSIM4acnqsMod;
                printf("Warning: acnqsMod has been set to its global value %d.\n",
                model->BSIM4acnqsMod);
            }

            /* process drain series resistance */
            if (((here->BSIM4rgeoMod != 0) || (model->BSIM4rdsMod != 0)
                || (model->BSIM4tnoiMod != 0)) && (here->BSIM4dNodePrime == 0))
            {   error = CKTmkVolt(ckt,&tmp,here->BSIM4name,"drain");
                if(error) return(error);
                here->BSIM4dNodePrime = tmp->number;
            }
            else
            {   here->BSIM4dNodePrime = here->BSIM4dNode;
            }
            /* process source series resistance */
            if (((here->BSIM4rgeoMod != 0) || (model->BSIM4rdsMod != 0)
		|| (model->BSIM4tnoiMod != 0)) && (here->BSIM4sNodePrime == 0))
            {   error = CKTmkVolt(ckt,&tmp,here->BSIM4name,"source");
                if(error) return(error);
                here->BSIM4sNodePrime = tmp->number;
            }
            else
                here->BSIM4sNodePrime = here->BSIM4sNode;
            

            if ((here->BSIM4rgateMod > 0) && (here->BSIM4gNodePrime == 0))
            {   error = CKTmkVolt(ckt,&tmp,here->BSIM4name,"gate");
                if(error) return(error);
                   here->BSIM4gNodePrime = tmp->number;
                if (here->BSIM4rgateMod == 1)
                {   if (model->BSIM4rshg <= 0.0)
	                printf("Warning: rshg should be positive for rgateMod = 1.\n");
		}
                else if (here->BSIM4rgateMod == 2)
                {   if (model->BSIM4rshg <= 0.0)
                        printf("Warning: rshg <= 0.0 for rgateMod = 2!!!\n");
                    else if (model->BSIM4xrcrg1 <= 0.0)
                        printf("Warning: xrcrg1 <= 0.0 for rgateMod = 2!!!\n");
                }
            }
            else
                here->BSIM4gNodePrime = here->BSIM4gNodeExt;

            if ((here->BSIM4rgateMod == 3) && (here->BSIM4gNodeMid == 0))
            {   error = CKTmkVolt(ckt,&tmp,here->BSIM4name,"midgate");
                if(error) return(error);
                   here->BSIM4gNodeMid = tmp->number;
                if (model->BSIM4rshg <= 0.0)
                    printf("Warning: rshg should be positive for rgateMod = 3.\n");
                else if (model->BSIM4xrcrg1 <= 0.0)
                    printf("Warning: xrcrg1 should be positive for rgateMod = 3.\n");
            }
            else
                here->BSIM4gNodeMid = here->BSIM4gNodeExt;
            

            /* internal body nodes for body resistance model */
            if (here->BSIM4rbodyMod)
            {   if (here->BSIM4dbNode == 0)
		{   error = CKTmkVolt(ckt,&tmp,here->BSIM4name,"dbody");
                    if(error) return(error);
                    here->BSIM4dbNode = tmp->number;
		}
		if (here->BSIM4bNodePrime == 0)
                {   error = CKTmkVolt(ckt,&tmp,here->BSIM4name,"body");
                    if(error) return(error);
                    here->BSIM4bNodePrime = tmp->number;
                }
		if (here->BSIM4sbNode == 0)
                {   error = CKTmkVolt(ckt,&tmp,here->BSIM4name,"sbody");
                    if(error) return(error);
                    here->BSIM4sbNode = tmp->number;
                }
            }
	    else
	        here->BSIM4dbNode = here->BSIM4bNodePrime = here->BSIM4sbNode
				  = here->BSIM4bNode;

            /* NQS node */
            if ((here->BSIM4trnqsMod) && (here->BSIM4qNode == 0)) 
	    {   error = CKTmkVolt(ckt,&tmp,here->BSIM4name,"charge");
                if(error) return(error);
                here->BSIM4qNode = tmp->number;
            }
	    else 
	        here->BSIM4qNode = 0;

/* set Sparse Matrix Pointers 
 * macro to make elements with built-in out-of-memory test */
#define TSTALLOC(ptr,first,second) \
if((here->ptr = SMPmakeElt(matrix,here->first,here->second))==(double *)NULL){\
    return(E_NOMEM);\
}

            TSTALLOC(BSIM4DPbpPtr, BSIM4dNodePrime, BSIM4bNodePrime)
            TSTALLOC(BSIM4GPbpPtr, BSIM4gNodePrime, BSIM4bNodePrime)
            TSTALLOC(BSIM4SPbpPtr, BSIM4sNodePrime, BSIM4bNodePrime)

            TSTALLOC(BSIM4BPdpPtr, BSIM4bNodePrime, BSIM4dNodePrime)
            TSTALLOC(BSIM4BPgpPtr, BSIM4bNodePrime, BSIM4gNodePrime)
            TSTALLOC(BSIM4BPspPtr, BSIM4bNodePrime, BSIM4sNodePrime)
            TSTALLOC(BSIM4BPbpPtr, BSIM4bNodePrime, BSIM4bNodePrime)

            TSTALLOC(BSIM4DdPtr, BSIM4dNode, BSIM4dNode)
            TSTALLOC(BSIM4GPgpPtr, BSIM4gNodePrime, BSIM4gNodePrime)
            TSTALLOC(BSIM4SsPtr, BSIM4sNode, BSIM4sNode)
            TSTALLOC(BSIM4DPdpPtr, BSIM4dNodePrime, BSIM4dNodePrime)
            TSTALLOC(BSIM4SPspPtr, BSIM4sNodePrime, BSIM4sNodePrime)
            TSTALLOC(BSIM4DdpPtr, BSIM4dNode, BSIM4dNodePrime)
            TSTALLOC(BSIM4GPdpPtr, BSIM4gNodePrime, BSIM4dNodePrime)
            TSTALLOC(BSIM4GPspPtr, BSIM4gNodePrime, BSIM4sNodePrime)
            TSTALLOC(BSIM4SspPtr, BSIM4sNode, BSIM4sNodePrime)
            TSTALLOC(BSIM4DPspPtr, BSIM4dNodePrime, BSIM4sNodePrime)
            TSTALLOC(BSIM4DPdPtr, BSIM4dNodePrime, BSIM4dNode)
            TSTALLOC(BSIM4DPgpPtr, BSIM4dNodePrime, BSIM4gNodePrime)
            TSTALLOC(BSIM4SPgpPtr, BSIM4sNodePrime, BSIM4gNodePrime)
            TSTALLOC(BSIM4SPsPtr, BSIM4sNodePrime, BSIM4sNode)
            TSTALLOC(BSIM4SPdpPtr, BSIM4sNodePrime, BSIM4dNodePrime)

            TSTALLOC(BSIM4QqPtr, BSIM4qNode, BSIM4qNode)
            TSTALLOC(BSIM4QbpPtr, BSIM4qNode, BSIM4bNodePrime) 
            TSTALLOC(BSIM4QdpPtr, BSIM4qNode, BSIM4dNodePrime)
            TSTALLOC(BSIM4QspPtr, BSIM4qNode, BSIM4sNodePrime)
            TSTALLOC(BSIM4QgpPtr, BSIM4qNode, BSIM4gNodePrime)
            TSTALLOC(BSIM4DPqPtr, BSIM4dNodePrime, BSIM4qNode)
            TSTALLOC(BSIM4SPqPtr, BSIM4sNodePrime, BSIM4qNode)
            TSTALLOC(BSIM4GPqPtr, BSIM4gNodePrime, BSIM4qNode)

            if (here->BSIM4rgateMod != 0)
            {   TSTALLOC(BSIM4GEgePtr, BSIM4gNodeExt, BSIM4gNodeExt)
                TSTALLOC(BSIM4GEgpPtr, BSIM4gNodeExt, BSIM4gNodePrime)
                TSTALLOC(BSIM4GPgePtr, BSIM4gNodePrime, BSIM4gNodeExt)
		TSTALLOC(BSIM4GEdpPtr, BSIM4gNodeExt, BSIM4dNodePrime)
		TSTALLOC(BSIM4GEspPtr, BSIM4gNodeExt, BSIM4sNodePrime)
		TSTALLOC(BSIM4GEbpPtr, BSIM4gNodeExt, BSIM4bNodePrime)

                TSTALLOC(BSIM4GMdpPtr, BSIM4gNodeMid, BSIM4dNodePrime)
                TSTALLOC(BSIM4GMgpPtr, BSIM4gNodeMid, BSIM4gNodePrime)
                TSTALLOC(BSIM4GMgmPtr, BSIM4gNodeMid, BSIM4gNodeMid)
                TSTALLOC(BSIM4GMgePtr, BSIM4gNodeMid, BSIM4gNodeExt)
                TSTALLOC(BSIM4GMspPtr, BSIM4gNodeMid, BSIM4sNodePrime)
                TSTALLOC(BSIM4GMbpPtr, BSIM4gNodeMid, BSIM4bNodePrime)
                TSTALLOC(BSIM4DPgmPtr, BSIM4dNodePrime, BSIM4gNodeMid)
                TSTALLOC(BSIM4GPgmPtr, BSIM4gNodePrime, BSIM4gNodeMid)
                TSTALLOC(BSIM4GEgmPtr, BSIM4gNodeExt, BSIM4gNodeMid)
                TSTALLOC(BSIM4SPgmPtr, BSIM4sNodePrime, BSIM4gNodeMid)
                TSTALLOC(BSIM4BPgmPtr, BSIM4bNodePrime, BSIM4gNodeMid)
            }	

            if (here->BSIM4rbodyMod)
            {   TSTALLOC(BSIM4DPdbPtr, BSIM4dNodePrime, BSIM4dbNode)
                TSTALLOC(BSIM4SPsbPtr, BSIM4sNodePrime, BSIM4sbNode)

                TSTALLOC(BSIM4DBdpPtr, BSIM4dbNode, BSIM4dNodePrime)
                TSTALLOC(BSIM4DBdbPtr, BSIM4dbNode, BSIM4dbNode)
                TSTALLOC(BSIM4DBbpPtr, BSIM4dbNode, BSIM4bNodePrime)
                TSTALLOC(BSIM4DBbPtr, BSIM4dbNode, BSIM4bNode)

                TSTALLOC(BSIM4BPdbPtr, BSIM4bNodePrime, BSIM4dbNode)
                TSTALLOC(BSIM4BPbPtr, BSIM4bNodePrime, BSIM4bNode)
                TSTALLOC(BSIM4BPsbPtr, BSIM4bNodePrime, BSIM4sbNode)

                TSTALLOC(BSIM4SBspPtr, BSIM4sbNode, BSIM4sNodePrime)
                TSTALLOC(BSIM4SBbpPtr, BSIM4sbNode, BSIM4bNodePrime)
                TSTALLOC(BSIM4SBbPtr, BSIM4sbNode, BSIM4bNode)
                TSTALLOC(BSIM4SBsbPtr, BSIM4sbNode, BSIM4sbNode)

                TSTALLOC(BSIM4BdbPtr, BSIM4bNode, BSIM4dbNode)
                TSTALLOC(BSIM4BbpPtr, BSIM4bNode, BSIM4bNodePrime)
                TSTALLOC(BSIM4BsbPtr, BSIM4bNode, BSIM4sbNode)
	        TSTALLOC(BSIM4BbPtr, BSIM4bNode, BSIM4bNode)
	    }

            if (model->BSIM4rdsMod)
            {   TSTALLOC(BSIM4DgpPtr, BSIM4dNode, BSIM4gNodePrime)
		TSTALLOC(BSIM4DspPtr, BSIM4dNode, BSIM4sNodePrime)
                TSTALLOC(BSIM4DbpPtr, BSIM4dNode, BSIM4bNodePrime)
                TSTALLOC(BSIM4SdpPtr, BSIM4sNode, BSIM4dNodePrime)
                TSTALLOC(BSIM4SgpPtr, BSIM4sNode, BSIM4gNodePrime)
                TSTALLOC(BSIM4SbpPtr, BSIM4sNode, BSIM4bNodePrime)
            }
        }
    }
    return(OK);
}  

int
BSIM4unsetup(inModel,ckt)
    GENmodel *inModel;
    CKTcircuit *ckt;
{
#ifndef HAS_BATCHSIM
    BSIM4model *model;
    BSIM4instance *here;

    for (model = (BSIM4model *)inModel; model != NULL;
            model = model->BSIM4nextModel)
    {
        for (here = model->BSIM4instances; here != NULL;
                here=here->BSIM4nextInstance)
        {
            if (here->BSIM4dNodePrime
                    && here->BSIM4dNodePrime != here->BSIM4dNode)
            {
                CKTdltNNum(ckt, here->BSIM4dNodePrime);
                here->BSIM4dNodePrime = 0;
            }
            if (here->BSIM4sNodePrime
                    && here->BSIM4sNodePrime != here->BSIM4sNode)
            {
                CKTdltNNum(ckt, here->BSIM4sNodePrime);
                here->BSIM4sNodePrime = 0;
            }
        }
    }
#endif
    return OK;
}
