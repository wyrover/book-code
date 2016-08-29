// PolyPt.cpp : implementation file
//

#include "stdafx.h"
#include "GDIExplorer.h"
#include "polypoly.h"
#include "PolyPt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPolyPoints dialog


CPolyPoints::CPolyPoints(CWnd* pParent /*=NULL*/)
        : CDialog(CPolyPoints::IDD, pParent)
{
        //{{AFX_DATA_INIT(CPolyPoints)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}


void CPolyPoints::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CPolyPoints)
        DDX_Control(pDX, IDOK, c_OK);
        DDX_Control(pDX, IDC_POINTS, c_Points);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPolyPoints, CDialog)
        //{{AFX_MSG_MAP(CPolyPoints)
        ON_WM_DESTROY()
        ON_LBN_SELCHANGE(IDC_POINTS, OnSelchangePoints)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPolyPoints message handlers

void CPolyPoints::OnOK() 
    {
     if(c_Points.GetSelCount() == 0)
        { /* whole thing */
         pd.count = 1;
         if(pd.points == NULL)
            delete [] pd.points;
         pd.points = new int[1];
         if(pd.points != NULL)
            pd.points[0] = pd.valuecount;
        } /* whole thing */
     else
        { /* has selections */
         c_Points.SetSel(0); // force 0th item on, no matter what
         int sels = c_Points.GetSelCount(); // might be different
         if(pd.points == NULL)
            delete [] pd.points;
         pd.points = new int[sels];
         if(pd.points != NULL)
            { /* load it */
             c_Points.GetSelItems(sels, pd.points);
             // Now convert the selection items to counts, that is
             // if the number of points is 20, 
             //         0       7       11      
             // becomes 7       4       9    <= 9 is (20 - 11)
             //

             for(int i = 0; i < sels - 1; i++)
                pd.points[i] = pd.points[i + 1] - pd.points[i];

             // i is now sels - 1
             pd.points[i] = pd.valuecount - pd.points[i];
            } /* load it */
         pd.count = sels;
        } /* has selections */
        
        CDialog::OnOK();
}

BOOL CPolyPoints::OnInitDialog() 
{
        CDialog::OnInitDialog();
        
        for(int i = 0; i < pd.valuecount; i++)
           { /* add point */
            CString s;
            s.Format(_T("%d, %d"), pd.data[i].x, pd.data[i].y);
            c_Points.AddString(s);
           } /* add point */
        

        // Initialize the selections.  What we do is select the start
        // of each polygon by highlighting it.  The selection of the 0th
        // position is redundant; selected or unselected it is always
        // the start of the polypolygon sequence.

        
        c_Points.SetSel(0, TRUE);

        // The pd.proints array contains *counts* of points in each polygon, 
        // e.g., if there are 20 points in the array, then 10, 10 splits it
        // evenly, and we would have selections at 0 and 10.  The last
        // point, computed as 20, is outside the range and is ignored
        // by the loop below.

        int base = 0;
        for(i = 0; i < pd.count; i++)
           { /* make selections */
            if(base + pd.points[i] < pd.valuecount)
               { /* select and load */
                base += pd.points[i];
                c_Points.SetSel(base, TRUE);
               } /* select and load */
           } /* make selections */

        OnSelchangePoints();

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CPolyPoints::OnDestroy() 
{
        CDialog::OnDestroy();
        
}

/****************************************************************************
*                       CPolyPoints::OnSelchangePoints
* Result: void
*       
* Effect: 
*       Enforces a restriction that PolyPolygon must have more than one 
*       point defining a polygon; if there are two adjacent selections,
*       disables the OK button.
* Notes:
*       The first item must always be selected; the last item must not
*       be selected.  This function enforces those restrictions.
****************************************************************************/

void CPolyPoints::OnSelchangePoints() 
{
 if(!c_Points.GetSel(0))
    { /* force 0 */
     c_Points.SetRedraw(FALSE);
     int top = c_Points.GetTopIndex();
     c_Points.SetSel(0);  // enforce [0] always being selected...
     c_Points.SetTopIndex(top);
     c_Points.SetRedraw(TRUE);
    } /* force 0 */

 if(c_Points.GetSel(c_Points.GetCount() - 1) > 0)
    { /* last is selected */
     c_Points.SetRedraw(FALSE);
     int top = c_Points.GetTopIndex();
     c_Points.SetSel(c_Points.GetCount() - 1, FALSE);
     c_Points.SetTopIndex(top);
     c_Points.SetRedraw(TRUE);
    } /* last is selected */

 int sels = c_Points.GetSelCount();
 if(sels == 0)
    { /* none selected */
     c_OK.EnableWindow(TRUE);  // default is 0 is selected
     return;
    } /* none selected */

 LPINT selections = new int[sels];
 if(selections == NULL)
    { /* failed */
     c_OK.EnableWindow(FALSE); // should never happen...
     return;
    } /* failed */

 c_Points.GetSelItems(sels, selections);
 
 for(int i = 0; i < sels - 1; i++)
    { /* check adjacency */
     if(selections[i] + 1 == selections[i+1])
        { /* infeasible */
         c_OK.EnableWindow(FALSE);
         return;
        } /* infeasible */
    } /* check adjacency */

 c_OK.EnableWindow(TRUE);
}
