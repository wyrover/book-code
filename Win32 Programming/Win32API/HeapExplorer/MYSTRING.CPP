#include <stdafx.h>
#include "mystring.h"

HANDLE CMyData::heap = NULL;
DWORD CMyData::count = 0;
BOOL CMyData::auto_deallocate = FALSE;

/****************************************************************************
*                              CMyData::CMyData
* Effect: 
*       Initializes a data object
****************************************************************************/

CMyData::CMyData()
    {
     s = NULL;
    }

/****************************************************************************
*                              CMyData::~CMyData
* Effect: 
*       Deletes the storage allocated to the data object
****************************************************************************/

CMyData::~CMyData()
    {
     if(s != NULL)
        { /* deallocate */
	 HeapFree(heap, 0, (LPVOID)s);
	} /* deallocate */
     s = NULL;
    }

/****************************************************************************
*                           CMyData::operator new
* Inputs:
*       size_t size: Size
*	LPCSTR name: File name
*	int line: Line number
* Result: void *
*       Pointer to allocated storage
* Effect: 
*       Allocates the storage
****************************************************************************/

void * CMyData::operator new(size_t size, LPCSTR name, int line)
    {
     // for now, this doesn't do anything special about debug info
     return CMyData:: operator new(size);
    }


/****************************************************************************
*			    CMyData::operator new
* Inputs:
*       size_t size: Size to allocate
* Result: LPVOID
*       Pointer to the storage, or an exception
* Effect: 
*       Allocates storage for an object
****************************************************************************/

LPVOID CMyData::operator new(size_t size)
    {
     if(heap == NULL)
        { /* allocate heap */
	 heap = HeapCreate(0, 0, 0);
	 if(heap == NULL)
	    AfxThrowMemoryException();
	} /* allocate heap */
     
     LPVOID p = HeapAlloc(heap, 0, size);
     if(p == NULL)
	AfxThrowMemoryException();
     count++;
     return p;
    }

/****************************************************************************
*			       CMyData::operator delete
* Inputs:
*       LPVOID p: Pointer to string to be deleted
* Result: void
*       
* Effect: 
*       Frees up the storage allocated to the object
* Notes:
*	Destructor has already been called to free internal resources.
****************************************************************************/

void CMyData::operator delete(LPVOID p)
    {
     if(!HeapFree(heap, 0, p))
	AfxThrowMemoryException();
     count--;
     if(count == 0 && auto_deallocate)
        { /* deallocate */
	 if(!HeapDestroy(heap))
	    AfxThrowMemoryException();
	 heap = NULL;
	} /* deallocate */
    }

/****************************************************************************
*                         CMyData::setAutoDeallocate
* Inputs:
*       BOOL automode: TRUE to automatically deallocate on count == 0
* Result: BOOL
*       Previous setting
* Effect: 
*       Sets the autodeallocate flage
****************************************************************************/

BOOL CMyData::setAutoDeallocate(BOOL automode)
    {
     BOOL result = auto_deallocate;
     auto_deallocate = automode;
     return result;
    }

/****************************************************************************
*                         CMyData::getAutoDeallocate
* Inputs:
*       BOOL automode: TRUE to automatically deallocate on count == 0
* Result: BOOL
*       Previous setting
* Effect: 
*       Sets the autodeallocate flage
****************************************************************************/

BOOL CMyData::getAutoDeallocate()
    {
     return auto_deallocate;
    }

/****************************************************************************
*                          CMyData::getHeapHandle
* Result: HANDLE
*       Heap handle for debugging purposes
****************************************************************************/

HANDLE CMyData::getHeapHandle()
    {
     return heap;
    }

/****************************************************************************
*                             CMyData::setString
* Inputs:
*       LPCTSTR str: String to set, or NULL to free the string
* Result: void
*       
* Effect: 
*       Sets the string
* Exceptions:
*	CMemoryException
****************************************************************************/

void CMyData::setString(LPCTSTR str)
    {
     LPTSTR p;

     if(str != NULL)
        { /* has string */
	 p = (LPTSTR)HeapAlloc(heap, 0, (lstrlen(str) + 1) * sizeof(TCHAR));
	 if(p == NULL)
	    AfxThrowMemoryException();
         lstrcpy(p, str);
	} /* has string */
     else
        { /* no string */
	 p = NULL;
	} /* no string */
     if(s == NULL)
	HeapFree(heap, 0, (LPVOID)s);
     s = p;
    }

/****************************************************************************
*                             CMyData::getString
* Result: LPCTSTR
*       Pointer to string, or NULL if string not set
****************************************************************************/

LPCTSTR CMyData::getString()
    {
     return s;
    }
