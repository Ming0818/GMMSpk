#include "GMMSpkId.h"
//=========================================================================
//=========================================================================
int main(int argc,char *argv[])
{
#ifdef TIME
	struct _timeb timebuffer0, timebuffer1;
	int timeconsum = 0;	char *timeline;
	_ftime(&timebuffer0);
	timeline = ctime( & ( timebuffer0.time ) );
	printf("%25.19s.%u\n", timeline, timebuffer0.millitm );
#endif

	//<-------------------------------------------------------	
	//if(argc!=4)
	//{
	//	printf("Train/Identify <speakerID> <speech file name>\n");
	//	getchar();
	//	return 0;
	//}
	
    char Operation[100] = "Identify";
	char SpeakerID[SPEAKERNAMELENGTH] = "man";
	char SpchFilename[_MAX_PATH];

	//read feature
	FILE *fp_Vector = NULL;
	fp_Vector = fopen(".\\feature\\man.txt","rb");//
	if(fp_Vector == NULL)
	{
		printf("speech file open error\n");
		getchar();
		return 0;
	}
	
	
	int SpkFrmNm = 100; //_filelength(_fileno(fp_Vector)) / FEATURE_ORDER_4MUL / sizeof(float);//
	float **Vectors, *VectorBuff;
	VectorMemAlloc(Vectors, VectorBuff,SpkFrmNm, FEATURE_ORDER_4MUL);
	
	fseek(fp_Vector, 0, SEEK_SET);
	fread(VectorBuff, sizeof(float), SpkFrmNm * FEATURE_ORDER_4MUL, fp_Vector);//read features
	printf("vector = %g\n", Vectors[0][2]);
	
	if(fp_Vector != NULL)
	{
		fclose(fp_Vector);
		fp_Vector = NULL;
	}	
	//>-------------------------------------------------------------------------------

	
	//<----------------------------3.Train and Identify--------------------------------
	int RtnValue = 1;
	if(!strcmp(Operation, "Train"))
		RtnValue = Train(Vectors, SpeakerID, SpkFrmNm);
	
	else if(!strcmp(Operation, "Identify"))
		RtnValue = Identify(Vectors, SpeakerID, SpkFrmNm);
    
	VectorMemFree(Vectors, VectorBuff);

#ifdef TIME
	_ftime(&timebuffer1);
	timeline = ctime( & ( timebuffer1.time ) );
	timeconsum = timebuffer1.millitm + timebuffer1.time*1000 - (timebuffer0.millitm + timebuffer0.time*1000);
	printf("\n%25.19s.%u\n      consumed:%14d\n", timeline, timebuffer1.millitm, timeconsum );
	double ReatMul = (double)timeconsum / SpkFrmNm / 10;  
	printf("      realtime multiple:  %lf\n", ReatMul);
#endif

	//getchar();
	return RtnValue;
}
