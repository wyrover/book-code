#include "stdafx.h"
#include "odinfo.h"

int odinfo::uidcounter = 1;

/****************************************************************************
*                             odinfo::odinfo
* Effect: 
*       Initializes the fields
****************************************************************************/

odinfo::odinfo()
    {
     subtractCheckWidth = FALSE;
     style = 0; // 0 is not a legal style
     width = 0;
     height = 0;

     // Initialize the Style1 variables:

     text = ::GetSysColor(COLOR_MENUTEXT);
     background = ::GetSysColor(COLOR_MENU);
     hilitetext = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
     hilitebackground = ::GetSysColor(COLOR_HIGHLIGHT);
     bmcheck = NULL;
     uid = uidcounter++;
    }

/****************************************************************************
*                             odinfo::~odinfo
* Effect: 
*       Destructor
****************************************************************************/

odinfo::~odinfo()
    {
    }
