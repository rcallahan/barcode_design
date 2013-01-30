#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "khash.h"
KHASH_SET_INIT_STR(str);

#include <zlib.h>
#include "kseq.h"
#define BUF_SIZE 4096
KSTREAM_INIT (gzFile, gzread, BUF_SIZE)
float std_extra (const int *arr, int extra,int n)
{
  int       sum = 0;           /* The sum of all elements so far. */
  int       sum_sqrs = 0;      /* The sum of their squares. */
  float     variance;
  int       i;
  float average,std_dev;
  for (i = 0; i < n; i++)
  {
    sum += arr[i];
    sum_sqrs += arr[i]*arr[i];
  }
	sum+=extra;
	sum_sqrs+=extra*extra;		
  /* Compute the average and variance,
     using the equality: Var(X) = E(X^2) - E(X)*E(X) */
  average = (float)sum / (float)(n+1.);
  variance = (float)sum_sqrs / (float)(n+1.) - (average)*(average);
 
  /* Compute the standard deviation. */
  std_dev = sqrt(variance);
  return std_dev;
}
float std (const int *arr, int n)
{
  int       sum = 0;           /* The sum of all elements so far. */
  int       sum_sqrs = 0;      /* The sum of their squares. */
  float     variance;
  int       i;
  float average,std_dev;
  for (i = 0; i < n; i++)
  {
    sum += arr[i];
    sum_sqrs += arr[i]*arr[i];
  }
  /* Compute the average and variance,
     using the equality: Var(X) = E(X^2) - E(X)*E(X) */
  average = (float)sum / (float)n;
  variance = (float)sum_sqrs / (float)n - (average)*(average);
 
  /* Compute the standard deviation. */
  std_dev = sqrt(variance);
  return std_dev;
}
int min(float *arr, int n){
	int i,j=0;
	for(i=0;i<n;++i){
		if(arr[j]>arr[i])j=i;
	}
	return j;
}
int ** init_mx(int firstn){
	int **mx=(int **)malloc(sizeof(int*)*firstn);
	int i, j;
	for(i=0;i<firstn;++i){
		mx[i]=(int *)malloc(sizeof(int)*4);
		for(j=0;j<4;j++){
			mx[i][j]=0;
		}
	}
	return mx;
}
void stat_target( char *s, int *vec, int firstn){
	int i;
	for(i=0;i<firstn;i++){
		switch( s[i]){
			case 'A':
				vec[i]=0;
				break;
			case 'C':
				vec[i]=1;
				break;
			case 'G':
				vec[i]=2;
				break;
			case 'T':
				vec[i]=3;
				break;
			default:
				break;
		}
	}

}
void update_mx(int **mx_current,  int *target, int firstn ){
	int i,j;
	for(i=0;i<firstn;i++){
		switch(target[i]){
			case 0:
				mx_current[i][0]++;
				break;
			case 1:
				mx_current[i][1]++;
				break;
			case 2:
				mx_current[i][2]++;
				break;
			case 3:
				mx_current[i][3]++;
				break;
			default:
				break;
		}
		
	}
}

	
float  bias2(int **mx,  int *target, int firstn){
	float b=0.0;
	int v[4];
	int i,j;
	for(i=0;i<firstn;i++ ){
		for( j=0;j<4;j++){
			v[j]=mx[i][j];
		}	
		switch( target[i]){
			case 0:
				v[0]++;
				break;
			case 1:
				v[1]++;
				break;
			case 2:
				v[2]++;
				break;
			case 3:
				v[3]++;
				break;
			default:
				break;
		}
		b+=std(v, 4);
	}
	return b;
}


float bias(kh_str_t *current, char * target, int firstn){
	int vec[firstn][4];
	int i,j;
	for(i=0;i<firstn;i++){
		for(j=0;j<4;j++){
			vec[i][j]=0;
		}
	}
	khiter_t k;
	for(i=0;i<firstn;i++){
		switch(target[i]){
			case 'A':
				vec[i][0]+=1;
				break;
			case 'C':
				vec[i][1]+=1;
				break;
			case 'G':
				vec[i][2]+=1;
				break;
			case 'T':
				vec[i][3]+=1;
				break;
			default:
				break;
		}
	}
	for(k=kh_begin(current);k!=kh_end(current);k++){
		if(!kh_exist(current, k)) continue;
		char *c=(char *)kh_key(current, k);
		for(i=0;i<firstn;++i){
			switch(c[i]){
				case 'A':
					vec[i][0]+=1;
					break;
				case 'C':
					vec[i][1]+=1;
					break;
				case 'G':
					vec[i][2]+=1;
					break;
				case 'T':
					vec[i][3]+=1;
					break;
				default:
					break;
			}
		}
	}
	float b=0.0;
	for(i=0;i<firstn;++i){
		b+=std(vec[i], 4);	
	}
	return b;	
}

