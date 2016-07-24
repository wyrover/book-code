// SoundIn.cpp: implementation of the CSoundIn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Phone.h"
#include "SoundIn.h"

#pragma comment(lib, "winmm")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

UINT WaveInThreadProc(void * pParam);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSoundIn::CSoundIn()
{
	m_WaveInSampleRate = 11025;
	m_TerminateThread=FALSE;
	m_nBlock=1024;
	m_nBlockNum=2;
	m_nBits=8;
	m_nCurrent=0;
	m_WaveHeader=NULL;
	m_pInputBuffer=NULL;
}

CSoundIn::~CSoundIn()
{
	if(m_WaveHeader!=NULL)
		free(m_WaveHeader);
	if(m_pInputBuffer!=NULL)
		free(m_pInputBuffer);
}

MMRESULT CSoundIn::OpenMic()
{
	m_WaveHeader=(WAVEHDR*)malloc(m_nBlockNum*sizeof(WAVEHDR));
	int n=(int)m_nBits/8;
	m_pInputBuffer=(char*)malloc(n*m_nBlockNum*m_nBlock);

	m_cpSend->SetWaveFormat(1,m_WaveInSampleRate,m_nBits);
	m_cpSend->SetSrcSamples(m_nBlock,(unsigned char*)m_pInputBuffer);
	m_cpSend->Initialize();
	m_cpSend->PrepareSpace(TRUE);

	MMRESULT result;

    result=waveInGetNumDevs(); 
	if (result == 0)
	{
        AfxMessageBox("No Sound Device");
		return result;
	}
    // test for Mic available   
    result=waveInGetDevCaps (0, &m_WaveInDevCaps, sizeof(WAVEINCAPS));
    if ( result!= MMSYSERR_NOERROR)
    {
       AfxMessageBox(_T("Cannot determine sound card capabilities !"));
    }
	// The Sound Devive is OK now we can create an Event  and start the Thread
	m_WaveInEvent = CreateEvent(NULL,FALSE,FALSE,"WaveInThreadEvent");
	m_TerminateThread = FALSE;
	m_WaveInThread= AfxBeginThread(WaveInThreadProc,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED,NULL);   
	m_WaveInThread->m_bAutoDelete = TRUE;
	m_WaveInThread->ResumeThread();
	// init format 
	WaveInitFormat(1/* mono*/,m_WaveInSampleRate /* khz */,m_nBits /* bits */); 
	// Open Input 
	result = waveInOpen( &m_WaveIn,0, &m_WaveFormat,(DWORD)m_WaveInEvent ,NULL ,CALLBACK_EVENT); 
	if ( result!= MMSYSERR_NOERROR)
	{
        AfxMessageBox(_T("Cannot Open Sound Input Device!"));
	    return result;
	}

	for(int i=0;i<m_nBlockNum;i++){
		(*(m_WaveHeader+i)).lpData = (LPSTR)(m_pInputBuffer+i*m_nBlock*n);
		(*(m_WaveHeader+i)).dwBufferLength=m_nBlock*n;
		(*(m_WaveHeader+i)).dwFlags = 0;
		result = waveInPrepareHeader( m_WaveIn, m_WaveHeader+i, sizeof(WAVEHDR) ); 
		if ( (result!= MMSYSERR_NOERROR)||((*(m_WaveHeader+i)).dwFlags != WHDR_PREPARED))
		{
			// AfxMessageBox(_T("Cannot Prepare Header !"));
			return result;
		}
	    result = waveInAddBuffer( m_WaveIn,m_WaveHeader+i, sizeof(WAVEHDR));
		if  (result!= MMSYSERR_NOERROR) 
		{
			AfxMessageBox(_T("Cannot Add Buffer !"));
			return result;
		}
	}
    // all is correct now we can start the process
    result = waveInStart( m_WaveIn );
    if  (result!= MMSYSERR_NOERROR) 
    {
        AfxMessageBox(_T("Cannot Start Wave In !"));
	    return result;
    }
    return result;
}

