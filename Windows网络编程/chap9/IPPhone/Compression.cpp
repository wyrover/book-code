// Compression.cpp: implementation of the CCompression class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Phone.h"
#include "Compression.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma comment(lib,"msacm32")

BOOL CALLBACK FindDriverEnum(HACMDRIVERID hadid, DWORD dwInstance,
							 DWORD fdwSupport);
BOOL CALLBACK FindFormatEnum(HACMDRIVERID hadid, LPACMFORMATDETAILS pafd, 
							 DWORD dwInstance, DWORD fdwSupport);
BOOL CALLBACK FindFormatEnumOld(HACMDRIVERID hadid, LPACMFORMATDETAILS pafd, 
							 DWORD dwInstance, DWORD fdwSupport);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCompression::CCompression()
{
	pDst1Data=NULL;
	pDst2Data=NULL;
	m_pSrcData=NULL;
}

CCompression::~CCompression()
{
	if(pDst1Data!=NULL)
		free(pDst1Data);
	if(pDst2Data!=NULL)
		free(pDst2Data);
	if(m_pSrcData!=NULL)
		free(m_pSrcData);
	if(pwfDrv!=NULL)
		free(pwfDrv);
	if(pwfPCM!=NULL)
		free(pwfPCM);
}

void CCompression::SetWaveFormat(int channels, long samplerate, int bitspersample)
{
	memset(&m_wfSrc, 0, sizeof(m_wfSrc));
	m_wfSrc.cbSize = 0;
	m_wfSrc.wFormatTag = WAVE_FORMAT_PCM; // pcm
	m_wfSrc.nChannels = channels; // mono
	m_wfSrc.nSamplesPerSec = samplerate; // 11.025 kHz
	m_wfSrc.wBitsPerSample = bitspersample; // 8 bit
	m_wfSrc.nBlockAlign = m_wfSrc.nChannels *m_wfSrc.wBitsPerSample / 8;
	m_wfSrc.nAvgBytesPerSec = m_wfSrc.nSamplesPerSec * m_wfSrc.nBlockAlign;
}

void CCompression::SetSrcSamples(long samples,unsigned char* pSrcData)
{
	int n=(int)(m_wfSrc.wBitsPerSample/8);

	m_dwSrcSamples=samples;
	m_dwSrcBytes=(DWORD)(samples*n);
	m_pSrcData=(BYTE*)malloc(m_dwSrcBytes);
	memcpy(m_pSrcData,pSrcData,m_dwSrcBytes);
}

void CCompression::SelectMethod(int i)
{
	if((i<0) || (i>5)){
		AfxMessageBox("No support for such compression method!");
		return;
	}
	switch(i){
	case 0:{
		wFormatTag = 0x32; // MSN
		break;
		   }
	case 1:{
		wFormatTag = WAVE_FORMAT_DSPGROUP_TRUESPEECH;
		break;}
	case 2:{
		wFormatTag = WAVE_FORMAT_ADPCM;
		break;}
	case 3:{
		wFormatTag = WAVE_FORMAT_GSM610;
		break;}
	case 4:{
		wFormatTag = WAVE_FORMAT_ALAW;
		break;}
	case 5:{
		wFormatTag = WAVE_FORMAT_MULAW;
		break;}
	}
}

HACMDRIVERID CCompression::FindDriver(WORD wFormatTag)
{
	FIND_DRIVER_INFO fdi;
	fdi.hadid = NULL;
	fdi.wFormatTag = wFormatTag;
	MMRESULT mmr = acmDriverEnum(FindDriverEnum, (DWORD)(VOID*)&fdi,0); 
	if (mmr) 
		return NULL;
	return fdi.hadid;
}

