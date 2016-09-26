// Implementation of the CCarusoeProvCommand
#include "stdafx.h"
#include "ATL_OLEDB_Prov.h"
#include "CarusoeProvRS.h"
/////////////////////////////////////////////////////////////////////////////
// CCarusoeProvCommand
HRESULT CCarusoeProvCommand::Execute(IUnknown * pUnkOuter, REFIID riid, DBPARAMS * pParams,
                                     LONG * pcRowsAffected, IUnknown ** ppRowset)
{
    CCarusoeProvRowset* pRowset;
    return CreateRowset(pUnkOuter, riid, pParams, pcRowsAffected, ppRowset, pRowset);
}
