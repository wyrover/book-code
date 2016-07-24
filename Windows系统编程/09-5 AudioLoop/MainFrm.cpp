// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include <mmsystem.h>

#include "test_audio_loop.h"
#include "MainFrm.h"
#include "SetDialog.h"
#include "G723_cod.h"
#include "codec.h"
#include "typedef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////////
PRECORDPLAYINFOR pInfo=NULL;
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_AUDIO_THREAD_START, OnAudioThreadStart)
	ON_COMMAND(ID_AUDIO_THREAD_CLOSE, OnAudioThreadClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here

}

CMainFrame::~CMainFrame()
{


}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnAudioThreadStart() 
{
///////////////////////////////////////
//为全局变量分配空间
	if (pInfo == NULL)
		pInfo = (PRECORDPLAYINFOR) GlobalLock(
				GlobalAlloc(GHND, sizeof(RECORDPLAYINFOR)));
	if (pInfo != NULL)
	{
		//Initialize the structure.
		int i;

		pInfo->hWaveIn		=NULL;
		pInfo->hWaveOut		=NULL;

		pInfo->buffersize	=BUFFER_SIZE_G7231;

		pInfo->hThreadIn	=NULL;
		pInfo->hThreadOut	=NULL;

		for (i=0;i<NUM_LEVEL_BUFFERS;i++)
		{
			pInfo->alpWave[i]=NULL;
			// allocate memory for vavehdr, size of lpData is pInfo->buffersize
			pInfo->alpWave[i] = (LPWAVEHDR)GlobalLock(
					GlobalAlloc(GMEM_MOVEABLE|GMEM_SHARE,
					sizeof(WAVEHDR) + pInfo->buffersize));

			if (pInfo->alpWave[i] == NULL) 
				AfxMessageBox("GlobalAlloc buffers failed");
			else
			{
				pInfo->alpWave[i]->lpData = (char*) (pInfo->alpWave[i] + 1);
				//"+1" to skip over the header, go to data
				pInfo->alpWave[i]->dwBufferLength = pInfo->buffersize;
				pInfo->alpWave[i]->dwBytesRecorded = 0;
				pInfo->alpWave[i]->dwUser = 0;
				pInfo->alpWave[i]->dwFlags = 0;
				pInfo->alpWave[i]->dwLoops = 0;
			}
		}
	}	
	
///Initialize the parameter dialog
	m_dlg.m_workrate="6.3 kbps";

    int i;
	PCMWAVEFORMAT wf;
	MMRESULT mmResult;
   	PARAMETER parameter;

///////////////////////////////////////////
 //set the parameters of G723 algorithmetic 
    m_dlg.DoModal();

   if(m_dlg.m_workrate=="5.3 kbps")
	   parameter.WorkRate=53;
   else 
	   parameter.WorkRate=63;

   Set_parameter(parameter);


//////////////////////////////////////////
// create the two threads for codec     
   //create the thread used for compression.
	if (pInfo->hThreadIn==NULL)
		pInfo->hThreadIn = CreateThread(NULL, 
										 0, 
										 (LPTHREAD_START_ROUTINE)CompressFunc, 
										 pInfo, 
										 0,
										 &(pInfo->idThreadIn));
    if (pInfo->hThreadIn == NULL) 
	{
        AfxMessageBox("Compress: CreateThread() error");
		return;
    }
//create the thread used for decodec.
	if (pInfo->hThreadOut==NULL)
		pInfo->hThreadOut = CreateThread(NULL, 
										 0, 
										 (LPTHREAD_START_ROUTINE)DecompressFunc, 
										 pInfo, 
										 0,
										 &(pInfo->idThreadOut));
    if (pInfo->hThreadOut == NULL)
	{
        AfxMessageBox("Decompress: CreateThread() error");
		return;
    }

///////////////////////////////////////////
//设置录音线程的声卡参数//

	//set the audio input pcmwaveformat
    wf.wf.nChannels = 1;
    wf.wBitsPerSample = 16;
    wf.wf.nSamplesPerSec = 8000;
	// complete remaining areas of wf
    wf.wf.wFormatTag = WAVE_FORMAT_PCM;
    wf.wf.nBlockAlign = wf.wf.nChannels * wf.wBitsPerSample / 8;
    wf.wf.nAvgBytesPerSec = wf.wf.nSamplesPerSec * wf.wf.nBlockAlign;

	// open the wave in device
    mmResult=waveInOpen(
        &(pInfo->hWaveIn),
        WAVE_MAPPER,
        (LPWAVEFORMATEX)&wf,
        pInfo->idThreadIn,               
        0,
        CALLBACK_THREAD);
	if (mmResult!=MMSYSERR_NOERROR )
	{
            AfxMessageBox("open wave in failed");
            return;
    }

    // alloc, prepare and add all the buffers to input sequences
    for (i = 0; i < NUM_LEVEL_BUFFERS; i++)
	{

        if (pInfo->alpWave[i]!=NULL) 
		{
			waveInPrepareHeader(pInfo->hWaveIn, pInfo->alpWave[i], sizeof(WAVEHDR)); 
			waveInAddBuffer(pInfo->hWaveIn, pInfo->alpWave[i], sizeof(WAVEHDR));
		}
    }
	
///////////////////////////////////////////
//设置播放声卡的参数 
 
	//set the audio input pcmwaveformat
    wf.wf.nChannels = 1;
    wf.wBitsPerSample = 16;
    wf.wf.nSamplesPerSec = 8000;
    wf.wf.wFormatTag = WAVE_FORMAT_PCM;
    wf.wf.nBlockAlign = wf.wf.nChannels * wf.wBitsPerSample/8;
    wf.wf.nAvgBytesPerSec = wf.wf.nSamplesPerSec * wf.wf.nBlockAlign;

    //open the output audio device
    mmResult=waveOutOpen(
		&(pInfo->hWaveOut), 
		WAVE_MAPPER,
        (LPWAVEFORMATEX)&wf,
        pInfo->idThreadOut,               
        0,
        CALLBACK_THREAD);
	if (mmResult)
	{
            AfxMessageBox("open waveOutOpen failed");
            return;
    }

///////////////////////////////////////////
// Start the audio input                 
    waveInStart(pInfo->hWaveIn);
	
}

