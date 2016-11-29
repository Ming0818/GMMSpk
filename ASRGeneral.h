#include "memory.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "io.h"
#include "string.h"
//#include "iostream.h"

#include "cst_lib.h"

const int SPEAKERNAMELENGTH = 100;
const int ADAPTLENTH = 16000*60;
const int DETECTLENTH = 16000*30;
const int IDENTIFYLENTH = 16000*3;

#define     CHOOSE_FRAME_RATE   0.2    
#define     FEATURE_ORDER       12//38     
#define     FEATURE_ORDER_4MUL  40     
#define		MIXTURE_NUM			10//512		
#define		CONVERGE_RATE		1e-3   
#define     TOPC                5     
#define     COHORT_SIZE         29     
#define		DECIMATION_RATE		5	   

#define LOG_ZERO	((float)(-1.0E10))	

typedef int Bool;

#define DEBUG   

const int ADAPTMODE =       1;	
const int ADAPT_UBM_Depend =1;	
const int DETECT_UBM_Depend=1;	
const int SMS_Transform	=   0;	
const int HNORM_Work=		0;	
const int TNORM_WORK=		0;  
const int HTNORM_WORK=		0;  