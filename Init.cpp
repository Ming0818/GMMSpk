#include "ASRGeneral.h"
#include "time.h"
/*===========================================================
===========================================================*/

void InitUBM(int FrmNmb, int Mixs,  int FeatureOder, float **Vectors, 
			 float* MixWeight, float **MixMean, float **MixVariance, float *MixDen)
{
	int iRandFrm=0;
	srand((unsigned)time(NULL));
	for (int iMixs=0; iMixs<Mixs; iMixs++)
	{
		MixWeight[iMixs] = (float)log(1.0 / Mixs);
		MixDen[iMixs] = 0;

		int TempRange = (FrmNmb-10)/RAND_MAX;
		if(TempRange==0)
			iRandFrm = int(rand()*(FrmNmb-10)/RAND_MAX)+2;
		else
			iRandFrm = int(rand()*TempRange)+2;

		for (int iFeatureOder=0; iFeatureOder<FeatureOder; iFeatureOder++)
		{
			MixVariance[iMixs][iFeatureOder] = 1;
			MixMean[iMixs][iFeatureOder] = Vectors[iRandFrm][iFeatureOder];
		}
		printf("InitUBM going:%d\r",iMixs);
	}
	printf("\n");
}
//>---------------------------------------------------------------------------



void KMeansInit(int SpkFrmNm, int UBMMixtures,  int SpkGnrlOrdr, float **Vectors, 
				float* LogUBMWeights,float **UBMMeans, float **RevUBMDevs, float *Den)
{
	int iRandKMean = 0;
	srand((unsigned)time(NULL));
	int iFeatureOder;
	int TempRange;
	
	
	for(int iMixs=0; iMixs<UBMMixtures; iMixs++)
	{		
		//UBMWeights[iMixs]=1.0/UBMMixtures;
		LogUBMWeights[iMixs] = (float)log(1.0/UBMMixtures);
		TempRange = (SpkFrmNm-10)/RAND_MAX;
		if(TempRange==0)
			iRandKMean = rand()*(SpkFrmNm-10)/RAND_MAX + 2;
		else
			iRandKMean = rand()*TempRange + 2;
		
		for (iFeatureOder=0; iFeatureOder<SpkGnrlOrdr; iFeatureOder++)
		{
			RevUBMDevs[iMixs][iFeatureOder]=1;
			UBMMeans[iMixs][iFeatureOder]=Vectors[iRandKMean][iFeatureOder];
		}
		Den[iMixs] = 0;
		
	}
	//printf("random initialization complete\n");
	
	int BelongMixNm,iFrmNmb;
	double TempDistance, MiniDistance,TempMulti;
	double DstSum, LastDstSum=0;
	int *MixsFrmNm = (int*)malloc(sizeof(int)*UBMMixtures);
	
	
	float **TempMean = (float **)malloc(sizeof(float *)*UBMMixtures);
	float *TempMeanBuffer = (float *)malloc(sizeof(float)*UBMMixtures*SpkGnrlOrdr);
	for (int iMixs=0; iMixs<UBMMixtures; iMixs++)
	{
		TempMean[iMixs]=&TempMeanBuffer[iMixs*SpkGnrlOrdr];
	}
	
	
	bool kconverge=false;   
	int kMaxIter=5;        
	int kIter=0;            
	double kConvergeRat = 1e-3;
	
	while (kIter<kMaxIter && !kconverge)
	{
#ifdef DEBUG
		printf("k-means initialize iteration: %d\n",kIter);
#endif
		DstSum = 0;
		memset(MixsFrmNm,0,sizeof(int)*UBMMixtures);
		memset(TempMeanBuffer,0,sizeof(float)*UBMMixtures*SpkGnrlOrdr);

		
		for(iFrmNmb=2; iFrmNmb<(SpkFrmNm-2); iFrmNmb++)
		{	
#ifdef DEBUG
			if(iFrmNmb%10000==0)
				printf("Total Frame Number:%d,Current Frame: %d\r",(SpkFrmNm-2),iFrmNmb);
#endif
			BelongMixNm = 0;
			
			TempDistance = 0;
			for( iFeatureOder=0; iFeatureOder<SpkGnrlOrdr; iFeatureOder++)//
			{
				TempMulti=Vectors[iFrmNmb][iFeatureOder]-UBMMeans[0][iFeatureOder];				
				TempDistance += TempMulti*TempMulti;
			}
			MiniDistance = TempDistance;

			for(int iMixs=1; iMixs<UBMMixtures; iMixs++)//
			{
				TempDistance = 0;
				for( iFeatureOder=0; iFeatureOder<SpkGnrlOrdr; iFeatureOder++)
				{
					TempMulti=Vectors[iFrmNmb][iFeatureOder]-UBMMeans[iMixs][iFeatureOder];

					TempDistance += TempMulti*TempMulti;
				}
				//if(iMixs==0)
				//	MiniDistance = TempDistance;
				
				if(TempDistance<MiniDistance)
				{
					MiniDistance = TempDistance;
					BelongMixNm = iMixs;
				}
			}//
			
			DstSum += MiniDistance;
			MixsFrmNm[BelongMixNm] += 1;
			for (iFeatureOder=0; iFeatureOder<SpkGnrlOrdr; iFeatureOder++)
			{
				TempMean[BelongMixNm][iFeatureOder] += Vectors[iFrmNmb][iFeatureOder];
			}
			
		}//
#ifdef DEBUG
		printf("\n");
#endif
		
		//
		for(int iMixs=0; iMixs<UBMMixtures; iMixs++)
		{
			if(MixsFrmNm[iMixs] > 0)//
			{
				for (iFeatureOder=0; iFeatureOder<SpkGnrlOrdr; iFeatureOder++)
				{
					UBMMeans[iMixs][iFeatureOder] = TempMean[iMixs][iFeatureOder]/MixsFrmNm[iMixs];
				}
			}
		}
		
		//
		double TempRemainder = -DstSum + LastDstSum;
		if(TempRemainder<0)
			TempRemainder = -TempRemainder;
		printf("Distance Sum = %g  TempRemainder = %g\n",DstSum,TempRemainder);
		if (TempRemainder<kConvergeRat * LastDstSum)
			kconverge=true;
		LastDstSum = DstSum;
		kIter++;
		
	}//
	//printf("k-means initiate successfully!\n");
	
	free(MixsFrmNm);
	free(TempMean);
	free(TempMeanBuffer);

}	