BOOL CALLBACK FindDriverEnum(HACMDRIVERID hadid, DWORD dwInstance,
							 DWORD fdwSupport)
{
	FIND_DRIVER_INFO* pdi = (FIND_DRIVER_INFO*) dwInstance;

	// open the driver
	HACMDRIVER had = NULL;
	MMRESULT mmr = acmDriverOpen(&had, hadid, 0);
	if (mmr) {
		// some error
		return FALSE; // stop enumerating
	}
	// enumerate the formats it supports
	DWORD dwSize = 0;
	mmr = acmMetrics((HACMOBJ)had, ACM_METRIC_MAX_SIZE_FORMAT, &dwSize);
	if (dwSize < sizeof(WAVEFORMATEX)) dwSize = sizeof(WAVEFORMATEX); // for MS-PCM
	WAVEFORMATEX* pwf = (WAVEFORMATEX*) malloc(dwSize);
	memset(pwf, 0, dwSize);
	pwf->cbSize = LOWORD(dwSize) - sizeof(WAVEFORMATEX);
	pwf->wFormatTag = pdi->wFormatTag;
	ACMFORMATDETAILS fd;
	memset(&fd, 0, sizeof(fd));
	fd.cbStruct = sizeof(fd);
	fd.pwfx = pwf;
	fd.cbwfx = dwSize;
	fd.dwFormatTag = pdi->wFormatTag;
	mmr = acmFormatEnum(had, &fd, FindFormatEnumOld, (DWORD)(VOID*)pdi, 0);  
	free(pwf);
	acmDriverClose(had, 0);
	if (pdi->hadid || mmr) {
		// found it or some error
		return FALSE; // stop enumerating
	}
	return TRUE; // continue enumeration
}

BOOL CALLBACK FindFormatEnum(HACMDRIVERID hadid, LPACMFORMATDETAILS pafd, DWORD dwInstance, DWORD fdwSupport)
{
	FIND_DRIVER_INFO* pdi = (FIND_DRIVER_INFO*) dwInstance;
	if (pafd->dwFormatTag == (DWORD)pdi->wFormatTag) {
		// found it
		WAVEFORMATEX pwfPCM;
		pwfPCM=*(pafd->pwfx);
		pdi->hadid = hadid;
		TRACE("PCM format: %u bits, %lu samples per second\n", pwfPCM.wBitsPerSample, pwfPCM.nSamplesPerSec);
		if((pwfPCM.nSamplesPerSec==(unsigned long)pdi->lSampleRate) && (pwfPCM.wBitsPerSample==(unsigned short)pdi->nBits)){
			pdi->hadid = hadid;
			return FALSE; // stop enumerating
		}
	}

	return TRUE; // continue enumerating
}

BOOL CALLBACK FindFormatEnumOld(HACMDRIVERID hadid, LPACMFORMATDETAILS pafd, DWORD dwInstance, DWORD fdwSupport)
{
	FIND_DRIVER_INFO* pdi = (FIND_DRIVER_INFO*) dwInstance;
	if (pafd->dwFormatTag == (DWORD)pdi->wFormatTag) {
		// found it
		pdi->hadid = hadid;
		return FALSE; // stop enumerating
	}

	return TRUE; // continue enumerating
}

WAVEFORMATEX* CCompression::GetDriverFormat(HACMDRIVERID hadid, WORD wFormatTag,BOOL bPCM)
{
	// open the driver
	HACMDRIVER had = NULL;
	MMRESULT mmr = acmDriverOpen(&had, hadid, 0);
	if (mmr) {
		return NULL;
	}

	// allocate a structure for the info
	DWORD dwSize = 0;
	mmr =acmMetrics((HACMOBJ)had, ACM_METRIC_MAX_SIZE_FORMAT, &dwSize);
	if (dwSize < sizeof(WAVEFORMATEX)) dwSize = sizeof(WAVEFORMATEX); // for MS-PCM
	WAVEFORMATEX* pwf = (WAVEFORMATEX*) malloc(dwSize);
	memset(pwf, 0, dwSize);
	pwf->cbSize = LOWORD(dwSize) - sizeof(WAVEFORMATEX);
	pwf->wFormatTag = wFormatTag;

	ACMFORMATDETAILS fd;
	memset(&fd, 0, sizeof(fd));
	fd.cbStruct = sizeof(fd);
	fd.pwfx = pwf;
	fd.cbwfx = dwSize;
	fd.dwFormatTag = wFormatTag;

	// set up a struct to control the enumeration
	FIND_DRIVER_INFO fdi;
	fdi.hadid = NULL;
	fdi.wFormatTag = wFormatTag;
	fdi.nBits=(unsigned short)m_wfSrc.wBitsPerSample;
	fdi.lSampleRate=8000;//(unsigned long)m_wfSrc.nSamplesPerSec;
	
	if(!bPCM)
		mmr=acmFormatEnum(had, &fd, FindFormatEnumOld, (DWORD)(VOID*)&fdi,0);
	else
		mmr = acmFormatEnum(had, &fd, FindFormatEnum, (DWORD)(VOID*)&fdi,0);  
	
	acmDriverClose(had, 0);
	if ((fdi.hadid == NULL) || mmr) {
		free(pwf);
		return NULL;
	}
	
	return pwf;
}

