/*
Copyright (c) 张佩 01/05/2010
联系：zhang.mibox@gmail.com

Module Name:
Virtual ASIO User Driver

Abstract:
虚拟ASIO声卡驱动，配合ASIO音频软件，实现音频数据从输出到输入的循环。

Revision History:
1.0.001
*/

#include <stdio.h>
#include <string.h>
#include "asiosmpl.h"
#include <winioctl.h>
#include <ks.h>
#include <ksmedia.h>
#include <strsafe.h>
#include "combase.cpp"
#include "windows.h"
#include "mmsystem.h"
#include "dllentry.cpp"
#include "register.cpp"

typedef struct {
	PVOID TotalInChannelBuffer[MAX_IN_CHANNELS][ASIO_MAX_BUFFERS];
}TOTALINPOINTERSTRUCT, *TOTALPINPOINTERSTRUCT;

typedef struct {
	PVOID TotalOutChannelBuffer[MAX_OUT_CHANNELS][ASIO_MAX_BUFFERS];
}TOTALOUTPOINTERSTRUCT, *TOTALPOUTPOINTERSTRUCT;

typedef struct {
	PVOID InChannelBuffer[ASIO_MAX_CHANNELS][ASIO_MAX_BUFFERS];
	PVOID OutChannelBuffer[ASIO_MAX_CHANNELS][ASIO_MAX_BUFFERS];
	ULONG BufferLength;
}BUFPOINTERSTRUCT, *PBUFPOINTERSTRUCT;

static const double twoRaisedTo32 = 4294967296.;
static const double twoRaisedTo32Reciprocal = 1. / twoRaisedTo32;

CLSID IID_ASIO_DRIVER = { 0x188135e1, 0xd565, 0x11d2, { 0x86, 0x34, 0x0, 0xa0, 0xc9, 0x9f, 0x5d, 0x19 } };

CFactoryTemplate g_templates[1] = 
{
    {L"VirtualASIO", &IID_ASIO_DRIVER, VirtualAsio::CreateInstance} 
};

int g_templateCount = SIZEOF_ARRAY(g_templates);

//
// Server registration, called on REGSVR32.EXE "the dllname.dll"
//
HRESULT _stdcall DllRegisterServer()
{
	LONG	rc;
	char	errstr[128];

	rc = RegisterAsioDriver (
		IID_ASIO_DRIVER,
		"V-ASIODrv.dll",	
		"Virtual ASIO Driver",	
		"V-ASIO",
		"Apartment");

	if (rc) 
	{
		memset(errstr,0,128);
		sprintf(errstr,"Register Server failed ! (%d)",rc);
		MessageBox(0,(LPCTSTR)errstr,(LPCTSTR)"ASIO sample Driver",MB_OK);
		return -1;
	}

	return S_OK;
}

//
// Server un-registration
//
HRESULT _stdcall DllUnregisterServer()
{
	LONG	rc;
	char	errstr[128];

	rc = UnregisterAsioDriver (
		IID_ASIO_DRIVER,
		"ASIODrv.dll",
		"Virtual ASIO Driver");

	if (rc) 
	{
		memset(errstr,0,128);
		sprintf(errstr,"Unregister Server failed ! (%d)",rc);
		MessageBox(0,(LPCTSTR)errstr,(LPCTSTR)"ASIO Korg1212 I/O Driver",MB_OK);
		return -1;
	}

	return S_OK;
}

CUnknown* VirtualAsio::CreateInstance (LPUNKNOWN pUnk, HRESULT *phr)
{
	return (CUnknown*)new VirtualAsio (pUnk,phr);
};

STDMETHODIMP VirtualAsio::NonDelegatingQueryInterface (REFIID riid, void ** ppv)
{
	if (riid == IID_ASIO_DRIVER)
	{
		return GetInterface (this, ppv);
	}

	return CUnknown::NonDelegatingQueryInterface (riid, ppv);
}

