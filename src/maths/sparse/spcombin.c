/*
 *  MATRIX UTILITY MODULE
 *
 *  Author:                     Advising professor:
 *      Kenneth S. Kundert          Alberto Sangiovanni-Vincentelli
 *      UC Berkeley
 *
 *  This file contains various optional utility routines.
 *
 *  >>> User accessible functions contained in this file:
 *  spCombine
 *
 *  >>> Other functions contained in this file:
 */


/*
 *  Revision and copyright information.
 *
 *  Copyright (c) 1985,86,87,88,89,90
 *  by Kenneth S. Kundert and the University of California.
 *
 *  Permission to use, copy, modify, and distribute this software and
 *  its documentation for any purpose and without fee is hereby granted,
 *  provided that the copyright notices appear in all copies and
 *  supporting documentation and that the authors and the University of
 *  California are properly credited.  The authors and the University of
 *  California make no representations as to the suitability of this
 *  software for any purpose.  It is provided `as is', without express
 *  or implied warranty.
 */

#ifdef notdef
static char copyright[] =
    "Sparse1.3: Copyright (c) 1985,86,87,88,89,90 by Kenneth S. Kundert";
static char RCSid[] =
    "@(#)$Header$";
#endif



/*
 *  IMPORTS
 *
 *  >>> Import descriptions:
 *  spConfig.h
 *      Macros that customize the sparse matrix routines.
 *  spMatrix.h
 *      Macros and declarations to be imported by the user.
 *  spDefs.h
 *      Matrix type and macro definitions for the sparse matrix routines.
 */

#define spINSIDE_SPARSE
#include "spconfig.h"
#include "spmatrix.h"
#include "spdefs.h"






#ifdef PARALLEL_ARCH
#define COMBINE 1
#endif /* PARALLEL_ARCH */


#if COMBINE
#ifdef __STDC__
#if spSEPARATED_COMPLEX_VECTORS
static void CombineComplexMatrix( MatrixPtr,
                        RealVector, RealVector, RealVector, RealVector );
#else
static void CombineComplexMatrix( MatrixPtr, RealVector, RealVector );
#endif
static void ClearBuffer( MatrixPtr, int, int, ElementPtr );
static void ClearComplexBuffer( MatrixPtr, int, int, ElementPtr );
#else /* __STDC__ */
static void CombineComplexMatrix();
static void ClearBuffer();
static void ClearComplexBuffer();
#endif /* __STDC__ */

/*
 *  COMBINE MATRICES ON A MULTIPROCESSOR
 *
 *  >>> Arguments:
 *  eMatrix  <input> (char *)
 *      Pointer to the matrix to be combined.
 *
 *  >>> Local variables:
 *  Size  (int)
 *      Local version of the size of the matrix.
 *  pElement  (ElementPtr)
 *      Pointer to an element in the matrix.
 */

#define SPBSIZE 256*1024
static double Buffer[SPBSIZE];

void
spCombine( eMatrix, RHS, Spare IMAG_VECTORS )

char *eMatrix;
RealVector RHS, Spare IMAG_VECTORS;
{
MatrixPtr  Matrix = (MatrixPtr)eMatrix;
register ElementPtr  pElement;
register int  I, Size;
ElementPtr FirstBufElement, pLastElement;
int FirstBufCol, BufIndex;
struct ElementListNodeStruct  *pListNode;
long type = MT_COMBINE, length = Matrix->Size + 1;

/* Begin `spCombine'. */
    ASSERT( IS_VALID(Matrix) AND NOT Matrix->Factored );
    if (NOT Matrix->InternalVectorsAllocated)
	spcCreateInternalVectors( Matrix );

#if spCOMPLEX
    if (Matrix->Complex) {
	CombineComplexMatrix( Matrix, RHS, Spare IMAG_VECTORS );
	return;
    }
#endif

#if REAL
    Size = Matrix->Size;

/* Mark original non-zeroes. */
    pListNode = Matrix->FirstElementListNode;
    while (pListNode != NULL)
    {   pElement = pListNode->pElementList;
	if (pListNode == Matrix->LastElementListNode) {
	    pLastElement = Matrix->NextAvailElement - 1;
	} else {
	    pLastElement = &(pElement[ pListNode->NumberOfElementsInList - 1 ]);
	}
        while (pElement <= pLastElement)
        {
	    (pElement++)->Col = -1;
	}
	pListNode = pListNode->Next;
    }

/* Stripmine the communication to reduce overhead */ 
    BufIndex = 0;
    FirstBufCol = 1;
    FirstBufElement = Matrix->FirstInCol[ FirstBufCol ];
    for (I = 1; I <= Size; I++)
    {
        pElement = Matrix->FirstInCol[I];
        while (pElement != NULL)
        {
	    if ( BufIndex >= SPBSIZE )
	    {	/* Buffer is Full. */
		ClearBuffer( Matrix, BufIndex, FirstBufCol, FirstBufElement );
		BufIndex = 0;
		FirstBufCol = I;
		FirstBufElement = pElement;
	    }
	    if ( pElement->Col == -1 ) {
		Buffer[ BufIndex++ ] = pElement->Real;
	    }
            pElement = pElement->NextInCol;
        }
    }
/* Clean out the last, partially full buffer. */
    if ( BufIndex != 0 )
    {
	ClearBuffer( Matrix, BufIndex, FirstBufCol, FirstBufElement );
    }

/* Sum all RHS's together */
    DGOP_( &type, RHS, &length, "+" );

    return;
#endif /* REAL */
}

