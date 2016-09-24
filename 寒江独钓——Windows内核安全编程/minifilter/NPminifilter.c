#include "NPminifilter.h"
//  Global variables

PFLT_FILTER gFilterHandle;
ULONG gTraceFlags = 0;

PFLT_FILTER gFilterHandle;
PFLT_PORT 	gServerPort;
PFLT_PORT 	gClientPort;

NPMINI_COMMAND gCommand = ENUM_PASS;

#define PT_DBG_PRINT( _dbgLevel, _string )          \
    (FlagOn(gTraceFlags,(_dbgLevel)) ?              \
        DbgPrint _string :                          \
        ((void)0))

BOOLEAN NPUnicodeStringToChar(PUNICODE_STRING UniName, char Name[])
{
    ANSI_STRING	AnsiName;			
    NTSTATUS	ntstatus;
    char*		nameptr;			
    
    __try {	    		   		    		
		ntstatus = RtlUnicodeStringToAnsiString(&AnsiName, UniName, TRUE);		
						
		if (AnsiName.Length < 260) {
			nameptr = (PCHAR)AnsiName.Buffer;
			//Convert into upper case and copy to buffer
			strcpy(Name, _strupr(nameptr));						    	    
			DbgPrint("NPUnicodeStringToChar : %s\n", Name);	
		}		  	
		RtlFreeAnsiString(&AnsiName);		 
	} 
	__except(EXCEPTION_EXECUTE_HANDLER) {
		DbgPrint("NPUnicodeStringToChar EXCEPTION_EXECUTE_HANDLER\n");	
		return FALSE;
	}
    return TRUE;
}      

NTSTATUS
NPInstanceSetup (
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in FLT_INSTANCE_SETUP_FLAGS Flags,
    __in DEVICE_TYPE VolumeDeviceType,
    __in FLT_FILESYSTEM_TYPE VolumeFilesystemType
    )
{
    UNREFERENCED_PARAMETER( FltObjects );
    UNREFERENCED_PARAMETER( Flags );
    UNREFERENCED_PARAMETER( VolumeDeviceType );
    UNREFERENCED_PARAMETER( VolumeFilesystemType );

    PAGED_CODE();

    PT_DBG_PRINT( PTDBG_TRACE_ROUTINES,
                  ("NPminifilter!NPInstanceSetup: Entered\n") );

    return STATUS_SUCCESS;
}


NTSTATUS
NPInstanceQueryTeardown (
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in FLT_INSTANCE_QUERY_TEARDOWN_FLAGS Flags
    )
{
    UNREFERENCED_PARAMETER( FltObjects );
    UNREFERENCED_PARAMETER( Flags );

    PAGED_CODE();

    PT_DBG_PRINT( PTDBG_TRACE_ROUTINES,
                  ("NPminifilter!NPInstanceQueryTeardown: Entered\n") );

    return STATUS_SUCCESS;
}


VOID
NPInstanceTeardownStart (
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in FLT_INSTANCE_TEARDOWN_FLAGS Flags
    )
{
    UNREFERENCED_PARAMETER( FltObjects );
    UNREFERENCED_PARAMETER( Flags );

    PAGED_CODE();

    PT_DBG_PRINT( PTDBG_TRACE_ROUTINES,
                  ("NPminifilter!NPInstanceTeardownStart: Entered\n") );
}


VOID
NPInstanceTeardownComplete (
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in FLT_INSTANCE_TEARDOWN_FLAGS Flags
    )
{
    UNREFERENCED_PARAMETER( FltObjects );
    UNREFERENCED_PARAMETER( Flags );

    PAGED_CODE();

    PT_DBG_PRINT( PTDBG_TRACE_ROUTINES,
                  ("NPminifilter!NPInstanceTeardownComplete: Entered\n") );
}


/*************************************************************************
    MiniFilter initialization and unload routines.
*************************************************************************/