//------------------------------------------------------------------------------------------
VirtualAsio::VirtualAsio (LPUNKNOWN pUnk, HRESULT *phr)
	: CUnknown("ASIOSAMPLE", pUnk, phr)
{
	long i;

	blockFrames = kBlockFrames;
	inputLatency = blockFrames;		// typically
	outputLatency = blockFrames * 2;

	samplePosition = 0;
	sampleRate = 44100.;
	milliSeconds = (long)((double)(kBlockFrames * 1000) / sampleRate);
	active = false;
	started = false;
	timeInfoMode = false;
	tcRead = false;
	for (i = 0; i < kNumInputs; i++)
	{
		inputBuffers[i] = 0;
		inMap[i] = 0;
	}

	for (i = 0; i < kNumOutputs; i++)
	{
		outputBuffers[i] = 0;
		outMap[i] = 0;
	}
	callbacks = 0;
	activeInputs = activeOutputs = 0;

	hVASIODriver = NULL;
	hSyncEvent[0] = NULL;
	hSyncEvent[1] = NULL;
	hThreadEvent = NULL;
	hASIOThread = NULL;
}

//------------------------------------------------------------------------------------------
VirtualAsio::~VirtualAsio ()
{
	if(hVASIODriver != INVALID_HANDLE_VALUE && 
		hVASIODriver != NULL)
	{
		CloseHandle(hVASIODriver);
	}

	stop ();
	outputClose ();
	inputClose ();
	disposeBuffers ();
}

//------------------------------------------------------------------------------------------
void VirtualAsio::getDriverName (char *name)
{
	strcpy (name, "Sample ASIO");
}

//------------------------------------------------------------------------------------------
long VirtualAsio::getDriverVersion ()
{
	return 0x00000001L;
}

//------------------------------------------------------------------------------------------
void VirtualAsio::getErrorMessage (char *string)
{
	strcpy (string, errorMessage);
}

//------------------------------------------------------------------------------------------
ASIOBool VirtualAsio::init (void* sysRef)
{
	if (active)
		return true;

	hVASIODriver = CreateFile(
		"\\\\.\\V-ASIO", 
		GENERIC_ALL, 
		FILE_SHARE_READ, 
		NULL, 
		OPEN_EXISTING,
		0, NULL);

	if(INVALID_HANDLE_VALUE == hVASIODriver)
	{
		hVASIODriver = NULL;
		UDBG("无法打开V-ASIO设备");
		return false;
	}

	strcpy (errorMessage, "ASIO Driver open Failure!");

	if (inputOpen ())
	{
		if (outputOpen ())
		{
			active = true;
			return true;
		}
	}

	outputClose ();
	inputClose ();

	CloseHandle(hASIOThread);
	hASIOThread = NULL;

	return false;
}

static DWORD __stdcall ASIOThread (void *param)
{
	int nEvent;
	VirtualAsio* pASIO = (VirtualAsio*) param;

	while (!pASIO->bCloseEvent)
	{
		nEvent = WaitForMultipleObjects(
			2, 
			pASIO->hSyncEvent,
			FALSE, 
			INFINITE);

		if((pASIO->started) && 
			(!pASIO->bCloseEvent))
		{
			nEvent -= WAIT_OBJECT_0;
			pASIO->bufferSwitch (nEvent == 0 ? 1:0);
		}
	}

	return 0;
}

