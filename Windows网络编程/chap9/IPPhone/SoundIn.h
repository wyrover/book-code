// SoundIn.h: interface for the CSoundIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUNDIN_H__8F8DC634_0576_11D4_8D3A_000021E6766A__INCLUDED_)
#define AFX_SOUNDIN_H__8F8DC634_0576_11D4_8D3A_000021E6766A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mmsystem.h"
#include "soundout.h"
#include "winsock.h"
#include "compression.h"

class CSoundIn  
{
public:
	MMRESULT OpenMic();
	void     AddBuffer();
	void     StopMic();
	void	 CloseMic();
	void     SetBits(int bits){m_nBits=bits;}
	void     SetBuffer(char *pBuffer){m_pInputBuffer=pBuffer;}
	void     SetBlockProp(long samples,int num){m_nBlock=samples;m_nBlockNum=num;}
	void     SetSampleRate(UINT nSampleRate){m_WaveInSampleRate=nSampleRate;}
	void	 SetTestProp(BOOL test){m_bTest=test;}
	void	 SetSoundOut(CSoundOut* sndOut){m_sndOut=sndOut;}
	void     SetSocketParam(SOCKET s,SOCKADDR_IN addr){m_sSend=s;m_addrTo=addr;}
	void     SetCompressionObject(CCompression* c){m_cpSend=c;}
	
	CSoundIn();
	virtual ~CSoundIn();
private:
	void WaveInitFormat(WORD    nCh, // number of channels (mono, stereo)
						DWORD   nSampleRate, // sample rate
						WORD    BitsPerSample);
	
	char *			m_pInputBuffer;
	WAVEINCAPS		m_WaveInDevCaps;
    HWAVEIN			m_WaveIn;
	WAVEHDR*		m_WaveHeader;
    WAVEFORMATEX	m_WaveFormat;
	
	BOOL			m_bTest;
	int				m_nCurrent;
	UINT            m_WaveInSampleRate;
	long            m_nBlock;
	int				m_nBlockNum;
	int				m_nBits;
	SOCKET			m_sSend;
	SOCKADDR_IN     m_addrTo;		
	CSoundOut*		m_sndOut;       
	CCompression*   m_cpSend;
public:
	BOOL            m_TerminateThread;
	HANDLE          m_WaveInEvent;
	CWinThread*     m_WaveInThread;
};

#endif // !defined(AFX_SOUNDIN_H__8F8DC634_0576_11D4_8D3A_000021E6766A__INCLUDED_)
