/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
**********/

/*
 * Date and time utility functions
 */

#include <config.h>
#include "ngspice.h"
#include <stdio.h>
#include "misc_time.h"

#ifdef HAVE_LOCALTIME
#include <time.h>
#endif

#ifdef HAVE_GETRUSAGE
#  include <sys/types.h>
#  include <sys/time.h>
#  include <sys/resource.h>
#else
#  ifdef HAVE_TIMES
#    include <sys/types.h>
#    include <sys/times.h>
#    include <sys/param.h>
#  else
#    ifdef HAVE_FTIME
/* default to ftime if we can't get real CPU times */
#      include <sys/types.h>
#      include <sys/timeb.h>
#    endif
#  endif
#endif



/* Return the date. Return value is static data. */

char *
datestring(void)
{

#ifdef HAVE_LOCALTIME
    static char tbuf[45];
    struct tm *tp;
    char *ap;
    int i;

    time_t tloc;
    time(&tloc);
    tp = localtime(&tloc);
    ap = asctime(tp);
    (void) sprintf(tbuf, "%.20s", ap);
    (void) strcat(tbuf, ap + 19);
    i = strlen(tbuf);
    tbuf[i - 1] = '\0';
    return (tbuf);

#else

    return ("today");

#endif
}

/* return time interval in seconds and milliseconds */

#ifndef HAVE_GETRUSAGE
#ifndef HAVE_TIMES
#ifdef HAVE_FTIME

struct timeb timebegin;

void timediff(struct timeb *now, struct timeb *begin, int *sec, int *msec)
{

    *msec = now->millitm - begin->millitm;
    *sec = now->time - begin->time;
    if (*msec < 0) {
      *msec += 1000;
      (*sec)--;
    }
    return;

}

#endif
#endif
#endif

/* How many seconds have elapsed in running time. */

double
seconds(void)
{
#ifdef HAVE_GETRUSAGE
    struct rusage ruse;

#ifdef PARALLEL_ARCH
    return (TCGTIME_());
#else
    (void) getrusage(RUSAGE_SELF, &ruse);
    return (ruse.ru_utime.tv_sec + (double) ruse.ru_utime.tv_usec / 1000000.0);
#endif /* PARALLEL_ARCH */
#else
#ifdef HAVE_TIMES

    struct tms tmsbuf;

    times(&tmsbuf);
    return((double) tmsbuf.tms_utime / HZ);

#else
#ifdef HAVE_FTIME
    struct timeb timenow;
    int sec, msec;

    ftime(&timenow);
    timediff(&timenow, &timebegin, &sec, &msec);
    return(sec + (double) msec / 1000.0);

#else /* unknown */
    /* don't know how to do this in general. */
    return(-1.0);	/* Obvious error condition */

#endif /* !FTIME */
#endif /* !SYSV */
#endif /* !BSD */
}
