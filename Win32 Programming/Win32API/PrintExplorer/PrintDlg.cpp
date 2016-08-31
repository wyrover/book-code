#include "stdafx.h"
#include "printdlg.h"

/****************************************************************************
*                            CPrintDlg::CPrintDlg
* Inputs:
*       PRINTDLG * pd
* Effect: 
*       ²
****************************************************************************/

CPrintDlg::CPrintDlg(PRINTDLG * npd)
    {
     pd = npd;
    }


/****************************************************************************
*                             CPrintDlg::DoModal
* Inputs:
*       PRINTDLG * pd
* Result: int
*       result of CPrintDialog()
* Effect: 
*       Pops up a CPrintDialog
****************************************************************************/

int CPrintDlg::DoModal()
    {
     int result = ::PrintDlg(pd);

     return result;
    }
