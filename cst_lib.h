
#pragma once

#define		FALSE				0
#define		TRUE				1
#define		ERR_NOERROR			1
#define		PI					3.14159265358979f
#define		M_PI				PI
#define		TPI					6.28318530717959     /* PI*2 */


#define		SAMPLE_FREQUENCY	8000.0					   
#define		FRAME_LEN			160						
#define		OVERLAP_NUM			FRAME_LEN/2						
#define		FRAME_STEP			(FRAME_LEN-OVERLAP_NUM)	


#define		PRE_EMPHASIS_FACTOR	0.98f	


/****************************
MFCC
****************************/
#define		CEP_NUM				19//12						
#define		DCEP_NUM			19//12						
#define		DDCEP_NUM			0//1						
#define		ENERGY_NUM			0//1						
#define		D_ENERGY_NUM		0//1						
#define		DD_ENERGY_NUM		0//1						

#define		FEATURE_NUM			12//(CEP_NUM + DCEP_NUM + DDCEP_NUM + ENERGY_NUM+D_ENERGY_NUM+DD_ENERGY_NUM)
#define		FILTERS_NUM         24						
#define		MELFLOOR			1.0f					
#define		FFT_ORDER			9						
#define		FFT_NUM				512						
#define		MAX_FREQ_POINT		(FFT_NUM / 2)

#define		FIRSTDELT_FACTOR		0.4f					
#define		SENCODELT_FACTOR		1.5f					

//#define		HIGH_PASS_FREQUENCY	8000
#define		HIGH_PASS_FREQUENCY	3450                 
//#define		LOW_PASS_FREQUENCY	0
#define		LOW_PASS_FREQUENCY	250                  



#define		MFCC				0x0001				
#define		LPCC				0x0002				
#define		PLP					0x0004				

#define		VAD					0x0008              
#define		CMS					0x0010              
#define     VARN				0x1000
#define		PREEMPHASIS			0x0020
#define		HIGHPASS			0x0040				
#define		DELTFEATURE			0x0080				
#define		ENERGY				0x0100
#define		DELTENERGY			0x0200				
#define		DDELTENERGY			0x0400				
#define		DDELTFEATURE		0x0800				


#define  LPC_ORDER         14								
//-------------------------------------------------------------------------------->

#define		COMPRESSFACT	1.0f/3

const long max_sentence_mem = 30*30*240;
const long max_sentence_mfcc_num = (max_sentence_mem - FRAME_LEN)/FRAME_STEP + 1;


#define		FAIL	0
#define		SUCCESS	1

