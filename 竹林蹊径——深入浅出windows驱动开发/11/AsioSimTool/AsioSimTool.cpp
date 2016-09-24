/*++
Abstract: 
 A Simple ASIO Application
 Based on ASIO SDK 2.0 Sample

Copyright (c) Moore.Zhang 09/21/2010
 
Contact: 
 zhang.mibox@gmail.com

Last modified: 
 12/01/2010
*/

#include "stdafx.h"
#include <Windows.h>
#include <Mmsystem.h>
#include <assert.h>
#include "asiotool.h"
#include "command.h"
#include "playmusicfile.h"
#include "asiolist.h"

// 下列四个Message回调
ASIOTime *bufferSwitchTimeInfo(ASIOTime *timeInfo, long index, ASIOBool processNow);
void bufferSwitch(long index, ASIOBool processNow);
void sampleRateChanged(ASIOSampleRate sRate);
long asioMessages(long selector, long value, void* message, double* opt);


IASIO*		g_pASIODrv = NULL;
DriverInfo  g_AsioInfo;

int _tmain(int argc, _TCHAR* argv[])
{
	ASIOError result;
	char strErr[128];
	ASIOCallbacks asioCallbacks;

	__try
	{
		//
		// 获取ASIO驱动接口类的实例；这是第一步工作，如果成功才能进行下面的初始化工作
		//

		g_pASIODrv = loadDriver(argv[1]);
		if(g_pASIODrv == NULL)
		{
			printf("找不到ASIO设备\n");
			__leave;
		}

		// 
		// 初始化
		//

		result = g_pASIODrv->init(NULL);
		if(ASIOTrue != result)
		{
			g_pASIODrv->getErrorMessage(strErr); // 获取错误信息
			printf("调用init方法失败:%d %s", result, strErr);
			__leave;
		}

		// 获取ASIO驱动名称
		g_pASIODrv->getDriverName(g_AsioInfo.driverInfo.name);
		g_AsioInfo.driverInfo.driverVersion = g_pASIODrv->getDriverVersion();
		printf ("\n名称: %s\n版本: %d\n", 
			g_AsioInfo.driverInfo.name,
			g_AsioInfo.driverInfo.driverVersion
			);

		// 获取缓冲信息
		result = g_pASIODrv->getBufferSize(&g_AsioInfo.minSize, &g_AsioInfo.maxSize, 
			&g_AsioInfo.preferredSize, &g_AsioInfo.granularity);
		if(result != ASE_OK)
		{
			g_pASIODrv->getErrorMessage(strErr); // 获取错误信息
			printf("调用getBufferSize方法失败:%d %s", result, strErr);
			__leave;
		}

		// 获取当前采样频率
		result = g_pASIODrv->getSampleRate(&g_AsioInfo.sampleRate);
		if(result != ASE_OK)
		{
			g_pASIODrv->getErrorMessage(strErr); // 获取错误信息
			printf("调用getSampleRate方法失败:%d %s", result, strErr);
			__leave;
		}
		
		// 判断获取的当前采样频率值是否在有效区间
		if (g_AsioInfo.sampleRate <= 0.0 || g_AsioInfo.sampleRate > 96000.0)
		{
			result = g_pASIODrv->setSampleRate(44100.0);
			if(ASE_OK != result)
			{
				printf("不支持默认采样频率：%d", result);
				__leave;
			}

			result = g_pASIODrv->getSampleRate(&g_AsioInfo.sampleRate);
			if(result != ASE_OK)
			{
				g_pASIODrv->getErrorMessage(strErr); // 获取错误信息
				printf("调用getSampleRate方法失败:%d %s", result, strErr);
				__leave;
			}
		}
		
		// 获取输入/输出声道数
		result = g_pASIODrv->getChannels(&g_AsioInfo.inputChannels, &g_AsioInfo.outputChannels);
		if(result != ASE_OK)
		{
			g_pASIODrv->getErrorMessage(strErr); // 获取错误信息
			printf("调用getChannels方法失败:%d %s", result, strErr);
			__leave;
		}
		
		// 针对每个声道，初始化ASIO缓冲
		//
		ASIOBufferInfo *info = g_AsioInfo.bufferInfos;

		// input
		if (g_AsioInfo.inputChannels > kMaxInputChannels)
			g_AsioInfo.inputBuffers = kMaxInputChannels;
		else
			g_AsioInfo.inputBuffers = g_AsioInfo.inputChannels;

		for(int i = 0; i < g_AsioInfo.inputBuffers; i++, info++)
		{
			info->isInput = ASIOTrue;
			info->channelNum = i;
			info->buffers[0] = info->buffers[1] = 0;
		}

		// outputs
		if (g_AsioInfo.outputChannels > kMaxOutputChannels)
			g_AsioInfo.outputBuffers = kMaxOutputChannels;
		else
			g_AsioInfo.outputBuffers = g_AsioInfo.outputChannels;

		for(int i = 0; i < g_AsioInfo.outputBuffers; i++, info++)
		{
			info->isInput = ASIOFalse;
			info->channelNum = i;
			info->buffers[0] = info->buffers[1] = 0;
		}

		// 初始化回调函数数组。这些函数必须由音频程序自己提供，被ASIO驱动调用。
		// bufferSwitch是实现音乐播放的关键，此处传入音乐文件数据，就能播放出相应的音乐了。
		// sampleRateChanged在设备采样率改变时被调用，以提醒播放软件更改显示信息。
		// bufferSwitch和bufferSwitchTimeInfo两个回调函数，ASIO驱动在需要更新音频数据的时候调用它们。
		// 后者是前者的升级版，即ASIO驱动在调用的时候会传入一个时间信息结构指针，便于音频软件做同步、定位等操作。
		// asioMessages回调可以让ASIO驱动更好地了解音频软件的“能力”，比如它所能支持的ASIO版本，是1.0还是2.0等。
		asioCallbacks.bufferSwitch = &bufferSwitch;
		asioCallbacks.sampleRateDidChange = &sampleRateChanged;
		asioCallbacks.asioMessage = &asioMessages;
		asioCallbacks.bufferSwitchTimeInfo = &bufferSwitchTimeInfo;

		// 创建缓冲区，注册回调函数
		result = g_pASIODrv->createBuffers(g_AsioInfo.bufferInfos,
			g_AsioInfo.inputBuffers + g_AsioInfo.outputBuffers,
			g_AsioInfo.preferredSize, 
			&asioCallbacks
			);

		if(result != ASE_OK)
		{
			g_pASIODrv->getErrorMessage(strErr); 
			printf("调用createBuffers方法失败:%d %s", result, strErr);
			__leave;
		}
	
		// 调用createBuffers后，可获取设备的输入输出延迟。
		// 延迟是一定存在的，除非操作系统能够快到收到一个字节，立刻播放一个字节的速度。
		// 既然有缓冲区，就有先后、等待。等待的时间就是延迟。
		// 输入延迟：一个音频采样从被设备获取到被传入音频软件，所经历的时间
		// 输出延迟：一个采样数据从音频软件输出去，直到它被声卡播放出来，期间所经历的时间
		g_pASIODrv->getLatencies(&g_AsioInfo.inputLatency, &g_AsioInfo.outputLatency);

		// 获取声道的详细信息
		printf("声道信息...\n\n输入声道：%d个\t输出声道：%d个\n", g_AsioInfo.inputBuffers, g_AsioInfo.outputBuffers);
		for (int i = 0; i < g_AsioInfo.inputBuffers + g_AsioInfo.outputBuffers; i++)
		{
			g_AsioInfo.channelInfos[i].channel = g_AsioInfo.bufferInfos[i].channelNum;
			g_AsioInfo.channelInfos[i].isInput = g_AsioInfo.bufferInfos[i].isInput;
			result = g_pASIODrv->getChannelInfo(&g_AsioInfo.channelInfos[i]);

			if (result == ASE_OK)
				printf("%d %s声道%d:%s group: %d type: %d\n", 
					i, g_AsioInfo.channelInfos[i].isInput ? "输入":"输出",
					g_AsioInfo.channelInfos[i].channel,
					g_AsioInfo.channelInfos[i].name,
					g_AsioInfo.channelInfos[i].channelGroup,
					g_AsioInfo.channelInfos[i].type);
		}

		printf("\n音频信息...\n\n");
		printf( "采样率:%d\n", g_AsioInfo.sampleRate);
		printf( "输入延迟:%d, 输出延迟:%d\n", 
			g_AsioInfo.inputLatency, g_AsioInfo.outputLatency);
		printf( "最大缓冲:%d, 最小缓冲:%d, 当前缓冲:%d, 缓冲粒度:%d\n",
			g_AsioInfo.maxSize, g_AsioInfo.minSize, 
			g_AsioInfo.preferredSize, g_AsioInfo.granularity);

		// 测试ASIO驱动是否支持outputReady接口。
		// 如果ASIO驱动不支持，按照SDK中所做说明，返回值当为ASE_NotPresent；
		// 这种情况下，我们程序下面就不应当调用outputReady与ASIO驱动进行数据同步。
		if(ASE_OK == g_pASIODrv->outputReady())
			g_AsioInfo.postOutput = TRUE;
		else
			g_AsioInfo.postOutput = FALSE;

		InitializeCommandVaribles();
		
		// 提高线程优先级
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

		// 等待并处理用户命令，直到退出。
		while (!g_AsioInfo.bMainQuit)
		{
			CommandProcess();
		}		
	}
	__finally
	{
		if(g_pASIODrv)
		{
			g_pASIODrv->disposeBuffers();
			unloadASIO();
			g_pASIODrv = 0;
		}

		if(g_pDataBuf)
			delete g_pDataBuf;
	}
	

	return 0;
}


