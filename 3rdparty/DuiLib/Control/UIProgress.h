#ifndef __UIPROGRESS_H__
#define __UIPROGRESS_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CProgressUI : public CLabelUI
	{
	public:
		CProgressUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		bool IsHorizontal();
		void SetHorizontal(bool bHorizontal = true);
		bool IsStretchForeImage();
		void SetStretchForeImage(bool bStretchForeImage = true);
		int GetMinValue() const;
		void SetMinValue(int nMin);
		int GetMaxValue() const;
		void SetMaxValue(int nMax);
		int GetValue() const;
		void SetValue(int nValue);
		LPCTSTR GetForeImage() const;
		void SetForeImage(LPCTSTR pStrImage);
		void SetForeColor(DWORD dwForeColor);
		void SetForeColor2(DWORD dwForeColor2);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void PaintStatusImage(HDC hDC);
		void SetStepValue(int nStep);
	protected:
		bool m_bHorizontal;
		bool m_bStretchForeImage;
		int m_nMax;
		int m_nMin;
		int m_nValue;
		int m_nStep;
		DWORD   m_dwForeColor;
		DWORD   m_dwForeColor2;
		CDuiString m_sForeImage;
		CDuiString m_sForeImageModify;
	};

} // namespace DuiLib

#endif // __UIPROGRESS_H__