#if spCOMPLEX
static void
CombineComplexMatrix( Matrix, RHS, Spare IMAG_VECTORS )

MatrixPtr  Matrix;
RealVector  RHS, Spare IMAG_VECTORS;
{
register ElementPtr  pElement;
register int  I, Size;
ElementPtr FirstBufElement, pLastElement;
int FirstBufCol, BufIndex;
struct ElementListNodeStruct  *pListNode;
long type = MT_COMBINE, length = Matrix->Size + 1;

/* Begin `CombineComplexMatrix'. */
    ASSERT(Matrix->Complex);
    Size = Matrix->Size;

/* Mark original non-zeroes. */
    pListNode = Matrix->FirstElementListNode;
    while (pListNode != NULL)
    {   pElement = pListNode->pElementList;
	if (pListNode == Matrix->LastElementListNode) {
	    pLastElement = Matrix->NextAvailElement - 1;
	} else {
	    pLastElement = &(pElement[ pListNode->NumberOfElementsInList - 1 ]);
	}
        while (pElement <= pLastElement)
        {
	    (pElement++)->Col = -1;
	}
	pListNode = pListNode->Next;
    }

/* Stripmine the communication to reduce overhead */ 
    BufIndex = 0;
    FirstBufCol = 1;
    FirstBufElement = Matrix->FirstInCol[ FirstBufCol ];
    for (I = 1; I <= Size; I++)
    {
        pElement = Matrix->FirstInCol[I];
        while (pElement != NULL)
        {
	    if ( BufIndex >= SPBSIZE/2 )
	    {	/* Buffer is Full. */
		ClearComplexBuffer( Matrix, BufIndex, FirstBufCol,
		        FirstBufElement );
		BufIndex = 0;
		FirstBufCol = I;
		FirstBufElement = pElement;
	    }
	    if ( pElement->Col == -1 ) {
		Buffer[ BufIndex++ ] = pElement->Real;
		Buffer[ BufIndex++ ] = pElement->Imag;
	    }
            pElement = pElement->NextInCol;
        }
    }
/* Clean out the last, partially full buffer. */
    if ( BufIndex != 0 )
    {
	ClearComplexBuffer( Matrix, BufIndex, FirstBufCol, FirstBufElement );
    }

/* Sum all RHS's together */
#if spSEPARATED_COMPLEX_VECTORS
    DGOP_( &type, RHS, &length, "+" );
    DGOP_( &type, iRHS, &length, "+" );
#else
    length *= 2;
    DGOP_( &type, RHS, &length, "+" );
#endif

    return;
}
#endif /* spCOMPLEX */

#if REAL
static void
ClearBuffer( Matrix, NumElems, StartCol, StartElement )

MatrixPtr  Matrix;
int NumElems, StartCol;
ElementPtr StartElement;
{
    register ElementPtr pElement = StartElement;
    register int Index, Col = StartCol;
    long type = MT_COMBINE;

/* First globalize the buffer. */
    DGOP_( &type, Buffer, &NumElems, "+" );

/* Now, copy all of the data back into the matrix. */
    for ( Index = 0; Index < NumElems; Index++ )
    {
	if ( pElement == NULL )
	{
	    pElement = Matrix->FirstInCol[ ++Col ];
	}
	while ( pElement->Col != -1 ) {
	    pElement = pElement->NextInCol;
	    if ( pElement == NULL )
	    {
		pElement = Matrix->FirstInCol[ ++Col ];
	    }
	}
	pElement->Real = Buffer[ Index ];
	pElement->Col = Col;
	pElement = pElement->NextInCol;
    }
}
#endif REAL

#if spCOMPLEX
static void
ClearComplexBuffer( Matrix, DataCount, StartCol, StartElement )

MatrixPtr  Matrix;
int DataCount, StartCol;
ElementPtr StartElement;
{
    register ElementPtr pElement = StartElement;
    register int Index, Col = StartCol;
    long type = MT_COMBINE;

/* First globalize the buffer. */
    DGOP_( &type, Buffer, &DataCount, "+" );

/* Now, copy all of the data back into the matrix. */
    for ( Index = 0; Index < DataCount; )
    {
	if ( pElement == NULL )
	{
	    pElement = Matrix->FirstInCol[ ++Col ];
	}
	while ( pElement->Col != -1 ) {
	    pElement = pElement->NextInCol;
	    if ( pElement == NULL )
	    {
		pElement = Matrix->FirstInCol[ ++Col ];
	    }
	}
	pElement->Real = Buffer[ Index++ ];
	pElement->Imag = Buffer[ Index++ ];
	pElement->Col = Col;
	pElement = pElement->NextInCol;
    }
}
#endif /* spCOMPLEX */
#endif /* COMBINE */
