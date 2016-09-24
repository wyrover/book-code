#ifndef ASIO_TOOL_H
#define ASIO_TOOL_H

#include "iasiodrv.h"
#include "asio.h"

#pragma warning(disable:4996)

extern  IASIO*  g_pASIODrv;

enum 
{
	// number of input and outputs supported by the host application
	// you can change these to higher or lower values
	kMaxInputChannels = 32,
	kMaxOutputChannels = 32
};

// conversion from 64 bit ASIOSample/ASIOTimeStamp to double float
#if NATIVE_INT64
#define ASIO64toDouble(a)  (a)
#else
static const double twoRaisedTo32 = 4294967296.;
#define ASIO64toDouble(a)  ((a).lo + (a).hi * twoRaisedTo32)
#endif

// 统装数据. 
typedef struct DriverInfo
{
	// ASIOInit()
	ASIODriverInfo driverInfo;

	// ASIOGetChannels()
	long           inputChannels;
	long           outputChannels;

	// ASIOGetBufferSize()
	long           minSize;
	long           maxSize;
	long           preferredSize;
	long           granularity;

	// ASIOGetSampleRate()
	ASIOSampleRate sampleRate;

	// ASIOOutputReady()
	bool           postOutput;

	// ASIOGetLatencies ()
	long           inputLatency;
	long           outputLatency;

	// ASIOCreateBuffers ()
	long inputBuffers;	// becomes number of actual created input buffers
	long outputBuffers;	// becomes number of actual created output buffers
	ASIOBufferInfo bufferInfos[kMaxInputChannels + kMaxOutputChannels]; // buffer info's

	// ASIOGetChannelInfo()
	ASIOChannelInfo channelInfos[kMaxInputChannels + kMaxOutputChannels]; // channel info's
	// The above two arrays share the same indexing, as the data in them are linked together

	// Information from ASIOGetSamplePosition()
	// data is converted to double floats for easier use, however 64 bit integer can be used, too
	double         nanoSeconds;
	double         samples;
	double         tcSamples;	// time code samples

	// bufferSwitchTimeInfo()
	ASIOTime       tInfo;			// time info state
	unsigned long  sysRefTime;      // system reference time, when bufferSwitch() was called

	// Signal the end of processing in this example
	bool           stopped;

	
	bool	bAsioStarted;	// IASIO::start是否已调用？
	bool    bAudioRecord;
	bool	bMainQuit;		// 用户输入命令'q’，主线程退出
} DriverInfo;

extern  DriverInfo g_AsioInfo;

#endif