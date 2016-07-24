// item.h  header file
/////////////////////////////////////
#ifndef __LINEDATA_H__
#define __LINEDATA_H__

#include "afxtempl.h"
#include <afxcoll.h>
#include <mmsystem.h>

#define LINE_BUFF_NUM		10

class  CLineData:public CObject
{
// Constructors 
protected:
	DECLARE_DYNAMIC(CLineData);
	CLineData();
public:
	char	 lineData[24];
};
/////////////////////////////////////////////////
typedef CTypedPtrList<CObList, CLineData*> CLineDataList;
/////////////////////////////////////////////////

class COutWaveHdr:public CObject
{
// Constructors 
protected:
	DECLARE_DYNAMIC(COutWaveHdr);
	COutWaveHdr();
public:
	LPWAVEHDR	lpWaveHdr;
};

typedef CTypedPtrList<CObList, COutWaveHdr*> COutWaveHdrList;

#endif // __LINEDATA_H__