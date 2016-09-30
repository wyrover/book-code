// Add.cpp : Implementation of CAdd
#include "stdafx.h"
#include "ConnectionCOM.h"
#include "Add.h"

/////////////////////////////////////////////////////////////////////////////
// CAdd


STDMETHODIMP CAdd::Add(int a, int b)
{
    // TODO: Add your implementation code here
    Sleep(2000);// to simulate a long process]
    //ok process over now,lets notify the client now
    Fire_ExecutionOver(a + b);
    return S_OK;
}
