/*++
Copyright (c) Advanced Windows Debugging (ISBN 0321374460) from Addison-Wesley Professional.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/
#include "sysexts.h"
#include "..\\bstree\\bstree.h"

#define SYSEXTS_KNOWNSTRUCT_1  "CBinaryTree::_TreeNode"
#define SYSEXTS_KNOWNSTRUCT_OUT "\nBinary tree node contents\n  Data: %d\n  Left Child Pointer: 0x%x\n  Right Child Pointer: 0x%x\n"

// Helper function prototypes
VOID InOrderTraversal(ULONG_PTR pNode);
VOID PrintNode(ULONG ulLeft, ULONG ulData, ULONG ulRight);
HRESULT GetNodeValues(ULONG64 pNode, ULONG* pulData, ULONG* pulLeft, ULONG* pulRight);
VOID ReleaseComPointers();

BOOL bInitialized=FALSE;

// Required COM interfaces
IDebugClient* pDebugClient=NULL ;
IDebugControl* pDebugControl=NULL ;
IDebugSymbols* pSymbols=NULL;
IDebugSymbols3* pSymbols3=NULL;
IDebugDataSpaces* pDataSpaces=NULL;

// Globals
ULONG ulLeftOffset=0;
ULONG ulRightOffset=0;
ULONG ulDataOffset=0;
ULONG pSize=NULL;
ULONG pSizeData=NULL;
ULONG pSizeChildPtr=NULL;
ULONG pNodeIndex=NULL;
ULONG64 pBase=NULL;

extern "C" HRESULT CALLBACK DebugExtensionInitialize(PULONG Version, PULONG Flags)
{
    HRESULT hRes=E_FAIL;

    if(bInitialized)
    {
        return S_OK;
    }

    //
    // Initialize the version information
    //
    *Version=DEBUG_EXTENSION_VERSION(1, 0);
    *Flags=0;

    //
    // Initialize required COM interface pointers
    //
    if(FAILED(hRes=DebugCreate(__uuidof(IDebugClient), 
                               (void**) &pDebugClient)))
    {
        return hRes;
    }

    if(FAILED(hRes=pDebugClient->QueryInterface(__uuidof(IDebugControl), (void**) &pDebugControl)))
    {
        ReleaseComPointers();
        return hRes;
    }

    //
    // Initialize WinDbg extension data
    //
    ExtensionApis.nSize=sizeof(ExtensionApis);
    hRes=pDebugControl->GetWindbgExtensionApis64(&ExtensionApis);

    if(FAILED((hRes=pDebugClient->QueryInterface(__uuidof(IDebugDataSpaces), (void**) &pDataSpaces))))
    {
        dprintf( "Failed to get required COM interface\n");
        ReleaseComPointers();
        return hRes;
    }
       
    if(FAILED(hRes=pDebugClient->QueryInterface(__uuidof(IDebugSymbols), (void**) &pSymbols)))
    {
        dprintf( "Failed to get required COM interface\n");
        ReleaseComPointers();
        return hRes;
    }

    if(FAILED(hRes=pDebugClient->QueryInterface(__uuidof(IDebugSymbols3), (void**) &pSymbols3)))
    {
        dprintf( "Failed to get required COM interface\n");
        ReleaseComPointers();        
        return hRes;
    }

    //
    // Initialize type information
    //
    if(FAILED(hRes=pSymbols->GetModuleByModuleName("11bstree", 
                                                   0, 
                                                   NULL, 
                                                   &pBase)))
    {
        dprintf("Failed to get module information for bstree.exe\n");
        ReleaseComPointers();        
        return hRes;
    }
    if(FAILED(hRes=pSymbols3->GetTypeId(pBase, 
                                        "CBinaryTree::_TreeNode",    
                                        &pNodeIndex)))
    {
        dprintf("Failed to get type id\n");
        ReleaseComPointers();        
        return hRes;
    }
    if(FAILED(hRes=pSymbols->GetTypeSize(pBase, 
                                         pNodeIndex, 
                                         &pSize)))
    {
        dprintf("Failed to get type size\n");
        ReleaseComPointers();        
        return hRes;
    }

    if(FAILED(hRes=pSymbols->GetFieldOffset(pBase, 
                                            pNodeIndex, 
                                            "pLeftChild", 
                                            &ulLeftOffset)))
    {
        dprintf("Failed to get left child offset\n");			
        ReleaseComPointers();        
        return hRes;
    }

    if(FAILED(hRes=pSymbols->GetFieldOffset(pBase, 
                                            pNodeIndex, 
                                            "pRightChild", 
                                            &ulRightOffset)))
    {
        dprintf("Failed to get right child offset\n");			
        ReleaseComPointers();        
        return hRes;
    }

    if(FAILED(hRes=pSymbols->GetFieldOffset(pBase, 
                                            pNodeIndex, 
                                            "data", 
                                            &ulDataOffset)))
    {
        dprintf("Failed to get data offset\n");			
        ReleaseComPointers();        
        return hRes;
    }

    bInitialized=TRUE;
    hRes=S_OK;

    return hRes;
}


extern "C" void CALLBACK DebugExtensionUninitialize(void)
{
    ReleaseComPointers();
    return;
}


HRESULT CALLBACK help(PDEBUG_CLIENT Client, PCSTR Args)
{
	dprintf("SYSYEXTS.DLL commands:\n"
			"\thelp                  = Shows this help\n"
            "\tdumptree <address>    = Traverses a binary tree. Address must point to the root node of the tree");
	
	return S_OK;
}

HRESULT CALLBACK dumptree(PDEBUG_CLIENT pClient, PCSTR szArgs)
{
    HRESULT hRes=S_OK;

    ULONG_PTR pAddress=(ULONG_PTR) GetExpression(szArgs);
    if(!pAddress)
    {
        dprintf("Invalid head pointer address specified: %s\n", szArgs);
        return E_FAIL;
    }

    InOrderTraversal(pAddress);
    return hRes;
}



VOID InOrderTraversal(ULONG_PTR pNode)
{
	HRESULT hRes=S_OK;
    PULONG pOffset=NULL;
    BYTE* pBuffer=NULL;
    ULONG ulData=0;
    ULONG ulLeft=0;
    ULONG ulRight=0;

    if(CheckControlC()==TRUE)
    {
        dprintf ("Control C hit, canceling command\n" );
        return;
    }

    if(!pNode)
    {
        return;
    }

    if(FAILED(hRes=GetNodeValues(pNode, &ulData, &ulLeft, &ulRight)))
    {
		return;
    }

	if(ulLeft)
	{
		InOrderTraversal(ulLeft);
		delete[] pBuffer;			
	}

	PrintNode(ulLeft, ulData, ulRight);

	if(ulRight)
	{
		InOrderTraversal(ulRight);
		delete[] pBuffer;
	}			
}

HRESULT GetNodeValues(ULONG64 pNode, ULONG* pulData, ULONG* pulLeft, ULONG* pulRight)
{
    HRESULT hRes = S_OK;
    if(FAILED(hRes=pDataSpaces->ReadVirtual((ULONG_PTR) pNode+ulDataOffset, pulData, sizeof(ULONG), NULL)))
    {
		dprintf("Failed to read memory at address: 0x%X\n", pNode+ulDataOffset);
        return E_FAIL;
	}    

	if(FAILED(hRes=pDataSpaces->ReadVirtual((ULONG_PTR) pNode+ulLeftOffset, pulLeft, sizeof(ULONG), NULL)))
    {
		dprintf("Failed to read memory at address: 0x%X\n", pNode+ulLeftOffset);
        return E_FAIL;
	}    

    if(FAILED(hRes=pDataSpaces->ReadVirtual((ULONG_PTR) pNode+ulRightOffset, pulRight, sizeof(ULONG), NULL)))
    {
		dprintf("Failed to read memory at address: 0x%X\n", pNode+ulRightOffset);
        return E_FAIL;
	}    

    return S_OK;
}

VOID PrintNode(ULONG ulLeft, ULONG ulData, ULONG ulRight)
{
	dprintf("** Node **\n");
	dprintf("   Data: %d\n", ulData);
	dprintf("   Left child pointer: 0x%x\n", ulLeft);
	dprintf("   Right child pointer: 0x%x\n", ulRight);	
}


HRESULT CALLBACK KnownStructOutput (
    IN ULONG  Flag,
    IN ULONG64  Address,
    IN PSTR  StructName,
    OUT PSTR  Buffer,
    IN OUT PULONG  BufferSize
    )
{
  HRESULT hRes=E_FAIL;

    if(Flag==DEBUG_KNOWN_STRUCT_GET_NAMES)
    {
        if ((*BufferSize)<strlen(SYSEXTS_KNOWNSTRUCT_1)+2)
        {
            // Not enough buffer available, return S_FALSE
            (*BufferSize)=strlen(SYSEXTS_KNOWNSTRUCT_1)+2;
            hRes=S_FALSE;
        }
        else
        {
            hRes=StringCchPrintfA(Buffer,(*BufferSize)-2,"%s\0",SYSEXTS_KNOWNSTRUCT_1 ); 
            if (FAILED(hRes))
            {
              dprintf ("Failed to copy the data type name into buffer\n");
            }
        }
    }
  else if(Flag==DEBUG_KNOWN_STRUCT_GET_SINGLE_LINE_OUTPUT)
  {
    ULONG ulData=0;
    ULONG ulLeft=0;
    ULONG ulRight=0;

    if(FAILED(hRes=GetNodeValues(Address, &ulData, &ulLeft, &ulRight)))
    {
		return hRes;
    }    

    DWORD dwLen=_scprintf(SYSEXTS_KNOWNSTRUCT_OUT, 
                          ulData, 
                          ulLeft, 
                          ulRight)+1; 

    if(dwLen>(*BufferSize))
    {
      dprintf("KnownStructOutput unable to fit return data into buffer\n");
      hRes=E_FAIL;
      return hRes;
    }

    hRes=StringCchPrintfA(Buffer, 
                        dwLen, 
                        SYSEXTS_KNOWNSTRUCT_OUT, 
                        ulData, 
                        ulLeft, 
                        ulRight);
    if(FAILED(hRes))
    {
        dprintf ("KnownStructOutput unable to write data into buffer\n");	
    }
  }
  else if(Flag==DEBUG_KNOWN_STRUCT_SUPPRESS_TYPE_NAME)
  {
        return S_OK;
  }
  else 
  {
    dprintf("KnownStructOutput called with invalid flags\n");
  }
  return hRes;
}

VOID ReleaseComPointers()
{
    if(pDebugClient) pDebugClient->Release();
    if(pDebugControl) pDebugControl->Release();
    if(pSymbols) pSymbols->Release();
    if(pSymbols3) pSymbols3->Release();
    if(pDataSpaces) pDataSpaces->Release();
}