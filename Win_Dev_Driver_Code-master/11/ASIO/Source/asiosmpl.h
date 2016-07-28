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

#ifndef _asiosmpl_
#define _asiosmpl_

#include "asiosys.h"
#include "rpc.h"
#include "rpcndr.h"
#include <windows.h>
#include "ole2.h"
#include "combase.h"
#include "iasiodrv.h"

enum
{
	kBlockFrames = 256,
	kNumInputs = 16,
	kNumOutputs = 16
};


#define		ASIO_MAX_CHANNELS	20
#define     MAX_IN_CHANNELS     20
#define		MAX_OUT_CHANNELS	20
#define     ASIO_MAX_BUFFERS	2

// 定义一组IOCTL值。用户驱动通过这一组值，和内核驱动
// 进行通信，比如：建立同步，启动、停止ASIO数据流等
// 目前的驱动版本只实现了有限的功能，将来只要扩充这组
// IOCTL值，就可以进行功能扩充。
#define IOCTL_ASIO_START \
	CTL_CODE( FILE_DEVICE_UNKNOWN, 0x800, METHOD_NEITHER, FILE_ANY_ACCESS )

#define IOCTL_ASIO_STOP \
	CTL_CODE( FILE_DEVICE_UNKNOWN, 0x801, METHOD_NEITHER, FILE_ANY_ACCESS )

#define IOCTL_ASIO_EVENT \
	CTL_CODE( FILE_DEVICE_UNKNOWN, 0x802, METHOD_NEITHER, FILE_ANY_ACCESS )

#define IOCTL_ASIO_BUFFERS \
	CTL_CODE( FILE_DEVICE_UNKNOWN, 0x803, METHOD_NEITHER, FILE_ANY_ACCESS )

#define IOCTL_ASIO_BUFFER_READY \
	CTL_CODE( FILE_DEVICE_UNKNOWN, 0x804, METHOD_NEITHER, FILE_ANY_ACCESS )

#define IOCTL_ASIO_BUFFER_SIZE \
	CTL_CODE( FILE_DEVICE_UNKNOWN, 0x805, METHOD_NEITHER, FILE_ANY_ACCESS )

void UDBG(char *msg, ...);
extern LONG RegisterAsioDriver (CLSID,char *,char *,char *,char *);
extern LONG UnregisterAsioDriver (CLSID,char *,char *);

class VirtualAsio : 
	public IASIO, 
	public CUnknown
{
public:
	VirtualAsio(LPUNKNOWN pUnk, HRESULT *phr);
	~VirtualAsio();

	DECLARE_IUNKNOWN
    //STDMETHODIMP QueryInterface(REFIID riid, void **ppv) {      \
    //    return GetOwner()->QueryInterface(riid,ppv);            \
    //};                                                          \
    //STDMETHODIMP_(ULONG) AddRef() {                             \
    //    return GetOwner()->AddRef();                            \
    //};                                                          \
    //STDMETHODIMP_(ULONG) Release() {                            \
    //    return GetOwner()->Release();                           \
    //};

	// Factory method
	static CUnknown *CreateInstance(LPUNKNOWN pUnk, HRESULT *phr);
	// IUnknown
	virtual HRESULT STDMETHODCALLTYPE 
		NonDelegatingQueryInterface(REFIID riid,void **ppvObject);

	ASIOBool init (void* sysRef);
	void getDriverName (char *name);	// max 32 bytes incl. terminating zero
	long getDriverVersion ();
	void getErrorMessage (char *string);	// max 128 bytes incl.

	ASIOError start ();
	ASIOError stop ();

	ASIOError getChannels (long *numInputChannels, long *numOutputChannels);
	ASIOError getLatencies (long *inputLatency, long *outputLatency);
	ASIOError getBufferSize (long *minSize, long *maxSize,
		long *preferredSize, long *granularity);

	ASIOError canSampleRate (ASIOSampleRate sampleRate);
	ASIOError getSampleRate (ASIOSampleRate *sampleRate);
	ASIOError setSampleRate (ASIOSampleRate sampleRate);
	ASIOError getClockSources (ASIOClockSource *clocks, long *numSources);
	ASIOError setClockSource (long index);

	ASIOError getSamplePosition (ASIOSamples *sPos, ASIOTimeStamp *tStamp);
	ASIOError getChannelInfo (ASIOChannelInfo *info);

	ASIOError createBuffers (ASIOBufferInfo *bufferInfos, long numChannels,
		long bufferSize, ASIOCallbacks *callbacks);
	ASIOError disposeBuffers ();

	ASIOError controlPanel ();
	ASIOError future (long selector, void *opt);
	ASIOError outputReady ();

	void bufferSwitch(USHORT toggle);
	long getMilliSeconds () {return milliSeconds;}

	BOOLEAN bCloseEvent;
	HANDLE hVASIODriver;
	HANDLE hSyncEvent[2];
	HANDLE hThreadEvent;
	HANDLE hASIOThread;

private:

	bool inputOpen ();
	void inputClose ();
	void input ();

	bool outputOpen ();
	void outputClose ();
	void output (USHORT toggle);

	void bufferSwitchX (USHORT toggle);

	double samplePosition;
	double sampleRate;
	ASIOCallbacks *callbacks;
	ASIOTime asioTime;
	ASIOTimeStamp theSystemTime;
	ULONG *inputBuffers[kNumInputs * 2];
	ULONG *outputBuffers[kNumOutputs * 2];
	long inMap[kNumInputs];
	long outMap[kNumOutputs];
	long blockFrames;
	long inputLatency;
	long outputLatency;
	long activeInputs;
	long activeOutputs;
	long milliSeconds;
	bool active, started;
	bool timeInfoMode, tcRead;
	char errorMessage[128];

	friend static DWORD __stdcall ASIOThread (void *param);
};

#endif

