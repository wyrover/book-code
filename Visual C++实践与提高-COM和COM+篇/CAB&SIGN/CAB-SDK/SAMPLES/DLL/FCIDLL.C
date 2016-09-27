/*
 *  FCIDLL.C -- FCI interface using CABINET.DLL
 *
 *  Copyright (C) Microsoft Corporation 1997
 *  All Rights Reserved.
 *
 *  Overview:
 *      This code is a wrapper which provides access to the actual FCI code
 *      in CABINET.DLL.  CABINET.DLL dynamically loads/unloads as needed.
 */

#include <windows.h>

#include "fci.h"

static HINSTANCE hCabinetDll;   /* DLL module handle */

/* pointers to the functions in the DLL */

static HFCI(FAR DIAMONDAPI *pfnFCICreate)(
    PERF                perf,
    PFNFCIFILEPLACED    pfnfiledest,
    PFNFCIALLOC         pfnalloc,
    PFNFCIFREE          pfnfree,
    PFNFCIOPEN          pfnopen,
    PFNFCIREAD          pfnread,
    PFNFCIWRITE         pfnwrite,
    PFNFCICLOSE         pfnclose,
    PFNFCISEEK          pfnseek,
    PFNFCIDELETE        pfndelete,
    PFNFCIGETTEMPFILE   pfntemp,
    PCCAB               pccab,
    void FAR *          pv);
static BOOL (FAR DIAMONDAPI *pfnFCIAddFile)(
    HFCI                hfci,
    char                *pszSourceFile,
    char                *pszFileName,
    BOOL                fExecute,
    PFNFCIGETNEXTCABINET GetNextCab,
    PFNFCISTATUS        pfnProgress,
    PFNFCIGETOPENINFO   pfnOpenInfo,
    TCOMP               typeCompress);
static BOOL (FAR DIAMONDAPI *pfnFCIFlushCabinet)(
    HFCI                hfci,
    BOOL                fGetNextCab,
    PFNFCIGETNEXTCABINET GetNextCab,
    PFNFCISTATUS        pfnProgress);
static BOOL (FAR DIAMONDAPI *pfnFCIFlushFolder)(
    HFCI                hfci,
    PFNFCIGETNEXTCABINET GetNextCab,
    PFNFCISTATUS        pfnProgress);
static BOOL (FAR DIAMONDAPI *pfnFCIDestroy)(
    HFCI                hfci);


/*
 *  FCICreate -- Create an FCI context
 *
 *  See fci.h for entry/exit conditions.
 */

HFCI DIAMONDAPI FCICreate(PERF              perf,
                          PFNFCIFILEPLACED  pfnfiledest,
                          PFNFCIALLOC       pfnalloc,
                          PFNFCIFREE        pfnfree,
                          PFNFCIOPEN        pfnopen,
                          PFNFCIREAD        pfnread,
                          PFNFCIWRITE       pfnwrite,
                          PFNFCICLOSE       pfnclose,
                          PFNFCISEEK        pfnseek,
                          PFNFCIDELETE      pfndelete,
                          PFNFCIGETTEMPFILE pfntemp,
                          PCCAB             pccab,
                          void FAR *        pv)
{
    HFCI hfci;
    hCabinetDll = LoadLibrary("CABINET");

    if (hCabinetDll == NULL) {
        return (NULL);
    }

    pfnFCICreate = (void *) GetProcAddress(hCabinetDll, "FCICreate");
    pfnFCIAddFile = (void *) GetProcAddress(hCabinetDll, "FCIAddFile");
    pfnFCIFlushCabinet = (void *) GetProcAddress(hCabinetDll, "FCIFlushCabinet");
    pfnFCIFlushFolder = (void *) GetProcAddress(hCabinetDll, "FCIFlushFolder");
    pfnFCIDestroy = (void *) GetProcAddress(hCabinetDll, "FCIDestroy");

    if ((pfnFCICreate == NULL) ||
        (pfnFCIAddFile == NULL) ||
        (pfnFCIFlushCabinet == NULL) ||
        (pfnFCIDestroy == NULL)) {
        FreeLibrary(hCabinetDll);
        return (NULL);
    }

    hfci = pfnFCICreate(perf, pfnfiledest, pfnalloc, pfnfree,
                        pfnopen, pfnread, pfnwrite, pfnclose, pfnseek, pfndelete, pfntemp,
                        pccab, pv);

    if (hfci == NULL) {
        FreeLibrary(hCabinetDll);
    }

    return (hfci);
}


/*
 *  FCIAddFile -- Add file to cabinet
 *
 *  See fci.h for entry/exit conditions.
 */

BOOL DIAMONDAPI FCIAddFile(HFCI                  hfci,
                           char                 *pszSourceFile,
                           char                 *pszFileName,
                           BOOL                  fExecute,
                           PFNFCIGETNEXTCABINET  GetNextCab,
                           PFNFCISTATUS          pfnProgress,
                           PFNFCIGETOPENINFO     pfnOpenInfo,
                           TCOMP                 typeCompress)
{
    if (pfnFCIAddFile == NULL) {
        return (FALSE);
    }

    return (pfnFCIAddFile(hfci, pszSourceFile, pszFileName, fExecute, GetNextCab,
                          pfnProgress, pfnOpenInfo, typeCompress));
}


/*
 *  FCIFlushCabinet -- Complete the current cabinet under construction
 *
 *  See fci.h for entry/exit conditions.
 */

BOOL DIAMONDAPI FCIFlushCabinet(HFCI                  hfci,
                                BOOL                  fGetNextCab,
                                PFNFCIGETNEXTCABINET  GetNextCab,
                                PFNFCISTATUS          pfnProgress)
{
    if (pfnFCIFlushCabinet == NULL) {
        return (FALSE);
    }

    return (pfnFCIFlushCabinet(hfci, fGetNextCab, GetNextCab, pfnProgress));
}


/*
 *  FCIFlushFolder -- Complete the current folder under construction
 *
 *  See fci.h for entry/exit conditions.
 */

BOOL DIAMONDAPI FCIFlushFolder(HFCI                  hfci,
                               PFNFCIGETNEXTCABINET  GetNextCab,
                               PFNFCISTATUS          pfnProgress)
{
    if (pfnFCIFlushFolder == NULL) {
        return (FALSE);
    }

    return (pfnFCIFlushFolder(hfci, GetNextCab, pfnProgress));
}


/*
 *  FCIDestroy -- Destroy an FCI context
 *
 *  See fci.h for entry/exit conditions.
 */

BOOL DIAMONDAPI FCIDestroy(HFCI hfci)
{
    BOOL rc;

    if (pfnFCIDestroy == NULL) {
        return (FALSE);
    }

    rc = pfnFCIDestroy(hfci);

    if (rc == TRUE) {
        FreeLibrary(hCabinetDll);
    }

    return (rc);
}
