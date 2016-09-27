# This is a part of the Microsoft Foundation Classes C++ library.
# Copyright (C) 1992-1997 Microsoft Corporation
# All rights reserved.
#
# This source code is only intended as a supplement to the
# Microsoft Foundation Classes Reference and Microsoft
# QuickHelp and/or WinHelp documentation provided with the library.
# See these sources for detailed information regarding the
# Microsoft Foundation Classes product.

PROJ=SCRIBBLE
ROOT=..\..\..
OBJS=scribble.obj scribdoc.obj scribvw.obj mainfrm.obj childfrm.obj pendlg.obj



GOAL: $(PROJ).exe $(PROJ).hlp
$(PROJ).hlp : hlp\$(PROJ).hpj resource.h
	makehelp
	if exist hlp\$(PROJ).hlp copy hlp\$(PROJ).hlp .


!include <mfcsamps.mak>