NTSTATUS
DriverEntry (
    __in PDRIVER_OBJECT DriverObject,
    __in PUNICODE_STRING RegistryPath
    )
{
    NTSTATUS status;
    PSECURITY_DESCRIPTOR sd;
    OBJECT_ATTRIBUTES oa;
    UNICODE_STRING uniString;		//for communication port name
    
    UNREFERENCED_PARAMETER( RegistryPath );

    PT_DBG_PRINT( PTDBG_TRACE_ROUTINES,
                  ("NPminifilter!DriverEntry: Entered\n") );

    //
    //  Register with FltMgr to tell it our callback routines
    //

    status = FltRegisterFilter( DriverObject,
                                &FilterRegistration,
                                &gFilterHandle );

    ASSERT( NT_SUCCESS( status ) );

    if (NT_SUCCESS( status )) {

        //
        //  Start filtering i/o
        //

        status = FltStartFiltering( gFilterHandle );

        if (!NT_SUCCESS( status )) {

            FltUnregisterFilter( gFilterHandle );
        }
    }
		//Communication Port
    status  = FltBuildDefaultSecurityDescriptor( &sd, FLT_PORT_ALL_ACCESS );
    
    if (!NT_SUCCESS( status )) {
       	goto final;
    }


    status  = FltBuildDefaultSecurityDescriptor( &sd, FLT_PORT_ALL_ACCESS );

    if (!NT_SUCCESS( status )) {
        goto final;
    }                                 
                     
    RtlInitUnicodeString( &uniString, MINISPY_PORT_NAME );

    InitializeObjectAttributes( &oa,
                                &uniString,
                                OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE,
                                NULL,
                                sd );

    status = FltCreateCommunicationPort( gFilterHandle,
                                         &gServerPort,
                                         &oa,
                                         NULL,
                                         NPMiniConnect,
                                         NPMiniDisconnect,
                                         NPMiniMessage,
                                         1 );

    FltFreeSecurityDescriptor( sd );

    if (!NT_SUCCESS( status )) {
        goto final;
    }            

final :
    
    if (!NT_SUCCESS( status ) ) {

         if (NULL != gServerPort) {
             FltCloseCommunicationPort( gServerPort );
         }

         if (NULL != gFilterHandle) {
             FltUnregisterFilter( gFilterHandle );
         }       
    } 
    return status;
}

NTSTATUS
NPUnload (
    __in FLT_FILTER_UNLOAD_FLAGS Flags
    )
{
    UNREFERENCED_PARAMETER( Flags );

    PAGED_CODE();

    PT_DBG_PRINT( PTDBG_TRACE_ROUTINES,
                  ("NPminifilter!NPUnload: Entered\n") );

		FltCloseCommunicationPort( gServerPort );
		
    FltUnregisterFilter( gFilterHandle );

    return STATUS_SUCCESS;
}


/*************************************************************************
    MiniFilter callback routines.
*************************************************************************/

FLT_PREOP_CALLBACK_STATUS
NPPreCreate (
    __inout PFLT_CALLBACK_DATA Data,
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __deref_out_opt PVOID *CompletionContext
    )
{
		char FileName[260] = "X:";
		
		NTSTATUS status;
		PFLT_FILE_NAME_INFORMATION nameInfo;

    UNREFERENCED_PARAMETER( FltObjects );
    UNREFERENCED_PARAMETER( CompletionContext );

    PAGED_CODE();        

		__try {	                             								
		    status = FltGetFileNameInformation( Data,
		                                        FLT_FILE_NAME_NORMALIZED |
		                                            FLT_FILE_NAME_QUERY_DEFAULT,
		                                        &nameInfo );
		    if (NT_SUCCESS( status )) {
		    		//判斷是否阻擋
		    		if (gCommand == ENUM_BLOCK) {
						    FltParseFileNameInformation( nameInfo );
								if (NPUnicodeStringToChar(&nameInfo->Name, FileName)) {		
									
								    if (strstr(FileName, "NOTEPAD.EXE") > 0) {
								    		
												Data->IoStatus.Status = STATUS_ACCESS_DENIED;
												Data->IoStatus.Information = 0;
												FltReleaseFileNameInformation( nameInfo );
												return FLT_PREOP_COMPLETE;						    		
								    }																			
								}						 
						}   			    
				    //release resource
				    FltReleaseFileNameInformation( nameInfo );        
		    }                                    
		}
		__except(EXCEPTION_EXECUTE_HANDLER) {
		    DbgPrint("NPPreCreate EXCEPTION_EXECUTE_HANDLER\n");				
		}

    return FLT_PREOP_SUCCESS_WITH_CALLBACK;
}


