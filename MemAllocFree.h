void GMMMemAlloc(float *&Weights, float **&Means, float **&Devs,
				float *&MeanBuffer,float *&DevBuffer,float *&Den,int UBMMixtures,int SpkGnrlOrdr);
void GMMMemFree(float *&Weights, float **&Means, float **&Devs,
				float *&MeanBuffer,float *&DevBuffer,float *&Den,int UBMMixtures,int SpkGnrlOrdr);

void VectorMemAlloc(float **&Vecors, float *&VectorBuff,int SpkFrmNm, int SpkGnrlOrdr);
void VectorMemFree(float **&Vecors, float *&VectorBuff);

void SpeakerNameAlloc( char **&Speaker, int SpeakerCount );
void SpeakerNameFree( char **&Speaker, int SpeakerCount );

void FwriteGMM(FILE *fp_mod, float *Weights, float **Means, float **Devs, float *Den, int UBMMixtures,int SpkGnrlOrdr);
void FreadGMM(FILE *fp_mod, float *Weights, float **Means, float **Devs, float *Den, int UBMMixtures,int SpkGnrlOrdr);