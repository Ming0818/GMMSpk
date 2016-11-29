#include "ASRGeneral.h"
#include "Init.h"
#include "mathTool.h"
#include "MemAllocFree.h"
#include "GaussLogScore.h"
//=========================================================================
//=========================================================================
int Train(float **Vectors, char *SpeakerID, int SpkFrmNm)
{			
	//<-------------------------Read speaker's number and list--------------------
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

    fscanf( fp_speakerlist, "%d", &SpeakerCount );//
	SpeakerNameAlloc( Speaker, SpeakerCount + 1 );

	for( int iSp = 0; iSp < SpeakerCount; iSp++ )
	{
		fscanf( fp_speakerlist, "%s", Speaker[iSp] );//
	}
	//-------------------------------------------------------------------------------------->
    
	//<-----------add a new speaker and check if he is already in the list-------------
	strcpy(Speaker[SpeakerCount], SpeakerID );//

	for(int iSp=0; iSp < SpeakerCount; iSp++ )
		if( strcmp( Speaker[SpeakerCount],Speaker[iSp] ) == 0 )
		{
			//return 0 if the speaker already exists
			printf("the speaker %s already exist!\n", SpeakerID);
			SpeakerNameFree( Speaker, SpeakerCount + 1 );
            fclose(fp_speakerlist);		fp_speakerlist = NULL;
			return 0;
		}


	//add this speaker
	SpeakerCount++;
	 
	//add new speaker and count
	rewind(fp_speakerlist);
	fprintf( fp_speakerlist, "%d\n", SpeakerCount );
    
	for(int iSp = 0; iSp < SpeakerCount; iSp++ )
	{
		fprintf( fp_speakerlist, "%s\n", Speaker[iSp] );
	}
	fclose( fp_speakerlist );	fp_speakerlist = NULL;
	SpeakerNameFree( Speaker, SpeakerCount);
    //-------------------------------------------------------------------------------------->
	

	//--------------------------Initialization-------------------------
	printf("****%s training********\n", SpeakerID);
	float *LogWeights, **Means, **RevDevs,*MeanBuffer,*DevBuffer,*LogDen;
	GMMMemAlloc(LogWeights,Means,RevDevs,MeanBuffer,DevBuffer,LogDen,MIXTURE_NUM,FEATURE_ORDER_4MUL);
	
	//---------------------------1.Initial GMM-----------------------------------------------
	InitUBM(SpkFrmNm,MIXTURE_NUM,FEATURE_ORDER,Vectors,LogWeights,Means,RevDevs,LogDen);//数据有限，不采用kmean
	
	//<----------------------------2.Train,store infor in Weights,Means,Devs

	float tmp,TempDev,acc,acct,TempRemainder,lastacct = 0;	
	float *accpmt,**accpmtx,**accpmtxx,*accpmtxBuffer,*accpmtxxBuffer,*pmt;
	GMMMemAlloc(accpmt,accpmtx,accpmtxx,accpmtxBuffer,accpmtxxBuffer,pmt,MIXTURE_NUM,FEATURE_ORDER_4MUL);
	
	int MaxIter = 20;			// 
	int m,dim,t,Iter=0;			//
	
	while (Iter<MaxIter)
	{
		printf("UBM training ,Iter : %d\n",Iter);
		
		acct = 0;
		memset(pmt,0,sizeof(float)*MIXTURE_NUM);//*&Den
		memset(accpmt,0,sizeof(float)*MIXTURE_NUM);	//*&Weights					
		memset(accpmtxBuffer,0,sizeof(float)*MIXTURE_NUM*FEATURE_ORDER_4MUL);//**&Means
		memset(accpmtxxBuffer,0,sizeof(float)*MIXTURE_NUM*FEATURE_ORDER_4MUL);//**&Devs
		
		//step through all frames
		for (t= 0; t<SpkFrmNm; t++)
		{
			if(t%1000==0)
				printf("Total Frame number:%d,Current Frame Number: %d\r",(SpkFrmNm-2),t);			
			
			acc = LOG_ZERO;//
			for (m=0; m<MIXTURE_NUM; m++)//calculate pmt for each GMM
			{
				pmt[m] = GaussLogScore(Vectors[t],LogWeights[m],Means[m],RevDevs[m],LogDen[m],FEATURE_ORDER);
				acc = LogAdd_Fast(pmt[m],acc);
			}
			
			for (m=0; m<MIXTURE_NUM; m++)
			{
				pmt[m] -= acc;//pmt[m]=pmt[m]-acc;
				if(pmt[m]<-100)//
					continue;
				pmt[m] = (float)exp(pmt[m]);//pmt[mn]=P(m/Xt,^)
				
				accpmt[m] += pmt[m];//accpmt[m]=accpmt[m]+pmt[m]
				for (dim=0; dim<FEATURE_ORDER; dim++)//
				{
					tmp = pmt[m] * Vectors[t][dim];//p(t)*x(t)
					accpmtx[m][dim] += tmp;//
					tmp *= Vectors[t][dim];
					accpmtxx[m][dim] += tmp;//
				}
			}
			acct += acc;//
		
		}//frame end
		printf("\n");
		
		for (m=0; m<MIXTURE_NUM; m++)
		{
			if(accpmt[m]<10)//
				continue;
			LogDen[m]=0;
			for (dim=0; dim<FEATURE_ORDER; dim++)
			{
				Means[m][dim] = accpmtx[m][dim] / accpmt[m];	
				tmp = Means[m][dim]*Means[m][dim];
				
				TempDev = accpmtxx[m][dim] / accpmt[m] - tmp;
				if(TempDev<1e-10)//
					continue;

				RevDevs[m][dim] = 1 / TempDev;
				LogDen[m] += (float)log(TempDev) / 2;	 
			}
			LogWeights[m] = (float)log(accpmt[m]/SpkFrmNm);
		}
		
		TempRemainder = acct - lastacct; 
		printf("Current value: %g Raise: %g\n",acct,TempRemainder);
		lastacct = acct;
		Iter++;
	}//end frame
	
	GMMMemFree(accpmt,accpmtx,accpmtxx,accpmtxBuffer,accpmtxxBuffer,pmt,MIXTURE_NUM,FEATURE_ORDER_4MUL);
	//>------------------------------------------------------------------------------------------
	
	
	//<--------------------------3.Write model info into UBM.mod
	char ModeFile[_MAX_PATH];
	sprintf(ModeFile,".\\model\\%s.mod",SpeakerID);
	FILE *fp_mod = fopen(ModeFile, "wb");
	
	if(fp_mod==NULL)
	{		
		printf("UBM.mod file open error!\n");
		GMMMemFree(LogWeights,Means,RevDevs,MeanBuffer,DevBuffer,LogDen,MIXTURE_NUM,FEATURE_ORDER);
		return 0;
	}

	FwriteGMM(fp_mod, LogWeights, Means, RevDevs, LogDen, MIXTURE_NUM, FEATURE_ORDER);
	
	fclose( fp_mod );
	GMMMemFree(LogWeights,Means,RevDevs,MeanBuffer,DevBuffer,LogDen,MIXTURE_NUM,FEATURE_ORDER);
	//>----------------------------------------------------------------------
	
	printf("\n %s  trained successfully!\n", SpeakerID);	
	return 1;
	
}
