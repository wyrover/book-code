// PostItCtl.cpp : Implementation of CPostItCtl

#include "stdafx.h"
#include "PostIt.h"
#include "PostItCtl.h"

/////////////////////////////////////////////////////////////////////////////
// CPostItCtl

LRESULT CPostItCtl::OnLButtonDown(UINT uMsg, WPARAM wParam,
                                  LPARAM lParam, BOOL& bHandled)
{
    Fire_Click();
    return 0;
}