//------------------------------------------------------------------------------------------
ASIOError VirtualAsio::start ()
{
	DWORD	dwThreadId;
	DWORD	BytesReturned;
	ULONG	Data;

	ASIOSampleRate	CurrentRate;
	ASIOError	retValue = ASE_NotPresent;

	samplePosition = 0;
	theSystemTime.lo = theSystemTime.hi = 0;
	started = false;
	bCloseEvent = false;

	UDBG("[start]");
	 
	if (callbacks && hVASIODriver)
	{
		hSyncEvent[0] = CreateEvent(NULL,FALSE,FALSE,NULL);
		hSyncEvent[1] = CreateEvent(NULL,FALSE,FALSE,NULL);

		BOOLEAN bSet = TRUE;

		if(FALSE == DeviceIoControl(hVASIODriver,
			IOCTL_ASIO_EVENT, 
			hSyncEvent,
			2*sizeof(HANDLE),
			&bSet, 
			sizeof(BOOLEAN),
			&BytesReturned, 
			NULL))
		{
			UDBG("failed to set event: 0x%x", GetLastError());
			return retValue;
		}

		hASIOThread = CreateThread (
			0, 0, 
			&ASIOThread, 
			this, 
			0, 
			&dwThreadId);

		SetThreadPriority(
			hASIOThread, 
			THREAD_PRIORITY_TIME_CRITICAL);

		if(DeviceIoControl(
			hVASIODriver,
			IOCTL_ASIO_START,
			0, NULL, 0, NULL,
			&BytesReturned, 
			NULL)) 
		{
			started = true;
			retValue = ASE_OK;
		}
		else
		{
			UDBG("failed to start: 0x%x", GetLastError());

			stop();
			return retValue;
		}
	}

	return retValue;
}

//------------------------------------------------------------------------------------------
ASIOError VirtualAsio::stop ()
{
	KSPROPERTY Property;
	DWORD	BytesReturned;
	ULONG	Data;

	UDBG("[stop]");

	started = false;
	bCloseEvent = true;

	if(NULL == hVASIODriver)
		return ASE_HWMalfunction;

	if(FALSE == DeviceIoControl(hVASIODriver,
		IOCTL_ASIO_STOP,
		NULL, 0, NULL, 0,
		&BytesReturned,
		NULL))
	{
		UDBG("failed to stop: 0x%x", GetLastError());
		return ASE_HWMalfunction;
	}

	if(hSyncEvent[0] && hSyncEvent[1])
	{
		BOOLEAN bSet = false;
		DeviceIoControl(hVASIODriver,
			IOCTL_ASIO_EVENT,
			NULL, 0,
			&bSet,
			sizeof(BOOLEAN),			
			&BytesReturned,
			NULL);

		SetEvent(hSyncEvent[0]);
		SetEvent(hSyncEvent[1]);

		if (hASIOThread)
			WaitForSingleObject(hASIOThread, 1000);

		CloseHandle(hSyncEvent[0]);
		CloseHandle(hSyncEvent[1]);
		//CloseHandle(hASIOThread);

		hSyncEvent[0] = NULL;
		hSyncEvent[1] = NULL;
		hASIOThread = NULL;
	}

	return ASE_OK;
}

//------------------------------------------------------------------------------------------
ASIOError VirtualAsio::getChannels (long *numInputChannels, long *numOutputChannels)
{
	*numInputChannels = kNumInputs;
	*numOutputChannels = kNumOutputs;
	return ASE_OK;
}

//------------------------------------------------------------------------------------------
ASIOError VirtualAsio::getLatencies (long *_inputLatency, long *_outputLatency)
{
	*_inputLatency = inputLatency;
	*_outputLatency = outputLatency;
	return ASE_OK;
}

//------------------------------------------------------------------------------------------
ASIOError VirtualAsio::getBufferSize (long *minSize, long *maxSize,
	long *preferredSize, long *granularity)
{
	*minSize = *maxSize = *preferredSize = blockFrames;		// allow this size only
	*granularity = 0;
	return ASE_OK;
}

//------------------------------------------------------------------------------------------
ASIOError VirtualAsio::canSampleRate (ASIOSampleRate sampleRate)
{
	if (sampleRate == 44100. || sampleRate == 48000.)		// allow these rates only
		return ASE_OK;
	return ASE_NoClock;
}

//------------------------------------------------------------------------------------------
ASIOError VirtualAsio::getSampleRate (ASIOSampleRate *sampleRate)
{
	*sampleRate = this->sampleRate;
	return ASE_OK;
}

