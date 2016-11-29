#include "memory.h"
#include "stdlib.h"
#include "stdio.h"
#include <xmmintrin.h> 

void GMMMemAlloc(float *&Weights, float **&Means, float **&Devs,
				float *&MeanBuffer,float *&DevBuffer,float *&Den,int UBMMixtures,int SpkGnrlOrdr)
{
	Weights = (float*)_mm_malloc(sizeof(float)*UBMMixtures,16);
	Means = (float**)malloc(sizeof(float)*UBMMixtures);
	Devs = (float**)malloc(sizeof(float)*UBMMixtures);
	MeanBuffer = (float*)_mm_malloc(sizeof(float)*UBMMixtures*SpkGnrlOrdr,16);//
	DevBuffer = (float*)_mm_malloc(sizeof(float)*UBMMixtures*SpkGnrlOrdr,16);//
	memset(DevBuffer,0,sizeof(float)*UBMMixtures*SpkGnrlOrdr);//
	memset(MeanBuffer,0,sizeof(float)*UBMMixtures*SpkGnrlOrdr);//
	Den = (float*)_mm_malloc(sizeof(float)*UBMMixtures,16);
	
	for (int iMix=0; iMix<UBMMixtures; iMix++)
	{
		Means[iMix]=&MeanBuffer[iMix*SpkGnrlOrdr];
		Devs[iMix]=&DevBuffer[iMix*SpkGnrlOrdr];
	}
//	printf("\nmalloc!\n");
	return;

}


void GMMMemFree(float *&Weights, float **&Means, float **&Devs,
				float *&MeanBuffer,float *&DevBuffer,float *&Den,int UBMMixtures,int SpkGnrlOrdr)
{
	_mm_free(Weights);
	_mm_free(MeanBuffer);
	_mm_free(DevBuffer);
	free(Means);
	free(Devs);
	_mm_free(Den);
//	printf("\nfree!\n");
	return ;
}

void VectorMemAlloc(float **&Vectors, float *&VectorBuff,int SpkFrmNm, int SpkGnrlOrdr)
{
	//short SpkGnrlOrdr_Temp=SpkGnrlOrdr+4-SpkGnrlOrdr%4;
	Vectors = (float**)malloc(sizeof(float*)*SpkFrmNm);
	VectorBuff = (float*)_mm_malloc(sizeof(float)*SpkFrmNm*SpkGnrlOrdr,16);
	memset(VectorBuff,0,sizeof(float)*SpkFrmNm*SpkGnrlOrdr);//
	                                                       //


	for(int iSpkFrmNm = 0; iSpkFrmNm<SpkFrmNm; iSpkFrmNm++)
	{
		Vectors[iSpkFrmNm]=&VectorBuff[iSpkFrmNm*SpkGnrlOrdr];
	}
	return;
}

/*==============================================================================
=============================================================================*/
void VectorMemFree(float **&Vectors, float *&VectorBuff)
{
	if(Vectors != NULL)
	{
		free(Vectors);
		Vectors = NULL;
	}
	if(VectorBuff != NULL)
	{
		_mm_free(VectorBuff);
		VectorBuff = NULL;
	}
	return;
}

/*==============================================================================
==============================================================================*/
void SpeakerNameAlloc( char **&Speaker, int SpeakerCount )
{
	Speaker = new char * [SpeakerCount];
	
	for( int i=0; i<SpeakerCount; i++ ){
		Speaker[i] = new char [100];
	}
	
	return;
}
/*==============================================================================
==============================================================================*/
void SpeakerNameFree( char **&Speaker, int SpeakerCount )
{
	for( int i=0; i<SpeakerCount; i++ ){
		if( Speaker[i] != NULL ){
			delete Speaker[i];
			Speaker[i] = NULL;
		}
	}
	
	delete Speaker;
	Speaker = NULL;
	
	return;
}

/*==============================================================================
==============================================================================*/
void FwriteGMM(FILE *fp_mod, float *Weights, float **Means, float **Devs, float *Den, int UBMMixtures,int SpkGnrlOrdr)
{
	fwrite(Weights, sizeof(float), UBMMixtures, fp_mod );
	for(int i=0; i<UBMMixtures; i++)
	{
		fwrite(Means[i],sizeof(float),SpkGnrlOrdr,fp_mod);//
	}
	for(int i=0; i<UBMMixtures; i++)
	{
		fwrite(Devs[i],sizeof(float),SpkGnrlOrdr,fp_mod);
	}
	fwrite(Den, sizeof(float), UBMMixtures, fp_mod );
	
	return;
}

/*==============================================================================
==============================================================================*/
void FreadGMM(FILE *fp_mod, float *Weights, float **Means, float **Devs, float *Den, int UBMMixtures,int SpkGnrlOrdr)
{
	fread(Weights, sizeof(float), UBMMixtures, fp_mod );
	for(int i=0; i<UBMMixtures; i++)
	{
		fread(Means[i],sizeof(float),SpkGnrlOrdr,fp_mod);//
	}
	for(int i=0; i<UBMMixtures;i++)
	{
		fread(Devs[i],sizeof(float),SpkGnrlOrdr,fp_mod);
	}	
	fread(Den,sizeof(float),UBMMixtures,fp_mod); 
	
	return;
}