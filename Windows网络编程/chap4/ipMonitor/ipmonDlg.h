// ipmonDlg.h : header file
//

#if !defined(AFX_IPMONDLG_H__47076949_0324_11D5_924F_00010219EDF4__INCLUDED_)
#define AFX_IPMONDLG_H__47076949_0324_11D5_924F_00010219EDF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Part of winsock2.h
/*
 * Protocols

#define IPPROTO_IP              0               dummy for IP 
#define IPPROTO_ICMP            1               control message protocol 
#define IPPROTO_IGMP            2               internet group management protocol 
#define IPPROTO_GGP             3               gateway^2 (deprecated)
#define IPPROTO_TCP             6               tcp 
#define IPPROTO_PUP             12              pup 
#define IPPROTO_UDP             17              user datagram protocol 
#define IPPROTO_IDP             22              xns idp 
#define IPPROTO_ND              77              UNOFFICIAL net disk proto 

#define IPPROTO_RAW             255             raw IP packet
#define IPPROTO_MAX             256
*/
typedef struct _PROTN2T
{ 
	int  proto ;
	char *pprototext ;
}PROTN2T ; 
						
#define PROTO_NUM  11 

/* The IP header */
typedef struct _IPHEADER {
        unsigned char  header_len:4;
        unsigned char  version:4;   
        unsigned char  tos;            // type of service
        unsigned short total_len;      // length of the packet
        unsigned short ident;          // unique identifier
        unsigned short flags;          
        unsigned char  ttl;            
        unsigned char  proto;          // protocol ( IP , TCP, UDP etc)
        unsigned short checksum;       
        unsigned int   sourceIP;
        unsigned int   destIP;

}IPHEADER;


/////////////////////////////////////////////////////////////////////////////
// CIpmonDlg dialog

class CIpmonDlg : public CDialog
{
// Construction
public:
	CIpmonDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CIpmonDlg)
	enum { IDD = IDD_IPMON_DIALOG };
	CIPAddressCtrl	m_ipctrlhost;
	CButton	m_check;
	CButton	m_start;
	CListBox	m_list;
	CIPAddressCtrl	m_ipctrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIpmonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

// AF added members and methods
	DWORD  m_ipsource     ; // IP in net format
	DWORD  m_iphostsource ; // same IP in host format
	DWORD  m_iphost;
	DWORD  m_ipcheckedhost;
	SOCKET m_s            ;
	DWORD  m_threadID     ;
	BOOL   m_Multihomed   ;
	BOOL   m_Local        ;
	CDWordArray m_IPArr   ;

	BOOL   IsIpofHost( IPHEADER *pIpHeader ) ;
    void   close_thread() ;
    BOOL   IsThisHostIP() ; 
friend UINT threadFunc ( LPVOID p ) ;


	// Generated message map functions
	//{{AFX_MSG(CIpmonDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPMONDLG_H__47076949_0324_11D5_924F_00010219EDF4__INCLUDED_)
