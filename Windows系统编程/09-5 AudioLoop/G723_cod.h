
//////////////////////////////////////////////////////////
//G723_cod.dll's interface functions,export them        //
////////////////////////////////////////////////////////// 

//////////////////////////////////////////////////////
//the stucture for the G723 coder/decoder parameters//
//////////////////////////////////////////////////////
typedef struct {
	int WorkRate;
}  PARAMETER ;


/////////////////////////////////////////////
// The interface functions                 //
/////////////////////////////////////////////

//used for the parameter settings for the coder and decoder
void __declspec(dllexport) Set_parameter(PARAMETER par);

//used for coder
void  __declspec(dllexport)Init_Coder(void);
int   __declspec(dllexport)Coder(FLOAT *DataBuff, char *Vout);

//used for decoder
void  __declspec(dllexport)Init_Decod(void);
int   __declspec(dllexport)Decod(FLOAT *DataBuff, char *Vinp, short int Crc);
 
//Used for Initilizing the VAD/CNG
void  __declspec(dllexport)Init_Dec_Cng(void);
void  __declspec(dllexport)Init_Cod_Cng(void);
void  __declspec(dllexport)Init_Vad(void);