void CSoundIn::WaveInitFormat(WORD    nCh, // number of channels (mono, stereo)
							  DWORD   nSampleRate, // sample rate
							  WORD    BitsPerSample)
{
	m_WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
	m_WaveFormat.nChannels = nCh;
	m_WaveFormat.nSamplesPerSec = nSampleRate;
	m_WaveFormat.nAvgBytesPerSec = nSampleRate * nCh * BitsPerSample/8;
	m_WaveFormat.nBlockAlign = m_WaveFormat.nChannels * BitsPerSample/8;
	m_WaveFormat.wBitsPerSample = BitsPerSample;
	m_WaveFormat.cbSize = 0;
}

void CSoundIn::AddBuffer()
{
	MMRESULT result;
	int n=(int)m_nBits/8;

	result = waveInUnprepareHeader(m_WaveIn, m_WaveHeader+m_nCurrent, sizeof(WAVEHDR)); 
	if(result!= MMSYSERR_NOERROR) 
    {
           return;
	}
	//Here to call the data process procedue!
	if(m_bTest)
		m_sndOut->WriteData(m_pInputBuffer+m_nCurrent*m_nBlock*n,m_nBlock*n);
	else{
		m_cpSend->SetSrcSamples(m_nBlock,(unsigned char*)m_pInputBuffer+m_nCurrent*m_nBlock*n);
		m_cpSend->ConvertSend(m_sSend,m_addrTo);
		//int iError=sendto(m_sSend,m_pInputBuffer+m_nCurrent*m_nBlock*n,m_nBlock*n,0,(LPSOCKADDR)&m_addrTo,sizeof(m_addrTo));
		//if(iError==SOCKET_ERROR)
		//	TRACE("Error In the sending!");
	}
 	(*(m_WaveHeader+m_nCurrent)).lpData = (LPSTR)(m_pInputBuffer+m_nCurrent*m_nBlock*n);
    (*(m_WaveHeader+m_nCurrent)).dwBufferLength =m_nBlock*n;
	(*(m_WaveHeader+m_nCurrent)).dwFlags = 0;
    result = waveInPrepareHeader( m_WaveIn, m_WaveHeader+m_nCurrent, sizeof(WAVEHDR) ); 
	if ( (result!= MMSYSERR_NOERROR)||((*(m_WaveHeader+m_nCurrent)).dwFlags != WHDR_PREPARED) )
        AfxMessageBox(_T("Cannot Prepare Header !"));
    result = waveInAddBuffer( m_WaveIn, m_WaveHeader+m_nCurrent, sizeof(WAVEHDR) );
    if (result!= MMSYSERR_NOERROR) 
        AfxMessageBox(_T("Cannot Add Buffer !"));

    result = waveInStart( m_WaveIn );
    if  (result!= MMSYSERR_NOERROR) 
        AfxMessageBox(_T("Cannot Start Wave In !"));
	m_nCurrent++;
	m_nCurrent%=m_nBlockNum;
}

void CSoundIn::CloseMic()
{
	m_TerminateThread = TRUE;
    if (m_WaveInEvent )
		SetEvent(m_WaveInEvent);
    Sleep(50);  // wait for the thread to terminate

	if (m_WaveIn) 
	{
		waveInReset(m_WaveIn);
		waveInStop(m_WaveIn);
		waveInClose(m_WaveIn);
	}
}


void CSoundIn::StopMic()
{
	waveInStop(m_WaveIn);
	waveInReset(m_WaveIn);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    Glogal Thread procedure for the CSoundIn class
//    It cannot be included inside the Class
//   
// The LPARAM is the Class pointer (this) it can be the base class CSoundIn ptr or a derived new class
// The value of this parametre can change according the Topmost class of the process 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PT_S ((CSoundIn*)pParam) 

UINT WaveInThreadProc(void * pParam)
{
   UINT result;
   UINT FirstPass = TRUE;

	if ( FirstPass)
		result = WaitForSingleObject(((CSoundIn*)pParam)->m_WaveInEvent,INFINITE);
	FirstPass = FALSE;
    
	while (!((CSoundIn*)pParam)->m_TerminateThread)
	{
		result = WaitForSingleObject(PT_S->m_WaveInEvent,INFINITE);
		if ((result == WAIT_OBJECT_0)&&(!PT_S->m_TerminateThread ))
		{
			PT_S->AddBuffer();      
		}
		else
			return 0;  
	}
    return 0;
}
