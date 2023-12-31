/* Copyright 1990
   Regents of the University of California.
   All rights reserved.

   Author: 1985 Wayne A. Christopher

   The main routine for ngspice */
#include <stdio.h>
#include <setjmp.h>
#include <signal.h>

#include <sys/types.h>

#define _GNU_SOURCE
#include <getopt.h>

#include "ngspice.h"
#include "ifsim.h"
#include "inpdefs.h"
#include "iferrmsg.h"
#include "cpdefs.h"
#include "ftedefs.h"
#include "ftedev.h"
#include "ftedebug.h"
#include "const.h"

#ifdef HAVE_PWD_H
#include <pwd.h>
#endif

#ifndef HAVE_GETRUSAGE
#ifdef HAVE_FTIME
#include <sys/timeb.h>
#endif
#endif

/* Main options */
static bool ft_servermode = FALSE;
static bool ft_batchmode = FALSE;

/* Frontend options */
bool ft_intrpt = FALSE;     /* Set by the (void) signal handlers. */
bool ft_setflag = FALSE;    /* Don't abort after an interrupt. */
char *ft_rawfile = "rawspice.raw";

/* Frontend and circuit options */
IFsimulator *ft_sim = NULL;



/* (Virtual) Machine architecture parameters */
int ARCHme;
int ARCHsize;

char *errRtn;
char *errMsg;
char *cp_program;

struct variable *(*if_getparam)( );



jmp_buf jbuf;

static int started = FALSE;



#ifndef HAVE_GETRUSAGE
#ifdef HAVE_FTIME
extern struct timeb timebegin;		/* for use w/ ftime */
#endif
#endif

extern IFsimulator SIMinfo;

#ifdef SIMULATOR

bool ft_nutmeg = FALSE;
extern struct comm spcp_coms[ ];
struct comm *cp_coms = spcp_coms;

#else

bool ft_nutmeg = TRUE;
extern struct comm nutcp_coms[ ];
struct comm *cp_coms = nutcp_coms;
static IFfrontEnd nutmeginfo;

int
if_run(char *t, char *w, wordlist *s, char *b)
{
    return (0);
}

int
if_sens_run(char *t, char *w, wordlist *s, char *b)
{
    return (0);
}

void
if_dump(char *ckt, FILE *fp)
{}

char *
if_inpdeck(struct line *deck, char **tab)
{
    return ((char *) 0);
}

int
if_option(char *ckt, char *name, int type, char *value)
{
    return 0;
}

void if_cktfree(char *ckt, char *tab)
{}

void if_setndnames(char *line)
{}

char *
if_errstring(int code)
{
    return ("spice error");
}

void
if_setparam(char *ckt, char *name, char *param, struct variable *val)
{}

bool
if_tranparams(struct circ *ckt, double *start, double *stop, double *step)
{
    return (FALSE); 
}

struct variable *
if_getstat(char *n, char *c)
{
    return (NULL);
}

#endif /* SIMULATOR */

char *hlp_filelist[] = { "ngspice", 0 };


/* allocate space for global constants in 'CONST.h' */

double CONSTroot2;
double CONSTvt0;
double CONSTKoverQ;
double CONSTe;
IFfrontEnd *SPfrontEnd = NULL;



int SIMinit(IFfrontEnd *frontEnd, IFsimulator **simulator)
{
    SPfrontEnd = frontEnd;
    *simulator = &SIMinfo;
    CONSTroot2 = sqrt(2.);
    CONSTvt0 = CONSTboltz * (27 /* deg c */ + CONSTCtoK ) / CHARGE;
    CONSTKoverQ = CONSTboltz / CHARGE;
    CONSTe = exp((double)1.0);
    return(OK);
}


/* Shutdown gracefully. */
int 
shutdown(int exitval)
{
    cleanvars();
#ifdef PARALLEL_ARCH
    if (exitval == EXIT_BAD) {
	Error("Fatal error in SPICE", -1);
    } else {
	PEND_();
    }
#endif /* PARALLEL_ARCH */
    exit (exitval);
}