//------------------------------------------------------------------------------------------
ASIOError VirtualAsio::setSampleRate (ASIOSampleRate sampleRate)
{
	if (sampleRate != 44100. && sampleRate != 48000.)
		return ASE_NoClock;

	if (sampleRate != this->sampleRate)
	{
		this->sampleRate = sampleRate;
		asioTime.timeInfo.sampleRate = sampleRate;
		asioTime.timeInfo.flags |= kSampleRateChanged;
		milliSeconds = (long)((double)(kBlockFrames * 1000) / this->sampleRate);
		if (callbacks && callbacks->sampleRateDidChange)
			callbacks->sampleRateDidChange (this->sampleRate);
	}
	return ASE_OK;
}

//------------------------------------------------------------------------------------------
ASIOError VirtualAsio::getClockSources (ASIOClockSource *clocks, long *numSources)
{
	// internal
	clocks->index = 0;
	clocks->associatedChannel = -1;
	clocks->associatedGroup = -1;
	clocks->isCurrentSource = ASIOTrue;
	strcpy(clocks->name, "Internal");
	*numSources = 1;
	return ASE_OK;
}

//------------------------------------------------------------------------------------------
ASIOError VirtualAsio::setClockSource (long index)
{
	if (!index)
	{
		asioTime.timeInfo.flags |= kClockSourceChanged;
		return ASE_OK;
	}
	return ASE_NotPresent;
}

//------------------------------------------------------------------------------------------
ASIOError VirtualAsio::getSamplePosition (ASIOSamples *sPos, ASIOTimeStamp *tStamp)
{
	tStamp->lo = theSystemTime.lo;
	tStamp->hi = theSystemTime.hi;
	if (samplePosition >= twoRaisedTo32)
	{
		sPos->hi = (unsigned long)(samplePosition * twoRaisedTo32Reciprocal);
		sPos->lo = (unsigned long)(samplePosition - (sPos->hi * twoRaisedTo32));
	}
	else
	{
		sPos->hi = 0;
		sPos->lo = (unsigned long)samplePosition;
	}
	return ASE_OK;
}

//------------------------------------------------------------------------------------------
ASIOError VirtualAsio::getChannelInfo (ASIOChannelInfo *info)
{
	if (info->channel < 0 || 
		(info->isInput ? 
		info->channel >= kNumInputs : 
		info->channel >= kNumOutputs))
	{
		return ASE_InvalidParameter;
	}

	info->type = ASIOSTInt32LSB;
	info->channelGroup = 0;
	info->isActive = ASIOFalse;
	long i;

	if (info->isInput)
	{
		for (i = 0; i < activeInputs; i++)
		{
			if (inMap[i] == info->channel)
			{
				info->isActive = ASIOTrue;
				break;
			}
		}
	}
	else
	{
		for (i = 0; i < activeOutputs; i++)
		{
			if (outMap[i] == info->channel)
			{
				info->isActive = ASIOTrue;
				break;
			}
		}
	}
	strcpy(info->name, "Sample ");
	return ASE_OK;
}

