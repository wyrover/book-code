/*-----------------------------------------------------------------------------
 * DriverDebug.cpp
 *-----------------------------------------------------------------------------
 * Window Vista 이후 버전에서는 메세지 출력이 보이지 않음. 아래명령으로 메세지 필터링
 * 마스크를 조정해야 보임
 * 
 *	kd> ed nt!Kd_IHVDRIVER_Mask 0xff 
 * 
 *-----------------------------------------------------------------------------
 * All rights reserved by somma (fixbrain@gmail.com, unsorted@msn.com)
 *-----------------------------------------------------------------------------
 * - 10.11.2010 created
**---------------------------------------------------------------------------*/
#include "DriverHeaders.h"
#include "DriverDebug.h"


#if DBG

// debug level and debug area mask
//
DWORD g_DebugLevel = /*DPFLTR_TRACE_LEVEL*/DPFLTR_INFO_LEVEL;

// Buffers for debug messages are allocated globally instead of 
// on a stack, therefore we need g_DebugBufferBusy flags to 
// protect access to them.
//
DWORD g_DebugBufferBusy[] = {0, 0, 0, 0, 0, 0, 0, 0}; 
CHAR  g_DebugBuffer[NUMBER_DEBUG_BUFFERS][1024];


/**	-----------------------------------------------------------------------
	\brief	debug message output routine

	\param	
		IN  level
				Debug level (DBG_ERR, DBG_INFO, etc..)
				
		IN  format
				Debug Message format

	\return			
		NONE
	\code	
	\endcode		
-------------------------------------------------------------------------*/
VOID drv_debug_print(IN UINT32 level, IN const char* function,IN const char* format, IN ...)
{
    ULONG i;
    va_list vaList;
    va_start(vaList, format);

    // check mask for debug area and debug level
	//
    if (level <= g_DebugLevel)
    {
        // find a free buffer
		//
        for (i = 0; i < NUMBER_DEBUG_BUFFERS; ++i)
        {
            if (InterlockedCompareExchange((LONG*)&g_DebugBufferBusy[i], 1, 0) == 0)
            {
                __try
				{
					if (TRUE != NT_SUCCESS( RtlStringCbVPrintfA(
										            g_DebugBuffer[i], 
										            sizeof(g_DebugBuffer[i]),
										            format,
										            vaList
										            )))
					{
						return;
					}
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
                    return;
				}


								
                if (DPFLTR_ERROR_LEVEL == level)
                {
                    DbgPrintEx(
						DPFLTR_IHVDRIVER_ID, 
						DPFLTR_ERROR_LEVEL, 
                        _driver_name"(IRQL %2.2d): [ERR ] %s(), %s\n", 
                        KeGetCurrentIrql(), 
						function, 
                        g_DebugBuffer[i]
                        ); 
                }
                else if (DPFLTR_WARNING_LEVEL == level)
                {
                    DbgPrintEx(
						DPFLTR_IHVDRIVER_ID, 
						DPFLTR_WARNING_LEVEL | DPFLTR_MASK,
						_driver_name"(IRQL %2.2d): [WARN] %s(), %s\n", 
                        KeGetCurrentIrql(), 
						function,                         
                        g_DebugBuffer[i]
                    );
                }
				else if (DPFLTR_TRACE_LEVEL == level)
				{
					DbgPrintEx(
						DPFLTR_IHVDRIVER_ID, 
						DPFLTR_TRACE_LEVEL | DPFLTR_MASK,
						_driver_name"(IRQL %2.2d): [TRCE] %s(), %s\n", 
						KeGetCurrentIrql(), 
						function,                         
						g_DebugBuffer[i]
					);					
				}
                else
                {
                    DbgPrintEx(
						DPFLTR_IHVDRIVER_ID, 
						DPFLTR_INFO_LEVEL |  DPFLTR_MASK, 
                        _driver_name"(IRQL %2.2d): [INFO] %s(), %s\n", 
                        KeGetCurrentIrql(), 
						function,                         
                        g_DebugBuffer[i]
                        );
                }

                InterlockedExchange((LONG*)&g_DebugBufferBusy[i], 0);
                break;
            }
        }
    }

    va_end(vaList);
}

#endif	// DBG