void
show_help(void)
{
    printf("Usage: %s [OPTION]... [FILE]...\n"
	   "Simulate the electical circuits in FILE.\n"
	   "\n"
	   "  -b, --batch               process FILE in batch mode\n"
	   "  -c, --circuitfile=FILE    set the circuitfile\n"
	   "  -i, --interactive         run in interactive mode\n"
	   "  -n, --no-spiceinit        don't load the .spiceinit configfile\n"
	   "  -o, --output=FILE         set the outputfile\n"
	   "  -q, --completion          activate command completion\n"
	   "  -r, --rawfile=FILE        set the rawfile output\n"             
	   "  -s, --server              run spice as a server process\n"
	   "  -t, --term=TERM           set the terminal type\n"
 	   "  -h, --help                display this help and exit\n"
	   "  -v, --version             output version information and exit\n"
	   "\n"
	   "Report bugs to %s.\n", cp_program, Bug_Addr);
}

void
show_version(void)
{
    printf("%s compiled from %s revision %s\n"
	   "Written originally by Berkeley University\n"
	   "Currently maintained by the NG-Spice Project\n\n"
	   "Copyright (C) 1985-1996,"
	   "  The Regents of the University of California\n"
	   "Copyright (C) 1999-2000,"
	   "  The NG-Spice Project\n", cp_program, PACKAGE, VERSION);
}

void
append_to_stream(FILE *dest, FILE *source)
{
    char *buf[BSIZE_SP];
    int i;

    while ((i = fread(buf, 1, BSIZE_SP, source)) > 0)
	fwrite(buf, i, 1, dest);
}