//------------------------------------------------------------------------------------------
ASIOError VirtualAsio::createBuffers(
	ASIOBufferInfo *bufferInfos, 
	long numChannels,
	long bufferSize,
	ASIOCallbacks *callbacks)
{
	DWORD BytesReturned;
	ASIOBufferInfo *info;
	long i;
	long lBufSiz = 0;
	BOOLEAN bSet;
	bool notEnoughMem = false;
	ASIOError ret = ASE_OK;

	BUFPOINTERSTRUCT BufPointers;
	TOTALINPOINTERSTRUCT TotalInPointers;
	TOTALOUTPOINTERSTRUCT TotalOutPointers;

	if(NULL == hVASIODriver || NULL == callbacks)
		return ASE_InvalidParameter;

	bSet = TRUE;

	if(!DeviceIoControl(hVASIODriver,
		IOCTL_ASIO_BUFFER_SIZE,
		&bSet,
		sizeof(BOOLEAN),
		&bufferSize,
		sizeof(long),
		&BytesReturned, 
		NULL))
	{
		UDBG("Failed: Set kspro_ASIO_BUFFER_SIZE %d", bufferSize);
		return ASE_InvalidParameter;
	}

	bSet = FALSE;

	if(!DeviceIoControl(hVASIODriver,
		IOCTL_ASIO_BUFFER_SIZE,
		&bSet,
		sizeof(BOOLEAN),
		&lBufSiz,
		sizeof(long),
		&BytesReturned, 
		NULL) || 
		lBufSiz != bufferSize)
	{
		UDBG("Failed to set buffer size: %d, %d ", lBufSiz, GetLastError());
		return ASE_InvalidParameter;
	}

	__try
	{
		activeInputs = 0;
		activeOutputs = 0;
		blockFrames = bufferSize;
		info = bufferInfos;

		for (i = 0; i < numChannels; i++, info++)
		{
			if (info->isInput)
			{
				if (info->channelNum < 0 || 
					info->channelNum >= kNumInputs)
				{
					ret = ASE_InvalidParameter;
					__leave;
				}

				inMap[activeInputs] = info->channelNum;
				inputBuffers[activeInputs] = new ULONG[blockFrames * 4];	// double buffer

				if (inputBuffers[activeInputs])
				{
					info->buffers[0] = inputBuffers[activeInputs];
					info->buffers[1] = inputBuffers[activeInputs] + blockFrames;
				}
				else
				{
					info->buffers[0] = info->buffers[1] = 0;
					notEnoughMem = true;
				}

				activeInputs++;
				if (activeInputs > kNumInputs)
				{
					ret = ASE_InvalidParameter;
					__leave;
				}
			}
			else	// output			
			{
				if (info->channelNum < 0 || info->channelNum >= kNumOutputs)
				{
					ret = ASE_InvalidParameter;
					__leave;
				}
					
				outMap[activeOutputs] = info->channelNum;
				outputBuffers[activeOutputs] = new ULONG[blockFrames * 2];	// double buffer

				if (outputBuffers[activeOutputs])
				{
					info->buffers[0] = outputBuffers[activeOutputs];
					info->buffers[1] = outputBuffers[activeOutputs] + blockFrames;
				}
				else
				{
					info->buffers[0] = info->buffers[1] = 0;
					notEnoughMem = true;
				}

				activeOutputs++;
				if (activeOutputs > kNumOutputs)
				{
					activeOutputs--;
					ret = ASE_InvalidParameter;
					__leave;
				}
			}
		}

		if (notEnoughMem)
		{
			ret = ASE_NoMemory;
			__leave;
		}

		this->callbacks = callbacks;
		if (callbacks->asioMessage (kAsioSupportsTimeInfo, 0, 0, 0))
		{
			timeInfoMode = true;
			asioTime.timeInfo.speed = 1.;
			asioTime.timeInfo.systemTime.hi = asioTime.timeInfo.systemTime.lo = 0;
			asioTime.timeInfo.samplePosition.hi = asioTime.timeInfo.samplePosition.lo = 0;
			asioTime.timeInfo.sampleRate = sampleRate;
			asioTime.timeInfo.flags = kSystemTimeValid | kSamplePositionValid | kSampleRateValid;

			asioTime.timeCode.speed = 1.;
			asioTime.timeCode.timeCodeSamples.lo = asioTime.timeCode.timeCodeSamples.hi = 0;
			asioTime.timeCode.flags = kTcValid | kTcRunning ;
		}
		else
		{
			timeInfoMode = false;	
		}

		memset(&TotalInPointers,0,sizeof(TotalInPointers));
		for(i = 0; i < activeInputs; i++)
		{
			if(inputBuffers[i])
			{
				TotalInPointers.TotalInChannelBuffer[inMap[i]][0] = (PVOID)inputBuffers[i];
				TotalInPointers.TotalInChannelBuffer[inMap[i]][1] = (PVOID)(inputBuffers[i] + blockFrames*4);
			}
		}

		memset(&TotalOutPointers,0,sizeof(TotalOutPointers));
		for(i = 0;i < activeOutputs; i++)
		{
			if(outputBuffers[i])
			{
				TotalOutPointers.TotalOutChannelBuffer[outMap[i]][0] = (PVOID)outputBuffers[i];
				TotalOutPointers.TotalOutChannelBuffer[outMap[i]][1] = (PVOID)(outputBuffers[i] + blockFrames*4);
			}
		}

		memset(&BufPointers,0,sizeof(BufPointers));
		BufPointers.BufferLength = blockFrames*4;

		for(i=0;i<activeInputs;i++)
		{
			BufPointers.InChannelBuffer[i][0] = (PVOID)TotalInPointers.TotalInChannelBuffer[i][0];
			BufPointers.InChannelBuffer[i][1] = (PVOID)TotalInPointers.TotalInChannelBuffer[i][1];
		}			

		for(i=0;i<activeOutputs;i++)
		{
			BufPointers.OutChannelBuffer[i][0] = (PVOID)TotalOutPointers.TotalOutChannelBuffer[i][0];
			BufPointers.OutChannelBuffer[i][1] = (PVOID)TotalOutPointers.TotalOutChannelBuffer[i][1];
		}

		BOOLEAN bSet = TRUE;

		if(FALSE == DeviceIoControl(hVASIODriver,
			IOCTL_ASIO_BUFFERS,
			&BufPointers,
			sizeof(BufPointers),
			&bSet,
			sizeof(BOOLEAN),
			&BytesReturned, 
			NULL))
		{
			UDBG("failed to set input buffers: 0x%x", GetLastError());
			ret = ASE_InvalidParameter;
			__leave;
		}
	}
	__finally
	{
		if(ret != ASE_OK)
		{
			disposeBuffers();
		}
	}

	return ret;
}

