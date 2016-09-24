/**************************************************************************

    AVStream Filter-Centric Sample

    Copyright (c) 2001, Microsoft Corporation

    File:

        avssamp.h

    Abstract:

        AVStream Filter-Centric Sample header file.  This is the main
        header.

    History:

        created 6/18/01

**************************************************************************/

/*************************************************

    Standard Includes

*************************************************/

extern "C" {
#include <wdm.h>
}

#include <windef.h>
#include <stdio.h>
#include <stdlib.h>
#include <ntstrsafe.h>
#define NOBITMAP
#include <mmreg.h>
#undef NOBITMAP
#include <unknown.h>
#include <ks.h>
#include <ksmedia.h>
#include <kcom.h>


void ClearObj (IN void* obj);

extern const KSFILTER_DISPATCH
gFilterDispatch;

extern const KSFILTER_DESCRIPTOR
gSplitterFilter;

extern const KSPIN_DESCRIPTOR_EX
gPins [2];

extern const GUID
gFilterCategories [2];

extern const KSDEVICE_DESCRIPTOR 
gKsSplitterDevice;


#include "filter.h"
#include "pin.h"

