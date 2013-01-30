#include "barGen_digit.h"

#ifndef _LOOKUP_H
#define _LOOKUP_H
/*
 * a quick look up table for converting a number with 4 NT bases 
 * */
char lookuptab[256][4]=
{
"AAAA","AAAC","AAAG","AAAT","AACA","AACC","AACG","AACT",
"AAGA","AAGC","AAGG","AAGT","AATA","AATC","AATG","AATT",
"ACAA","ACAC","ACAG","ACAT","ACCA","ACCC","ACCG","ACCT",
"ACGA","ACGC","ACGG","ACGT","ACTA","ACTC","ACTG","ACTT",
"AGAA","AGAC","AGAG","AGAT","AGCA","AGCC","AGCG","AGCT",
"AGGA","AGGC","AGGG","AGGT","AGTA","AGTC","AGTG","AGTT",
"ATAA","ATAC","ATAG","ATAT","ATCA","ATCC","ATCG","ATCT",
"ATGA","ATGC","ATGG","ATGT","ATTA","ATTC","ATTG","ATTT",
"CAAA","CAAC","CAAG","CAAT","CACA","CACC","CACG","CACT",
"CAGA","CAGC","CAGG","CAGT","CATA","CATC","CATG","CATT",
"CCAA","CCAC","CCAG","CCAT","CCCA","CCCC","CCCG","CCCT",
"CCGA","CCGC","CCGG","CCGT","CCTA","CCTC","CCTG","CCTT",
"CGAA","CGAC","CGAG","CGAT","CGCA","CGCC","CGCG","CGCT",
"CGGA","CGGC","CGGG","CGGT","CGTA","CGTC","CGTG","CGTT",
"CTAA","CTAC","CTAG","CTAT","CTCA","CTCC","CTCG","CTCT",
"CTGA","CTGC","CTGG","CTGT","CTTA","CTTC","CTTG","CTTT",
"GAAA","GAAC","GAAG","GAAT","GACA","GACC","GACG","GACT",
"GAGA","GAGC","GAGG","GAGT","GATA","GATC","GATG","GATT",
"GCAA","GCAC","GCAG","GCAT","GCCA","GCCC","GCCG","GCCT",
"GCGA","GCGC","GCGG","GCGT","GCTA","GCTC","GCTG","GCTT",
"GGAA","GGAC","GGAG","GGAT","GGCA","GGCC","GGCG","GGCT",
"GGGA","GGGC","GGGG","GGGT","GGTA","GGTC","GGTG","GGTT",
"GTAA","GTAC","GTAG","GTAT","GTCA","GTCC","GTCG","GTCT",
"GTGA","GTGC","GTGG","GTGT","GTTA","GTTC","GTTG","GTTT",
"TAAA","TAAC","TAAG","TAAT","TACA","TACC","TACG","TACT",
"TAGA","TAGC","TAGG","TAGT","TATA","TATC","TATG","TATT",
"TCAA","TCAC","TCAG","TCAT","TCCA","TCCC","TCCG","TCCT",
"TCGA","TCGC","TCGG","TCGT","TCTA","TCTC","TCTG","TCTT",
"TGAA","TGAC","TGAG","TGAT","TGCA","TGCC","TGCG","TGCT",
"TGGA","TGGC","TGGG","TGGT","TGTA","TGTC","TGTG","TGTT",
"TTAA","TTAC","TTAG","TTAT","TTCA","TTCC","TTCG","TTCT",
"TTGA","TTGC","TTGG","TTGT","TTTA","TTTC","TTTG","TTTT",
};
#endif
/*
 * string to int64_t numbers convertor
 * */
int64_t cton(char *s){
	//not work with more than 32 chars 
	if (strlen(s)>32) return UINT64MAX;
	int64_t d, i, sz=strlen(s);
	int64_t rst=0;
	for (i=0;i<sz; i++){
		rst=rst<<2;
		switch (s[i]){
			case 'A':
				d=0;
				break;		
			case 'C':
				d=1;
				break;		
			case 'G':
				d=2;
				break;		
			case 'T':
				d=3;
				break;
			default: 
				d=0;
				break;		
		}	
		rst=(rst|d);
	}
	return rst;	
}

#define HI0	0xff00000000000000UL
#define HI1	0x00ff000000000000UL
#define HI2	0x0000ff0000000000UL
#define HI3	0x000000ff00000000UL
#define HI4	0x00000000ff000000UL
#define HI5	0x0000000000ff0000UL
#define HI6	0x000000000000ff00UL
#define HI7	0x00000000000000ffUL
uint64_t tab[8]={  HI0,HI1,HI2,HI3,HI4,HI5,HI6,HI7 } ;
/*
 *number to string convert function using lookup table
* not too useful
 * */
char * _ntoc_aux( int64_t num, int64_t sz){ 
	int bytes =8;
	int i =0,j=0,k=0;
	uint64_t  idx;
	short li;
	static char out[33];
	
	for(i=j=k=0;i<bytes; ++i){
		li=(num&tab[bytes-1-i]  )>>i*16;
		printf("%d\n",li);
		continue ;
		out[k++]=lookuptab[li][0];
		out[k++]=lookuptab[li][1];
		out[k++]=lookuptab[li][2];
		out[k++]=lookuptab[li][3];
	}
	out[sz]='\0';	
	return out;	
}
/*
 * number to NT string  
 * */

char * ntoc( int64_t num, int64_t sz){ //number to a sequence
	if (sz>32)return NULL;
	int64_t c, i ;
	char t;
	static char out[33];
	for (i=0;i<sz; i++){
		c=(num&3);
		switch (c){
			case 0:
				t='A';
				break;
			case 1:
				t='C';
				break;
			case 2:
				t='G';
				break;
			case 3:
				t='T';
				break;
			default:
				break; 
		}
		out[sz-1-i]=t;
		num=num>>2;
	}
	out[sz]='\0';
	return out;		
}
