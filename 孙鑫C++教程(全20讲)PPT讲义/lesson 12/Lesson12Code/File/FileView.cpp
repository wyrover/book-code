// FileView.cpp : implementation of the CFileView class
//

#include "stdafx.h"
#include "File.h"

#include "FileDoc.h"
#include "FileView.h"

#include <fstream.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

IMPLEMENT_DYNCREATE(CFileView, CView)

BEGIN_MESSAGE_MAP(CFileView, CView)
	//{{AFX_MSG_MAP(CFileView)
	ON_COMMAND(IDM_FILE_WRITE, OnFileWrite)
	ON_COMMAND(IDM_FILE_READ, OnFileRead)
	ON_COMMAND(IDM_REG_WRITE, OnRegWrite)
	ON_COMMAND(IDM_REG_READ, OnRegRead)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileView construction/destruction

CFileView::CFileView()
{
	// TODO: add construction code here

}

CFileView::~CFileView()
{
}

BOOL CFileView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CFileView drawing

void CFileView::OnDraw(CDC* pDC)
{
	CFileDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CFileView printing

BOOL CFileView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CFileView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CFileView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CFileView diagnostics

#ifdef _DEBUG
void CFileView::AssertValid() const
{
	CView::AssertValid();
}

void CFileView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFileDoc* CFileView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFileDoc)));
	return (CFileDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFileView message handlers

void CFileView::OnFileWrite() 
{
	// TODO: Add your command handler code here
/*	FILE *pFile=fopen("1.txt","w");
	fwrite("http://www.sunxin.org",1,strlen("http://www.sunxin.org"),pFile);
	//fseek(pFile,0,SEEK_SET);
	//fwrite("ftp:",1,strlen("ftp:"),pFile);
	//fwrite("http://www.sunxin.org",1,strlen("http://www.sunxin.org"),pFile);
	fclose(pFile);*/
	//fflush(pFile);

/*	FILE *pFile=fopen("2.txt","wb");
	char ch[3];
	ch[0]='a';
	ch[1]=10;
	ch[2]='b';
	fwrite(ch,1,3,pFile);
	fclose(pFile);*/

	/*FILE *pFile=fopen("3.txt","w");
	int i=98341;
	char ch[5];*/
	/*ch[0]=9+48;
	ch[1]=8+48;
	ch[2]=3+48;
	ch[3]=4+48;
	ch[4]=1+48;*/
	/*itoa(i,ch,10);

	//fwrite(&i,4,1,pFile);
	fwrite(ch,1,5,pFile);
	fclose(pFile);*/

/*	ofstream ofs("4.txt");
	ofs.write("http://www.sunxin.org",strlen("http://www.sunxin.org"));
	ofs.close();*/
/*	HANDLE hFile;
	hFile=CreateFile("5.txt",GENERIC_WRITE,0,NULL,CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,NULL);
	DWORD dwWrites;
	WriteFile(hFile,"http://www.sunxin.org",strlen("http://www.sunxin.org"),
		&dwWrites,NULL);
	CloseHandle(hFile);*/
/*	CFile file("6.txt",CFile::modeCreate | CFile::modeWrite);
	file.Write("http://www.sunxin.org",strlen("http://www.sunxin.org"));
	file.Close();*/
	CFileDialog fileDlg(FALSE);
	fileDlg.m_ofn.lpstrTitle="我的文件保存对话框";
	fileDlg.m_ofn.lpstrFilter="Text Files(*.txt)\0*.txt\0All Files(*.*)\0*.*\0\0";
	fileDlg.m_ofn.lpstrDefExt="txt";
	if(IDOK==fileDlg.DoModal())
	{
		CFile file(fileDlg.GetFileName(),CFile::modeCreate | CFile::modeWrite);
		file.Write("http://www.sunxin.org",strlen("http://www.sunxin.org"));
		file.Close();
	}
}

void CFileView::OnFileRead() 
{
	// TODO: Add your command handler code here
/*	FILE *pFile=fopen("1.txt","r");
//	char ch[100];
//	memset(ch,0,100);
//	fread(ch,1,100,pFile);
//	MessageBox(ch);
	char *pBuf;
	fseek(pFile,0,SEEK_END);
	int len=ftell(pFile);
	pBuf=new char[len+1];
	rewind(pFile);
	fread(pBuf,1,len,pFile);
	pBuf[len]=0;
	MessageBox(pBuf);
	fclose(pFile);*/
/*	FILE *pFile=fopen("2.txt","rb");
	char ch[100];
	fread(ch,1,3,pFile);
	ch[3]=0;
	MessageBox(ch);
	fclose(pFile);*/

/*	ifstream ifs("4.txt");
	char ch[100];
	memset(ch,0,100);
	ifs.read(ch,100);
	ifs.close();
	MessageBox(ch);*/

/*	HANDLE hFile;
	hFile=CreateFile("5.txt",GENERIC_READ,0,NULL,OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,NULL);
	char ch[100];
	DWORD dwReads;
	ReadFile(hFile,ch,100,&dwReads,NULL);
	ch[dwReads]=0;
	CloseHandle(hFile);
	MessageBox(ch);*/
/*	CFile file("6.txt",CFile::modeRead);
	char *pBuf;
	DWORD dwFileLen;
	dwFileLen=file.GetLength();
	pBuf=new char[dwFileLen+1];
	pBuf[dwFileLen]=0;
	file.Read(pBuf,dwFileLen);
	file.Close();
	MessageBox(pBuf);*/
	CFileDialog fileDlg(TRUE);
	fileDlg.m_ofn.lpstrTitle="我的文件打开对话框";
	fileDlg.m_ofn.lpstrFilter="Text Files(*.txt)\0*.txt\0All Files(*.*)\0*.*\0\0";
	
	if(IDOK==fileDlg.DoModal())
	{
		CFile file(fileDlg.GetFileName(),CFile::modeRead);
		char *pBuf;
		DWORD dwFileLen;
		dwFileLen=file.GetLength();
		pBuf=new char[dwFileLen+1];
		pBuf[dwFileLen]=0;
		file.Read(pBuf,dwFileLen);
		file.Close();
		MessageBox(pBuf);
	}
}

void CFileView::OnRegWrite() 
{
	// TODO: Add your command handler code here
	HKEY hKey;
	DWORD dwAge=30;
	RegCreateKey(HKEY_LOCAL_MACHINE,"Software\\http://www.sunxin.org\\admin",&hKey);
	RegSetValue(hKey,NULL,REG_SZ,"zhangsan",strlen("zhangsan"));
	RegSetValueEx(hKey,"age",0,REG_DWORD,(CONST BYTE*)&dwAge,4);
	RegCloseKey(hKey);
}

void CFileView::OnRegRead() 
{
	// TODO: Add your command handler code here
/*	LONG lValue;
	RegQueryValue(HKEY_LOCAL_MACHINE,"Software\\http://www.sunxin.org\\admin",
		NULL,&lValue);
	char *pBuf=new char[lValue];
	RegQueryValue(HKEY_LOCAL_MACHINE,"Software\\http://www.sunxin.org\\admin",
		pBuf,&lValue);
	MessageBox(pBuf);*/
	HKEY hKey;
	RegOpenKey(HKEY_LOCAL_MACHINE,"Software\\http://www.sunxin.org\\admin",&hKey);
	DWORD dwType;
	DWORD dwValue;
	DWORD dwAge;
	RegQueryValueEx(hKey,"age",0,&dwType,(LPBYTE)&dwAge,&dwValue);
	CString str;
	str.Format("age=%d",dwAge);
	MessageBox(str);
}
