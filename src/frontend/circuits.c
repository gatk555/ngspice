/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
**********/

/*
 *
 * Routines for dealing with the circuit database.  This is currently
 * unimplemented.
 */

#include "ngspice.h"
#include "cpdefs.h"
#include "ftedefs.h"
#include "ftedata.h"
#include "circuits.h"


struct circ *ft_curckt = NULL;  /* The default active circuit. */
struct circ *ft_circuits = NULL;

struct subcirc *ft_subcircuits = NULL;

/* Now stuff to deal with circuits */


void
ft_setccirc(char *name)
{
}

/* Add a circuit to the circuit list */

void
ft_newcirc(struct circ *ckt)
{
    ckt->ci_next = ft_circuits;
    ft_circuits = ckt;
    return;
}

/* Add a new subcircuit to the subcircuit list */


void
ft_newsubcirc(struct subcirc *sckt)
{
}