//stop the audio input/output threads
void CMainFrame::OnAudioThreadClose() 
{
/////////////////////////////
//终止录音放音线程
	    //stop the record thread

        PostThreadMessage(pInfo->idThreadIn,
			              WM_QUIT,
						  0,
						  0);

		//stop the play thread
        PostThreadMessage(pInfo->idThreadOut,
			              WM_QUIT,
						  0,
						  0);


	CloseHandle(pInfo->hThreadIn);
		pInfo->hThreadIn=NULL;
		CloseHandle(pInfo->hThreadOut);
		pInfo->hThreadOut=NULL;

/////////////////////////////////////////////
//关闭声卡并释放为声卡分配的内存
	int i;

	if (pInfo!=NULL)
	{
        //stop  the record and record thread
		if (pInfo->hWaveIn!=NULL) 
		{
			waveInReset(pInfo->hWaveIn);
			waveInClose(pInfo->hWaveIn);
		}
	}
    // unlock and free buffers
	if (pInfo!=NULL)
	{
		for (i = 0; i < NUM_LEVEL_BUFFERS; i++) 
		{
			if (pInfo->alpWave[i]) 
			{
				GlobalFree(GlobalHandle(pInfo->alpWave[i]));
				pInfo->alpWave[i] = NULL;
			}

		}
	}

	if (pInfo->hWaveOut!=NULL)
	{
		// release all buffers
		waveOutReset(pInfo->hWaveOut);
		waveOutClose(pInfo->hWaveOut);
	}

	GlobalFree(GlobalHandle(pInfo));
    pInfo=NULL;


}
	


 
