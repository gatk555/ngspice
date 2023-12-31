/*************
 * Definitions header file
 * 1999 E. Rouat
 ************/

/* 
 * This file will contain all #defines needed
 * by ngspice code (in construction)
 * It should only #define numeric constants, not macros.
 */


#ifndef _DEFINES_H_
#define _DEFINES_H_


#ifndef	M_PI
#  define M_PI       3.14159265358979323846
#endif
#ifndef	M_E
#  define M_E  	     2.7182818284590452354
#endif
#ifndef	M_LOG2E
#  define M_LOG2E    1.4426950408889634074
#endif
#ifndef	M_LOG10E
#  define M_LOG10E   0.43429448190325182765
#endif


/*
 *  IEEE Floating point
 */

#define MAX_EXP_ARG	709.0

#ifndef DBL_EPSILON
# define DBL_EPSILON	8.9e-15
#endif
#ifndef DBL_MAX
# define DBL_MAX	1.79769313486231e+308
#endif
#ifndef DBL_MIN
# define DBL_MIN	2.22507385850721e-308
#endif
#ifndef SHRT_MAX
# define SHRT_MAX	32766
#endif
#ifndef INT_MAX
# define INT_MAX	2147483646
#endif
#ifndef LONG_MAX
# define LONG_MAX	2147483646
#endif

#define MAXPOSINT	INT_MAX

/* 
 * Physical constants (const.h)
 */

#define CHARGE (1.6021918e-19)
#define CONSTCtoK (273.15)
#define CONSTboltz (1.3806226e-23)
#define REFTEMP 300.15 /* 27 degrees C */



#define DIR_PATHSEP	"/"
#define DIR_TERM	'/'
#define DIR_CWD		"."

#define TEMPFORMAT	"/tmp/%s%d"
#define SYSTEM_PLOT5LPR	"lpr -P%s -g %s"
#define SYSTEM_PSLPR	"lpr -P%s %s"
#define SYSTEM_MAIL	"Mail -s \"%s (%s) Bug Report\" %s"


/*
 *  #define-s that are always on
 */

#define CAPZEROBYPASS
#define NEWCONV
/* #define CAPBYPASS	Internal use only */


/* On Unix the following should always be true, so they should jump out */

#define HAS_ASCII
#define HAS_TTY_
#define HAS_TIME_
#define HAS_RLIMIT_



#ifndef SIGNAL_FUNCTION
#  define SIGNAL_FUNCTION RETSIGTYPE (*)( )
#endif

#define BSIZE_SP      512


#define EXIT_NORMAL 0
#define EXIT_BAD    1

#define TRUE 1
#define FALSE 0



#define DIR_PATHSEP	"/"
#define DIR_TERM	'/'
#define DIR_CWD		"."

#define TEMPFORMAT	"/tmp/%s%d"
#define SYSTEM_PLOT5LPR	"lpr -P%s -g %s"
#define SYSTEM_PSLPR	"lpr -P%s %s"
#define SYSTEM_MAIL	"Mail -s \"%s (%s) Bug Report\" %s"





#endif /* _DEFINES_H_ */