ASIOTime *bufferSwitchTimeInfo(ASIOTime *timeInfo, long index, ASIOBool processNow)
{
	// 处理时间信息，ASIO的时间戳为64位数据。
	// ASIO支持多种时间戳的格式：系统时间（时分秒）、采样位置、TC时间码
	g_AsioInfo.tInfo = *timeInfo;

	if (timeInfo->timeInfo.flags & kSystemTimeValid)
		g_AsioInfo.nanoSeconds = ASIO64toDouble(timeInfo->timeInfo.systemTime);
	else
		g_AsioInfo.nanoSeconds = 0;

	if (timeInfo->timeInfo.flags & kSamplePositionValid)
		g_AsioInfo.samples = ASIO64toDouble(timeInfo->timeInfo.samplePosition);
	else
		g_AsioInfo.samples = 0;

	if (timeInfo->timeCode.flags & kTcValid)
		g_AsioInfo.tcSamples = ASIO64toDouble(timeInfo->timeCode.timeCodeSamples);
	else
		g_AsioInfo.tcSamples = 0;

	g_AsioInfo.sysRefTime = timeGetTime();

	// 由于此软件不支持用户对播放声道进行选择，所以默认只对最前面的两个输入/输出声道进行有效处理。
	// 下面两个布尔量就是用来判断是否已经处理过最前面的两个声道。
	bool bOutEnd = false;
	bool bInEnd = false;
	
	// 采样个数
	long buffSize = g_AsioInfo.preferredSize;

	// 音乐数据处理，针对每一个声道（声道的缓冲）
	for (int i = 0; i < g_AsioInfo.inputBuffers + g_AsioInfo.outputBuffers; i++)
	{
		// 输出声道缓冲
		if (g_AsioInfo.bAsioStarted && g_AsioInfo.bufferInfos[i].isInput == false)
		{
			switch (g_AsioInfo.channelInfos[i].type)
			{
			case ASIOSTInt16LSB:
				memset (g_AsioInfo.bufferInfos[i].buffers[index], 0, buffSize * 2);
				break;

			case ASIOSTInt24MSB:
			case ASIOSTInt24LSB:// used for 20 bits as well
				memset (g_AsioInfo.bufferInfos[i].buffers[index], 0, buffSize * 3);
				break;

			// 仅支持32位对齐格式,其他的格式未支持。
			// ASIO 音频软件有责任支持全部的对齐格式，而ASIO驱动只需要支持至少一种格式即可。
			// 32位对齐格式在我测试过的Mi-Box，ASIO4ALL等ASIO驱动中都能使用。
			case ASIOSTInt32LSB:
				
				memset (g_AsioInfo.bufferInfos[i].buffers[index], 0, buffSize * 4);
				if(bOutEnd == false)
				{
					static int iBufs = 0;
					iBufs++;

					bOutEnd = true; // 后续的输出声道不再处理

					// 自动立体声支持。如果下一个声道也是输出声道，就自动与当前声道合并为立体声。
					unsigned char* channel0 = (unsigned char*)g_AsioInfo.bufferInfos[i].buffers[index];
					unsigned char* channel1 = NULL;
					if(g_AsioInfo.bufferInfos[i+1].isInput == false)
					{
						channel1 = (unsigned char*)g_AsioInfo.bufferInfos[i++].buffers[index];
					}

					// 读取音频数据
					if(EOF == readWaveFile(channel0, channel1, buffSize, ASIOSTInt32LSB))
					{
						fprintf(stdout, "文件播放结束(%d)\ncmd>", iBufs);
						iBufs = 0;
						closeWaveFile();
						return 0L;
					}
				};
				break;

			case ASIOSTFloat32LSB:		// IEEE 754 32 bit float, as found on Intel x86 architecture
				memset (g_AsioInfo.bufferInfos[i].buffers[index], 0, buffSize * 4);
				break;

			case ASIOSTFloat64LSB: 		// IEEE 754 64 bit double float, as found on Intel x86 architecture
				memset (g_AsioInfo.bufferInfos[i].buffers[index], 0, buffSize * 8);
				break;

				// these are used for 32 bit data buffer, with different alignment of the data inside
				// 32 bit PCI bus systems can more easily used with these
			case ASIOSTInt32LSB16:		// 32 bit data with 18 bit alignment
			case ASIOSTInt32LSB18:		// 32 bit data with 18 bit alignment
			case ASIOSTInt32LSB20:		// 32 bit data with 20 bit alignment
			case ASIOSTInt32LSB24:		// 32 bit data with 24 bit alignment
				memset (g_AsioInfo.bufferInfos[i].buffers[index], 0, buffSize * 4);
				break;

			case ASIOSTInt16MSB:
				memset (g_AsioInfo.bufferInfos[i].buffers[index], 0, buffSize * 2);
				break;

			case ASIOSTInt32MSB:
				memset (g_AsioInfo.bufferInfos[i].buffers[index], 0, buffSize * 4);
				break;

			case ASIOSTFloat32MSB:		// IEEE 754 32 bit float, as found on Intel x86 architecture
				memset (g_AsioInfo.bufferInfos[i].buffers[index], 0, buffSize * 4);
				break;

			case ASIOSTFloat64MSB: 		// IEEE 754 64 bit double float, as found on Intel x86 architecture
				memset (g_AsioInfo.bufferInfos[i].buffers[index], 0, buffSize * 8);
				break;

				// these are used for 32 bit data buffer, with different alignment of the data inside
				// 32 bit PCI bus systems can more easily used with these
			case ASIOSTInt32MSB16:		// 32 bit data with 18 bit alignment
			case ASIOSTInt32MSB18:		// 32 bit data with 18 bit alignment
			case ASIOSTInt32MSB20:		// 32 bit data with 20 bit alignment
			case ASIOSTInt32MSB24:		// 32 bit data with 24 bit alignment
				memset (g_AsioInfo.bufferInfos[i].buffers[index], 0, buffSize * 4);
				break;
			}
		}else if(g_AsioInfo.bufferInfos[i].isInput)
		{
			// 这里处理Input的事务

			switch(g_AsioInfo.channelInfos[i].type)
			{
			case ASIOSTInt32LSB:
				if(g_AsioInfo.bAudioRecord && bInEnd == false)
				{
					bInEnd = true; // 后续的输出声道不再处理

					static char* charTmp = NULL;
					if(charTmp == NULL)	charTmp = new char[g_AsioInfo.maxSize*4];

					// 和输出部分的处理相同，也是自动双声道对齐
					for(int j = 0; j < buffSize; j++)
					{
						*(long*)(charTmp + j*8) = ((long*)(g_AsioInfo.bufferInfos[i].buffers[index]))[j];
						
						if(g_AsioInfo.bufferInfos[i+1].isInput)
							*(long*)(charTmp + j*8 + 4) = ((long*)(g_AsioInfo.bufferInfos[i+1].buffers[index]))[j];
						else
							*(long*)(charTmp + j*8 + 4) = 0;
					}

					i++;

					// 保存录入的音频数据
					writeWaveFile(charTmp, buffSize*2);
				}
			default: // 其他格式都不支持，但支持也很容易
				break;
			}			
		}
	}

	// 通知ASIO Driver，数据已准备好。
	// 只有在ASIO驱动支持outputReady接口的情况下，才需调用之。
	// 利用outputReady接口调用，ASIO驱动可能省掉一个临时缓冲，从而降低了延迟。
	if (g_AsioInfo.postOutput) g_pASIODrv->outputReady();

	return 0L;
}

