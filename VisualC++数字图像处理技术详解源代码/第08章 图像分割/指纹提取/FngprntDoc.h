// FngprntDoc.h : CFngprntDoc 类的接口
//


#pragma once
#include "Dib.h"

class CFngprntDoc : public CDocument
{
protected: // 仅从序列化创建
	CFngprntDoc();
	DECLARE_DYNCREATE(CFngprntDoc)

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CFngprntDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
private:
    CDib dib;
public:
    CDib* GetDib(void);

public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};



