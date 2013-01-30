#ifndef _DP_H
#define _DP_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "ED.h"

#define N_NUCL 4
typedef struct {
	int  *s; //solutions
	int n; //number of solutions
	int m; //current loaded solutions
	int bz; //barsize
}solution_t;
typedef struct{
	int dimer ;
	int trimer ;
	int neartri ;
}nmer_stat_t;

solution_t * solve( int, int, int);
int * balance(int nb,int ib , int *bvalret,solution_t * ss) ;

#endif
