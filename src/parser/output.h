/*************
 * Header file for output.c
 * 1999 E. Rouat
 ************/

#ifndef OUTPUT_H_INCLUDED
#define OUTPUT_H_INCLUDED

void out_init(void);
void outbufputc(void);
void promptreturn(void);
void out_send(char *string);
void out_printf(char *fmt, char *s1, char *s2, char *s3, char *s4, char *s5, char *s6, 
		char *s7, char *s8, char *s9, char *s10);
void  term_clear(void);
void  term_home(void);
void  term_cleol(void);
void tcap_init(void);


#endif
