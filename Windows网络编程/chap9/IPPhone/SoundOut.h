// SoundOut.h: interface for the CSoundOut class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUNDOUT_H__B67ADB63_05A3_11D4_8D3A_000021E6766A__INCLUDED_)
#define AFX_SOUNDOUT_H__B67ADB63_05A3_11D4_8D3A_000021E6766A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mmsystem.h"
#include "math.h"

class CSoundOut  
{
public:
	CSoundOut();
	virtual ~CSoundOut();

public:
	MMRESULT OpenSpk();
	void     WriteData(char *lpData,long length);
	void     StartSpk();
	void     CloseSpk();
	void     AddBuffer();
	void     SetBits(int bits){m_nBits=bits;}
	void     SetBuffer(char *pBuffer){m_pOutputBuffer=pBuffer;}
	void     SetBlockProp(long samples,int num){m_nBlock=samples;m_nBlockNum=num;}
	void     SetSampleRate(UINT nSampleRate){m_WaveOutSampleRate=nSampleRate;}
	
	HANDLE			m_WaveOutEvent;
	CWinThread *	m_WaveOutThread;
	BOOL			m_TerminateThread;
	
private:
	void WaveInitFormat(WORD nCh,DWORD nSampleRate,WORD BitsPerSample);
	char *			m_pOutputBuffer;
	
	WAVEOUTCAPS		m_WaveOutDevCaps;
    HWAVEOUT		m_WaveOut;
	WAVEHDR*		m_WaveHeader;
    WAVEFORMATEX	m_WaveFormat;
	UINT			m_WaveOutSampleRate;
	long			m_nBlock;
	int				m_nBlockNum;
	int				m_nBits;
	int				m_nCurrent;
};

#endif // !defined(AFX_SOUNDOUT_H__B67ADB63_05A3_11D4_8D3A_000021E6766A__INCLUDED_)
