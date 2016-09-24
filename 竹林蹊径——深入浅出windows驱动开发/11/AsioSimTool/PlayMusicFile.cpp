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
#include <math.h>
#include <assert.h>
#include "asiotool.h"
#include "asio.h"
#include "playmusicfile.h"
#include "command.h"

HANDLE g_hWaveFile = NULL;
HANDLE g_hWaveFileRecord = NULL;
DWORD g_recordFileSize = 0;

unsigned char *g_pDataBuf = NULL;
unsigned int g_dataBufSize = 0;
WAVE_FORMAT g_fmtBlock;

void closeWaveFile()
{
	stop("p");

	if(g_hWaveFile)
	{
		CloseHandle(g_hWaveFile);
		g_hWaveFile = NULL;
	}
}

int readAudioData(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead,
			   LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
	if(!ReadFile(
		hFile, 
		lpBuffer,
		nNumberOfBytesToRead, 
		lpNumberOfBytesRead, 
		lpOverlapped))
	{
		return GetLastError();
	}

	if(*lpNumberOfBytesRead == 0)
		return EOF;

	return 0;
}

// 这个对外输出
int playWaveFile(char *filePath)
{
	DWORD dwDataSize = 0;
	int nRet = openWaveFile(filePath, &g_hWaveFile, &g_fmtBlock, &dwDataSize);

	// 创建新的数据缓冲区
	if(nRet == 0){
		unsigned int cur_buf_size = 
			g_AsioInfo.preferredSize*g_fmtBlock.wChannels*g_fmtBlock.wBitsPerSample/8;
		if(cur_buf_size > g_dataBufSize){
			g_dataBufSize = cur_buf_size;
			delete[] g_pDataBuf;
			g_pDataBuf = new unsigned char[cur_buf_size];
			if(g_pDataBuf == NULL)
				return -1;
		}
	}

	return nRet;
}