void bufferSwitch(long index, ASIOBool processNow)
{
	// bufferSwitchTimeInfo是bufferSwitch的升级版本，就是说前者比后者多了一个时间参数。
	// 我们在bufferSwitch中调用bufferSwitchTimeInfo，并传入一个自己的时间参数。
	// 时间参数不能为空。但可以把flags值置为0，这样bufferSwitchTimeInfo就会忽略它了。

	ASIOTime  timeInfo;
	memset (&timeInfo, 0, sizeof (timeInfo));

	if(g_pASIODrv->getSamplePosition(&timeInfo.timeInfo.samplePosition, &timeInfo.timeInfo.systemTime) == ASE_OK)
		timeInfo.timeInfo.flags = kSystemTimeValid | kSamplePositionValid;

	bufferSwitchTimeInfo (&timeInfo, index, processNow);
}

void sampleRateChanged(ASIOSampleRate sRate)
{
	printf("采样率改变为:%f", sRate);
}

// 
long asioMessages(long selector, long value, void* message, double* opt)
{
	int ret = 0;
	switch(selector)
	{
	case kAsioSelectorSupported:
		if(value == kAsioResetRequest
			|| value == kAsioEngineVersion
			|| value == kAsioResyncRequest
			|| value == kAsioLatenciesChanged
			// the following three were added for ASIO 2.0, you don't necessarily have to support them
			|| value == kAsioSupportsTimeInfo
			|| value == kAsioSupportsTimeCode
			|| value == kAsioSupportsInputMonitor)
			ret = 1L;
		break;

	case kAsioResetRequest:
		// defer the task and perform the reset of the driver during the next "safe" situation
		// You cannot reset the driver right now, as this code is called from the driver.
		// Reset the driver is done by completely destruct is. I.e. ASIOStop(), ASIODisposeBuffers(), Destruction
		// Afterwards you initialize the driver again.
		g_AsioInfo.stopped = TRUE;  // In this sample the processing will just stop
		g_AsioInfo.bAsioStarted = FALSE;
		g_AsioInfo.bAudioRecord = FALSE;

		ret = 1L;
		break;

	case kAsioResyncRequest:
		// This informs the application, that the driver encountered some non fatal data loss.
		// It is used for synchronization purposes of different media.
		// Added mainly to work around the Win16Mutex problems in Windows 95/98 with the
		// Windows Multimedia system, which could loose data because the Mutex was hold too long
		// by another thread.
		// However a driver can issue it in other situations, too.
		ret = 1L;
		break;

	case kAsioLatenciesChanged:
		// This will inform the host application that the drivers were latencies changed.
		// Beware, it this does not mean that the buffer sizes have changed!
		// You might need to update internal delay data.
		ret = 1L;
		break;

	case kAsioEngineVersion:
		// return the supported ASIO version of the host application
		// If a host applications does not implement this selector, ASIO 1.0 is assumed
		// by the driver
		ret = 2L;
		break;

	case kAsioSupportsTimeInfo:
		// informs the driver wether the asioCallbacks.bufferSwitchTimeInfo() callback
		// is supported.
		// For compatibility with ASIO 1.0 drivers the host application should always support
		// the "old" bufferSwitch method, too.
		ret = 1;
		break;

	case kAsioSupportsTimeCode:
		// informs the driver wether application is interested in time code info.
		// If an application does not need to know about time code, the driver has less work
		// to do.
		ret = 0;
		break;
	}

	return ret;
}