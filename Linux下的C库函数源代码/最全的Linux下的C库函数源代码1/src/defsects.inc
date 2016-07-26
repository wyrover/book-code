;***
;defsects.inc - defines sections.
;
;   Copyright (c) Microsoft Corporation.  All rights reserved.
;
;Purpose:
;   This file defines sections for the C and C++ libs.
;
;   NOTE:  As needed, special "CRT" sections can be added into the existing
;   init/term tables.  These will be for our use only -- users who put
;   stuff in here do so at their own risk.
;
;******************************************************************************

;*******
;*
;* beginSection - a macro for declaring and beginning a section
;*
;* endSection - a macro for ending a previously declared section
;*
;*******

beginSection    MACRO   SectName
.CRT$&SectName  SEGMENT DWORD PUBLIC 'DATA'
                ENDM

endSection      MACRO   SectName
.CRT$&SectName  ENDS
                ENDM

;  XIA  Begin C Initializer Sections
;  XIC   Microsoft Reserved
;  XIU   User
;  XIZ  End C Initializer Sections
;
;  XCA  Begin C++ Constructor Sections
;  XCC   Compiler (MS)
;  XCL   Library
;  XCU   User
;  XCZ  End C++ Constructor Sections
;
;  XPA  Begin C Pre-Terminator Sections
;  XPU   User
;  XPX   Microsoft Reserved
;  XPZ  End C Pre-Terminator Sections
;
;  XTA  Begin C Pre-Terminator Sections
;  XTU   User
;  XTX   Microsoft Reserved
;  XTZ  End C Pre-Terminator Sections
