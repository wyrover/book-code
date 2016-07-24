// CTelnetView.cpp : implementation of the CTelnetView class
//

#include "stdafx.h"
#include "CTelnet.h"

#include "CTelnetDoc.h"
#include "CTelnetView.h"
#include "MainFrm.h"
#include "ClientSocket.h"
#include "Process.h"

#include "HostDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMultiDocTemplate * pDocTemplate;


/////////////////////////////////////////////////////////////////////////////
// CTelnetView

IMPLEMENT_DYNCREATE(CTelnetView, CScrollView)

BEGIN_MESSAGE_MAP(CTelnetView, CScrollView)
	//{{AFX_MSG_MAP(CTelnetView)
	ON_WM_CHAR()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTelnetView construction/destruction

CTelnetView::CTelnetView()
{
	cTextColor = RGB(200,200,000);
	cBackgroundColor = RGB(000,000,222);
	cSock = NULL;
	bOptionsSent = FALSE;
	TempCounter = 0;
	cCursX = 0;
	for(int x = 0; x < 80; x++)
	{
		for(int y = 0; y < bufferLines; y++)
		{
			cText[x][y] = ' ';
		}
	}
}

CTelnetView::~CTelnetView()
{
	if(cSock != NULL)
		delete cSock;
	cSock = NULL;
}

BOOL CTelnetView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTelnetView drawing

void CTelnetView::OnDraw(CDC* pDC)
{
	CTelnetDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDC->SelectObject(GetStockObject(ANSI_FIXED_FONT));

	DrawCursor(pDC,FALSE);
	DoDraw(pDC);
	DrawCursor(pDC,TRUE);
}

void CTelnetView::DoDraw(CDC* pDC)
{
	CString strLine;
	BOOL bSkip = FALSE;
	CRect clip;
	pDC->GetClipBox(clip);
	clip.top -= dtY;

	pDC->SetTextColor(cTextColor);
	pDC->SetBkColor(cBackgroundColor);

	char text[2] = {0x00, 0x00};

	for(int y = 0; y < bufferLines; y++)
	{
		if(y * dtY >= clip.top)
		{
			for(int x = 0; x < 80; x++)
			{
				text[0] = cText[x][y];
				if(text[0] == 27)
					bSkip = TRUE;
				if(!bSkip)
					strLine += text[0];
				if(text[0] == 'm' && bSkip)
					bSkip = FALSE;
			}
			pDC->TextOut(0, y * dtY, strLine);
			strLine.Empty();
		}
	}
}

void CTelnetView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = dtX * 80 + 3;
	sizeTotal.cy = dtY * bufferLines + 3;
	SetScrollSizes(MM_TEXT, sizeTotal);

	ScrollToPosition(CPoint(0, bufferLines * 1000)); //go way past the end

	CHostDialog host;
	host.DoModal();
	cHostName = host.m_HostName;

	//create the socket and hook up to the host
	BOOL bOK;
	cSock = new CClientSocket(this);
	if(cSock != NULL)
	{
		bOK = cSock->Create();
		if(bOK == TRUE)
		{
			cSock->AsyncSelect(FD_READ | FD_WRITE | FD_CLOSE | FD_CONNECT | FD_OOB);
			cSock->Connect(cHostName, 23);
			GetDocument()->SetTitle(cHostName);
			Sleep(90);
		}
		else
		{
			ASSERT(FALSE);  //Did you remember to call AfxSocketInit()?
			delete cSock;
			cSock = NULL;
		}
	}
	else
	{
		AfxMessageBox("Could not create new socket",MB_OK);
	}

}

/////////////////////////////////////////////////////////////////////////////
// CTelnetView printing

BOOL CTelnetView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTelnetView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTelnetView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTelnetView diagnostics

