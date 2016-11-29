#include "ASRGeneral.h"
#include "Init.h"
#include "mathTool.h"
#include "MemAllocFree.h"
#include "GaussLogScore.h"
#include "UBMGaussLogScore.h"
//=========================================================================
//Identify
//success return 1, else return 0
//=========================================================================
int  Identify(float **Vectors, char *SpeakerID, int SpkFrmNm)
{
	//<------------------------read speakers' number and list from speaker.lst------
	FILE *fp_speakerlist = NULL;
	fp_speakerlist = fopen( ".\\model\\speaker.lst", "r+" );
	
	if(fp_speakerlist == NULL)
	{
		printf("file speaker.lst open error!\n");
		getchar();
		return 0;
	}
	
    int SpeakerCount = 0;   
	char **Speaker = NULL;
	
	fscanf( fp_speakerlist, "%d", &SpeakerCount);//read speakers' number
	SpeakerNameAlloc( Speaker, SpeakerCount );//
	
	for( int iSp = 0 ; iSp < SpeakerCount ; iSp++ )
	{
		fscanf(fp_speakerlist,"%s",Speaker[iSp]);
	}
	fclose( fp_speakerlist );
	fp_speakerlist = NULL;
	//------------------------------------------------------------------------------------>
	
	//<----------------------------Calculate GMM Scores----------------
	float *TgtWeights, **TgtMeans, **TgtDevs,*TgtMeanBuffer,*TgtDevBuffer,*TgtDen;
	GMMMemAlloc(TgtWeights,TgtMeans,TgtDevs,TgtMeanBuffer,TgtDevBuffer,TgtDen,MIXTURE_NUM,FEATURE_ORDER_4MUL);
	
	char TgtModeFile[_MAX_PATH];	
	FILE *fp_Tgtmod = NULL;

	float **GMMScore, *GMMScoreBuff;
	VectorMemAlloc(GMMScore,GMMScoreBuff,SpkFrmNm,MIXTURE_NUM);//

	int iFrm, iMix;	float Likelihood, *LogLikelihood;
	LogLikelihood = (float*)malloc(sizeof(float) * SpeakerCount); 

	for(int iSp = 0; iSp < SpeakerCount; iSp++)
	{
		sprintf(TgtModeFile,".\\model\\%s.mod", Speaker[iSp]);//
		
		fp_Tgtmod = fopen(TgtModeFile, "rb");//
		if(fp_Tgtmod == NULL)//
		{	
			printf("%s.mod file open error!\n", Speaker[iSp]);
			GMMMemFree(TgtWeights,TgtMeans,TgtDevs,TgtMeanBuffer,TgtDevBuffer,TgtDen,MIXTURE_NUM,FEATURE_ORDER);
			return 0;
		}
		
		FreadGMM(fp_Tgtmod, TgtWeights, TgtMeans, TgtDevs, TgtDen, MIXTURE_NUM, FEATURE_ORDER);	
		fclose(fp_Tgtmod); fp_Tgtmod = NULL;
		
		
		UBMGaussLogScore(Vectors,TgtWeights,TgtMeans,TgtDevs,TgtDen,SpkFrmNm,GMMScore);
		
		LogLikelihood[iSp] = 0;
		for(iFrm = 0; iFrm < SpkFrmNm; iFrm++)
		{ 
			Likelihood = LOG_ZERO; //

			for(iMix = 0; iMix < MIXTURE_NUM; iMix++)
			{								
				Likelihood = LogAdd_Fast(Likelihood,GMMScore[iFrm][iMix]);
				
			}//

			LogLikelihood[iSp] += Likelihood;

		}//frame end		
	}
	
	GMMMemFree(TgtWeights,TgtMeans,TgtDevs,TgtMeanBuffer,TgtDevBuffer,TgtDen,MIXTURE_NUM,FEATURE_ORDER_4MUL);
	VectorMemFree(GMMScore, GMMScoreBuff);
	//>--------------------------------------------------------------------------------
	
	//<-----------------------------Find the highest score---------------------
	float TempScore = LogLikelihood[0];
	int SpeakerNum = 0;
	for(int iSp=1; iSp < SpeakerCount; iSp++)
	{
		if(TempScore < LogLikelihood[iSp])
		{
			TempScore = LogLikelihood[iSp];
			SpeakerNum = iSp;
		}	
	}
	printf("the speaker is: %10s ",Speaker[SpeakerNum]);
	
	if(strcmp(SpeakerID, Speaker[SpeakerNum]) == 0)
		printf("right!");
	else
		printf("wrong!");
	printf("\n");
	//>-----------------------------------------

	
	SpeakerNameFree( Speaker, SpeakerCount);
	return 1;
}