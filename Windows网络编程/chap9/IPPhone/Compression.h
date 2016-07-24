// Compression.h: interface for the CCompression class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPRESSION_H__D764BF21_0E55_11D4_8D3A_00002100A114__INCLUDED_)
#define AFX_COMPRESSION_H__D764BF21_0E55_11D4_8D3A_00002100A114__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mmsystem.h"
#include "mmreg.h"  // Multimedia registration
#include "msacm.h"	// Audio Compression Manager
#include "winsock.h"

#define ADPCM      2
#define TRUESPEECH 1
#define MSN        0
#define GSM610     3
#define ALAW       4
#define MULAW      5

typedef struct {
	HACMDRIVERID hadid;
	WORD wFormatTag;
	unsigned short nBits;
	unsigned long lSampleRate;
} FIND_DRIVER_INFO;

class CCompression  
{
public:
	long UnConvert(BOOL bQuery,unsigned char* pSrcData,long bytes);
	BOOL PrepareSpace(BOOL bCompression);
	void SetDstSamples(long bytes,unsigned char* pDstData);
	BOOL ConvertSend(SOCKET s,SOCKADDR_IN addr);
	long MyConvert(BOOL bQuery,unsigned char *pDetData, long detLength);
	BOOL Initialize();
	void SelectMethod(int i);
	void SetWaveFormat(int channels,long samplerate,int bitspersample);
	void SetSrcSamples(long samples,unsigned char* pSrcData);
	CCompression();
	virtual ~CCompression();
	WORD wFormatTag;
	WAVEFORMATEX m_wfSrc;
	DWORD m_dwSrcSamples;
	DWORD m_dwSrcBytes;
	BYTE* m_pSrcData;
	WAVEFORMATEX* pwfDrv;
	WAVEFORMATEX* pwfPCM;
	HACMDRIVERID hadid;
private:
	WAVEFORMATEX* GetDriverFormat(HACMDRIVERID hadid, WORD wFormatTag,BOOL bPCM);
	HACMDRIVERID FindDriver(WORD wFormatTag);
	DWORD dwDst2Bytes;
	DWORD dwDst1Bytes;
	DWORD dwDst1Samples;
	long length;
	long unlength;
	BYTE* pDst1Data;
	BYTE* pDst2Data;
};

#endif // !defined(AFX_COMPRESSION_H__D764BF21_0E55_11D4_8D3A_00002100A114__INCLUDED_)