//---------------------------------------------------------------------------------------------
ASIOError VirtualAsio::disposeBuffers()
{
	long i;
	DWORD BytesReturned;
	
	callbacks = NULL;

	stop();

	BOOLEAN bSet = FALSE;
	if(FALSE == DeviceIoControl(hVASIODriver,
		IOCTL_ASIO_BUFFERS,
		NULL,
		0,
		&bSet,
		sizeof(BOOLEAN),
		&BytesReturned, 
		NULL))
	{
		UDBG("failed to free buffers: 0x%x", GetLastError());
		return ASE_InvalidParameter;
	}
	else
	{
		for (i = 0; i < activeInputs; i++)
			delete inputBuffers[i];
		activeInputs = 0;

		for (i = 0; i < activeOutputs; i++)
			delete outputBuffers[i];
		activeOutputs = 0;
	}

	return ASE_OK;
}

//---------------------------------------------------------------------------------------------
ASIOError VirtualAsio::controlPanel()
{
	return ASE_NotPresent;
}

//---------------------------------------------------------------------------------------------
ASIOError VirtualAsio::future (long selector, void* opt)	// !!! check properties 
{
	ASIOTransportParameters* tp = (ASIOTransportParameters*)opt;
	switch (selector)
	{
		case kAsioEnableTimeCodeRead:	tcRead = true;	return ASE_SUCCESS;
		case kAsioDisableTimeCodeRead:	tcRead = false;	return ASE_SUCCESS;
		case kAsioSetInputMonitor:		return ASE_SUCCESS;	// for testing!!!
		case kAsioCanInputMonitor:		return ASE_SUCCESS;	// for testing!!!
		case kAsioCanTimeInfo:			return ASE_SUCCESS;
		case kAsioCanTimeCode:			return ASE_SUCCESS;
	}
	return ASE_NotPresent;
}

//--------------------------------------------------------------------------------------------------------
// private methods
//--------------------------------------------------------------------------------------------------------

bool VirtualAsio::inputOpen ()
{
	return true;
}


//---------------------------------------------------------------------------------------------
void VirtualAsio::inputClose ()
{
}