#ifdef _DEBUG
void CTelnetView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTelnetView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTelnetDoc* CTelnetView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTelnetDoc)));
	return (CTelnetDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTelnetView message handlers



void CTelnetView::ProcessMessage(CClientSocket * pSock)
{
	if(!IsWindow(m_hWnd)) return;
	if(!IsWindowVisible()) return;
	int nBytes = pSock->Receive(m_bBuf ,ioBuffSize );
	if(nBytes != SOCKET_ERROR)
	{
		int ndx = 0;
		while(GetLine(m_bBuf, nBytes, ndx) != TRUE);
		ProcessOptions();
		MessageReceived(m_strNormalText);
	}
	m_strLine.Empty();
	m_strResp.Empty();
}


void CTelnetView::ProcessOptions()
{
	CString m_strTemp;
	CString m_strOption;
	unsigned char ch;
	int ndx;
	int ldx;
	BOOL bScanDone = FALSE;

	m_strTemp = m_strLine;

	while(!m_strTemp.IsEmpty() && bScanDone != TRUE)
	{
		ndx = m_strTemp.Find(IAC);
		if(ndx != -1)
		{
			m_strNormalText += m_strTemp.Left(ndx);
			ch = m_strTemp.GetAt(ndx + 1);
			switch(ch)
			{
			case DO:
			case DONT:
			case WILL:
			case WONT:
				m_strOption		= m_strTemp.Mid(ndx, 3);
				m_strTemp		= m_strTemp.Mid(ndx + 3);
				m_strNormalText	= m_strTemp.Left(ndx);
				m_ListOptions.AddTail(m_strOption);
				break;
			case IAC:
				m_strNormalText	= m_strTemp.Left(ndx);
				m_strTemp		= m_strTemp.Mid(ndx + 1);
				break;
			case SB:
				m_strNormalText = m_strTemp.Left(ndx);
				ldx = Find(m_strTemp, SE);
				m_strOption		= m_strTemp.Mid(ndx, ldx);
				m_ListOptions.AddTail(m_strOption);
				m_strTemp		= m_strTemp.Mid(ldx);
				//AfxMessageBox(m_strOption,MB_OK);
				break;
			default:
				bScanDone = TRUE;
			}
		}
		else
		{
			m_strNormalText = m_strTemp;
			bScanDone = TRUE;
		}
	} 
	
	RespondToOptions();
}


void CTelnetView::RespondToOptions()
{
	CString strOption;
	
	while(!m_ListOptions.IsEmpty())
	{
		strOption = m_ListOptions.RemoveHead();

		ArrangeReply(strOption);
	}

	DispatchMessage(m_strResp);
	m_strResp.Empty();
}

void CTelnetView::ArrangeReply(CString strOption)
{

	unsigned char Verb;
	unsigned char Option;
	unsigned char Modifier;
	unsigned char ch;
	BOOL bDefined = FALSE;

	if(strOption.GetLength() < 3) return;

	Verb = strOption.GetAt(1);
	Option = strOption.GetAt(2);

	switch(Option)
	{
	case 1:	// Echo
	case 3: // Suppress Go-Ahead
		bDefined = TRUE;
		break;
	}

	m_strResp += IAC;

	if(bDefined == TRUE)
	{
		switch(Verb)
		{
		case DO:
			ch = WILL;
			m_strResp += ch;
			m_strResp += Option;
			break;
		case DONT:
			ch = WONT;
			m_strResp += ch;
			m_strResp += Option;
			break;
		case WILL:
			ch = DO;
			m_strResp += ch;
			m_strResp += Option;
			break;
		case WONT:
			ch = DONT;
			m_strResp += ch;
			m_strResp += Option;
			break;
		case SB:
			Modifier = strOption.GetAt(3);
			if(Modifier == SEND)
			{
				ch = SB;
				m_strResp += ch;
				m_strResp += Option;
				m_strResp += IS;
				m_strResp += IAC;
				m_strResp += SE;
			}
			break;
		}
	}

	else
	{
		switch(Verb)
		{
		case DO:
			ch = WONT;
			m_strResp += ch;
			m_strResp += Option;
			break;
		case DONT:
			ch = WONT;
			m_strResp += ch;
			m_strResp += Option;
			break;
		case WILL:
			ch = DONT;
			m_strResp += ch;
			m_strResp += Option;
			break;
		case WONT:
			ch = DONT;
			m_strResp += ch;
			m_strResp += Option;
			break;
		}
	}
}

//send to the telnet server
void CTelnetView::DispatchMessage(CString strText)
{
	ASSERT(cSock);
	cSock->Send(strText, strText.GetLength());
}

BOOL CTelnetView::GetLine( unsigned char * bytes, int nBytes, int& ndx )
{
	BOOL bLine = FALSE;
	while ( bLine == FALSE && ndx < nBytes )
	{
		unsigned char ch = bytes[ndx];
		
		switch( ch )
		{
		case '\r': // ignore
			m_strLine += "\r\n"; //"CR";
			break;
		case '\n': // end-of-line
			break;
		default:   // other....
			m_strLine += ch;
			break;
		} 

		ndx ++;

		if (ndx == nBytes)
		{
			bLine = TRUE;
		}
	}
	return bLine;
}

void CTelnetView::MessageReceived(LPCSTR pText)
{
	BOOL bSkip = FALSE;
	CDC * pDC = GetDC();
	OnPrepareDC(pDC);
	DrawCursor(pDC,FALSE);
	CMainFrame * frm = (CMainFrame*)GetTopLevelFrame();
	pDC->SetTextColor(cTextColor);
	pDC->SetBkColor(cBackgroundColor);

	pDC->SelectObject(GetStockObject(ANSI_FIXED_FONT));
	int length = strlen(pText);
	char text[2] = {0x00, 0x00};
	for(int loop = 0; loop < length; loop++)
	{
		switch(pText[loop])
		{
		case 8: //Backspace
			cCursX--;
			if(cCursX < 0) cCursX = 0;
			break;
		case 9: //TAB
			cCursX++; //TBD make this smarter
			break;
		case 13: //CR
			m_strline.Empty();
			cCursX = 0;
			break;
		case 27:
			bSkip = TRUE;
			break;
		case 10: //LF
			{
				for(int row = 0; row < bufferLines-1; row++)
				{
					for(int  col = 0; col < 80; col++)
					{
						cText[col][row] = cText[col][row+1];
					}
				}
				for(int  col = 0; col < 80; col++)
				{
					cText[col][bufferLines-1] = ' ';
				}
				DoDraw(pDC);
			}
			break;
		case 'H':
		case 'K':
		case 'm':
			if(bSkip){
				bSkip = FALSE;
				break;
			}
		default:
			{
				if(!bSkip){
					cText[cCursX][bufferLines-1] = pText[loop];
					text[0] = cText[cCursX][bufferLines-1];
					m_strline += text[0];
					pDC->TextOut(0, (bufferLines-1) * dtY, m_strline);
					cCursX++;
				}
				
				if(cCursX >= 80)
				{
//					pDC->TextOut(0, (bufferLines-1) * dtY, m_strline);
					m_strline.Empty();
					for(int row = 0; row < bufferLines-1; row++)
					{
						for(int  col = 0; col < 80; col++)
						{
							cText[col][row] = cText[col][row+1];
						}
					}
					for(int  col = 0; col < 80; col++)
					{
						cText[col][bufferLines-1] = ' ';
					}
					cCursX = 0;
					DoDraw(pDC);
				}
			}
			break;
		}
	}
	DrawCursor(pDC,TRUE);
	ReleaseDC(pDC);
}

void CTelnetView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_RETURN)
	{
		DispatchMessage("\r\n");
	}
	else
	{
		DispatchMessage(nChar);
	}
}

