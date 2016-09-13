// OutlookExtension
// Copyright Ric Vieler, 2006
// Filter Outlook email

#include "stdafx.h"
#include <STDIO.h>
#include <WINDOWS.H> 
#include <COMMCTRL.H>

#define MIDL_PASS

#include <MAPIX.H>
#include <MAPIUTIL.H>
#include <MAPIFORM.H>
#include <INITGUID.h>
#include "EXCHEXT.H"
#include "OutlookExtension.h"

// Microsoft Exchange Client Extension entry point
extern "C" _declspec(dllexport) LPEXCHEXT CALLBACK ExchEntryPoint(void);

// DLL entry point
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved )
{
    return TRUE;
}

// Must provide pointer to CClientExtension for construction
CMessageEvents::CMessageEvents (LPUNKNOWN pParentInterface)
{ 
	m_pExchExt = pParentInterface;
	m_submittingMessage = false;
	m_referenceCount = 0; 
};

void CMessageEvents::LogContent( char* content, int contentType )
{
	// Put content into one big file for this example
	char buffer[ MAX_PATH ];
	size_t contentLength;
	FILE* sourceFile;
	FILE* destinationFile;

	// open the destination file - LN_LOG_FILE
	strcpy( buffer, OL_LOG_FILE );
	if( (destinationFile = fopen( buffer, "a+b" )) != NULL )
	{
		if( contentType == OL_LOG_ATTACHMENT )
		{
			// content is a filename
			if( (sourceFile = fopen( content, "r" )) != NULL )
			{
				// write header
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
		else
		{
			// content is a string
			// write header
			if( contentType == OL_LOG_BODY )
			{
				fwrite( "BODY:\n", sizeof(char), 6, destinationFile );
			}
			else
			{
				fwrite( "DESTINATION(S):\n", sizeof(char), 16, destinationFile );
			}
			// write data
			contentLength = strlen( content );
			fwrite( content, sizeof( char ), contentLength, destinationFile );
			// write footer
			fwrite( "\n\n", sizeof( char ), 2, destinationFile );
		}
		fclose( destinationFile );
	}
}

// Log message body
void CMessageEvents::LogBody( LPMESSAGE pMessage )
{
	char* bodybuf = 0;
	unsigned int bodysize = 0;
	IStream* stream;
	HRESULT hr;

	// Get body of message as a stream
	hr = pMessage->OpenProperty(PR_BODY,
								&IID_IStream,
								STGM_DIRECT | STGM_READ,
								0,
								(IUnknown**)&stream );
	if ( !FAILED(hr) )
	{
		// Get size of stream
		STATSTG status = { 0 };
		hr = stream->Stat( &status, STATFLAG_NONAME );
		if ( !FAILED(hr) )
		{
			// Read the stream into a local buffer
			bodysize = status.cbSize.LowPart;
			bodybuf = new char[ bodysize + 1 ];
			ULONG count;
			hr = stream->Read( bodybuf, bodysize, &count );
			if ( !FAILED(hr) )
			{
				if ( count < bodysize)
					bodysize = count;
				bodybuf[bodysize] = 0;
				stream->Release();
				// Log the content
				LogContent( bodybuf, OL_LOG_BODY );
			}
		}
	}
}

// Log message attachments
void CMessageEvents::LogAttachments( LPMESSAGE pMessage )
{
	HRESULT hr;
	LPMAPITABLE pAttachmentTable;
	LPATTACH pAttachment;

	// Get the attachment table
	hr = pMessage->GetAttachmentTable( MAPI_UNICODE, &pAttachmentTable );
	if ( !FAILED(hr) )
	{					
		SizedSPropTagArray(1,columns) = { 1, PR_ATTACH_NUM };
		SRowSet* pRowSet;
		hr = HrQueryAllRows( pAttachmentTable, 
							(SPropTagArray*)&columns,
							NULL, NULL, 0, &pRowSet);
		if ( !FAILED(hr) )
		{
			for (unsigned int row = 0; row < pRowSet->cRows; row++ )
			{
				if (pRowSet->aRow[row].lpProps[0].ulPropTag == PR_ATTACH_NUM )
				{
					// Open the attachment
					hr = pMessage->OpenAttach(pRowSet->aRow[row].lpProps[0].Value.ul,
											NULL,
											MAPI_BEST_ACCESS,
											&pAttachment );
					if ( !FAILED(hr) )
					{					
						// Get the attachment type
						ULONG count;
						SPropValue* property = 0;
						SizedSPropTagArray(1, tag) = { 1, PR_ATTACH_METHOD };
						pAttachment->GetProps((SPropTagArray*)&tag,
							MAPI_UNICODE,
							&count,
							&property);
						// Process attachment based on attachment type
						if( (property[0].ulPropTag) &&
							(property[0].Value.ul == ATTACH_BY_REF_ONLY ||
							 property[0].Value.ul == ATTACH_BY_REF_RESOLVE ||
							 property[0].Value.ul == ATTACH_BY_REFERENCE ))
						{
							// Attachment is by filename
							ULONG count;
							SPropValue* path = 0;
							SizedSPropTagArray(2, tag) =
								{ 2, { PR_ATTACH_LONG_PATHNAME,PR_ATTACH_PATHNAME } };
							pAttachment->GetProps((SPropTagArray*)&tag,
								MAPI_UNICODE,
								&count,
								&path);
							if( path[0].ulPropTag == PR_ATTACH_LONG_PATHNAME )
							{
								LogContent(	path[0].Value.LPSZ, OL_LOG_ATTACHMENT );
							}
							else if( path[1].ulPropTag == PR_ATTACH_PATHNAME )
							{
								LogContent(	path[1].Value.LPSZ, OL_LOG_ATTACHMENT );
							}
						}
						else if(property[0].ulPropTag &&
								property[0].Value.ul == ATTACH_BY_VALUE)
						{
							// Attachment is in memory
							// Convert it to a temp file
							char tempFile[20];
							strcpy( tempFile, OL_TEMP_LOG_FILE );
							STATSTG StatInfo;
							LPSTREAM pSourceStream = NULL;
							LPSTREAM pDestinationStream = NULL;
							hr = pAttachment->OpenProperty(PR_ATTACH_DATA_BIN,
                                 (LPIID)&IID_IStream,
                                 0,
                                 MAPI_MODIFY,
                                 (LPUNKNOWN*)&pSourceStream);
							if ( !FAILED(hr) )
							{
								hr = OpenStreamOnFile(
									 MAPIAllocateBuffer,
									 MAPIFreeBuffer,
									 STGM_CREATE | STGM_READWRITE | STGM_SHARE_DENY_NONE | STGM_DELETEONRELEASE,
									 tempFile,
									 NULL,
									 &pDestinationStream);
								if ( !FAILED(hr) )
								{
									// Get size of Source Stream
									pSourceStream->Stat(&StatInfo, STATFLAG_NONAME);
									// Write the stream to the temp file
									hr = pSourceStream->CopyTo(pDestinationStream,
										StatInfo.cbSize, NULL, NULL);
									if ( !FAILED(hr) )
									{
										// Commit changes to new stream
										pSourceStream->Commit(0);
										// Log the attachment
										LogContent( tempFile, OL_LOG_ATTACHMENT );
										// Release the streams
										// This should also delete the temp file
										pDestinationStream->Release();
										pSourceStream->Release();
									}
								}
							}
						}
						// Release the attachment
						pAttachment->Release();
					}
				}
			}
			FreeProws( pRowSet );
		}
		pAttachmentTable->Release();
	}
}

// Delete a MAPI message
// Called by CMessageEvents::OnWriteComplete before returning S_OK
void CMessageEvents::DeleteMessage( LPMESSAGE pMessage )
{
	HRESULT hr;

	// Remove the recipients
	LPMAPITABLE pRecipientTable;
	hr = pMessage->GetRecipientTable( MAPI_UNICODE, &pRecipientTable );
	if ( !FAILED(hr) )
	{
		// Need PR_ROWID for ModifyRecipients
		SizedSPropTagArray(1,columns) = { 1, PR_ROWID };
		SRowSet* pRowSet;
		hr = HrQueryAllRows( pRecipientTable, 
							(SPropTagArray*)&columns,
							NULL, NULL, 0, &pRowSet);
		if ( !FAILED(hr) )
		{
			pMessage->ModifyRecipients( MODRECIP_REMOVE, (ADRLIST*)pRowSet );
			FreeProws( pRowSet );
		}
		pRecipientTable->Release();
	}
	// Set PR_DELETE_AFTER_SUBMIT
	ULONG count;
	SPropValue* property = 0;
	SizedSPropTagArray(1, tag) = { 1, PR_DELETE_AFTER_SUBMIT };
	if( pMessage->GetProps((SPropTagArray*)&tag,
								NULL,
								&count,
								&property) == S_OK )
	{
		if( property[0].ulPropTag == PR_DELETE_AFTER_SUBMIT )
		{
			property[0].Value.b = TRUE;
			pMessage->SetProps( 1, property, NULL );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//    CMessageEvents::QueryInterface()
//
//    Parameters
//    riid   -- Interface ID.
//    ppvObj -- address of interface object pointer.
//
//    Purpose
//    Return interface object upon request
//    
//    Return Value - none
//
//    Comments
//    Currently the Exchange client does not call QueryInterface from any object
//    except for IExchExt.  This is implemented in case features are added to
//    Exchange to require QueryInterface from any object.  Also, as a "rule of
//    OLE COM" this is the proper implementation of QueryInterface.
//

STDMETHODIMP CMessageEvents::QueryInterface(REFIID riid, LPVOID FAR * ppvObj)          
{   
	*ppvObj = NULL;
	if (riid == IID_IExchExtMessageEvents)
	{
		*ppvObj = (LPVOID)this;
		// Increase usage count of this object
		AddRef();
		return S_OK;
	}
	if (riid == IID_IUnknown)
	{
		*ppvObj = (LPVOID)m_pExchExt;  // return parent interface
		m_pExchExt->AddRef();
		return S_OK;
	}
	return E_NOINTERFACE;

}

///////////////////////////////////////////////////////////////////////////////
//    CMessageEvents::OnRead()
//
//    Parameters
//    lpeecb -- pointer to IExchExtCallback interface
//
//    Purpose
//    To extend or inhibit Exchange when displaying the send or read note form.
//    
//    Return Value
//    S_OK Microsoft Exchange will consider the task handled
//    S_FALSE signals Exchange to continue calling extensions
//    Other MAPI Code errors will abort the send or read note form.
//
//

STDMETHODIMP CMessageEvents::OnRead(LPEXCHEXTCALLBACK lpeecb)
{
	return S_FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//    CMessageEvents::OnReadComplete()
//
//    Parameters
//    lpeecb -- pointer to IExchExtCallback interface
//
//    Purpose
//    To do processing after message has been read.
//    
//    Return Value
//    S_OK Microsoft Exchange will consider the task handled
//    S_FALSE signals Exchange to continue calling extensions
//    Some MAPI Code error indicates a problem and will not display the send
//    or read note form.
//
//    Comments.
//    If an error code, such as MAPI_E_CALL_FAILED, is returned, Exchange will
//    call OnReadComplete again with the ulFlags parameter set to 
//    EEME_COMPLETE_FAILED.  Returning the error code again will cause Exchange
//    to not display the UI.
//

STDMETHODIMP CMessageEvents::OnReadComplete(LPEXCHEXTCALLBACK lpeecb, ULONG ulFlags)
{
	return S_FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//    CMessageEvents::OnWrite()
//
//    Parameters
//    lpeecb -- pointer to IExchExtCallback interface
//
//    Purpose
//    This method is called when a message is about to be written.  The message
//    only has default properties at this point.  It does not contain 
//    properties which the user has added by way of recipients, subject,
//    message text, or attachments.
//    This method is called when the user Sends or Saves a message
//    
//    Return Value
//    S_OK Microsoft Exchange will consider the task handled
//    S_FALSE signals Exchange to continue calling extensions
//
//

STDMETHODIMP CMessageEvents::OnWrite(LPEXCHEXTCALLBACK lpeecb)
{
	return S_FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//    CMessageEvents::OnWriteComplete()
//
//    Parameters
//    lpeecb -- pointer to IExchExtCallback interface
//
//    Purpose
//    This method is called after the data (recipients, attachments, body, 
//    subject, etc.) has been written to the message.
//    
//    Return Value
//    S_OK Microsoft Exchange will consider the task handled
//    (you must also call DeleteMessage( pMessage ) if returning S_OK)
//    S_FALSE signals Exchange to continue calling extensions
//

STDMETHODIMP CMessageEvents::OnWriteComplete(LPEXCHEXTCALLBACK lpeecb, ULONG ulFlags)
{
	// Only check if writing for the purpose of submitting
	if( m_submittingMessage == false )
		return S_FALSE;

	// This is the only event of interest
	HRESULT hr;
	LPMESSAGE pMessage = NULL;
	LPMDB pMDB = NULL;

	// Get the message
	hr = lpeecb->GetObject(&pMDB, (LPMAPIPROP*)&pMessage);
	if ( !FAILED(hr) )
	{
		// Get the recipients
		LPMAPITABLE pRecipientTable;
		hr = pMessage->GetRecipientTable( MAPI_UNICODE, &pRecipientTable );
		if ( !FAILED(hr) )
		{
			SizedSPropTagArray(1,columns) = { 1, PR_EMAIL_ADDRESS };
			SRowSet* pRowSet;
			hr = HrQueryAllRows( pRecipientTable, 
								(SPropTagArray*)&columns,
								NULL, NULL, 0, &pRowSet);
			if ( !FAILED(hr) )
			{
				if ( pRowSet->cRows > 0 )
				{
					int stringLength;
					int addressCount = 0;
					unsigned int arraySize = 0;
					char** addresses = new char* [pRowSet->cRows - 1];
					for (unsigned int row = 0; row < pRowSet->cRows; row++ )
					{
						// Gather the addresses
						stringLength = strlen( pRowSet->aRow[row].lpProps[0].Value.LPSZ ) + 1;
						addresses[addressCount] = new char[stringLength];
						strcpy( addresses[addressCount], pRowSet->aRow[row].lpProps[0].Value.LPSZ );
						arraySize += stringLength;
						addressCount++;
					}

					// Format and log addresses
					if ( arraySize )
					{
						unsigned int arrayIndex = 0;
						char* formattedArray = new char[arraySize];
						if ( formattedArray )
						{
							while( addressCount-- )
							{
								// reformat addresses into one big buffer
								strcpy( formattedArray + arrayIndex, addresses[addressCount] );
								arrayIndex += strlen( addresses[addressCount] );
								*(formattedArray + arrayIndex) = ',';
								arrayIndex++;
								// free addresses array
								delete addresses[addressCount];
							}
							arrayIndex--;
							*(formattedArray + arrayIndex) = 0;
							// Log message addresses
							LogContent( formattedArray, OL_LOG_ADDRESSES );
							delete formattedArray;
						}
					}
				}
				FreeProws( pRowSet );
			}
			pRecipientTable->Release();
			// Log message body
			LogBody( pMessage );
			// Log message attachments
			LogAttachments( pMessage );
		}
		// Release resources
		UlRelease( pMDB );
		UlRelease( pMessage );
	}
	return S_FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//    CMessageEvents::OnSubmit()
//
//    Parameters
//    lpeecb -- pointer to IExchExtCallback interface
//
//    Purpose
//    Called just before message data is written to MAPI.
//    
//    Return Value
//    S_OK Microsoft Exchange will consider the task handled
//    S_FALSE signals Exchange to continue calling extensions
//

STDMETHODIMP CMessageEvents::OnSubmit(LPEXCHEXTCALLBACK lpeecb)
{
	m_submittingMessage = true;
	return S_FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//    CMessageEvents::OnSubmitComplete()
//
//    Parameters
//    lpeecb -- pointer to IExchExtCallback interface
//
//    Purpose
//    Called after message has been submitted to MAPI.
//    
//    Return Value - none
//

STDMETHODIMP_ (VOID) CMessageEvents::OnSubmitComplete(LPEXCHEXTCALLBACK lpeecb, ULONG ulFlags)
{
	m_submittingMessage = false;
}

///////////////////////////////////////////////////////////////////////////////
//    CMessageEvents::OnCheckNames()
//
//    Parameters
//    lpeecb -- pointer to IExchExtCallback interface
//
//    Purpose
//    Called when user selects the Check Names button and just before message
//    is submitted to MAPI.
//    
//    Return Value
//    S_OK Microsoft Exchange will consider the task handled
//    S_FALSE signals Exchange to continue calling extensions
//

STDMETHODIMP CMessageEvents::OnCheckNames(LPEXCHEXTCALLBACK lpeecb)
{
	return S_FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//    CMessageEvents::OnCheckNamesComplete()
//
//    Parameters
//    lpeecb -- pointer to IExchExtCallback interface
//
//    Purpose
//    Called after exchange has completed resolving names in the message
//    recipients table.
//    
//    Return Value
//    S_OK Microsoft Exchange will consider the task handled
//    S_FALSE signals Exchange to continue calling extensions
//

STDMETHODIMP CMessageEvents::OnCheckNamesComplete(LPEXCHEXTCALLBACK lpeecb, ULONG ulFlags)
{
	return S_FALSE;
}

///////////////////////////////////////////////////////////////////////////////

CClientExtension::CClientExtension()
{
	m_referenceCount = 0;
	m_pMessageEvents = new CMessageEvents(this);
};

STDMETHODIMP CClientExtension::QueryInterface(REFIID riid,void** ppvObj)
{ 
    HRESULT hResult = S_OK;

    *ppvObj = NULL;

    if (( IID_IUnknown == riid) || ( IID_IExchExt == riid) )
    {
        *ppvObj = (LPUNKNOWN)this;
    }
    else if (IID_IExchExtMessageEvents == riid)
    {
        *ppvObj = (LPUNKNOWN) m_pMessageEvents;
    }
    else
        hResult = E_NOINTERFACE;

    if (NULL != *ppvObj)
        ((LPUNKNOWN)*ppvObj)->AddRef();

    return hResult;
}

///////////////////////////////////////////////////////////////////////////////
//    CClientExtension::Install()
//
//    Parameters
//    peecb     -- pointer to Exchange Extension callback function
//    context -- context code at time of being called.
//
//    Purpose
//    Called once for each new context that is entered.
//
//    Return Value
//    S_OK - the installation succeeded for the context
//    S_FALSE - deny the installation fo the extension for the context
//
STDMETHODIMP CClientExtension::Install( IExchExtCallback *pmecb, ULONG context, ULONG ulFlags )
{
	ULONG version;

	// Make sure this is the right major version 
	pmecb->GetVersion(&version, EECBGV_GETBUILDVERSION);
	if (EECBGV_BUILDVERSION_MAJOR != (version & EECBGV_BUILDVERSION_MAJOR_MASK))
		return S_FALSE;

	switch (context)
	{
		case EECONTEXT_SENDNOTEMESSAGE:
		case EECONTEXT_SENDPOSTMESSAGE:
		case EECONTEXT_SENDRESENDMESSAGE:
			return S_OK;
	}

	return S_FALSE;
}


// The sole purpose of ExchEntryPoint is to return a new instance
// of the Extension Interface to Outlook or Exchange.
LPEXCHEXT CALLBACK ExchEntryPoint()
{
   return new CClientExtension;
}