BOOL CCompression::Initialize()
{
	hadid = FindDriver(wFormatTag);
	if (hadid == NULL) {
		AfxMessageBox("No driver found\n");
		return FALSE;
	}
	TRACE("Driver found (hadid: %4.4lXH)\n", hadid);
	// show some information about the driver
	ACMDRIVERDETAILS dd;
	dd.cbStruct = sizeof(dd);
	MMRESULT mmr = acmDriverDetails(hadid, &dd, 0);
	TRACE("   Short name: %s\n", dd.szShortName);
	TRACE("   Long name:  %s\n", dd.szLongName);
	TRACE("   Copyright:  %s\n", dd.szCopyright);
	TRACE("   Licensing:  %s\n", dd.szLicensing);
	TRACE("   Features:   %s\n", dd.szFeatures);

	// get the details of the format
	// Note: this is just the first of one or more possible formats for the given tag
	pwfDrv = GetDriverFormat(hadid, wFormatTag,FALSE);
	if (pwfDrv == NULL) {
		AfxMessageBox("Error getting format info\n");
		return FALSE;
	}
	TRACE("Driver format: %u bits, %lu samples per second\n", pwfDrv->wBitsPerSample, pwfDrv->nSamplesPerSec);

	// get a PCM format tag the driver supports
	// Note: we just pick the first supported PCM format which might not really
	// be the best choice.
	pwfPCM = GetDriverFormat(hadid, WAVE_FORMAT_PCM,TRUE);
	if (pwfPCM == NULL) {
		AfxMessageBox("Error getting PCM format info\n");
		return FALSE;
	}
	TRACE("PCM format: %u bits, %lu samples per second\n", pwfPCM->wBitsPerSample, pwfPCM->nSamplesPerSec);
	return TRUE;
}

long CCompression::MyConvert(BOOL bQuery,unsigned char *pDetData, long detLength)
{
	if(bQuery){
		MMRESULT mmr;
		HACMSTREAM hstr = NULL;
		mmr = acmStreamOpen(&hstr,
						NULL, // any driver
						&m_wfSrc, // source format
						pwfPCM, // destination format
						NULL, // no filter
						NULL, // no callback
						0, // instance data (not used)
						ACM_STREAMOPENF_NONREALTIME); // flags
		if (mmr) {
			AfxMessageBox("Failed to open a stream to do PCM to PCM conversion\n");
			return 0;
		}
		// fill in the conversion info
		ACMSTREAMHEADER strhdr;
		memset(&strhdr, 0, sizeof(strhdr));
		strhdr.cbStruct = sizeof(strhdr);
		strhdr.pbSrc = m_pSrcData; // the source data to convert
		strhdr.cbSrcLength = m_dwSrcBytes;
		strhdr.pbDst = pDst1Data;
		strhdr.cbDstLength = dwDst1Bytes;

		// prep the header
		mmr = acmStreamPrepareHeader(hstr, &strhdr, 0); 
	
		// convert the data
		TRACE("Converting to intermediate PCM format...\n");
		mmr = acmStreamConvert(hstr, &strhdr, 0);
		if (mmr) {
			AfxMessageBox("Failed to do PCM to PCM conversion\n");
			return 0;
		}
		TRACE("Converted OK\n");
	
		// close the stream
		acmStreamClose(hstr, 0);

		///////////////////////////////////////////////////////////////////////////////////
		// convert the intermediate PCM format to the final format
		// open the driver
		HACMDRIVER had = NULL;
		mmr = acmDriverOpen(&had, hadid, 0);
		if (mmr) {
			AfxMessageBox("Failed to open driver\n");
			return 0;
		}

		// open the conversion stream
		// Note the use of the ACM_STREAMOPENF_NONREALTIME flag. Without this
		// some software compressors will report error 512 - not possible
		mmr = acmStreamOpen(&hstr,
						had, // driver handle
						pwfPCM, // source format
						pwfDrv, // destination format
						NULL, // no filter
						NULL, // no callback
						0, // instance data (not used)
						ACM_STREAMOPENF_NONREALTIME ); // flags
		if (mmr) {
			AfxMessageBox("Failed to open a stream to do PCM to driver format conversion\n");
			return 0;
		}

		// fill in the conversion info
		ACMSTREAMHEADER strhdr2;
		memset(&strhdr2, 0, sizeof(strhdr2));
		strhdr2.cbStruct = sizeof(strhdr2);
		strhdr2.pbSrc = pDst1Data; // the source data to convert
		strhdr2.cbSrcLength = dwDst1Bytes;
		strhdr2.pbDst = pDst2Data;
		strhdr2.cbDstLength = dwDst2Bytes;

		// prep the header
		mmr = acmStreamPrepareHeader(hstr, &strhdr2, 0); 

		// convert the data
		TRACE("Converting to final format...\n");
		mmr = acmStreamConvert(hstr, &strhdr2, 0);
		if (mmr) {
			AfxMessageBox("Failed to do PCM to driver format conversion\n");
			return 0;
		}
		TRACE("Converted OK\n");

		// close the stream and driver
		mmr = acmStreamClose(hstr, 0);
		mmr = acmDriverClose(had, 0);

		// show the conversion stats
		TRACE("Source wave had %lu bytes\n", m_dwSrcBytes);
		TRACE("Converted wave has %lu bytes\n", strhdr2.cbDstLengthUsed);
		TRACE("Compression ratio is %f\n", (double)m_dwSrcBytes / (double) strhdr2.cbDstLengthUsed); 
		length=strhdr2.cbDstLengthUsed;
		
		return length;
	}
	else{
		if(detLength<length){
			AfxMessageBox("Error using the function!");
			return 0;
		}
		memcpy(pDetData,pDst2Data,length);
		return length;
	}
}


