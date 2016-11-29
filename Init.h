void InitUBM(int FrmNmb, int Mixs,  int FeatureOder, float **Vectors, 
			 float* MixWeight, float **MixMean, float **MixVariance, float *MixDen);
void KMeansInit(int SpkFrmNm, int UBMMixtures,  int SpkGnrlOrdr, float **Vectors, 
				float* LogUBMWeights,float **UBMMeans, float **RevUBMDevs, float *Den);