char ** balancer(kh_str_t * current, kh_str_t* pool,int goalsz, int firstn){
	khiter_t k;
	int i,j,pool_sz,kret;
	int **mx=init_mx(firstn);
	int target[firstn];
	char **ordered=malloc(sizeof(char *) * kh_size(pool)  );
	int ordered_i=0; // index for ordered
	while(kh_size(current)<goalsz && kh_size(pool)>0 ){
		pool_sz=kh_size(pool) ;
		float vec[pool_sz] ;
		khiter_t kl[pool_sz];
		for(k=kh_begin(pool),i=0; k!=kh_end(pool);k++){
			if (!kh_exist(pool,k))continue ;
			char *c=(char *)kh_key(pool,k);
			stat_target( c, target, firstn);
			kl[i]=k;
			vec[i]=bias2(mx, target, firstn);
			i++;
		}	
		j=min(vec,pool_sz );
		char *sel=(char *)kh_key( pool, kl[j]) ;
		stat_target(sel, target, firstn);	
		kh_put(str, current, sel, &kret );
		kh_del(str,pool,kl[j] );
		update_mx(mx, target,firstn);
		//fprintf(stderr,"%s\n", sel);
		ordered[ordered_i++]=strdup( sel );
	}
	fprintf(stderr, "current %d\tpool %d\n", kh_size(current), kh_size(pool));	
	return ordered;
}

int load( kh_str_t * h,char *fn){
	int dret, kret,i=0;
	gzFile fp=gzopen(fn, "r");
	kstream_t *ks = ks_init (fp);
        kstring_t *s = calloc (1, sizeof (kstring_t));
	int barlen=0;
	while( ks_getuntil (ks, '\n', s, &dret)  >=0 ){
		//printf("%s , s->l %d\n",s->s,s->l);
		barlen=s->l ;
		if(i>=6144) break;
		char * c=strdup(s->s);	
		if(c!=NULL)	
		kh_put(str, h, c,&kret );
		i++;
	}
	
	if (s->s) free (s->s);
        free (s);
        ks_destroy (ks);
	
	gzclose(fp);
	return barlen;
	
}


void kh_str_dest(kh_str_t* pool ){
	khiter_t k;
	for(k=kh_begin(pool); k!=kh_end(pool);k++){
		if (!kh_exist(pool,k))continue ;
		if(kh_key(pool,k)!=NULL) free((char*)kh_key(pool,k));
		kh_del(str, pool, k);
	}
	kh_destroy(str, pool);
}

void kh_str_print( kh_str_t* pool ){
	khiter_t k;
	for(k=kh_begin(pool); k!=kh_end(pool);k++){
		if (!kh_exist(pool,k))continue ;
		printf("%s\n", (char*)kh_key(pool,k));
	}
}



int main(int argc, char *argv[]){
	if(argc<=1){
		fprintf(stderr, "./balancer bacode_file number_of_barcode_need_balance[optional, defaut all]\n" );
		exit(-1);
	}
	kh_str_t * pool=kh_init(str);
	kh_str_t * current=kh_init(str);
	int expect_num_barcode=0;
	int barlen=load( pool, argv[1]);
	if(argc>=3){
		expect_num_barcode =atoi(argv[2]); 	
	}else  expect_num_barcode=(int)kh_size(pool);
	//if input is not acceptable 	
	if( expect_num_barcode  > (int)kh_size(pool) || expect_num_barcode <=0 )
		expect_num_barcode=(int)kh_size(pool);	
	
	char ** ordered=balancer(current, pool, expect_num_barcode ,barlen);	
	
	//kh_str_print(current);
	int i;
	for(i=0;i<expect_num_barcode;i++){
		printf("%s\n", ordered[i]);
	}	
	//destroy 	
	kh_str_dest(current );
	kh_str_dest(pool );
}


