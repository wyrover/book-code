// MailslotSrvDoc.h : interface of the CMailslotSrvDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAILSLOTSRVDOC_H__88FBA2EB_C7D6_4CAB_9CE6_8CAB9FDDE9FB__INCLUDED_)
#define AFX_MAILSLOTSRVDOC_H__88FBA2EB_C7D6_4CAB_9CE6_8CAB9FDDE9FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMailslotSrvDoc : public CDocument
{
protected: // create from serialization only
	CMailslotSrvDoc();
	DECLARE_DYNCREATE(CMailslotSrvDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMailslotSrvDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMailslotSrvDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMailslotSrvDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAILSLOTSRVDOC_H__88FBA2EB_C7D6_4CAB_9CE6_8CAB9FDDE9FB__INCLUDED_)