BOOL CCompression::ConvertSend(SOCKET s, SOCKADDR_IN addr)
{
	MyConvert(TRUE,NULL,0);
	int iError=sendto(s,(char *)pDst2Data,length,0,(LPSOCKADDR)&addr,sizeof(addr));
	if(iError==SOCKET_ERROR)
		return FALSE;
	return TRUE;
}

void CCompression::SetDstSamples(long bytes, unsigned char *pDstData)
{
	dwDst2Bytes=bytes;
	pDst2Data=(BYTE*)malloc(dwDst2Bytes);
	memcpy(pDst2Data,pDstData,dwDst2Bytes);
}

BOOL CCompression::PrepareSpace(BOOL bCompression)
{
	if(bCompression){
		dwDst1Samples = m_dwSrcSamples * pwfPCM->nSamplesPerSec / m_wfSrc.nSamplesPerSec;
		dwDst1Bytes = dwDst1Samples * pwfPCM->wBitsPerSample / 8;
 		pDst1Data =(BYTE*)malloc(dwDst1Bytes);
	
#ifdef _DEBUG
		memset(pDst1Data, 0, dwDst1Bytes);
#endif
		dwDst2Bytes = pwfDrv->nAvgBytesPerSec * dwDst1Samples / pwfPCM->nSamplesPerSec;
		dwDst2Bytes = dwDst2Bytes * 3 / 2; // add a little room
		pDst2Data =(BYTE*)malloc(dwDst2Bytes);
#ifdef _DEBUG
		memset(pDst2Data, 0, dwDst2Bytes);
#endif
	}
	else{
		dwDst1Samples=pwfPCM->nSamplesPerSec*dwDst2Bytes/pwfDrv->nAvgBytesPerSec;
		dwDst1Bytes = dwDst1Samples* 3 / 2; // add a little room
		pDst1Data = (BYTE*)malloc(dwDst1Bytes);
#ifdef _DEBUG
		memset(pDst1Data, 0, dwDst1Bytes);
#endif
		m_dwSrcSamples=dwDst1Samples*m_wfSrc.nSamplesPerSec/pwfPCM->nSamplesPerSec;
		m_dwSrcBytes=m_dwSrcSamples*m_wfSrc.wBitsPerSample / 8;
		m_pSrcData = (BYTE*)malloc(m_dwSrcBytes);
	
#ifdef _DEBUG
		memset(m_pSrcData, 0, m_dwSrcBytes);
#endif
	}
	return TRUE;
}

