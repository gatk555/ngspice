/*************
 * Header file for cmath2.c
 * 1999 E. Rouat
 ************/

#ifndef CMATH2_H_INCLUDED
#define CMATH2_H_INCLUDED


void * cx_tan(void *data, short int type, int length, int *newlength, short int *newtype);
void * cx_atan(void *data, short int type, int length, int *newlength, short int *newtype);
void * cx_norm(void *data, short int type, int length, int *newlength, short int *newtype);
void * cx_uminus(void *data, short int type, int length, int *newlength, short int *newtype);
void * cx_rnd(void *data, short int type, int length, int *newlength, short int *newtype);
void * cx_mean(void *data, short int type, int length, int *newlength, short int *newtype);
void * cx_length(void *data, short int type, int length, int *newlength, short int *newtype);
void * cx_vector(void *data, short int type, int length, int *newlength, short int *newtype);
void * cx_unitvec(void *data, short int type, int length, int *newlength, short int *newtype);
void * cx_plus(void *data1, void *data2, short int datatype1, short int datatype2, int length);
void * cx_minus(void *data1, void *data2, short int datatype1, short int datatype2, int length);
void * cx_times(void *data1, void *data2, short int datatype1, short int datatype2, int length);
void * cx_mod(void *data1, void *data2, short int datatype1, short int datatype2, int length);


#endif
