/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/

#ifndef INP
#define INP

    /* structure declarations used by either/both input package */

#include "ifsim.h"
#include "gendefs.h"
#include "inpptree.h"

struct INPtab {
    char *t_ent;
    struct INPtab *t_next;
};

struct INPnTab {
    char *t_ent;
    void* t_node;
    struct INPnTab *t_next;
};

typedef struct sINPtables{
    struct INPtab **INPsymtab;
    struct INPnTab **INPtermsymtab;
    int INPsize;
    int INPtermsize;
    void *defAmod;
    void *defBmod;
    void *defCmod;
    void *defDmod;
    void *defEmod;
    void *defFmod;
    void *defGmod;
    void *defHmod;
    void *defImod;
    void *defJmod;
    void *defKmod;
    void *defLmod;
    void *defMmod;
    void *defNmod;
    void *defOmod;
    void *defPmod;
    void *defQmod;
    void *defRmod;
    void *defSmod;
    void *defTmod;
    void *defUmod;
    void *defVmod;
    void *defWmod;
    void *defYmod;
    void *defZmod;
} INPtables;

typedef struct card{
    int linenum;
    char *line;
    char *error;
    struct card *nextcard;
    struct card *actualLine;
} card;

/* structure used to save models in after they are read during pass 1 */
typedef struct sINPmodel{
    IFuid INPmodName;   /* uid of model */
    int INPmodType;     /* type index of device type */
    struct sINPmodel *INPnextModel;  /* link to next model */
    int INPmodUsed;     /* flag to indicate it has already been used */
    card *INPmodLine;   /* pointer to line describing model */
    void *INPmodfast;   /* high speed pointer to model for access */
} INPmodel;

/* listing types - used for debug listings */
#define LOGICAL 1
#define PHYSICAL 2

#ifdef __STDC__
int IFnewUid(void*,IFuid*,IFuid,char*,int,void**);
int IFdelUid(void*,IFuid,int);
int INPaName(char*,IFvalue*,void*,int*,char*,void**,IFsimulator*,int*,
        IFvalue*);
int INPapName(void*,int,void*,char*,IFvalue*);
void INPcaseFix(char*);
char * INPdevParse(char**,void*,int,void*,double*,int*,INPtables*);
char *INPdomodel(void *,card*, INPtables*);
void INPdoOpts(void*,void*,card*,INPtables*);
char *INPerrCat(char *, char *);
char *INPerror(int);
double INPevaluate(char**,int*,int);
char * INPfindLev(char*,int*);
char * INPgetMod(void*,char*,INPmodel**,INPtables*);
int INPgetTok(char**,char**,int);
void INPgetTree(char**,INPparseTree**,void*,INPtables*);
IFvalue * INPgetValue(void*,char**,int,INPtables*);
int INPgndInsert(void*,char**,INPtables*,void**);
int INPinsert(char**,INPtables*);
int INPretrieve(char**,INPtables*);
int INPremove(char*,INPtables*);
int INPlookMod(char*);
int INPmakeMod(char*,int,card*);
char *INPmkTemp(char*);
void INPpas1(void*,card*,INPtables*);
void INPpas2(void*,card*,INPtables*,void *);
int INPpName(char*,IFvalue*,void*,int,void*);
int INPtermInsert(void*,char**,INPtables*,void**);
int INPmkTerm(void*,char**,INPtables*,void**);
int INPtypelook(char*);
void INP2B(void*,INPtables*,card*);
void INP2C(void*,INPtables*,card*);
void INP2D(void*,INPtables*,card*);
void INP2E(void*,INPtables*,card*);
void INP2F(void*,INPtables*,card*);
void INP2G(void*,INPtables*,card*);
void INP2H(void*,INPtables*,card*);
void INP2I(void*,INPtables*,card*);
void INP2J(void*,INPtables*,card*);
void INP2K(void*,INPtables*,card*);
void INP2L(void*,INPtables*,card*);
void INP2M(void*,INPtables*,card*);
void INP2O(void*,INPtables*,card*);
void INP2Q(void*,INPtables*,card*,void*);
void INP2R(void*,INPtables*,card*);
void INP2S(void*,INPtables*,card*);
void INP2T(void*,INPtables*,card*);
void INP2U(void*,INPtables*,card*);
void INP2V(void*,INPtables*,card*);
void INP2W(void*,INPtables*,card*);
void INP2Z(void*,INPtables*,card*);
int INP2dot(void*,INPtables*,card*,void*,void*);
INPtables *INPtabInit(int);
void INPkillMods(void);
void INPtabEnd(INPtables *);
#else /* stdc */
int IFnewUid();
int IFdelUid();
int INPaName();
IFvalue * INPgetValue();
INPtables *INPtabInit();
char * INPdevParse();
char * INPdomodel();
char * INPerrCat();
char * INPfindLev();
char * INPgetMod();
char *INPerror();
char *INPmkTemp();
double INPevaluate();
int INPapName();
int INPgetTitle();
int INPgetTok();
int INPgndInsert();
int INPlookMod();
int INPmakeMod();
int INPpName();
int INPreadAll();
int INPtermInsert();
int INPmkTerm();
int INPtypelook();
void INPcaseFix();
void INPdoOpts();
int INPinsert();
int INPretrieve();
int INPremove();
void INPkillMods();
void INPlist();
void INPpas1() ;
void INPpas2() ;
void INPtabEnd();
void INPptPrint();
void INPgetTree();
void INP2B();
void INP2C();
void INP2D();
void INP2E();
void INP2F();
void INP2G();
void INP2H();
void INP2I();
void INP2J();
void INP2K();
void INP2L();
void INP2M();
void INP2O();
void INP2Q();
void INP2R();
void INP2S();
void INP2T();
void INP2U();
void INP2V();
void INP2W();
void INP2Z();
int INP2dot();
#endif /* stdc */

#endif /*INP*/
