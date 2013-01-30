#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h> 
#include <math.h>  //pow
#include "khash.h" 
#include <time.h>  //for random mode
KHASH_SET_INIT_INT(int);

void
strrev (char *p)
{
  char *q = p;
  while (q && *q)
    ++q;
  for (--q; p < q; ++p, --q)
    *p = *p ^ *q, *q = *p ^ *q, *p = *p ^ *q;
}
inline int isNearHomo(int num , int sz){
        int i=0 ;
        while(i<(sz-2)){
                if ((num&3 )==((num>>4) &3))     return 1;
                num=num>>2;
                i++;
        }
        return 0;
}

inline int isHomo(int num , int sz){
        int i=0; 
        while (i<(sz-2)){
                if ((num&3) == ((num>>2) &3) && (num&3)==((num>>4) &3)  ) return 1; 
                num=num>>2;
                i++     ;
        }
        return 0;
        
}
inline int isDimer(int num , int sz){
        int i=0 ;
        while(i<(sz-1)){
                if ((num&3 )==((num>>2) &3))     return 1;
                num=num>>2;
                i++;
        }
        return 0;       
}
int fd(const int a,const  int b, int bz){
	int score=0;
	int i;
	int c=(a^b);
	for(i=0;i<bz;++i){
		if((c&3)>0)score++;	
		c>>=2;
	}
	return score;
}

char * ntoc(int n, int bz, char *s){
	static char lookup[]={'A', 'C','G','T'};
	int i=0, t,j=0;
	do{
		t=n%4;
		s[j++]=lookup[t];	
		n/=4;
		i++;
	}while( i<bz );
	s[j]='\0';
	strrev(s);
	return s;
}

int cton(char *s, int sz){
	static int lookup[]={0,1,2,3};
	int i,n=0;
	for(i=0;i<sz;++i){
		n<<=2;
		switch(s[i]){
			case 'A':
				break;
			case 'C':
				n+=1;
				break;
			case 'G':
				n+=2;
				break;
			case 'T':
				n+=3;
				break;
			default:
				break;
		}
	}
	return n;
}

kh_int_t *  add( kh_int_t* rst, int b, int sz, int *m , int f){
	int mm=*m;
	int i, insertable=1, curr,kret;
	khiter_t k1,k2;
	/*
 	*filter ==1 no dimer
	*filter==2 no nearHomno polymer and no dimer
 	* */
	if(isHomo(b,sz)) return rst;// homopolymer
	if(f>=1)
		if(isDimer(b,sz)) return rst;// dimer
	if(f>=2)
		if(isNearHomo(b,sz)) return rst;// incomplete homopolymer
	
	k1=kh_get(int, rst, b);
	if(k1!=kh_end(rst)) return  rst ;// exist
	for(k1=kh_begin(rst); k1!=kh_end(rst);k1++ ){
		if(!kh_exist(rst, k1)) continue ;
		curr=kh_key(rst, k1 );
		if(fd( curr, b, sz )<mm){ // not pass mismatch
			insertable=0;
			return rst;
			break;
		}
	}
	if(insertable==1){
		kh_put(int, rst, b, &kret);
	}
	return rst;
}
void kh_int_print(kh_int_t* rst, int sz  ){
	int i, insertable=1, curr,kret;
	khiter_t k1,k2;	
	char s[sz+1], s2[sz+1];
	for(k1=kh_begin(rst); k1!=kh_end(rst);k1++ ){
		if(!kh_exist(rst, k1)) continue ;
		char *bar=ntoc(kh_key(rst,k1), sz, s  ) ;
		//int b=cton(bar , sz) ;
		//char *rbar=ntoc(b, sz, s2 );
		//strrev(rbar);
		printf("%s\n",bar);
	}

}

int main(int argc, char * argv[]){
	if(argc<=1){
		fprintf(stderr, "./bargen  mismatch barcode_length filter_level[optional default :2]\n"
			"filter>=1  no dimer\n"
			"filter>=2  no dimer plus no near homopolymer \n"
			"homopolymer is not allowed anyway\n"
		);
		exit(-1);
	}
	int mm=atoi(argv[1]);
	int sz=atoi(argv[2]);
	int filter=2;
	if(argc>3)
		filter=atoi(argv[3]);
	
	int i, kret;
	kh_int_t * rst=kh_init(int);
	
	fprintf(stderr, "barcode:\n"
			"\tlength %d\n"
			"\tfilter %d\n" 
			"\tmismatch %d\n", sz,filter, mm);
	int started;
	int start_pos=0;
	int range=(int)pow(4,sz) ;
	#ifdef _RANDOM_START
		srand(time(NULL));
		start_pos=rand()%range;
	#endif
	start_pos%=range;
	fprintf(stderr, "\tstart position: %d\n", start_pos );
	for(i=start_pos; ; ){
		add( rst, i, sz, &mm, filter);
		i=(i+1)%range ;
		if (i==start_pos)
			break;
	}	
	fprintf(stderr, "barcodes total %d\n", kh_size(rst));
	kh_int_print(rst, sz);
	kh_destroy(int, rst);
}


