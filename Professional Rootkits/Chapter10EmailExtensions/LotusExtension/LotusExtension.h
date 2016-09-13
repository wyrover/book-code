// Copyright Ric Vieler, 2006
// Support header for LotusExtension.c

#ifndef _LOTUS_FILTER_H_
#define _LOTUS_FILTER_H_

//	LOTUS C API FOR DOMINO AND NOTES HEADER FILES
#include <nsferr.h>
#include <extmgr.h>

#define DLL_EXPORT

// Data types for LogContent
#define BODY_FILENAME_CONTENT       1
#define ATTACHMENT_FILENAME_CONTENT 2
#define ADDRESS_STRING_CONTENT      3

// File Managment Definitions
#define LN_BODY "C:\\LN_Body"
#define LN_ATTACHMENT "C:\\LN_Attachment"
#define LN_LOG_FILE "C:\\LN_LOG"

#endif
