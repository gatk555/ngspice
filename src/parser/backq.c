/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
**********/

/*
 *
 * Do backquote substitution on a word list. 
 */

#include <config.h>
#include "ngspice.h"
#include "cpdefs.h"
#include "backq.h"


static wordlist *backeval(char *string);

char cp_back = '`';

wordlist *
cp_bquote(wordlist *wlist)
{
    wordlist *wl, *nwl;
    char *s, *t, buf[BSIZE_SP], wbuf[BSIZE_SP], tbuf[BSIZE_SP];
    int i;

    for (wl = wlist; wl; wl = wl->wl_next) {
				
        t = wl->wl_word;
        if (!t)
            continue;
        i = 0;
loop:   	s =strchr(t, cp_back);
        if (s == NULL)
            continue;
        while (t < s)
            wbuf[i++] = *t++;
        wbuf[i] = '\0';
        (void) strcpy(buf, ++s);
        s = buf;
        t++;
        while (*s && (*s != cp_back)) {
            t++;    /* Get s and t past the next backquote. */
            s++;
        }
        /* What the heck, let "echo `foo" work... */
        *s = '\0';
        t++;    /* Get past the second ` */
	if (!(nwl = backeval(buf))) {
            wlist->wl_word = NULL;
            return (wlist);
        }
            (void) strcpy(buf, wbuf);
	    if (nwl->wl_word) {
                (void) strcat(buf, nwl->wl_word);
                tfree(nwl->wl_word);
	    }
            nwl->wl_word = copy(buf);
	
	    (void) strcpy(tbuf, t);
    	    wl = wl_splice(wl, nwl);
		for(wlist = wl; wlist->wl_prev; wlist = wlist->wl_prev);
		/* MW. We must move to the begging of new wordlist. */
		
        (void) strcpy(buf, wl->wl_word);
	i = strlen(buf);
        (void) strcat(buf, tbuf);
        tfree(wl->wl_word);
	wl->wl_word = copy(buf);
        t = &wl->wl_word[i];
        s = wl->wl_word;
        for (i = 0; s < t; s++)
            wbuf[i++] = *s;
        goto loop;
    }
    return (wlist);
}

/* Do a popen with the string, and then reset the file pointers so that
 * we can use the first pass of the parser on the output.
 */

static wordlist *
backeval(char *string)
{
#ifdef HAVE_POPEN
    FILE *proc, *old;
    wordlist *wl;
    bool intv;
    extern FILE *popen(const char *, const char *);

    proc = popen(string, "r");
    if (proc == NULL) {
        fprintf(cp_err, "Error: can't evaluate %s.\n", string);
        return (NULL);
    }
    old = cp_inp_cur;
    cp_inp_cur = proc;
    intv = cp_interactive;
    cp_interactive = FALSE;
    cp_bqflag = TRUE;
    wl = cp_lexer((char *) NULL);
    cp_bqflag = FALSE;
    cp_inp_cur = old;
    cp_interactive = intv;
    (void) pclose(proc);
    return (wl);
#else
    wordlist *wl = alloc(struct wordlist);

    wl->wl_word = copy(string);
    return (wl);
#endif
}

