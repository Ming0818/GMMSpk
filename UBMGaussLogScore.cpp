#include "ASRGeneral.h"
#include "MemAllocFree.h"
#include "GaussLogScore.h"
#include "mathTool.h"
#include "sys/timeb.h"
#include "time.h"
#include <xmmintrin.h> 
/*==============================================================================
UBMGaussLogScore
=============================================================================*/
void UBMGaussLogScore(float **Vectors, float *UBMWeights, float **UBMMeans, float **UBMDevs, float *UBMDen, int SpkFrmNm, float**TempUBMGauRat_Temp2D)
{
	int iFrm,n,m;
	for (m=0; m<MIXTURE_NUM; m+=8)
	{
		for(iFrm = 0; iFrm < SpkFrmNm; iFrm++) 
		{ 
			TempUBMGauRat_Temp2D[iFrm][m] = GaussLogScore(Vectors[iFrm],UBMWeights[m],UBMMeans[m],UBMDevs[m],UBMDen[m],FEATURE_ORDER);
			n=m+1;
			TempUBMGauRat_Temp2D[iFrm][n] = GaussLogScore(Vectors[iFrm],UBMWeights[n],UBMMeans[n],UBMDevs[n],UBMDen[n],FEATURE_ORDER);
			n=n+1;
			TempUBMGauRat_Temp2D[iFrm][n] = GaussLogScore(Vectors[iFrm],UBMWeights[n],UBMMeans[n],UBMDevs[n],UBMDen[n],FEATURE_ORDER);
			n=n+1;
			TempUBMGauRat_Temp2D[iFrm][n] = GaussLogScore(Vectors[iFrm],UBMWeights[n],UBMMeans[n],UBMDevs[n],UBMDen[n],FEATURE_ORDER);
			n=n+1;
			TempUBMGauRat_Temp2D[iFrm][n] = GaussLogScore(Vectors[iFrm],UBMWeights[n],UBMMeans[n],UBMDevs[n],UBMDen[n],FEATURE_ORDER);
			n=n+1;
			TempUBMGauRat_Temp2D[iFrm][n] = GaussLogScore(Vectors[iFrm],UBMWeights[n],UBMMeans[n],UBMDevs[n],UBMDen[n],FEATURE_ORDER);
			n=n+1;
			TempUBMGauRat_Temp2D[iFrm][n] = GaussLogScore(Vectors[iFrm],UBMWeights[n],UBMMeans[n],UBMDevs[n],UBMDen[n],FEATURE_ORDER);
			n=n+1;
			TempUBMGauRat_Temp2D[iFrm][n] = GaussLogScore(Vectors[iFrm],UBMWeights[n],UBMMeans[n],UBMDevs[n],UBMDen[n],FEATURE_ORDER);
		}
	}	
	return;
	
}
