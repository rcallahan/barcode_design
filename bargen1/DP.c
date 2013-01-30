#include "DP.h"
#include <time.h>
#define ABS(x) (((x)<0)?(-1*(x)):(x) )
/*
 *solution_init
 * creation of solutions.
 * solutions saving only numbers in array
 * */
solution_t *solution_init(int n, int bz){
	solution_t * s=(solution_t *) calloc(1, sizeof(solution_t));
	s->n=n;
	s->m=0;	
	s->bz=bz;
	s->s=(int *)calloc(s->n,sizeof(int) );
}
/*
 *balance: taking a set of barcode from  solution_t* to re-organize the order
 * so that barcodes coming out the order of balanced at first N bases
 * parameters:
 *	nb:  number of barcode needs to balanced 
 *	ib: left most N bases of barcodes need to balanced
 *	bvalret: values used for balancing bias
 *	ss: solution_t * for barcode candidates
 * return : 
 * 	an array save balanced order of digital barcodes 
 * */
int * balance(int nb, /*number of bars need to balance*/
	 int ib ,/*first ib bases need to balance*/
	int *bvalret, /*min balanced value return */
	solution_t * ss)
{
	srand(time(NULL));
	if (nb>ss->m)  return NULL ;
	
	int *vec=(int *) calloc((ss->m),sizeof(int)); //unused vector
	int cnt[ib][4] ; 
	int balancer= nb/ib ;
	int cache=2;
	int balanced;	
	{
	/// push the first nb th bars into vector
		int i;
		for(i=0;i<nb;++i){
			vec[i]=1;
		}
	}
	do{	
		balanced=1;	
		{
			int i, j, ibn;
			for (j=0;j<ib*4 ;j++){cnt[j/4][j%4]=0; }	
			for(i=0;i<ss->m;++i){
				if (vec[i]!=1) continue ;
					int n=ss->s[i] 	;
				// kick AGCT count into cnt
				j=0;
				ibn=0;
				do{
					int t=n%4;
					cnt[ibn++][t]++;
					n/=4;
					j++;
				}while(j<ss->bz && ibn<ib);
			}	
		}
		{///check balance
			int i, j;
			for(i=0;i<ib;++i){
				for(j=0;j<4;j++){
					int r=ABS( cnt[i][j] - balancer )  ;
					if ( r>(cache+i)) balanced=0;
					}
				}
		}
		{
			if (!balanced){
				int k=rand()%nb ;
				int kin=rand()%(ss->m-nb);
				int i, j;
				for(i=0,j=0;i<ss->m;++i ){
					if( vec[i] ==1){
						if( j==k){
						  vec[i]=0;
						//	printf("%d kicked\n",i);	
						}
						j++;
					}
				}
				for(i=0,j=0;i<ss->m;++i ){
					if( vec[i] !=1){
						if( j==kin){
						  vec[i]=1;	
						//	printf("%d set\n",i);	
							
						}
						j++;
					}
				}
				
					
			}
		}
	}while(!balanced);
	
	int bval=0;
		{///check balance
			int i, j;
			for(i=0;i<ib;++i){
				for(j=0;j<4;j++){
					int r=ABS( cnt[i][j] - balancer )  ;
					bval+=r;
					}
				}
		}
	/***	
		{///check balance
			int i, j;
			for(i=0;i<ib;++i){
				for(j=0;j<4;j++){
					printf( "%d " ,cnt[i][j]  )  ;
					}
					printf("\t(%d)\n",( cache+i));
				}
		}
	***/
	(*bvalret)=bval;	
	return vec;		
}

void solution_destroy( solution_t * t){
	if (t==NULL) return ;
	free(t->s);
	free(t);
}	

int _FD(const int a,const  int b, int bz){
	int score=0;
	int i;
	int c=(a^b);
	for(i=0;i<bz;++i){
		if((c&3)>0)score++;	
		c>>=2;
	}
	return score;
}

int _dimer(const int a, int bz){
	int c,h,r,i;
	c=(a>>2);
	r=(c^a);
	for(i=0;i<bz-1;++i){
		int t;
		if((r&3)==0)return 1;	
		r>>=2;
	}
	return 0;
}
inline int isHomo(uint64_t num , uint64_t sz){
	int64_t i=0; 
	while (i<(sz-2)){
		if ((num&3) == ((num>>2) &3) && (num&3)==((num>>4) &3)  ) return 1; 
		num=num>>2;
		i++	;
	}
	return 0;
	
}
inline int isDimer(uint64_t num , uint64_t sz){
	uint64_t i=0 ;
	while(i<(sz-1)){
		if ((num&3 )==((num>>2) &3))	 return 1;
		num=num>>2;
		i++;
	}
	return 0;	
}
inline int isNearHomo(uint64_t num , uint64_t sz){
	uint64_t i=0 ;
	while(i<(sz-2)){
		if ((num&3 )==((num>>4) &3))	 return 1;
		num=num>>2;
		i++;
	}
	return 0;
}
unsigned short int _nmer(const int a, int bz, nmer_stat_t * st){ ///synom homopolymer
	st->trimer=isHomo(a,bz);
	if(st->trimer){
		st->dimer=1;
		st->neartri=1;
	}else{
		st->dimer=isDimer(a, bz);
		st->neartri=isNearHomo(a,bz);
	}
	return (unsigned short int)((st->trimer<<2) | (st->neartri <<1)| (st->dimer));	
}
char * _ntoc(int n, int bz, char *s){
	static char lookup[]={'A', 'C','G','T'};
	int i=0, t,j=0;
	do{
		t=n%4;
		s[j++]=lookup[t];	
		n/=4;
		i++;
	}while( i<bz );
	s[j]='\0';
	return s;
}

