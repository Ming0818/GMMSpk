#include "stdio.h" 
/*==============================================================================
GaussLogScore
=============================================================================*/
float GaussLogScore(float *Vectors,float LogGMMWeights,float *GMMMeans,float *RevGMMDevs,
								 float LogDevDen,int SpkGnrlOrdr)
{
	float tmp,pmt,bmt=0;
	for (int dim=0; dim<SpkGnrlOrdr; dim++)
	{
		tmp=Vectors[dim]-GMMMeans[dim];
		bmt+=tmp*tmp*RevGMMDevs[dim];
	}
	bmt = (float)-0.5*bmt;
	bmt -= LogDevDen;//		
	pmt = LogGMMWeights + bmt;
	
	return pmt;
}


