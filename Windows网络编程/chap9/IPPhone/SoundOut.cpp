// SoundOut.cpp: implementation of the CSoundOut class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Phone.h"
#include "SoundOut.h"

#pragma comment(lib, "winmm")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

UINT WaveOutThreadProc(void * pParam); 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSoundOut::CSoundOut()
{
	m_nBits=8;
	m_nBlock=1024;
	m_WaveOutSampleRate=11025;
	m_nCurrent=1;
	m_TerminateThread=FALSE;
	m_pOutputBuffer=NULL;
	m_WaveHeader=NULL;
}

CSoundOut::~CSoundOut()
{
	if(m_pOutputBuffer!=NULL)
		free(m_pOutputBuffer);
	if(m_WaveHeader!=NULL)
		free(m_WaveHeader);
}

MMRESULT CSoundOut::OpenSpk()
{
	int n=(int)m_nBits/8;
	m_WaveHeader=(WAVEHDR*)malloc(sizeof(WAVEHDR));
	m_pOutputBuffer=(char*)malloc(n*m_nBlock*m_nBlockNum);
    
	MMRESULT result;
    result=waveOutGetNumDevs(); 
	if (result == 0)
	{
        AfxMessageBox("No Sound Output Device");
		return result;
	}
	// test for Mic available   
    result=waveOutGetDevCaps (0, &m_WaveOutDevCaps, sizeof(WAVEOUTCAPS));
    if ( result!= MMSYSERR_NOERROR)
    {
       AfxMessageBox(_T("Sound output Cannot determine card capabilities !"));
    }
	// The SoundOut Devive is OK now we can create an Event  and start the Thread
	m_WaveOutEvent = CreateEvent(NULL,FALSE,FALSE,"WaveOutThreadEvent");
	m_TerminateThread = FALSE;
	m_WaveOutThread= AfxBeginThread(WaveOutThreadProc,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED,NULL);   
	m_WaveOutThread->m_bAutoDelete = TRUE;
	m_WaveOutThread->ResumeThread();
 	// init format 
	WaveInitFormat(1/* mono*/,m_WaveOutSampleRate /* khz */,m_nBits /* bits */); 
	// Open Output 
	result = waveOutOpen( &m_WaveOut,0, &m_WaveFormat,(DWORD)m_WaveOutEvent ,NULL ,CALLBACK_EVENT);  
	if ( result!= MMSYSERR_NOERROR)
	{
        AfxMessageBox(_T("Sound output Cannot Open Device!"));
	    return result;
	}
	(*m_WaveHeader).lpData = (LPSTR)(m_pOutputBuffer);
	(*m_WaveHeader).dwBufferLength = m_nBlock*n*m_nBlockNum;
	(*m_WaveHeader).dwFlags = 0;
	result = waveOutPrepareHeader( m_WaveOut,m_WaveHeader, sizeof(WAVEHDR) ); 
	if ((result!= MMSYSERR_NOERROR)||((*m_WaveHeader).dwFlags != WHDR_PREPARED))
	{	
		//AfxMessageBox(_T(" Sound Output Cannot Prepare Header !"));
		return result;
	}
	result = waveOutWrite( m_WaveOut, m_WaveHeader, sizeof(WAVEHDR));
	if  (result!= MMSYSERR_NOERROR) 
	{
		AfxMessageBox(_T(" Sound Output Cannot Write Buffer !"));
		return result;
	}
    // all is correct now we can start the process
    result = waveOutRestart( m_WaveOut );
    if  (result!= MMSYSERR_NOERROR) 
    {
        AfxMessageBox(_T(" Sound Output Cannot Start Wave Out !"));
	    return result;
    }
    return result;
}

void CSoundOut::WaveInitFormat(WORD nCh,//channels of output
							   DWORD nSampleRate,//sample rate
							   WORD BitsPerSample)
{
	m_WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
	m_WaveFormat.nChannels = nCh;
	m_WaveFormat.nSamplesPerSec = nSampleRate;
	m_WaveFormat.nAvgBytesPerSec = nSampleRate * nCh * BitsPerSample/8;
	m_WaveFormat.nBlockAlign = m_WaveFormat.nChannels * BitsPerSample/8;
	m_WaveFormat.wBitsPerSample = BitsPerSample;
	m_WaveFormat.cbSize = 0;
}

void CSoundOut::CloseSpk()
{
	if (m_WaveOut) 
		waveOutPause(m_WaveOut);
    Sleep(50); 
	
	m_TerminateThread = TRUE;
    if (m_WaveOutEvent )
		SetEvent(m_WaveOutEvent);
    Sleep(50);
   if (m_WaveOut) 
   {
		waveOutReset(m_WaveOut);
		waveOutClose(m_WaveOut);
   }
}

void CSoundOut::StartSpk()
{
	waveOutRestart(m_WaveOut);
}

void CSoundOut::AddBuffer()
{
	MMRESULT result;
	
	if(m_nCurrent>2)
		m_nCurrent=1;
	result = waveOutUnprepareHeader( m_WaveOut,m_WaveHeader, sizeof(WAVEHDR) ); 
	if  (result!= MMSYSERR_NOERROR) 
    {
        return;
    }
 	int n=(int)m_nBits/8;
	(*m_WaveHeader).lpData = (LPSTR)(m_pOutputBuffer);
	(*m_WaveHeader).dwBufferLength = m_nBlock*n*m_nBlockNum;
	(*m_WaveHeader).dwFlags = 0;
	result = waveOutPrepareHeader( m_WaveOut,m_WaveHeader, sizeof(WAVEHDR) ); 
	if ((result!= MMSYSERR_NOERROR)||((*m_WaveHeader).dwFlags != WHDR_PREPARED))
	{	
		//AfxMessageBox(_T(" Sound Output Cannot Prepare Header !"));
		return ;
	}
	result = waveOutWrite( m_WaveOut, m_WaveHeader, sizeof(WAVEHDR));
	if  (result!= MMSYSERR_NOERROR) 
	{
		AfxMessageBox(_T(" Sound Output Cannot Write Buffer !"));
		return ;
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    Glogal Thread procedure for the CSoundOut class
//    It cannot be included inside the Class
//   
// The LPARAM is the Class pointer it can be the base class CSoundOut
// or a drived class like CFft 
// The value of this parametre can change according because 
//  OpenSpk() call a virtual funtion 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PT_S ((CSoundOut*)pParam) 

UINT WaveOutThreadProc(void * pParam)
{
    UINT result;
    UINT FirstPass = TRUE;

    if ( FirstPass)
		result = WaitForSingleObject(PT_S->m_WaveOutEvent,INFINITE);
	FirstPass = FALSE;
    
	while (!(PT_S->m_TerminateThread))
	{
		result = WaitForSingleObject(((CSoundOut*)pParam)->m_WaveOutEvent,INFINITE);
		if ((result == WAIT_OBJECT_0)&&(!(PT_S->m_TerminateThread )))
		{
			PT_S->AddBuffer();     
		}
		else
			return 0;  
	}
    return 0;
}

void CSoundOut::WriteData(char *lpData,long length)
{
	int n=(int)(m_nBits/8);
	m_nCurrent++;
	m_nCurrent%=m_nBlockNum;
	memcpy(m_pOutputBuffer+m_nCurrent*m_nBlock*n,lpData,length);
	if(length<m_nBlock*n)
		memset(m_pOutputBuffer+m_nCurrent*m_nBlock*n+length,127,m_nBlock*n-length-1);
	return;
}