void CTelnetView::DrawCursor(CDC * pDC, BOOL pDraw)
{
	COLORREF color;
	CMainFrame * frm = (CMainFrame*)GetTopLevelFrame();
	if(pDraw) //draw
	{
		color = cTextColor;
	}
	else //erase
	{
		color = cBackgroundColor;
	}
	CRect rect(cCursX * dtX + 2, (bufferLines-1) * dtY + 1, 
		cCursX * dtX + dtX - 2, (bufferLines-1) * dtY + dtY -1);
	pDC->FillSolidRect(rect, color);
}

void CTelnetView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	if(IsWindow(m_hWnd))
	{
		if(IsWindowVisible())
		{
			ScrollToPosition(CPoint(0, bufferLines * 1000)); //go way past the end
		}
	}
}

BOOL CTelnetView::OnEraseBkgnd(CDC* pDC) 
{
	CRect clip;
	pDC->GetClipBox(clip);
	CMainFrame * frm = (CMainFrame*)GetTopLevelFrame();
	pDC->FillSolidRect(clip,cBackgroundColor);
	return TRUE;
}

int CTelnetView::Find(CString str, char ch)
{
	char* data = str.GetBuffer(0);
	int len = str.GetLength();
	int i = 0;
	for(i = 0; i < len; i++){
		if(data[i] == ch)
			break;
	}
	str.ReleaseBuffer();
	return i;
}