// 返回值： 0: 正确读取
//			EOF: 文件结束
//			其他: 错误码
int openWaveFile(char* filePath, HANDLE* hFile, WAVE_FORMAT* wave_fmt, DWORD *dwDataSize)
{
	int nRet = 0;
	assert(filePath != NULL);
	assert(hFile != NULL);

	__try{
		
		*hFile = CreateFile(filePath, 
			GENERIC_ALL, 
			FILE_SHARE_READ,
			NULL, 
			OPEN_EXISTING, 
			0, NULL);

		if(*hFile == (HANDLE)-1)
		{
			nRet = GetLastError();
			__leave;
		}

		// 读RIFF头
		RIFF_HEADER riffhdr;
		DWORD dwRead = 0;
		if(nRet = readAudioData(*hFile, &riffhdr, sizeof(riffhdr), &dwRead, NULL))
			__leave;

		if(strncmp(riffhdr.szRiffID, "RIFF", 4) || strncmp(riffhdr.szRiffFormat, "WAVE", 4))
		{
			nRet = -1;
			__leave;
		}

		// 有些小畜崽子喜欢在这中间放些另类的东西，只有它们自己明白；判别并过滤之。
		char tszOther[4];
		if(nRet = readAudioData(*hFile, tszOther, 4, &dwRead, NULL))
			__leave;

		if(!strncmp(tszOther, "fmt ", 4))// 是它就退4
		{
			SetFilePointer(*hFile, -4, 0, FILE_CURRENT);
		}
		else
		{		
			// 寻找"fmt " 块
			char tcsFmt[20] = {0};
			char* p_Fmt = NULL;
			int  nPos = 0;
			int nTry = 1000;
			
			while(nTry--)
			{
				if(nRet = readAudioData(*hFile, tcsFmt, 4, &dwRead, NULL))
					__leave;
				
				for(int iFmt = 0; iFmt < 4; iFmt++)
					if(tcsFmt[iFmt] == NULL) tcsFmt[iFmt] = '1';

				if(!strncmp(tcsFmt, "fmt ", 4)){
					SetFilePointer(*hFile, -4, 0, FILE_CURRENT);
					break;
				}else if(NULL != (p_Fmt = strstr(tcsFmt, "f")) && 3 == p_Fmt-tcsFmt){
					SetFilePointer(*hFile, -1, 0, FILE_CURRENT);
					continue;
				}else if(NULL != (p_Fmt = strstr(tcsFmt, "fm")) && 2 == p_Fmt-tcsFmt){
					SetFilePointer(*hFile, -2, 0, FILE_CURRENT);
					continue;
				}else if(NULL != (p_Fmt = strstr(tcsFmt, "fmt")) && 1 == p_Fmt-tcsFmt){
					SetFilePointer(*hFile, -3, 0, FILE_CURRENT);
					continue;
				}
			}

			if(nTry < 0){
				nRet = 1;
				__leave;
			}
		}

		// 读格式块
		FMT_BLOCK fmtblock;
		if(nRet = readAudioData(*hFile, &fmtblock, sizeof(fmtblock), &dwRead, NULL))
			__leave;

		if(strncmp(fmtblock.szFmtID, "fmt ", 4) || 
			(fmtblock.dwFmtSize != 16 && fmtblock.dwFmtSize != 18) ||
			(fmtblock.wavFormat.wFormatTag != WAVE_FORMAT_PCM))
		{
			nRet = -2;
			__leave;
		}else if(wave_fmt)
			*wave_fmt = fmtblock.wavFormat;

		if(fmtblock.dwFmtSize == 18)
		{
			WORD wExternSize = 0;
			ReadFile(*hFile, &wExternSize, 2, &dwRead, NULL);
			SetFilePointer(*hFile, wExternSize, 0, FILE_CURRENT);
		}

		// 寻找"data" 块
		char tcsData[20];
		char* p_data = NULL;
		int nTry = 5000;
		
		while(nTry--)
		{
			if(nRet = readAudioData(*hFile, tcsData, 4, &dwRead, NULL))
				__leave;

			for(int iData = 0; iData < 4; iData++)
				if(tcsData[iData] == NULL) tcsData[iData] = '1';

			if(!strncmp(tcsData, "data", 4)){
				break;
			}else if(NULL != (p_data = strstr(tcsData, "d")) && 3 == p_data-tcsData){
				SetFilePointer(*hFile, -1, 0, FILE_CURRENT);
				continue;
			}else if(NULL != (p_data = strstr(tcsData, "da")) && 2 == p_data-tcsData){
				SetFilePointer(*hFile, -2, 0, FILE_CURRENT);
				continue;
			}else if(NULL != (p_data = strstr(tcsData, "dat")) && 1 == p_data-tcsData){
				SetFilePointer(*hFile, -3, 0, FILE_CURRENT);
				continue;
			}
		}

		if(dwDataSize && nTry > 0){
			if(nRet = readAudioData(*hFile, dwDataSize, 4, &dwRead, NULL))
				__leave;
		}

#if 0
		// I want to ignore below logic. It can't work fine.
		if(fmtblock.wavFormat.dwSamplesPerSec != g_AsioInfo.sampleRate)
		{
			ASIOSampleRate dummy64;
			memset(&dummy64, 0, sizeof(dummy64));
			nRet = g_pASIODrv->setSampleRate(fmtblock.wavFormat.dwSamplesPerSec);

			g_pASIODrv->getSampleRate((unsigned int*)&g_AsioInfo.sampleRate, &dummy64);
			
			if(fmtblock.wavFormat.dwSamplesPerSec != g_AsioInfo.sampleRate)
			{
				nRet = -3;
				__leave;
			}
		}
#endif

	}
	__finally
	{
		if(nRet && *hFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(*hFile);
			*hFile = NULL;
		}
	}

	return nRet;
}

// 这两个对外输出
int BeginRecord(char* filePath)
{
	return recordWaveFile(filePath, &g_hWaveFileRecord);
}

int EndRecord()
{
	if(g_hWaveFileRecord)
	{
		int nRet = recordWaveFileEnd(g_hWaveFileRecord);
		g_hWaveFileRecord = NULL;
		return nRet;
	}
	
	return 0;
}