long CCompression::UnConvert(BOOL bQuery, unsigned char *pSrcData, long bytes)
{
	if(bQuery){
		MMRESULT mmr;
		HACMSTREAM hstr = NULL;

		HACMDRIVER had = NULL;
		mmr = acmDriverOpen(&had, hadid, 0);
		if (mmr) {
			AfxMessageBox("Failed to open driver\n");
			return 0;
		}
		// open the conversion stream
		// Note the use of the ACM_STREAMOPENF_NONREALTIME flag. Without this
		// some software compressors will report error 512 - not possible
		mmr = acmStreamOpen(&hstr,
						had, // driver handle
						pwfDrv, // source format
						pwfPCM, // destination format
						NULL, // no filter
						NULL, // no callback
						0, // instance data (not used)
						ACM_STREAMOPENF_NONREALTIME ); // flags
		if (mmr) {
			AfxMessageBox("Failed to open a stream to do PCM to driver format conversion\n");
			return 0;
		}
		// fill in the conversion info
		ACMSTREAMHEADER strhdr2;
		memset(&strhdr2, 0, sizeof(strhdr2));
		strhdr2.cbStruct = sizeof(strhdr2);
		strhdr2.pbSrc = pDst2Data; // the source data to convert
		strhdr2.cbSrcLength = dwDst2Bytes;
		strhdr2.pbDst = pDst1Data;
		strhdr2.cbDstLength = dwDst1Bytes;

		// prep the header
		mmr = acmStreamPrepareHeader(hstr, &strhdr2, 0); 

		// convert the data
		TRACE("Converting to final format...\n");
		mmr = acmStreamConvert(hstr, &strhdr2, 0);
		if (mmr) {
			AfxMessageBox("Failed to do PCM to driver format conversion\n");
			return 0;
		}
		TRACE("Converted OK\n");
		// close the stream and driver
		mmr = acmStreamClose(hstr, 0);
		mmr = acmDriverClose(had, 0);
		
		DWORD tmpBytes=strhdr2.cbDstLengthUsed;
		// show the conversion stats
		TRACE("Source wave had %lu bytes\n", m_dwSrcBytes);
		TRACE("Converted wave has %lu bytes\n", strhdr2.cbDstLengthUsed);
		TRACE("Compression ratio is %f\n", (double)m_dwSrcBytes / (double) strhdr2.cbDstLengthUsed); 

		mmr = acmStreamOpen(&hstr,
						NULL, // any driver
						pwfPCM, // source format
						&m_wfSrc, // destination format
						NULL, // no filter
						NULL, // no callback
						0, // instance data (not used)
						ACM_STREAMOPENF_NONREALTIME ); // flags
		if (mmr) {
			AfxMessageBox("Failed to open a stream to do PCM to PCM conversion\n");
			return 0;
		}
		// fill in the conversion info
		ACMSTREAMHEADER strhdr;
		memset(&strhdr, 0, sizeof(strhdr));
		strhdr.cbStruct = sizeof(strhdr);
		strhdr.pbSrc = pDst1Data; // the source data to convert
		strhdr.cbSrcLength = tmpBytes;
		strhdr.pbDst = m_pSrcData;
		strhdr.cbDstLength = m_dwSrcBytes;
		// prep the header
		mmr = acmStreamPrepareHeader(hstr, &strhdr, 0); 
		// convert the data
		TRACE("Converting to intermediate PCM format...\n");
		mmr = acmStreamConvert(hstr, &strhdr, 0);
		if (mmr) {
			AfxMessageBox("Failed to do PCM to PCM conversion\n");
			return 0;
		}
		TRACE("Converted OK\n");
		// close the stream
		acmStreamClose(hstr, 0);

		///////////////////////////////////////////////////////////////////////////////////
		// convert the intermediate PCM format to the final format
		// open the driver
		unlength=strhdr.cbDstLengthUsed;
		
		return unlength;
	}
	else{
		if(bytes<unlength){
			memcpy(pSrcData,m_pSrcData,bytes);	
			return 0;
		}
		memcpy(pSrcData,m_pSrcData,unlength);
		return unlength;
	}
}
