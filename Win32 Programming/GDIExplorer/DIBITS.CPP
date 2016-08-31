// DIBits.cpp : implementation file
//

#include "stdafx.h"
#include "GDIExplorer.h"
#include "image.h"
#include "bmselect.h" 
#include "source.h"
#include "IntEdit.h"
#include "DIBits.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDIBits property page

IMPLEMENT_DYNCREATE(CDIBits, CPropertyPage)

CDIBits::CDIBits() : CPropertyPage(CDIBits::IDD)
{
        //{{AFX_DATA_INIT(CDIBits)
        //}}AFX_DATA_INIT
}

CDIBits::~CDIBits()
{
}

void CDIBits::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CDIBits)
        DDX_Control(pDX, IDC_BIYPELSPERMETER, c_biYPelsPerMeter);
        DDX_Control(pDX, IDC_BIXPELSPERMTER, c_biXPelsPerMeter);
        DDX_Control(pDX, IDC_BIWIDTH, c_biWidth);
        DDX_Control(pDX, IDC_BISIZEIMAGE, c_biSizeImage);
        DDX_Control(pDX, IDC_BISIZE, c_biSize);
        DDX_Control(pDX, IDC_BIPLANES, c_biPlanes);
        DDX_Control(pDX, IDC_BIHEIGHT, c_biHeight);
        DDX_Control(pDX, IDC_BICOMPRESSION, c_biCompression);
        DDX_Control(pDX, IDC_BIBITCOUNT, c_biBitCount);
        DDX_Control(pDX, IDC_BICLRIMPORTANT, c_biClrImportant);
        DDX_Control(pDX, IDC_BICLRUSED, c_biClrUsed);
        DDX_Control(pDX, IDC_SOURCE, c_Source);
        DDX_Control(pDX, IDC_BITMAP, c_BitmapSelect);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDIBits, CPropertyPage)
        //{{AFX_MSG_MAP(CDIBits)
        ON_CBN_SELENDOK(IDC_BITMAP, OnSelendokBitmap)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDIBits message handlers

void CDIBits::updateSource()
    {
     int item = c_BitmapSelect.GetCurItem();
     c_Source.setBitmap(item);

     CClientDC dc(&c_Source);

     CBitmap * bm = c_Source.getBitmap();

     BITMAPINFO bmi;
     bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
     bmi.bmiHeader.biBitCount = 0;
     ::GetDIBits(dc.m_hDC, *bm, 0, 0, NULL, &bmi, DIB_RGB_COLORS);

     c_biClrUsed.SetVal(bmi.bmiHeader.biClrUsed);
     c_biClrImportant.SetVal(bmi.bmiHeader.biClrImportant);
     c_biBitCount.SetVal(bmi.bmiHeader.biBitCount);
     c_biHeight.SetVal(bmi.bmiHeader.biHeight);
     c_biPlanes.SetVal(bmi.bmiHeader.biPlanes);
     c_biSize.SetVal(bmi.bmiHeader.biSize);
     c_biSizeImage.SetVal(bmi.bmiHeader.biSizeImage);
     c_biWidth.SetVal(bmi.bmiHeader.biWidth);
     c_biXPelsPerMeter.SetVal(bmi.bmiHeader.biXPelsPerMeter);
     c_biYPelsPerMeter.SetVal(bmi.bmiHeader.biYPelsPerMeter);

     CString s;

     switch(bmi.bmiHeader.biCompression)
        { /* compression */
         case BI_RGB:
                 s.LoadString(IDS_BI_RGB);
                 break;
         case BI_RLE8:
                 s.LoadString(IDS_BI_RLE8);
                 break;
         case BI_RLE4:
                 s.LoadString(IDS_BI_RLE4);
                 break;
         case BI_BITFIELDS:
                 s.LoadString(IDS_BI_BITFIELDS);
                 break;
        } /* compression */
     c_biCompression.SetWindowText(s);
    }

void CDIBits::OnSelendokBitmap() 
{
 updateSource();
        
}

BOOL CDIBits::OnInitDialog() 
{
        CPropertyPage::OnInitDialog();
        
        c_BitmapSelect.Load();
        c_BitmapSelect.SetCurSel(0);
        c_Source.setBorder(0);  // use full width
        
        updateSource();

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}
