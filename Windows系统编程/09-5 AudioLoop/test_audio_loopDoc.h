// test_audio_loopDoc.h : interface of the CTest_audio_loopDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEST_AUDIO_LOOPDOC_H__7F10950B_FC88_11D1_8334_00104B7B317F__INCLUDED_)
#define AFX_TEST_AUDIO_LOOPDOC_H__7F10950B_FC88_11D1_8334_00104B7B317F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CTest_audio_loopDoc : public CDocument
{
protected: // create from serialization only
	CTest_audio_loopDoc();
	DECLARE_DYNCREATE(CTest_audio_loopDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTest_audio_loopDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTest_audio_loopDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTest_audio_loopDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST_AUDIO_LOOPDOC_H__7F10950B_FC88_11D1_8334_00104B7B317F__INCLUDED_)
