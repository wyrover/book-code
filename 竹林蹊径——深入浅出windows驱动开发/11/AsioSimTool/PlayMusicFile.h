#ifndef PLAY_MUSIC_FILE_H
#define PLAY_MUSIC_FILE_H

extern unsigned char *g_pDataBuf;
extern unsigned int g_dataBufSize;
extern HANDLE g_hWaveFile;
extern HANDLE g_hWaveFileRecord;


typedef struct RIFF_HEADER
{
	TCHAR szRiffID[4];        // 'R','I','F','F'
	DWORD dwRiffSize;
	TCHAR szRiffFormat[4];    // 'W','A','V','E'
}RIFF_HEADER;

typedef struct WAVE_FORMAT
{
	WORD wFormatTag;
	WORD wChannels;
	DWORD dwSamplesPerSec;
	DWORD dwAvgBytesPerSec;
	WORD wBlockAlign;
	WORD wBitsPerSample;
}WAVE_FORMAT;

typedef struct WAVE_FORMATEX
{
	WORD wFormatTag;
	WORD wChannels;
	DWORD dwSamplesPerSec;
	DWORD dwAvgBytesPerSec;
	WORD wBlockAlign;
	WORD wBitsPerSample;
	WORD wExtenSize;
}WAVE_FORMATEX;

typedef struct FMT_BLOCK
{
	TCHAR szFmtID[4];    // 'f','m','t',' ' please note the	
	DWORD dwFmtSize;
	WAVE_FORMAT wavFormat;
}FMT_BLOCK;

typedef struct DATA_BLOCK
{
	TCHAR szDataID[4];    // 'd','a','t','a'
	DWORD dwDataSize;
}DATA_BLOCK;


void closeWaveFile();
int readWaveFile(unsigned char * channel0, unsigned char * channel1, unsigned int size, int type);
bool writeWaveFile(void* bufPnt, unsigned int bufferSize);

int recordWaveFileEnd(HANDLE hFile);
int openWaveFile(char* filePath, HANDLE* hFile, WAVE_FORMAT* wave_fmt, DWORD *dwDataSize);
int recordWaveFile(char* filePath, HANDLE* hFile, WAVE_FORMAT* wave_fmt = NULL);

int playWaveFile(char *filePath);
int BeginRecord(char* filePath);
int EndRecord();
#endif
