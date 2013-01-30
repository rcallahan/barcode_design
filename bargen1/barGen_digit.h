/*
 * using 64 bits integer represent  ACGT strings.
 * note: only support 32 bases or less barcode design		
 *
 * */
#ifndef _BARGEN_DIGIT_H
#define _BARGEN_DIGIT_H
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#define UINT64MAX 0xffffffffffffffff
#define _HI8       0xff00000000000000UL
#define _LO8	0xff
char * ntoc( int64_t num, int64_t sz);
int64_t cton(char *s) ;
#include <stdio.h>
#endif
