#ifndef _EDIT_DISTANCE_H
#define _EDIT_DISTANCE_H
#include <string.h>
#include <inttypes.h>
#define MIN2(a,b) (((a) < (b)) ? (a) : (b))
#define MIN3(x, y, z)  MIN2(MIN2(x,y), z)
#define MIN5(v, w, x, y, z)  MIN3(x, MIN2(y,z), MIN2(v,w))

uint64_t FD(char *s1, int64_t sz1 ,char *s2) ;
 int  ED(char *s1, char *s2);

#endif