int
main(int argc, char **argv)
{
    int c;
    int		err;
    bool	gotone = FALSE;

#ifdef SIMULATOR
    int error2;

    extern int OUTpBeginPlot(), OUTpData(), OUTwBeginPlot(), OUTwReference();
    extern int OUTwData(), OUTwEnd(), OUTendPlot(), OUTbeginDomain();
    extern int OUTendDomain(), OUTstopnow(), OUTerror(), OUTattributes();
    static IFfrontEnd nutmeginfo = {
	IFnewUid,
	IFdelUid,
	OUTstopnow,
	seconds,
	OUTerror,
	OUTpBeginPlot,
	OUTpData,
	OUTwBeginPlot,
	OUTwReference,
	OUTwData,
	OUTwEnd,
	OUTendPlot,
	OUTbeginDomain,
	OUTendDomain,
	OUTattributes
    };
#else  /* ~ SIMULATOR */
    bool gdata = TRUE;
#endif /* ~ SIMULATOR */


    char buf[BSIZE_SP];
    bool ciprefix();
    bool readinit = TRUE;
    bool rflag = FALSE;
    bool istty = TRUE;
    bool iflag = FALSE;
    bool qflag = FALSE;
    FILE *fp;
    FILE *circuit_file;


    /* MFB tends to jump to 0 on errors.  This tends to catch it. */
    if (started) {
        fprintf(cp_err, "main: Internal Error: jump to zero\n");
        shutdown(EXIT_BAD);
    }
    started = TRUE;

#ifdef PARALLEL_ARCH
    PBEGIN_(argc, argv);
    ARCHme = NODEID_();
    ARCHsize = NNODES_();
    SETDBG_(&debug_flag);
    fprintf( stderr, "On-line: process %d of %d total.\n", ARCHme, ARCHsize );
    evlog(EVKEY_ENABLE, EVKEY_EVENT, "On-line", EVKEY_DUMP, EVKEY_DISABLE,
	  EVKEY_LAST_ARG);
#else
    ARCHme = 0;
    ARCHsize = 1;
#endif /* PARALLEL_ARCH */


    ivars( );

    cp_in = stdin;
    cp_out = stdout;
    cp_err = stderr;

    circuit_file = stdin;



#ifdef MALLOCTRACE
    mallocTraceInit("malloc.out");
#endif
#ifdef HAVE_ISATTY
    istty = (bool) isatty(fileno(stdin));
#endif



    init_time( );

    err = SIMinit(&nutmeginfo,&ft_sim);
    if(err != OK) {
        ft_sperror(err,"SIMinit");
        shutdown(EXIT_BAD);
    }
    cp_program = ft_sim->simulator;

    srandom(getpid());


    while (1) {
	int option_index = 0;
	static struct option long_options[] = {
	    {"help", 0, 0, 'h'},
	    {"version", 0, 0, 'v'},
	    {"batch", 0, 0, 'b'},
	    {"circuitfile", 0, 0, 'c'},
	    {"interactive", 0, 0, 'i'},
	    {"no-spiceinit", 0, 0, 'n'},
	    {"output", 0, 0, 'o'},
	    {"completion", 0, 0, 'q'},
	    {"rawfile", 1, 0, 'r'},
	    {"server", 0, 0, 's'},
	    {"terminal", 1, 0, 't'},
	    {0, 0, 0, 0}
	};

	c = getopt_long (argc, argv, "hvbc:ihno:qr:st:",
			 long_options, &option_index);
	if (c == -1)
	    break;

	switch (c) {
	case 'h':		/* Help */
	    show_help();
	    shutdown (EXIT_NORMAL);
	    break;

	case 'v':		/* Version info */
	    show_version();
	    shutdown (EXIT_NORMAL);
	    break;

	case 'b':		/* Batch mode */
	    ft_batchmode = TRUE;
	    break;

	case 'c':		/* Circuit file */
	    if (optarg) {
		if (!(circuit_file = fopen(optarg, "r"))) {
		    perror("circuit file not available");
		    shutdown(EXIT_BAD);
		}
		istty = FALSE;
	    }
	    break;

	case 'i':		/* Interactive mode */
	    iflag = TRUE;
	    break;

	case 'n':		/* Don't read .spiceinit */
	    readinit = FALSE;
	    break;

	case 'o':		/* Output file */
	    if (optarg) {
#ifdef PARALLEL_ARCH
		sprintf (buf, "%s%03d", optarg, ARCHme);
#else
		sprintf (buf, "%s", optarg);
#endif
		if (!(freopen (buf, "w", stdout))) {
		    perror (buf);
		    shutdown (EXIT_BAD);
		}
	    }
	    break;

	case 'q':		/* Command completion */
	    qflag = TRUE;
	    break;

	case 'r':		/* The raw file */
	    if (optarg) {
		cp_vset("rawfile", VT_STRING, optarg);
	    }
	    rflag = TRUE;
	    break;

	case 's':		/* Server mode */
	    ft_servermode = TRUE;
	    break;

	case 't':
	    if (optarg) {
		cp_vset("term", VT_STRING, optarg);
	    }
	    break;

	case '?':
	    break;

	default:
	    printf ("?? getopt returned character code 0%o ??\n", c);
	}
    }


#ifdef SIMULATOR
    if_getparam = spif_getparam;
#else
    if_getparam = nutif_getparam;

    if (optind == argc) {
	/* No raw file */
	gdata = FALSE;
    }
#endif


    if ((!iflag && !istty) || ft_servermode)
        ft_batchmode = TRUE;
    if ((iflag && !istty) || qflag)
        cp_nocc = TRUE;
    if (ft_servermode)
        readinit = FALSE;
    if (!istty || ft_batchmode)
        out_moremode = FALSE;

    /* Would like to do this later, but cpinit evals commands */
    init_rlimits( );

    /* Have to initialize cp now. */
    ft_cpinit();

    /* To catch interrupts during .spiceinit... */
    if (setjmp(jbuf) == 1) {
        fprintf(cp_err, "Warning: error executing .spiceinit.\n");
        if (!ft_batchmode)
            goto bot;
    }
 
    /* Set up signal handling */
    if (!ft_batchmode) {
        signal(SIGINT, ft_sigintr);
        signal(SIGFPE, sigfloat);
#ifdef SIGTSTP
        signal(SIGTSTP, sigstop);
#endif
    }

    /* Set up signal handling for fatal errors. */
    signal(SIGILL, sigill);

#ifdef SIGBUS
    signal(SIGBUS, sigbus);
#endif
#ifdef SIGSEGV
    signal(SIGSEGV, sigsegv);
#endif
#ifdef SIGSYS
    signal(SIGSYS, sig_sys);
#endif


    if (readinit) {
#ifdef HAVE_PWD_H
	/* Try to source either .spiceinit or ~/.spiceinit. */
        if (access(".spiceinit", 0) == 0)
            inp_source(".spiceinit");
        else {
	    char *s;
	    struct passwd *pw;

            pw = getpwuid(getuid());
	    asprintf(&s, "%s/.spiceinit", pw->pw_dir);
            if (access(s, 0) == 0)
                inp_source(s);
	     /* free(s); */  
	    /* FIXME: Do we need to free() char* fields in pw as well? */
	   /* free(pw); */
	}
#else /* ~ HAVE_PWD_H */
	/* Try to source the file "spice.rc" in the current directory.  */
        if ((fp = fopen("spice.rc", "r")) != NULL) {
            (void) fclose(fp);
            inp_source("spice.rc");
        }
#endif /* ~ HAVE_PWD_H */
    }

    if (!ft_batchmode) {
	com_version(NULL);
        DevInit( );
	if (News_File && *News_File) {
	    fp = fopen(cp_tildexpand(News_File), "r");
	    if (fp) {
		while (fgets(buf, BSIZE_SP, fp))
		    fputs(buf, stdout);
		(void) fclose(fp);
	    }
	}
    }


bot:

    /* Pass 2 -- get the filenames. If we are spice, then this means
     * build a circuit for this file. If this is in server mode, don't
     * process any of these args.  */

    if (setjmp(jbuf) == 1)
        goto evl;


    cp_interactive = FALSE;
    err = 0;

#ifdef SIMULATOR
    if (!ft_servermode && !ft_nutmeg) {
	/* Concatenate all non-option arguments into a temporary file
	   and load that file into the spice core.
	   
	   The original routine took a special path if there was only
	   one non-option argument.  In that case, it didn't create
	   the temporary file but used the original file instead.  The
	   current algorithm is uniform at the expense of a little
	   startup time.  */
	FILE *tempfile;

	tempfile = tmpfile();
	if (optind == argc && !istty) {
	    append_to_stream(tempfile, stdin);
	}
	while (optind < argc) {
	    char *arg;
	    FILE *tp;

	    /* Copy all the arguments into the temporary file */
	    arg = argv[optind++];
	    tp = fopen(arg, "r");
	    if (!tp) {
		perror(arg);
		err = 1;
		break;
	    }
	    append_to_stream(tempfile, tp);
	    fclose(tp);
	}
	fseek(tempfile, (long) 0, 0);

        if (tempfile && (!err || !ft_batchmode)) {
            inp_spsource(tempfile, FALSE, NULL);
            gotone = TRUE;
        }
	if (ft_batchmode && err)
	    shutdown(EXIT_BAD);
    }

    if (!gotone && ft_batchmode && !ft_nutmeg)
        inp_spsource(circuit_file, FALSE, (char *) NULL);

evl:
    if (ft_batchmode) {
        /* If we get back here in batch mode then something is wrong,
         * so exit.  */
        bool st = FALSE;

        (void) setjmp(jbuf);

        if (st == TRUE) {
            shutdown(EXIT_BAD);
	}
        st = TRUE;
        if (ft_servermode) {
            if (ft_curckt == NULL) {
                fprintf(cp_err, "Error: no circuit loaded!\n");
                shutdown(EXIT_BAD);
            }
            if (ft_dorun(""))
		shutdown(EXIT_BAD);
            shutdown(EXIT_NORMAL);
        }

        /* If -r is specified, then we don't bother with the dot
         * cards. Otherwise, we use wrd_run, but we are careful not to
         * save too much.  */
        cp_interactive = FALSE;
        if (rflag) {
	    ft_dotsaves();
	    error2 = ft_dorun(ft_rawfile);
	    if (ft_cktcoms(TRUE) || error2)
		shutdown(EXIT_BAD);
        } else if (ft_savedotargs()) {
	    error2 = ft_dorun(NULL);
	    if (ft_cktcoms(FALSE) || error2)
		shutdown(EXIT_BAD);
	} else {
	    fprintf(stderr,
		    "Note: No \".plot\", \".print\", or \".fourier\" lines; "
		    "no simulations run\n");
	    shutdown(EXIT_BAD);
        }
    } else {
        (void) setjmp(jbuf);
        cp_interactive = TRUE;
	while (cp_evloop((char *) NULL) == 1) ;
    }

#else  /* ~ SIMULATOR */

    if (ft_nutmeg && gdata) {
	while (optind < argc) {
	  ft_loadfile(argv[optind++]);
	  gotone = TRUE;
	}
        if (!gotone)
            ft_loadfile(ft_rawfile);
    }

evl:
    /* Nutmeg "main" */
    (void) setjmp(jbuf);
    cp_interactive = TRUE;
    while (cp_evloop((char *) NULL) == 1) ;

#endif /* ~ SIMULATOR */

    shutdown(EXIT_NORMAL);
    return EXIT_NORMAL;
}
