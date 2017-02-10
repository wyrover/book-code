#ifndef __CHKMACROS_H__
#define __CHKMACROS_H__

#define ChkGetHr()                (static_cast<HRESULT>(__hr))
#define ChkSetHr(hrExpr)          { __hr = (hrExpr); }

//
// Common (internal) macros for Chk* macro definitions.
//
#define ___ChkMacroEndDefault___() \
    ; \
            CHK_MACROS_CHECK_TO_BREAK_ON_FAILURE(__hr); goto Exit; \
        } \
    } \

#define ___ChkBoolBegin___(bExpr, hrReturnOnFailure) \
    { \
        if (!(bExpr)) \
        { \
            ChkSetHr(hrReturnOnFailure); \

//
// ChkProlog() / ChkEpilog() / ChkCleanup() / ChkNoCleanup()
//
#define ChkProlog() \
    HRESULT __hr = S_OK; \

#define ChkEpilogVoid() \
    return; \

#define ChkEpilogRet(result) \
    return result; \

#define ChkEpilog() \
    ChkEpilogRet(__hr); \

#define ChkCleanup() \
    goto Exit; \
    Exit: \

#define ChkNoCleanupVoid() \
    ChkCleanup() \
    ChkEpilogVoid() \

#define ChkNoCleanupRet(result) \
    ChkCleanup() \
    ChkEpilogRet(result) \

#define ChkNoCleanup() \
    ChkCleanup() \
    ChkEpilog() \

//
// ChkHr(hrExpr)
//
#define ChkHr(hrExpr) \
    { \
        ChkSetHr(hrExpr); \
        if (FAILED(ChkGetHr())) { \
            ___ChkMacroEndDefault___() \

//
// ChkBool(bExpr, hrReturnOnFailure)
//
#define ChkBool(bExpr, hrReturnOnFailure) \
    ___ChkBoolBegin___(bExpr, hrReturnOnFailure) \
    ___ChkMacroEndDefault___() \

//
// ChkWin32Error(dwCall)
//
#define ChkWin32Error(dwCall) \
    { \
        if (dwCall != ERROR_SUCCESS) \
        { \
            ChkSetHr(HRESULT_FROM_WIN32(dwCall)); \
            ___ChkMacroEndDefault___() \

//
// ChkWin32(bWin32Expr)
// Note: This definition is similar to ChkBool with translated error code (HRESULT).
//
#define ChkWin32(bExpr) \
    ___ChkBoolBegin___(bExpr, HRESULT_FROM_WIN32(GetLastError())) \
    ___ChkMacroEndDefault___() \

//
// ChkAlloc(bAllocExpr)
// Note: This is the same definition as ChkBool but the error code is fixed (E_OUTOFMEMORY).
//
#define ChkAlloc(bAllocExpr) \
    ___ChkBoolBegin___(bAllocExpr, E_OUTOFMEMORY) \
    ___ChkMacroEndDefault___() \

//
// ChkArg(bArgEval)
// Note: This is the same definition as ChkBool but the error code is fixed (E_INVALIDARG).
//
#define ChkArg(bArgExpr) \
    ___ChkBoolBegin___(bArgExpr, E_INVALIDARG) \
    ___ChkMacroEndDefault___() \

//
// ChkData(bArgEval)
// Note: This is the same definition as ChkBool but the error code is fixed.
//
#define ChkData(bDataExpr) \
    ___ChkBoolBegin___(bDataExpr, HRESULT_FROM_WIN32(ERROR_INVALID_DATA)) \
    ___ChkMacroEndDefault___() \

//
// ChkReturn(hrExpr)
//
#define ChkReturn(hrExpr) \
    { \
        { \
            ChkSetHr(hrExpr); \
            if (FAILED(ChkGetHr())) { \
                ___ChkMacroEndDefault___() \
        goto Exit; \
    } \

#endif // __CHKMACROS_H__