inline    int _dist( const int a, int  bz, int mm,solution_t  const *  ss){
	if (ss->m<1) return 1; 	
	int i ,exceed=0, maxd=0;
	char *abuff=(char *)malloc(sizeof(char)*(bz+1));
	char *ibuff=(char *)malloc(sizeof(char)*(bz+1));
	for (i=0;i<ss->m;++i){
		//int d=_FD(a, ss->s[i] ,bz) ;
		//switch with FD/ED to using different algorithm 
		int d=ED(_ntoc(a,bz, ibuff) , _ntoc(ss->s[i] ,bz,abuff)) ;
		if (d < mm){ 
			exceed=1;
			//printf("%s\t%s\n", _ntoc(a,bz, ibuff) , _ntoc(ss->s[i] ,bz,abuff));
			break;
		}
	}
	free(abuff);
	free(ibuff);
	return exceed;
}
void solution_print(solution_t *ss)
{
	int i;
	int bz=ss->bz;
	char *sbuff=(char *)malloc(sizeof(char)*(bz+1));
	for(i=0;i<ss->m;++i){
		
		//printf("sln %d , %s  _ntoc %s\n",ss->s[i], ntoc(ss->s[i], bz),  _ntoc(ss->s[i],bz, sbuff));
		printf("%s\n",ntoc(ss->s[i], bz) ) ;
	}
	free(sbuff);
}
/*
 * solve:
 * precompute all barcodes using digital representation 
 * remote all barcodes in this pool not satisfy restritions 
 * parameters:
 * 	bz: barcode length for desgin
 * 	mm: mismatches allowed each other
 *	strict: restritions
 * return : 
 * 	solutions_t * a list of barcode satisfied all criteria
 *
 * */
solution_t * solve(
	int bz ,/*barsize*/ 
	int mm,/*Number of mismatches*/ 
	int strict /*7 : everything, 3: no trimer,1: no dimer , 0 : no neartri */
	)
{
	unsigned short int  *vec;
	int nvec=1;
	{
		int i;
		for(i=0, nvec=1;i<bz;++i){
			nvec*=(N_NUCL);
		}
		vec =( unsigned short int *) calloc(nvec, sizeof( unsigned short  int));
		if(vec==NULL){ fprintf(stderr,"failed vec\n"); exit(-1);}
	}
	solution_t * ss=solution_init(nvec, bz) ;
	{
		int i;
		nmer_stat_t *st=(nmer_stat_t *)calloc(1, sizeof(nmer_stat_t));	
		for( i=0;i<nvec;++i){
			//pre compute status of barcode into vec
			//possible values are 0, 1, 3 and 7	
			unsigned short  int stat=_nmer(i,bz,st );
			vec[i]=stat;
			//if (st->trimer || st->dimer || st->neartri) vec[i]=1 ; //1 means not to use
			//if (st->trimer || st->dimer ) vec[i]=1 ; //1 means not to use
			
		}
		free(st);
	}
	/**
	{	
		int i ;
		char *sb=(char *)malloc(sizeof(char)*(bz+1));
		for(i=0;i<nvec;++i){
			//if (vec[i]) continue ;
			printf("%d\t%s\n",vec[i],_ntoc(i,bz,sb));
		}
		free(sb);
	}
	**/
	do{
		int i;
		int start_pos=0;
		#ifdef _RANDOM_START
			srand(time(NULL));
			start_pos=rand()%nvec;
		#endif  //_RANDOM_START
		start_pos%=nvec ;	
		fprintf(stderr, "start position: %d\n", start_pos);
		for(i=start_pos;;){
			if ( vec[i]<=strict ) { //if restriction value is less than required
				if (ss->m==0 ){ //if mismatch is zero ,don't bother start the distance compute
					ss->s[ss->m++]=i;
				}else{ //otherwise, compute it using any distance function(FD/ED)
					int exceed=_dist(i,bz, mm, ss );
					if (!exceed){
						if (ss->m<ss->n)
							ss->s[ss->m++]=i; //everything is satisfied , save it into storage
					}
				}
			}
			i=(i+1)%nvec ;
			if (i==start_pos)
				break;
		}
	}while(0);

	free(vec);	
	//solution_destroy(ss);
	fprintf(stderr, "Found slolution %d [strict=%d]\n", ss->m,strict);
	return ss;
}

