========================================================================
	   MICROSOFT FOUNDATION CLASS LIBRARY : Container
========================================================================


AppWizard has created this Container application for you.  This application
not only demonstrates the basics of using the Microsoft Foundation classes
but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your Container application.


Contain.mak
	This project file is compatible with the Visual C++ development
	environment.

	It is also compatible with the NMAKE program provided with Visual C++.

	To build a debug version of the program from the MS-DOS prompt, type
nmake /f Contain.mak CFG="Win32 Debug"
	or to build a release version of the program, type
nmake /f Contain.mak CFG="Win32 Release"


Contain.h
	This is the main header file for the application.  It includes other
	project specific headers (including Resource.h) and declares the
	CContainerApp application class.

Contain.cpp
	This is the main application source file that contains the application
	class CContainerApp.

Contain.rc
	This is a listing of all of the Microsoft Windows resources that the
	program uses.  It includes the icons, bitmaps, and cursors that are stored
	in the RES subdirectory.  This file can be directly edited in the
	Visual C++ development environment.

res\Contain.ico
	This is an icon file, which is used as the application's icon.  This
	icon is included by the main resource file Contain.rc.

res\Contain.rc2
	This file contains resources that are not edited by the Visual C++
	development environment.  You should place all resources not
	editable by the resource editor in this file.

Contain.reg
	This is an example .REG file that shows you the kind of registration
	settings the framework will set for you.  You can use this as a .REG
	file to go along with your application or just delete it and rely
	on the default RegisterShellFileTypes registration.

Contain.clw
	This file contains information used by ClassWizard to edit existing
	classes or add new classes.  ClassWizard also uses this file to store
	information needed to create and edit message maps and dialog data
	maps and to create prototype member functions.

/////////////////////////////////////////////////////////////////////////////

For the main frame window:

MainFrm.h, MainFrm.cpp
	These files contain the frame class CMainFrame, which is derived from
	CMDIFrameWnd and controls all MDI frame features.

res\Toolbar.bmp
	This bitmap file is used to create tiled images for the toolbar.
	The initial toolbar and status bar are constructed in the
	CMainFrame class.  Edit this toolbar bitmap along with the
	array in MainFrm.cpp to add more toolbar buttons.

/////////////////////////////////////////////////////////////////////////////

AppWizard creates one document type and one view:

ContrDoc.h, ContrDoc.cpp - the document
	These files contain your CContainerDoc class.  Edit these files to
	add your special document data and to implement file saving and loading
	(via CContainerDoc::Serialize).

ContrVw.h, ContrVw.cpp - the view of the document
	These files contain your CContainerView class.
	CContainerView objects are used to view CContainerDoc objects.

res\ContrDoc.ico
	This is an icon file, which is used as the icon for MDI child windows
	for the CContainerDoc class.  This icon is included by the main
	resource file Contain.rc.

/////////////////////////////////////////////////////////////////////////////

AppWizard has also created classes specific to OLE

CntrItem.h, CntrItem.cpp - this class is used to
	manipulate OLE objects.  They are usually displayed by your
	CContainerView class and serialized as part of your CContainerDoc class.


/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
	These files are used to build a precompiled header (PCH) file
	named Contain.pch and a precompiled types file named StdAfx.obj.

Resource.h
	This is the standard header file, which defines new resource IDs.
	Visual C++ reads and updates this file.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

If your application is in a language other than the operating system's
current language, you will need to copy the corresponding localized
resources MFC40XXX.DLL from the Microsoft Visual C++ CD-ROM onto the
system or system32 directory, and rename it to be MFCLOC.DLL.  Otherwise,
some of the UI elements of your application will remain in the language
of the operating system.

/////////////////////////////////////////////////////////////////////////////
