#include "stdafx.h"
#include "codec.h"
#include "mmsystem.h"
#include "typedef.h"
#include "g723_cod.h"
#include "linedata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLineDataList			coderList,decoderList;
CRITICAL_SECTION		csCoderList,csDecoderList; 

DWORD WINAPI CompressFunc( LPVOID  lpParam)
{

    MSG					Message;      
	LPWAVEHDR			lpWaveHdr;
    HWAVEIN				hWaveIn;
	PRECORDPLAYINFOR	pInfo;
	short int			*pInt16;
	float               fDataBuff[240];
	int					i;
	CLineData			*pLineData;

	PeekMessage(&Message, NULL, WM_USER, WM_USER, PM_NOREMOVE);
    // 让系统创建消息队列。

	pInfo=(PRECORDPLAYINFOR)lpParam;

	// 初始化G723编码器。
	Init_Coder();

	// 分配编码器列表。
	for(i=0;i<LINE_BUFF_NUM;i++)
	{
		pLineData=new CLineData;
		coderList.AddTail(pLineData);
	}

	// 初始化两个临界区。
	InitializeCriticalSection(&csCoderList);
	InitializeCriticalSection(&csDecoderList);
    
	while (GetMessage(&Message, NULL, 0, 0)) 
	{
		switch (Message.message)
		{
		case MM_WIM_OPEN :
				break;
		case MM_WIM_DATA :
			    lpWaveHdr=(LPWAVEHDR)(Message.lParam);
				hWaveIn=(HWAVEIN)(Message.wParam);

				if (lpWaveHdr->dwBytesRecorded==480)
				{
					if (!coderList.IsEmpty())
					{
						EnterCriticalSection(&csCoderList);
						pLineData=coderList.RemoveHead();
						LeaveCriticalSection(&csCoderList);

						// 用于采集语音数据和编码数据的转换
						pInt16 =(short *)lpWaveHdr->lpData;
						for(i=0;i<240;i++)
						{
							fDataBuff[i]=(float) (* pInt16);
							pInt16++;
						}

						Coder(fDataBuff,pLineData->lineData);


						EnterCriticalSection(&csDecoderList);
						decoderList.AddTail(pLineData);
						LeaveCriticalSection(&csDecoderList);
						
						PostThreadMessage(pInfo->idThreadOut,MSG_AUDIO_DATA,0,0);
						// 通知解压线程。
					}
				}

 				waveInPrepareHeader(pInfo->hWaveIn, lpWaveHdr, sizeof(WAVEHDR)); 
				waveInAddBuffer(pInfo->hWaveIn, lpWaveHdr, sizeof(WAVEHDR));
			 			
				break;

		case MM_WIM_CLOSE :
				break;
		default:;
		}
      
    }  


	while (!coderList.IsEmpty())
		delete coderList.RemoveHead();
	
	DeleteCriticalSection(&csCoderList);
	DeleteCriticalSection(&csDecoderList);

	return 0;
}


DWORD WINAPI DecompressFunc( LPVOID  lpParam)
{
    MSG					Message;
	LPWAVEHDR			lpWaveHdr;
    HWAVEOUT			hWaveOut;
	PRECORDPLAYINFOR	pInfo;
	float               fDataBuff[240];
	int					i;
	short int			*pInt16;
	CLineData			*pLineData;
	COutWaveHdrList     outWaveHdrList;
	COutWaveHdr			*pOutWaveHdr;

	PeekMessage(&Message, NULL, WM_USER, WM_USER, PM_NOREMOVE);
    // 让系统创建消息队列。
	
	// 初始化解码器。
	Init_Decod();

	// 初始化声音头输出列表。
	for (i=0;i<NUM_LEVEL_BUFFERS;i++)
	{
		pOutWaveHdr= new COutWaveHdr;
		pOutWaveHdr->lpWaveHdr=NULL;
		pOutWaveHdr->lpWaveHdr = (LPWAVEHDR)GlobalLock(GlobalAlloc(GMEM_MOVEABLE|GMEM_SHARE,
													sizeof(WAVEHDR) + BUFFER_SIZE_G7231));

		if (pOutWaveHdr->lpWaveHdr == NULL) 
			AfxMessageBox("GlobalAlloc buffers failed");
		else
		{
			pOutWaveHdr->lpWaveHdr->lpData = (char*) (pOutWaveHdr->lpWaveHdr + 1);
			pOutWaveHdr->lpWaveHdr->dwBufferLength = BUFFER_SIZE_G7231;
			pOutWaveHdr->lpWaveHdr->dwBytesRecorded = 0;
			pOutWaveHdr->lpWaveHdr->dwUser = (DWORD)pOutWaveHdr;
			pOutWaveHdr->lpWaveHdr->dwFlags = 0;
			pOutWaveHdr->lpWaveHdr->dwLoops = 0;

			outWaveHdrList.AddTail(pOutWaveHdr);
		}
	}

	pInfo=(PRECORDPLAYINFOR)lpParam;

    while (GetMessage(&Message, NULL, 0, 0)) 
	{
		switch (Message.message)
		{
		case MM_WOM_OPEN :
				break;
		case MM_WOM_DONE :
				lpWaveHdr=(LPWAVEHDR)Message.lParam;
				hWaveOut=(HWAVEOUT)Message.wParam;

				waveOutUnprepareHeader(hWaveOut, lpWaveHdr, sizeof(WAVEHDR));
				if ((pOutWaveHdr= (COutWaveHdr*)(lpWaveHdr->dwUser))!=NULL)
					outWaveHdrList.AddTail(pOutWaveHdr);

				break;
		case MM_WOM_CLOSE :
				break;
		case MSG_AUDIO_DATA:
				if(!decoderList.IsEmpty())
				{
					EnterCriticalSection(&csDecoderList);
					pLineData=decoderList.RemoveHead();
					LeaveCriticalSection(&csDecoderList);

					Decod(fDataBuff,pLineData->lineData,(short int)0);

					EnterCriticalSection(&csCoderList);
					coderList.AddTail(pLineData);
					LeaveCriticalSection(&csCoderList);

					if (!outWaveHdrList.IsEmpty())
					{
						pOutWaveHdr=outWaveHdrList.RemoveHead();
						
						pInt16=(short*)(pOutWaveHdr->lpWaveHdr->lpData);
						for(i=0;i<240;i++)
							pInt16[i]=(short)fDataBuff[i];

 							waveOutPrepareHeader(pInfo->hWaveOut, pOutWaveHdr->lpWaveHdr, sizeof(WAVEHDR));
							waveOutWrite(pInfo->hWaveOut, pOutWaveHdr->lpWaveHdr, sizeof(WAVEHDR));
					}
				}

				break;

				
		default:
				break;
		}
	}

	// 释放声音头。
	while (!outWaveHdrList.IsEmpty())
	{
		pOutWaveHdr=outWaveHdrList.RemoveHead();
		GlobalFree(GlobalHandle(pOutWaveHdr->lpWaveHdr));
		delete pOutWaveHdr;
	}

	while (!decoderList.IsEmpty())
		delete decoderList.RemoveHead();

	return 0;

}
