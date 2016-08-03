/******************************************************************************
Module:  ChainParser.h
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#pragma once


///////////////////////////////////////////////////////////////////////////////


#include "..\CommonFiles\CmnHdr.h"     /* See Appendix A. */
#include "..\CommonFiles\WaitChainTraversal.h"
#include <windowsx.h>
#include <tchar.h>
#include <StrSafe.h>


///////////////////////////////////////////////////////////////////////////////


class CChainParser : public CWCT
{
public:
   CChainParser(HWND hEdit);

protected:
   // Implement this method to be called before each thread is analyzed
   // Note: if nodeCount is 0, it was not possible to analyze this thread
   virtual void OnThread(DWORD TID, BOOL bDeadlock, DWORD nodeCount);
   
   // Implement this method to be called for each wait node
   virtual void OnChainNodeInfo(DWORD rootTID, DWORD currentNode, 
      WAITCHAIN_NODE_INFO nodeInfo);

private:
   void AddText(PCTSTR pszFormat, ...);
   LPCTSTR GetWCTObjectType(WCT_OBJECT_TYPE objectType);
   LPCTSTR GetWCTObjectStatus(WCT_OBJECT_STATUS objectStatus);
   void GetLastErrorMessage(LPTSTR szMsg, size_t cchLength);

private:
   HWND _hEdit;
};


///////////////////////////////////////////////////////////////////////////////


CChainParser::CChainParser(HWND hEdit) {

   _hEdit = hEdit;
}


void CChainParser::OnThread(DWORD TID, BOOL bDeadlock, DWORD nodeCount) {

   // Check that it is possible to parse the chain for this thread
   if (nodeCount == 0) {
      // Get the reason of the failure
      TCHAR szMsg[1024];
      GetLastErrorMessage(szMsg, _countof(szMsg));

      AddText(TEXT("..Thread %4u - Error\r\n    %s\r\n"), TID, szMsg);
   } else {
      AddText(TEXT("..Thread %4u %s\r\n"), TID, bDeadlock ? TEXT("is in a deadlock") : TEXT(""));
   }
}


void CChainParser::OnChainNodeInfo(DWORD rootTID, DWORD currentNode, 
   WAITCHAIN_NODE_INFO nodeInfo) {
   
   // Add a separating space 
   if (currentNode == 0)
      AddText(TEXT("----------------------------\r\n"));

   // This is a thread in the chain.
   // The next element should be the reason of the wait if any
   if (nodeInfo.ObjectType == WctThreadType) {
      // Show if the thread is from another process
      if (GetPID() != nodeInfo.ThreadObject.ProcessId) {
         AddText(TEXT("    [%u:%u -> %s] "),
            nodeInfo.ThreadObject.ProcessId,
            nodeInfo.ThreadObject.ThreadId,
            GetWCTObjectStatus(nodeInfo.ObjectStatus));
      } else {  // Otherwise, just show the thread ID
         AddText(TEXT("    [%u -> %s] "),
            nodeInfo.ThreadObject.ThreadId,
            GetWCTObjectStatus(nodeInfo.ObjectStatus));
      }
   } else {  // Other nodes are lock reasons
      // Show kernel objects UNICODE name if any
      if (nodeInfo.LockObject.ObjectName[0] != L'\0') {
#ifdef UNICODE
         AddText(TEXT("%s - %s  (%s)\r\n"),
            GetWCTObjectType(nodeInfo.ObjectType),
            nodeInfo.LockObject.ObjectName, 
            GetWCTObjectStatus(nodeInfo.ObjectStatus));
#else // we need to translate the UNICODE name into multi bytes
         int iCount = WideCharToMultiByte(CP_ACP, 0, 
            nodeInfo.LockObject.ObjectName, 
            (int)_tcslen(nodeInfo.LockObject.ObjectName), 
            NULL, 0, NULL, NULL);
         CHAR* pszName = (CHAR*)HeapAlloc(GetProcessHeap(), 0,
             iCount * sizeof(CHAR));
         WideCharToMultiByte(CP_ACP, 0, nodeInfo.LockObject.ObjectName, 
            (int)_tcslen(nodeInfo.LockObject.ObjectName), pszName, iCount, 
            NULL, NULL);
         AddText(TEXT("%s - %s  (%s)\r\n"),
            GetWCTObjectType(nodeInfo.ObjectType), pszName, 
            GetWCTObjectStatus(nodeInfo.ObjectStatus));
         HeapFree(GetProcessHeap(), 0, pszName);
#endif
      } else {
         // nameless object or reason
         AddText(TEXT("%s  (%s)\r\n"), 
            GetWCTObjectType(nodeInfo.ObjectType),
            GetWCTObjectStatus(nodeInfo.ObjectStatus));
      }
   }
   
   // Change line at the end of the chain
   if (GetNodesInChain() == currentNode + 1) {
      AddText(TEXT("\r\n\r\n"));
   }
}