//---------------------------------------------------------------------------------------------
void VirtualAsio::input()
{
	return;

#if 0
	long i;
	UONG *in = 0;

	for (i = 0; i < activeInputs; i++)
	{
		in = inputBuffers[i];
		if (in)
		{
			if (toggle)
				in += blockFrames;
			if ((i & 1) && sawTooth)
				memcpy(in, sawTooth, (unsigned long)(blockFrames * 2));
			else if (sineWave)
				memcpy(in, sineWave, (unsigned long)(blockFrames * 2));
		}
	}
#endif
}

//---------------------------------------------------------------------------------------------
bool VirtualAsio::outputOpen()
{
	return true;
}

//---------------------------------------------------------------------------------------------
void VirtualAsio::outputClose ()
{
}

//---------------------------------------------------------------------------------------------
void VirtualAsio::output (USHORT toggle)
{
	ULONG ProcessBuffer;
	DWORD BytesReturned;

	ProcessBuffer = toggle;
	if(ProcessBuffer != 0)
		ProcessBuffer = 1;

	if(NULL == hVASIODriver)
		return;

	if(started)
	{
		if(FALSE == DeviceIoControl(hVASIODriver,
			IOCTL_ASIO_BUFFER_READY,			
			&ProcessBuffer,
			sizeof(ProcessBuffer),
			NULL, 0,
			&BytesReturned, 
			NULL))
		{
			UDBG("Failed to set buffer: 0x%d", GetLastError());
		}
	}
}

void getMMETimeStamp (ASIOTimeStamp* ts)
{
	double nanoSeconds = (double)((unsigned long)timeGetTime ()) * 1000000.;
	ts->hi = (unsigned long)(nanoSeconds / twoRaisedTo32);
	ts->lo = (unsigned long)(nanoSeconds - (ts->hi * twoRaisedTo32));
}

//---------------------------------------------------------------------------------------------
void VirtualAsio::bufferSwitch (USHORT toggle)
{
	if (started && callbacks)
	{
		getMMETimeStamp(&theSystemTime);

		samplePosition += blockFrames;
		if (timeInfoMode)
			bufferSwitchX (toggle);
		else
			callbacks->bufferSwitch (toggle, ASIOFalse);

		input();
		output(toggle);
	}
}

//---------------------------------------------------------------------------------------------
// asio2 buffer switch
void VirtualAsio::bufferSwitchX (USHORT toggle)
{
	getSamplePosition (&asioTime.timeInfo.samplePosition, &asioTime.timeInfo.systemTime);
	long offset = toggle ? blockFrames : 0;
	if (tcRead)
	{	// Create a fake time code, which is 10 minutes ahead of the card's sample position
		// Please note that for simplicity here time code will wrap after 32 bit are reached
		asioTime.timeCode.timeCodeSamples.lo = asioTime.timeInfo.samplePosition.lo + 600.0 * sampleRate;
		asioTime.timeCode.timeCodeSamples.hi = 0;
	}
	callbacks->bufferSwitchTimeInfo (&asioTime, toggle, ASIOFalse);
	asioTime.timeInfo.flags &= ~(kSampleRateChanged | kClockSourceChanged);
}

//---------------------------------------------------------------------------------------------
ASIOError VirtualAsio::outputReady ()
{
	return ASE_NotPresent;
}

//---------------------------------------------------------------------------------------------
double AsioSamples2double (ASIOSamples* samples)
{
	double a = (double)(samples->lo);
	if (samples->hi)
		a += (double)(samples->hi) * twoRaisedTo32;
	return a;
}


#if (DBG)
void UDBG(char *msg, ...)
{
	static char errstr[1024];

	va_list argp;

	va_start(argp, msg);
	StringCchVPrintfA(errstr, sizeof(errstr), msg, argp);
	va_end(argp);

	OutputDebugStringA("ASIODrv:");
	OutputDebugStringA(errstr);
	OutputDebugStringA("\r\n");
}
#else
void UDBG(char *msg, ...){};
#endif