#pragma once


// C3600Splash

class C3600Splash : public CWnd
{
	DECLARE_DYNAMIC(C3600Splash)

public:
	C3600Splash();
	virtual ~C3600Splash();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CBitmap m_bitmap;
	void Create(UINT nBitmapID);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


