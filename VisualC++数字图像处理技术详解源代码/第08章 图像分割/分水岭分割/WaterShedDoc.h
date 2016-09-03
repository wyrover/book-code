// WaterShedDoc.h : interface of the CWaterShedDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WATERSHEDDOC_H__68533D2C_2AC3_42C0_A159_CBDDD58AA973__INCLUDED_)
#define AFX_WATERSHEDDOC_H__68533D2C_2AC3_42C0_A159_CBDDD58AA973__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Imageload.h"
#include "ImageObject.h"
#include "MyMath.h"



typedef struct tagRgnInfo{
	BOOL  isflag;//备用标记；
	INT   ptcount;//该区所含的点数；
	FLOAT   l;//该区l均值
	FLOAT   u;//该区u均值
	FLOAT   v;//该区v均值
}MyRgnInfo;//分割后各个区的一些统计信息,图像中各点所属区域的信息存放在flag数组中；

typedef struct tagMyLUV{
	FLOAT l;
	FLOAT u;
	FLOAT v;
}MyLUV;	

class CWaterShedDoc : public CDocument
{
protected: // create from serialization only
	CWaterShedDoc();
	DECLARE_DYNCREATE(CWaterShedDoc)

// Attributes
public:
	CString imageName;
    BOOL isImageLoaded;
	BYTE* imageData;
	
	LONG dataLen;

	LONG  imageWidth;
	LONG  imageHeight;
	CImageObject* myImageObject;

	MyMath myMath;
	MyLUV* luvData;//
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaterShedDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL RgbtoLuvPcm(BYTE* inDatas, int width, int height, MyLUV* luvbuff);
	void MergeNearest(INT curid, MyRgnInfo* rginfoarr, CString* neiarr, INT* mergearr);
	void AddBNeiToANei(INT curid, INT nearid, CString* neiarr, INT* mergearr);
	void MergeTwoRgn(INT curid, INT nearid, CString* neiarr, MyRgnInfo* rginfoarr, INT* mergearr);
	int FindNearestNei(INT curid, CString neistr, MyRgnInfo* rginfoarr, INT* mergearr);
	int FindMergedRgn(INT idint, INT* mergearr);
	void AddNeiRgn(INT curid, INT neiid, CString* neiarr);
	void AddNeiOfCur(INT curid, INT left, INT right, INT up, INT down, INT* flag, CString* neiarr);
	BOOL Luv2Rgb(FLOAT l, FLOAT u, FLOAT v, INT& r, INT& g, INT& b);
	BOOL LuvToRgb(FLOAT* luvData, INT width, INT height, BYTE* rgbData);
	void MergeRgs(MyRgnInfo* rginfoarr, INT rgnumber, INT* flag, INT width, INT height, INT* outmerge, INT& rgnum);
	void FloodVincent(MyImageGraPt* imiarr, INT* graddarr, INT minh, INT maxh, INT* flagarr, INT& outrgnumber);
	void RefreshImageObject();
	void GetGradient(BYTE* image, INT width, INT height, FLOAT* deltar, FLOAT* deltasita);
	virtual ~CWaterShedDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWaterShedDoc)
	afx_msg void OnWatershed();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WATERSHEDDOC_H__68533D2C_2AC3_42C0_A159_CBDDD58AA973__INCLUDED_)