FLT_POSTOP_CALLBACK_STATUS
NPPostCreate (
    __inout PFLT_CALLBACK_DATA Data,
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in_opt PVOID CompletionContext,
    __in FLT_POST_OPERATION_FLAGS Flags
    )
{
    FLT_POSTOP_CALLBACK_STATUS returnStatus = FLT_POSTOP_FINISHED_PROCESSING;
    PFLT_FILE_NAME_INFORMATION nameInfo;
    NTSTATUS status;

    UNREFERENCED_PARAMETER( CompletionContext );
    UNREFERENCED_PARAMETER( Flags );
		
    //
    //  If this create was failing anyway, don't bother scanning now.
    //

    if (!NT_SUCCESS( Data->IoStatus.Status ) ||
        (STATUS_REPARSE == Data->IoStatus.Status)) {

        return FLT_POSTOP_FINISHED_PROCESSING;
    }

    //
    //  Check if we are interested in this file.
    //

    status = FltGetFileNameInformation( Data,
                                        FLT_FILE_NAME_NORMALIZED |
                                            FLT_FILE_NAME_QUERY_DEFAULT,
                                        &nameInfo );

    if (!NT_SUCCESS( status )) {

        return FLT_POSTOP_FINISHED_PROCESSING;
    }
    		
    return returnStatus;
}

//與user application Conect
NTSTATUS
NPMiniConnect(
    __in PFLT_PORT ClientPort,
    __in PVOID ServerPortCookie,
    __in_bcount(SizeOfContext) PVOID ConnectionContext,
    __in ULONG SizeOfContext,
    __deref_out_opt PVOID *ConnectionCookie
    )
{
		DbgPrint("[mini-filter] NPMiniConnect");
    PAGED_CODE();

    UNREFERENCED_PARAMETER( ServerPortCookie );
    UNREFERENCED_PARAMETER( ConnectionContext );
    UNREFERENCED_PARAMETER( SizeOfContext);
    UNREFERENCED_PARAMETER( ConnectionCookie );

    ASSERT( gClientPort == NULL );
    gClientPort = ClientPort;
    return STATUS_SUCCESS;
}

//與user application Disconect
VOID
NPMiniDisconnect(
    __in_opt PVOID ConnectionCookie
   )
{		
    PAGED_CODE();
    UNREFERENCED_PARAMETER( ConnectionCookie );
		DbgPrint("[mini-filter] NPMiniDisconnect");
    
    //  Close our handle
    FltCloseClientPort( gFilterHandle, &gClientPort );
}

NTSTATUS
NPMiniMessage (
    __in PVOID ConnectionCookie,
    __in_bcount_opt(InputBufferSize) PVOID InputBuffer,
    __in ULONG InputBufferSize,
    __out_bcount_part_opt(OutputBufferSize,*ReturnOutputBufferLength) PVOID OutputBuffer,
    __in ULONG OutputBufferSize,
    __out PULONG ReturnOutputBufferLength
    )
{
		
    NPMINI_COMMAND command;
    NTSTATUS status;

    PAGED_CODE();

    UNREFERENCED_PARAMETER( ConnectionCookie );
    UNREFERENCED_PARAMETER( OutputBufferSize );
    UNREFERENCED_PARAMETER( OutputBuffer );
    
		DbgPrint("[mini-filter] NPMiniMessage");
    
    //                      **** PLEASE READ ****    
    //  The INPUT and OUTPUT buffers are raw user mode addresses.  The filter
    //  manager has already done a ProbedForRead (on InputBuffer) and
    //  ProbedForWrite (on OutputBuffer) which guarentees they are valid
    //  addresses based on the access (user mode vs. kernel mode).  The
    //  minifilter does not need to do their own probe.
    //  The filter manager is NOT doing any alignment checking on the pointers.
    //  The minifilter must do this themselves if they care (see below).
    //  The minifilter MUST continue to use a try/except around any access to
    //  these buffers.    

    if ((InputBuffer != NULL) &&
        (InputBufferSize >= (FIELD_OFFSET(COMMAND_MESSAGE,Command) +
                             sizeof(NPMINI_COMMAND)))) {

        try  {
            //  Probe and capture input message: the message is raw user mode
            //  buffer, so need to protect with exception handler
            command = ((PCOMMAND_MESSAGE) InputBuffer)->Command;

        } except( EXCEPTION_EXECUTE_HANDLER ) {

            return GetExceptionCode();
        }

        switch (command) {
            //開放規則
            case ENUM_PASS:
            {            		
            		DbgPrint("[mini-filter] ENUM_PASS");
								gCommand = ENUM_PASS;
								status = STATUS_SUCCESS;         
            		break;
            }
            //阻擋規則
            case ENUM_BLOCK:
            {            		
            		DbgPrint("[mini-filter] ENUM_BLOCK");
								gCommand = ENUM_BLOCK;
								status = STATUS_SUCCESS;                          
            		break;
            }               		
            
            default:
            		DbgPrint("[mini-filter] default");
                status = STATUS_INVALID_PARAMETER;
                break;
        }
    } else {

        status = STATUS_INVALID_PARAMETER;
    }

    return status;
}