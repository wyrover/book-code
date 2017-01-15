/*
 * Copyright 2010 JiJie Shi
 *
 * This file is part of PEMaster.
 *
 * PEMaster is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PEMaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PEMaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once


// CaddsectionDlg 对话框

class CaddsectionDlg : public CDialog
{
	DECLARE_DYNCREATE(CaddsectionDlg)

public:
	CaddsectionDlg(CWnd* pParent = NULL);   // 标准构造函数
	CaddsectionDlg(void *context, CWnd* pParent = NULL ); 
	virtual ~CaddsectionDlg();

// 对话框数据
	enum { IDD = IDD_DLG_ADD_SECTION };

protected:
	void *context; 

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