///////////////////////////////////////////////////////////////////////////////


LPCTSTR CChainParser::GetWCTObjectType(WCT_OBJECT_TYPE objectType) {

   switch(objectType) {
      case WctCriticalSectionType:
         return(TEXT("CriticalSection"));
      break;

      case WctSendMessageType:
         return(TEXT("SendMessage"));
      break;

      case WctMutexType:
         return(TEXT("Mutex"));
      break;

      case WctAlpcType:
         return(TEXT("Alpc"));
      break;

      case WctComType:
         return(TEXT("COM"));
      break;

      case WctThreadWaitType:
         return(TEXT("ThreadWait"));
      break;

      case WctProcessWaitType:
         return(TEXT("ProcessWait"));
      break;

      case WctThreadType:
         return(TEXT("Thread"));
      break;

      case WctComActivationType:
         return(TEXT("COMActivation"));
      break;

      case WctUnknownType:
         return(TEXT("Unknown"));
      break;

      default:
         return(TEXT("???"));
      break;
   }
}


LPCTSTR CChainParser::GetWCTObjectStatus(WCT_OBJECT_STATUS objectStatus) {

   switch(objectStatus) {
      case WctStatusNoAccess:  // ACCESS_DENIED for this object
         return(TEXT("AccessDenied"));
      break;

      case WctStatusRunning:  // Thread status
         return(TEXT("Running"));
      break;

      case WctStatusBlocked:  // Thread status
         return(TEXT("Blocked"));
      break;

      case WctStatusPidOnly:  // Thread status
         return(TEXT("PidOnly"));
      break;

      case WctStatusPidOnlyRpcss:   // Thread status
         return(TEXT("PidOnlyRpcss"));
      break;

      case WctStatusOwned:  // Dispatcher object status
         return(TEXT("Owned"));
      break;

      case WctStatusNotOwned:  // Dispatcher object status
         return(TEXT("NotOwned"));
      break;

      case WctStatusAbandoned:  // Dispatcher object status
         return(TEXT("Abandoned"));
      break;

      case WctStatusUnknown:  // All objects
         return(TEXT("Unknown"));
      break;

      case WctStatusError:  // All objects
         return(TEXT("Error"));
      break;

      case WctStatusMax:  
         return(TEXT("?max?"));
      break;

      default:  
         return(TEXT("???"));
      break;
   }
}


void CChainParser::GetLastErrorMessage(LPTSTR szMsg, size_t cchLength) {

   DWORD dwLastError = GetLastError();

   HLOCAL hlocal = NULL;   // Buffer that gets the error message string

   // Use the default system locale since we look for Windows messages.
   // Note: this MAKELANGID combination has 0 as value
   DWORD systemLocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);

   // Get the error code's textual description
   BOOL fOk = FormatMessage(
      FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |
      FORMAT_MESSAGE_ALLOCATE_BUFFER, 
      NULL, dwLastError, systemLocale, 
      (PTSTR) &hlocal, 0, NULL);

   if (fOk && (hlocal != NULL)) {
      _tcscpy_s(szMsg, cchLength, (PCTSTR) LocalLock(hlocal));
      LocalFree(hlocal);
   } else {
      StringCchPrintf(szMsg, cchLength,  TEXT("Unknown error 0x%x"), dwLastError);
   }

   // Reset last error
   SetLastError(dwLastError);
}


void CChainParser::AddText(PCTSTR pszFormat, ...) {

   va_list argList;
   va_start(argList, pszFormat);

   TCHAR sz[20 * 1024];

   Edit_GetText(_hEdit, sz, _countof(sz));
   _vstprintf_s(
      _tcschr(sz, TEXT('\0')), _countof(sz) - _tcslen(sz), 
      pszFormat, argList);
   Edit_SetText(_hEdit, sz);
   va_end(argList);
}


//////////////////////////////// End of File //////////////////////////////////