// 0：正确
int recordWaveFile(char* filePath, HANDLE* hFile, WAVE_FORMAT* wave_fmt)
{
	assert(filePath != NULL);
	assert(hFile != NULL);

	// assert can't do everything.
	if(0 == filePath ||
		0 == hFile)
	{
		return -1;
	}

	*hFile = CreateFile(
		filePath, 
		GENERIC_ALL,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		0, NULL);

	if(*hFile == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}

	// 写RIFF头(12 Bytes)
	DWORD dwWrite = 0;
	RIFF_HEADER riffhdr;
	strncpy(riffhdr.szRiffID, "RIFF", 4);
	strncpy(riffhdr.szRiffFormat, "WAVE", 4);
	riffhdr.dwRiffSize = 0; // 在wave_file_finish中填写

	if(!WriteFile(*hFile, &riffhdr, sizeof(riffhdr), &dwWrite, NULL))
	{
		return GetLastError();
	}

	// 写格式块(24 Bytes)
	FMT_BLOCK fmtblock;
	strncpy(fmtblock.szFmtID, "fmt ", 4);
	fmtblock.dwFmtSize = 16;

	if(wave_fmt)
	{
		fmtblock.wavFormat = *wave_fmt;
	}
	else
	{
		fmtblock.wavFormat.wFormatTag = WAVE_FORMAT_PCM;
		fmtblock.wavFormat.wChannels = 1;
		fmtblock.wavFormat.wBlockAlign = 8;		// 全声道(4*2)
		fmtblock.wavFormat.wBitsPerSample = 32; // 采样的深度
		fmtblock.wavFormat.dwSamplesPerSec = 44100; // 采样频率
		fmtblock.wavFormat.dwAvgBytesPerSec = 44100*4; // 单声道Only
	}

	if(!WriteFile(*hFile, &fmtblock, sizeof(fmtblock), &dwWrite, NULL))
	{
		return GetLastError();
	}

	// 写"data" 块(8 Bytes)
	DATA_BLOCK datablock;
	datablock.dwDataSize = 0; // 在wave_file_finish中填写
	strncpy(datablock.szDataID, "data", 4);

	if(!WriteFile(*hFile, &datablock, sizeof(datablock), &dwWrite, NULL))
	{
		return GetLastError();
	}

	return 0;
}

int recordWaveFileEnd(HANDLE hFile)
{
	assert(hFile);
	int nRetValue = 0;
	g_AsioInfo.bAudioRecord = FALSE;

	__try{

		DWORD dwWrite = 0;
		DWORD dwFileSizeHigh = 0;
		DWORD dwFileSizeLow = GetFileSize(hFile, &dwFileSizeHigh);

		if(dwFileSizeLow == INVALID_FILE_SIZE)
		{
			nRetValue = -1;
			__leave;
		}

		// 写RIFF的Size域
		SetFilePointer(hFile, 4, 0, FILE_BEGIN);
		dwFileSizeLow -= 8;

		if(!WriteFile(hFile, (void*)&dwFileSizeLow, 4, &dwWrite, NULL))
		{
			nRetValue = GetLastError();
			__leave;
		}

		// 写Data块的Size域
		SetFilePointer(hFile, 40, 0, FILE_BEGIN);
		dwFileSizeLow -= 36;
		
		if(!WriteFile(hFile, (void*)&dwFileSizeLow, 4, &dwWrite, NULL))
		{
			nRetValue = GetLastError();
			__leave;
		}
	}
	__finally{
		CloseHandle(hFile); // 关闭文件句柄
	}

	return nRetValue;
}

bool writeWaveFile(void* bufPnt, unsigned int bufferSize)
{
	if(bufPnt == NULL)
		return false;

	bool bRet = false;
	DWORD dwWrite = 0;
	
	if(WriteFile(g_hWaveFileRecord, bufPnt, bufferSize*4, &dwWrite, NULL))
		bRet = true;

	g_recordFileSize += dwWrite;
	return bRet;
}

int readWaveFile(unsigned char * channel0, unsigned char * channel1, unsigned int nSamples, int type)
{
	unsigned int i, j;
	DWORD len, r_len;

	if(nSamples*g_fmtBlock.wChannels*g_fmtBlock.wBitsPerSample/8 > g_dataBufSize)
	{
		return EOF;
	}

	int nClusterSize = g_fmtBlock.wChannels*g_fmtBlock.wBitsPerSample/8;
	r_len = nSamples*nClusterSize;
	
	if(!ReadFile(g_hWaveFile, g_pDataBuf, r_len, &len, NULL) || 
		len == 0)
	{
		return EOF;
	}

	if(len<r_len)
	{
		memset(g_pDataBuf+len, 0, r_len-len);
	}

	if(g_fmtBlock.wBitsPerSample == 8)
	{
		for(i=0, j=0; i < r_len; )
		{
			switch(type)
			{
			case ASIOSTInt32LSB:
				//这里有变幻莫测的组合，TODO.吧
				channel0[j+0] = g_pDataBuf[i+0];
				channel0[j+1] = g_pDataBuf[i+0];
				channel0[j+2] = g_pDataBuf[i+0];
				channel0[j+3] = g_pDataBuf[i+0];

				if(g_fmtBlock.wChannels == 2)
				{
					channel1[j+0] = g_pDataBuf[i+1];
					channel1[j+1] = g_pDataBuf[i+1];
					channel1[j+2] = g_pDataBuf[i+1];
					channel1[j+3] = g_pDataBuf[i+1];
				}
				else
				{
					channel1[j+0] = g_pDataBuf[i+0];
					channel1[j+1] = g_pDataBuf[i+0];
					channel1[j+2] = g_pDataBuf[i+0];
					channel1[j+3] = g_pDataBuf[i+0];
				}

				i += nClusterSize;
				j += 4;
				break;

			case ASIOSTInt24MSB:
				channel0[j+0] = 0;
				channel0[j+1] = g_pDataBuf[i+0];
				channel0[j+2] = g_pDataBuf[i+0];

				if(g_fmtBlock.wChannels == 2)
				{
					channel1[j+0] = 0;
					channel1[j+1] = g_pDataBuf[i+1];
					channel1[j+2] = g_pDataBuf[i+1];
				}
				else
				{
					channel1[j+0] = 0;
					channel1[j+1] = g_pDataBuf[i+0];
					channel1[j+2] = g_pDataBuf[i+0];
				}

				i += nClusterSize;
				j += 3;
				break;

			default:
				break;
			}
		}
	}
	else if(g_fmtBlock.wBitsPerSample == 16)
	{
		for(i=0, j=0; i < r_len; )
		{
			switch(type)
			{
			case ASIOSTInt32LSB:
				//这里有变幻莫测的组合，TODO.吧
				channel0[j+0] = g_pDataBuf[i+0];
				channel0[j+1] = g_pDataBuf[i+1];
				channel0[j+2] = g_pDataBuf[i+0];
				channel0[j+3] = g_pDataBuf[i+1];

				if(g_fmtBlock.wChannels == 2)
				{
					channel1[j+0] = g_pDataBuf[i+2];
					channel1[j+1] = g_pDataBuf[i+3];
					channel1[j+2] = g_pDataBuf[i+2];
					channel1[j+3] = g_pDataBuf[i+3];
				}
				else
				{
					channel1[j+0] = g_pDataBuf[i+0];
					channel1[j+1] = g_pDataBuf[i+1];
					channel1[j+2] = g_pDataBuf[i+0];
					channel1[j+3] = g_pDataBuf[i+1];
				}

				i += nClusterSize;
				j += 4;
				break;

			case ASIOSTInt24MSB:
				channel0[j+0] = 0;
				channel0[j+1] = g_pDataBuf[i+0];
				channel0[j+2] = g_pDataBuf[i+1];

				if(g_fmtBlock.wChannels == 2)
				{
					channel1[j+0] = 0;
					channel1[j+1] = g_pDataBuf[i+2];
					channel1[j+2] = g_pDataBuf[i+3];
				}
				else
				{
					channel0[j+0] = 0;
					channel0[j+1] = g_pDataBuf[i+0];
					channel0[j+2] = g_pDataBuf[i+1];
				}

				i += nClusterSize;
				j += 3;
				break;

			default:
				break;
			}
		}

		return 0;
	}
	else if(g_fmtBlock.wBitsPerSample == 24)
	{
		for(i=0, j=0; i < r_len; )//g_fmtBlock.wChannels*g_fmtBlock.wBitsPerSample/8)
		{
			switch(type)
			{
			case ASIOSTInt32LSB:
				//这里有变幻莫测的组合，TODO.吧
				channel0[j+0] = 0;
				channel0[j+1] = g_pDataBuf[i+0];
				channel0[j+2] = g_pDataBuf[i+1];
				channel0[j+3] = g_pDataBuf[i+2];

				if(g_fmtBlock.wChannels == 2){
					channel1[j+0] = 0;
					channel1[j+1] = g_pDataBuf[i+3];
					channel1[j+2] = g_pDataBuf[i+4];
					channel1[j+3] = g_pDataBuf[i+5];
				}else{
					channel1[j+0] = 0;
					channel1[j+1] = g_pDataBuf[i+0];
					channel1[j+2] = g_pDataBuf[i+1];
					channel1[j+3] = g_pDataBuf[i+2];
				}

				i += nClusterSize;
				j += 4;
				break;

			case ASIOSTInt24MSB:
				channel0[j+0] = g_pDataBuf[i+0];
				channel0[j+1] = g_pDataBuf[i+1];
				channel0[j+2] = g_pDataBuf[i+2];

				if(g_fmtBlock.wChannels == 2){
					channel1[j+0] = g_pDataBuf[i+3];
					channel1[j+1] = g_pDataBuf[i+4];
					channel1[j+2] = g_pDataBuf[i+5];
				}else
				{
					channel0[j+0] = g_pDataBuf[i+0];
					channel0[j+1] = g_pDataBuf[i+1];
					channel0[j+2] = g_pDataBuf[i+2];
				}

				i += nClusterSize;
				j += 3;
				break;

			default:
				break;
			}
		}

		return 0;
	}
	else if(g_fmtBlock.wBitsPerSample == 32)
	{
		for(i=0, j=0; i < r_len; )//g_fmtBlock.wChannels*g_fmtBlock.wBitsPerSample/8)
		{
			switch(type)
			{
			case ASIOSTInt32LSB:
				//这里有变幻莫测的组合，TODO.吧
				channel0[j+0] = g_pDataBuf[i+0];
				channel0[j+1] = g_pDataBuf[i+1];
				channel0[j+2] = g_pDataBuf[i+2];
				channel0[j+3] = g_pDataBuf[i+3];

				if(g_fmtBlock.wChannels == 2){
					channel1[j+0] = g_pDataBuf[i+4];
					channel1[j+1] = g_pDataBuf[i+5];
					channel1[j+2] = g_pDataBuf[i+6];
					channel1[j+3] = g_pDataBuf[i+7];
				}else{
					channel1[j+0] = g_pDataBuf[i+0];
					channel1[j+1] = g_pDataBuf[i+1];
					channel1[j+2] = g_pDataBuf[i+2];
					channel1[j+3] = g_pDataBuf[i+3];
				}

				i += nClusterSize;
				j += 4;
				break;

			case ASIOSTInt24LSB:
				channel0[j+0] = g_pDataBuf[i+1];
				channel0[j+1] = g_pDataBuf[i+2];
				channel0[j+2] = g_pDataBuf[i+3];

				if(g_fmtBlock.wChannels == 2){
					channel1[j+0] = g_pDataBuf[i+5];
					channel1[j+1] = g_pDataBuf[i+6];
					channel1[j+2] = g_pDataBuf[i+7];
				}else
				{
					channel1[j+0] = g_pDataBuf[i+1];
					channel1[j+1] = g_pDataBuf[i+2];
					channel1[j+2] = g_pDataBuf[i+3];
				}

				i += nClusterSize;
				j += 3;
				break;

			default:
				break;
			}
		}

		return 0;
	}

	return 1;
}
