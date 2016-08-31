#ifndef __CACTUS_CACTUS_VERIFY_H__
#define __CACTUS_CACTUS_VERIFY_H__

#include <cactus/cactus.h>
#include "cactus_types.h"

namespace cactus
{

bool verify_signedfile(const wstring& filename, LPCTSTR szSign, LPCTSTR szCert);
bool verify_signedfile(const string16& filename);


} // namespace cactus


#include <windows.h>
#include <wincrypt.h>
#include <wintrust.h>
#include <stdio.h>
#include <tchar.h>

#pragma comment(lib, "crypt32.lib")






namespace cactus
{

#define ENCODING (X509_ASN_ENCODING | PKCS_7_ASN_ENCODING)

// 数字证书颁发者
#define ISSUER_SYMANTEC (L"Symantec Class 3 SHA256 Code Signing CA")
#define ISSUER_VERISIGN (L"VeriSign Class 3 Code Signing 2010 CA")

// 数字签名签名人姓名
#define SIGNER_KINGSOFT (L"Beijing Kingsoft Security software Co.,Ltd")

typedef struct {
    LPWSTR lpszProgramName;
    LPWSTR lpszPublisherLink;
    LPWSTR lpszMoreInfoLink;
} SPROG_PUBLISHERINFO, *PSPROG_PUBLISHERINFO;

BOOL GetProgAndPublisherInfo(PCMSG_SIGNER_INFO pSignerInfo,
                             PSPROG_PUBLISHERINFO Info);
BOOL GetDateOfTimeStamp(PCMSG_SIGNER_INFO pSignerInfo, SYSTEMTIME *st);
BOOL verify_kingsoft_CertificateInfo(PCCERT_CONTEXT pCertContext);
BOOL verify_kingsoft_CertificateInfo(PCCERT_CONTEXT pCertContext, LPCTSTR szSign, LPCTSTR szCert);
BOOL PrintCertificateInfo(PCCERT_CONTEXT pCertContext);
BOOL GetTimeStampSignerInfo(PCMSG_SIGNER_INFO pSignerInfo,
                            PCMSG_SIGNER_INFO *pCounterSignerInfo);


inline void my_wprintf(const char16* fmt, ...)
{
    /*
    std::auto_ptr<char16> buf;
    int size = 512;

    while (true) {
        buf = std::auto_ptr<char16>(new char16[size <<= 1]);
        va_list ap;
        va_start(ap, fmt);
        int written = _vsnwprintf(buf.get(), size, fmt, ap);
        va_end(ap);

        if (written == size) {
            if (buf.get()[size] == 0)
                break;
        } else if (written >= 0 && written < size) {
            assert(buf.get()[written] == 0);
            break;
        }

        // else continue growing the buffer...
    }

    ::MessageBox(NULL, buf.get(), L"", MB_OK);
    */
}

inline bool verify_signedfile(const wstring& filename, LPCTSTR szSign, LPCTSTR szCert)
{
    //    WCHAR szFileName[MAX_PATH];
    HCERTSTORE hStore = NULL;
    HCRYPTMSG hMsg = NULL;
    PCCERT_CONTEXT pCertContext = NULL;
    BOOL fResult;
    DWORD dwEncoding, dwContentType, dwFormatType;
    PCMSG_SIGNER_INFO pSignerInfo = NULL;
    PCMSG_SIGNER_INFO pCounterSignerInfo = NULL;
    DWORD dwSignerInfo;
    CERT_INFO CertInfo;
    SPROG_PUBLISHERINFO ProgPubInfo;
    SYSTEMTIME st;
    ZeroMemory(&ProgPubInfo, sizeof(ProgPubInfo));
    bool retval = false;

    do {
        if (filename.empty()) {
            OutputDebugString(_T("error 8"));
            break;
        }

        // Get message handle and store handle from the signed file.
        fResult = CryptQueryObject(CERT_QUERY_OBJECT_FILE,
                                   filename.c_str(),
                                   CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED,
                                   CERT_QUERY_FORMAT_FLAG_BINARY,
                                   0,
                                   &dwEncoding,
                                   &dwContentType,
                                   &dwFormatType,
                                   &hStore,
                                   &hMsg,
                                   NULL);

        if (!fResult) {
            OutputDebugString(_T("error 7"));
            break;
        }

        // Get signer information size.
        fResult = CryptMsgGetParam(hMsg,
                                   CMSG_SIGNER_INFO_PARAM,
                                   0,
                                   NULL,
                                   &dwSignerInfo);

        if (!fResult) {
            OutputDebugString(_T("error 6"));
            break;
        }

        // Allocate memory for signer information.
        pSignerInfo = (PCMSG_SIGNER_INFO)LocalAlloc(LPTR, dwSignerInfo);

        if (!pSignerInfo) {
            OutputDebugString(_T("error 5"));
            break;
        }

        // Get Signer Information.
        fResult = CryptMsgGetParam(hMsg,
                                   CMSG_SIGNER_INFO_PARAM,
                                   0,
                                   (PVOID)pSignerInfo,
                                   &dwSignerInfo);

        if (!fResult) {
            OutputDebugString(_T("error 4"));
            break;
        }

        // Get program name and publisher information from
        // signer info structure.
        if (GetProgAndPublisherInfo(pSignerInfo, &ProgPubInfo)) {
            if (ProgPubInfo.lpszProgramName != NULL) {
                my_wprintf(L"Program Name : %s\n",
                           ProgPubInfo.lpszProgramName);
            }

            if (ProgPubInfo.lpszPublisherLink != NULL) {
                my_wprintf(L"Publisher Link : %s\n",
                           ProgPubInfo.lpszPublisherLink);
            }

            if (ProgPubInfo.lpszMoreInfoLink != NULL) {
                my_wprintf(L"MoreInfo Link : %s\n",
                           ProgPubInfo.lpszMoreInfoLink);
            }
        }

        _tprintf(_T("\n"));
        // Search for the signer certificate in the temporary
        // certificate store.
        CertInfo.Issuer = pSignerInfo->Issuer;
        CertInfo.SerialNumber = pSignerInfo->SerialNumber;
        pCertContext = CertFindCertificateInStore(hStore,
                       ENCODING,
                       0,
                       CERT_FIND_SUBJECT_CERT,
                       (PVOID)&CertInfo,
                       NULL);

        if (!pCertContext) {
            OutputDebugString(_T("error 1"));
            break;
        }

        if (!verify_kingsoft_CertificateInfo(pCertContext, szSign, szCert)) {
            OutputDebugString(_T("error 2"));
            break;
        }

        // Get the timestamp certificate signerinfo structure.
        if (GetTimeStampSignerInfo(pSignerInfo, &pCounterSignerInfo)) {
            // Search for Timestamp certificate in the temporary
            // certificate store.
            CertInfo.Issuer = pCounterSignerInfo->Issuer;
            CertInfo.SerialNumber = pCounterSignerInfo->SerialNumber;
            pCertContext = CertFindCertificateInStore(hStore,
                           ENCODING,
                           0,
                           CERT_FIND_SUBJECT_CERT,
                           (PVOID)&CertInfo,
                           NULL);

            if (!pCertContext) {
                _tprintf(_T("CertFindCertificateInStore failed with %x\n"),
                         GetLastError());
                OutputDebugString(_T("error 3"));
                break;
            }

            // Print timestamp certificate information.
            _tprintf(_T("TimeStamp Certificate:\n\n"));
            PrintCertificateInfo(pCertContext);
            _tprintf(_T("\n"));

            // Find Date of timestamp.
            if (GetDateOfTimeStamp(pCounterSignerInfo, &st)) {
                _tprintf(_T("Date of TimeStamp : %02d/%02d/%04d %02d:%02d\n"),
                         st.wMonth,
                         st.wDay,
                         st.wYear,
                         st.wHour,
                         st.wMinute);
            }

            _tprintf(_T("\n"));
        }

        retval = true;
    } while (0);

    // Clean up.
    if (ProgPubInfo.lpszProgramName != NULL)
        LocalFree(ProgPubInfo.lpszProgramName);

    if (ProgPubInfo.lpszPublisherLink != NULL)
        LocalFree(ProgPubInfo.lpszPublisherLink);

    if (ProgPubInfo.lpszMoreInfoLink != NULL)
        LocalFree(ProgPubInfo.lpszMoreInfoLink);

    if (pSignerInfo != NULL) LocalFree(pSignerInfo);

    if (pCounterSignerInfo != NULL) LocalFree(pCounterSignerInfo);

    if (pCertContext != NULL) CertFreeCertificateContext(pCertContext);

    if (hStore != NULL) CertCloseStore(hStore, 0);

    if (hMsg != NULL) CryptMsgClose(hMsg);

    return retval;
}

inline bool verify_signedfile(const string16& filename)
{
    WCHAR szFileName[MAX_PATH];
    HCERTSTORE hStore = NULL;
    HCRYPTMSG hMsg = NULL;
    PCCERT_CONTEXT pCertContext = NULL;
    BOOL fResult;
    DWORD dwEncoding, dwContentType, dwFormatType;
    PCMSG_SIGNER_INFO pSignerInfo = NULL;
    PCMSG_SIGNER_INFO pCounterSignerInfo = NULL;
    DWORD dwSignerInfo;
    CERT_INFO CertInfo;
    SPROG_PUBLISHERINFO ProgPubInfo;
    SYSTEMTIME st;
    ZeroMemory(&ProgPubInfo, sizeof(ProgPubInfo));
    bool retval = false;

    do {
        if (filename.empty())
            break;

        // Get message handle and store handle from the signed file.
        fResult = CryptQueryObject(CERT_QUERY_OBJECT_FILE,
                                   filename.c_str(),
                                   CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED,
                                   CERT_QUERY_FORMAT_FLAG_BINARY,
                                   0,
                                   &dwEncoding,
                                   &dwContentType,
                                   &dwFormatType,
                                   &hStore,
                                   &hMsg,
                                   NULL);

        if (!fResult)
            break;

        // Get signer information size.
        fResult = CryptMsgGetParam(hMsg,
                                   CMSG_SIGNER_INFO_PARAM,
                                   0,
                                   NULL,
                                   &dwSignerInfo);

        if (!fResult)
            break;

        // Allocate memory for signer information.
        pSignerInfo = (PCMSG_SIGNER_INFO)LocalAlloc(LPTR, dwSignerInfo);

        if (!pSignerInfo)
            break;

        // Get Signer Information.
        fResult = CryptMsgGetParam(hMsg,
                                   CMSG_SIGNER_INFO_PARAM,
                                   0,
                                   (PVOID)pSignerInfo,
                                   &dwSignerInfo);

        if (!fResult)
            break;

        // Get program name and publisher information from
        // signer info structure.
        if (GetProgAndPublisherInfo(pSignerInfo, &ProgPubInfo)) {
            if (ProgPubInfo.lpszProgramName != NULL) {
                my_wprintf(L"Program Name : %s\n",
                           ProgPubInfo.lpszProgramName);
            }

            if (ProgPubInfo.lpszPublisherLink != NULL) {
                my_wprintf(L"Publisher Link : %s\n",
                           ProgPubInfo.lpszPublisherLink);
            }

            if (ProgPubInfo.lpszMoreInfoLink != NULL) {
                my_wprintf(L"MoreInfo Link : %s\n",
                           ProgPubInfo.lpszMoreInfoLink);
            }
        }

        _tprintf(_T("\n"));
        // Search for the signer certificate in the temporary
        // certificate store.
        CertInfo.Issuer = pSignerInfo->Issuer;
        CertInfo.SerialNumber = pSignerInfo->SerialNumber;
        pCertContext = CertFindCertificateInStore(hStore,
                       ENCODING,
                       0,
                       CERT_FIND_SUBJECT_CERT,
                       (PVOID)&CertInfo,
                       NULL);

        if (!pCertContext)
            break;

        if (!verify_kingsoft_CertificateInfo(pCertContext))
            break;

        // Get the timestamp certificate signerinfo structure.
        if (GetTimeStampSignerInfo(pSignerInfo, &pCounterSignerInfo)) {
            // Search for Timestamp certificate in the temporary
            // certificate store.
            CertInfo.Issuer = pCounterSignerInfo->Issuer;
            CertInfo.SerialNumber = pCounterSignerInfo->SerialNumber;
            pCertContext = CertFindCertificateInStore(hStore,
                           ENCODING,
                           0,
                           CERT_FIND_SUBJECT_CERT,
                           (PVOID)&CertInfo,
                           NULL);

            if (!pCertContext) {
                _tprintf(_T("CertFindCertificateInStore failed with %x\n"),
                         GetLastError());
                break;
            }

            // Print timestamp certificate information.
            _tprintf(_T("TimeStamp Certificate:\n\n"));
            PrintCertificateInfo(pCertContext);
            _tprintf(_T("\n"));

            // Find Date of timestamp.
            if (GetDateOfTimeStamp(pCounterSignerInfo, &st)) {
                _tprintf(_T("Date of TimeStamp : %02d/%02d/%04d %02d:%02d\n"),
                         st.wMonth,
                         st.wDay,
                         st.wYear,
                         st.wHour,
                         st.wMinute);
            }

            _tprintf(_T("\n"));
        }

        retval = true;
    } while (0);

    // Clean up.
    if (ProgPubInfo.lpszProgramName != NULL)
        LocalFree(ProgPubInfo.lpszProgramName);

    if (ProgPubInfo.lpszPublisherLink != NULL)
        LocalFree(ProgPubInfo.lpszPublisherLink);

    if (ProgPubInfo.lpszMoreInfoLink != NULL)
        LocalFree(ProgPubInfo.lpszMoreInfoLink);

    if (pSignerInfo != NULL) LocalFree(pSignerInfo);

    if (pCounterSignerInfo != NULL) LocalFree(pCounterSignerInfo);

    if (pCertContext != NULL) CertFreeCertificateContext(pCertContext);

    if (hStore != NULL) CertCloseStore(hStore, 0);

    if (hMsg != NULL) CryptMsgClose(hMsg);

    return retval;
}

inline BOOL verify_kingsoft_CertificateInfo(PCCERT_CONTEXT pCertContext)
{
    BOOL fReturn = FALSE;
    LPTSTR szName = NULL;
    DWORD dwData;

    do {
        // Print Serial Number.
        _tprintf(_T("Serial Number: "));
        dwData = pCertContext->pCertInfo->SerialNumber.cbData;

        for (DWORD n = 0; n < dwData; n++) {
            _tprintf(_T("%02x "),
                     pCertContext->pCertInfo->SerialNumber.pbData[dwData - (n + 1)]);
        }

        _tprintf(_T("\n"));

        // Get Issuer name size.
        if (!(dwData = CertGetNameString(pCertContext,
                                         CERT_NAME_SIMPLE_DISPLAY_TYPE,
                                         CERT_NAME_ISSUER_FLAG,
                                         NULL,
                                         NULL,
                                         0))) {
            _tprintf(_T("CertGetNameString failed.\n"));
            break;
        }

        // Allocate memory for Issuer name.
        szName = (LPTSTR)LocalAlloc(LPTR, dwData * sizeof(TCHAR));

        if (!szName) {
            _tprintf(_T("Unable to allocate memory for issuer name.\n"));
            break;
        }

        // Get Issuer name.
        if (!(CertGetNameString(pCertContext,
                                CERT_NAME_SIMPLE_DISPLAY_TYPE,
                                CERT_NAME_ISSUER_FLAG,
                                NULL,
                                szName,
                                dwData))) {
            _tprintf(_T("CertGetNameString failed.\n"));
            break;
        }

        // 颁发者名称
        if (wcscmp(szName, ISSUER_SYMANTEC) != 0 && 
            wcscmp(szName, ISSUER_VERISIGN) != 0) {
            break;
        }

        // print Issuer name.
        // my_wprintf(_T("Issuer Name: %s\n"), szName);
        LocalFree(szName);
        szName = NULL;

        // Get Subject name size.
        if (!(dwData = CertGetNameString(pCertContext,
                                         CERT_NAME_SIMPLE_DISPLAY_TYPE,
                                         0,
                                         NULL,
                                         NULL,
                                         0))) {
            _tprintf(_T("CertGetNameString failed.\n"));
            break;
        }

        // Allocate memory for subject name.
        szName = (LPTSTR)LocalAlloc(LPTR, dwData * sizeof(TCHAR));

        if (!szName) {
            _tprintf(_T("Unable to allocate memory for subject name.\n"));
            break;
        }

        // Get subject name.
        if (!(CertGetNameString(pCertContext,
                                CERT_NAME_SIMPLE_DISPLAY_TYPE,
                                0,
                                NULL,
                                szName,
                                dwData))) {
            _tprintf(_T("CertGetNameString failed.\n"));
            break;
        }

        // 签名人姓名
        if (wcscmp(szName, SIGNER_KINGSOFT) != 0) {
            break;
        }

        // Print Subject Name.
        // my_wprintf(_T("Subject Name: %s\n"), szName);
        fReturn = TRUE;
    } while (0);

    if (szName != NULL) LocalFree(szName);

    return fReturn;
}

inline BOOL verify_kingsoft_CertificateInfo(PCCERT_CONTEXT pCertContext, LPCTSTR szSign, LPCTSTR szCert)
{
    BOOL fReturn = FALSE;
    LPTSTR szName = NULL;
    DWORD dwData;

    do {
        // Print Serial Number.
        _tprintf(_T("Serial Number: "));
        dwData = pCertContext->pCertInfo->SerialNumber.cbData;

        for (DWORD n = 0; n < dwData; n++) {
            _tprintf(_T("%02x "),
                     pCertContext->pCertInfo->SerialNumber.pbData[dwData - (n + 1)]);
        }

        _tprintf(_T("\n"));

        // Get Issuer name size.
        if (!(dwData = CertGetNameString(pCertContext,
                                         CERT_NAME_SIMPLE_DISPLAY_TYPE,
                                         CERT_NAME_ISSUER_FLAG,
                                         NULL,
                                         NULL,
                                         0))) {
            _tprintf(_T("CertGetNameString failed.\n"));
            break;
        }

        // Allocate memory for Issuer name.
        szName = (LPTSTR)LocalAlloc(LPTR, dwData * sizeof(TCHAR));

        if (!szName) {
            _tprintf(_T("Unable to allocate memory for issuer name.\n"));
            break;
        }

        // Get Issuer name.
        if (!(CertGetNameString(pCertContext,
                                CERT_NAME_SIMPLE_DISPLAY_TYPE,
                                CERT_NAME_ISSUER_FLAG,
                                NULL,
                                szName,
                                dwData))) {
            _tprintf(_T("CertGetNameString failed.\n"));
            break;
        }

        //CStringW str = CA2W(szName);
        OutputDebugString(szName);

        if (wcscmp(szName, szCert) != 0) {
            break;
        }

        // print Issuer name.
        // my_wprintf(_T("Issuer Name: %s\n"), szName);
        LocalFree(szName);
        szName = NULL;

        // Get Subject name size.
        if (!(dwData = CertGetNameString(pCertContext,
                                         CERT_NAME_SIMPLE_DISPLAY_TYPE,
                                         0,
                                         NULL,
                                         NULL,
                                         0))) {
            _tprintf(_T("CertGetNameString failed.\n"));
            break;
        }

        // Allocate memory for subject name.
        szName = (LPTSTR)LocalAlloc(LPTR, dwData * sizeof(TCHAR));

        if (!szName) {
            _tprintf(_T("Unable to allocate memory for subject name.\n"));
            break;
        }

        // Get subject name.
        if (!(CertGetNameString(pCertContext,
                                CERT_NAME_SIMPLE_DISPLAY_TYPE,
                                0,
                                NULL,
                                szName,
                                dwData))) {
            _tprintf(_T("CertGetNameString failed.\n"));
            break;
        }

        //str = CA2W(szName);
        //Kingsoft Security Co.,Ltd
        OutputDebugStringW(szName);

        if (wcscmp(szName, szSign) != 0) {
            break;
        }

        // Print Subject Name.
        // my_wprintf(_T("Subject Name: %s\n"), szName);
        fReturn = TRUE;
    } while (0);

    if (szName != NULL) LocalFree(szName);

    return fReturn;
}

inline BOOL PrintCertificateInfo(PCCERT_CONTEXT pCertContext)
{
    BOOL fReturn = FALSE;
    LPTSTR szName = NULL;
    DWORD dwData;

    do {
        // Print Serial Number.
        _tprintf(_T("Serial Number: "));
        dwData = pCertContext->pCertInfo->SerialNumber.cbData;

        for (DWORD n = 0; n < dwData; n++) {
            _tprintf(_T("%02x "),
                     pCertContext->pCertInfo->SerialNumber.pbData[dwData - (n + 1)]);
        }

        _tprintf(_T("\n"));

        // Get Issuer name size.
        if (!(dwData = CertGetNameString(pCertContext,
                                         CERT_NAME_SIMPLE_DISPLAY_TYPE,
                                         CERT_NAME_ISSUER_FLAG,
                                         NULL,
                                         NULL,
                                         0))) {
            _tprintf(_T("CertGetNameString failed.\n"));
            break;
        }

        // Allocate memory for Issuer name.
        szName = (LPTSTR)LocalAlloc(LPTR, dwData * sizeof(TCHAR));

        if (!szName) {
            _tprintf(_T("Unable to allocate memory for issuer name.\n"));
            break;
        }

        // Get Issuer name.
        if (!(CertGetNameString(pCertContext,
                                CERT_NAME_SIMPLE_DISPLAY_TYPE,
                                CERT_NAME_ISSUER_FLAG,
                                NULL,
                                szName,
                                dwData))) {
            _tprintf(_T("CertGetNameString failed.\n"));
            break;
        }

        // print Issuer name.
        my_wprintf(_T("Issuer Name: %s\n"), szName);
        LocalFree(szName);
        szName = NULL;

        // Get Subject name size.
        if (!(dwData = CertGetNameString(pCertContext,
                                         CERT_NAME_SIMPLE_DISPLAY_TYPE,
                                         0,
                                         NULL,
                                         NULL,
                                         0))) {
            _tprintf(_T("CertGetNameString failed.\n"));
            break;
        }

        // Allocate memory for subject name.
        szName = (LPTSTR)LocalAlloc(LPTR, dwData * sizeof(TCHAR));

        if (!szName) {
            _tprintf(_T("Unable to allocate memory for subject name.\n"));
            break;
        }

        // Get subject name.
        if (!(CertGetNameString(pCertContext,
                                CERT_NAME_SIMPLE_DISPLAY_TYPE,
                                0,
                                NULL,
                                szName,
                                dwData))) {
            _tprintf(_T("CertGetNameString failed.\n"));
            break;
        }

        // Print Subject Name.
        my_wprintf(_T("Subject Name: %s\n"), szName);
        fReturn = TRUE;
    } while (0);

    if (szName != NULL) LocalFree(szName);

    return fReturn;
}

inline LPWSTR AllocateAndCopyWideString(LPCWSTR inputString)
{
    LPWSTR outputString = NULL;
    outputString = (LPWSTR)LocalAlloc(LPTR,
                                      (wcslen(inputString) + 1) * sizeof(WCHAR));

    if (outputString != NULL) {
        lstrcpyW(outputString, inputString);
    }

    return outputString;
}

inline BOOL GetProgAndPublisherInfo(PCMSG_SIGNER_INFO pSignerInfo,
                                    PSPROG_PUBLISHERINFO Info)
{
    BOOL fReturn = FALSE;
    PSPC_SP_OPUS_INFO OpusInfo = NULL;
    DWORD dwData;
    BOOL fResult;

    __try {
        // Loop through authenticated attributes and find
        // SPC_SP_OPUS_INFO_OBJID OID.
        for (DWORD n = 0; n < pSignerInfo->AuthAttrs.cAttr; n++) {
            if (lstrcmpA(SPC_SP_OPUS_INFO_OBJID,
                         pSignerInfo->AuthAttrs.rgAttr[n].pszObjId) == 0) {
                // Get Size of SPC_SP_OPUS_INFO structure.
                fResult = CryptDecodeObject(ENCODING,
                                            SPC_SP_OPUS_INFO_OBJID,
                                            pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].pbData,
                                            pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].cbData,
                                            0,
                                            NULL,
                                            &dwData);

                if (!fResult) {
                    _tprintf(_T("CryptDecodeObject failed with %x\n"),
                             GetLastError());
                    __leave;
                }

                // Allocate memory for SPC_SP_OPUS_INFO structure.
                OpusInfo = (PSPC_SP_OPUS_INFO)LocalAlloc(LPTR, dwData);

                if (!OpusInfo) {
                    _tprintf(_T("Unable to allocate memory for Publisher Info.\n"));
                    __leave;
                }

                // Decode and get SPC_SP_OPUS_INFO structure.
                fResult = CryptDecodeObject(ENCODING,
                                            SPC_SP_OPUS_INFO_OBJID,
                                            pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].pbData,
                                            pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].cbData,
                                            0,
                                            OpusInfo,
                                            &dwData);

                if (!fResult) {
                    _tprintf(_T("CryptDecodeObject failed with %x\n"),
                             GetLastError());
                    __leave;
                }

                // Fill in Program Name if present.
                if (OpusInfo->pwszProgramName) {
                    Info->lpszProgramName =
                        AllocateAndCopyWideString(OpusInfo->pwszProgramName);
                } else
                    Info->lpszProgramName = NULL;

                // Fill in Publisher Information if present.
                if (OpusInfo->pPublisherInfo) {
                    switch (OpusInfo->pPublisherInfo->dwLinkChoice) {
                    case SPC_URL_LINK_CHOICE:
                        Info->lpszPublisherLink =
                            AllocateAndCopyWideString(OpusInfo->pPublisherInfo->pwszUrl);
                        break;

                    case SPC_FILE_LINK_CHOICE:
                        Info->lpszPublisherLink =
                            AllocateAndCopyWideString(OpusInfo->pPublisherInfo->pwszFile);
                        break;

                    default:
                        Info->lpszPublisherLink = NULL;
                        break;
                    }
                } else {
                    Info->lpszPublisherLink = NULL;
                }

                // Fill in More Info if present.
                if (OpusInfo->pMoreInfo) {
                    switch (OpusInfo->pMoreInfo->dwLinkChoice) {
                    case SPC_URL_LINK_CHOICE:
                        Info->lpszMoreInfoLink =
                            AllocateAndCopyWideString(OpusInfo->pMoreInfo->pwszUrl);
                        break;

                    case SPC_FILE_LINK_CHOICE:
                        Info->lpszMoreInfoLink =
                            AllocateAndCopyWideString(OpusInfo->pMoreInfo->pwszFile);
                        break;

                    default:
                        Info->lpszMoreInfoLink = NULL;
                        break;
                    }
                } else {
                    Info->lpszMoreInfoLink = NULL;
                }

                fReturn = TRUE;
                break; // Break from for loop.
            } // lstrcmp SPC_SP_OPUS_INFO_OBJID
        } // for
    } __finally {
        if (OpusInfo != NULL) LocalFree(OpusInfo);
    }

    return fReturn;
}

inline BOOL GetDateOfTimeStamp(PCMSG_SIGNER_INFO pSignerInfo, SYSTEMTIME *st)
{
    BOOL fResult;
    FILETIME lft, ft;
    DWORD dwData;
    BOOL fReturn = FALSE;

    // Loop through authenticated attributes and find
    // szOID_RSA_signingTime OID.
    for (DWORD n = 0; n < pSignerInfo->AuthAttrs.cAttr; n++) {
        if (lstrcmpA(szOID_RSA_signingTime,
                     pSignerInfo->AuthAttrs.rgAttr[n].pszObjId) == 0) {
            // Decode and get FILETIME structure.
            dwData = sizeof(ft);
            fResult = CryptDecodeObject(ENCODING,
                                        szOID_RSA_signingTime,
                                        pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].pbData,
                                        pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].cbData,
                                        0,
                                        (PVOID)&ft,
                                        &dwData);

            if (!fResult) {
                _tprintf(_T("CryptDecodeObject failed with %x\n"),
                         GetLastError());
                break;
            }

            // Convert to local time.
            FileTimeToLocalFileTime(&ft, &lft);
            FileTimeToSystemTime(&lft, st);
            fReturn = TRUE;
            break; // Break from for loop.
        } //lstrcmp szOID_RSA_signingTime
    } // for

    return fReturn;
}

inline BOOL GetTimeStampSignerInfo(PCMSG_SIGNER_INFO pSignerInfo, PCMSG_SIGNER_INFO *pCounterSignerInfo)
{
    PCCERT_CONTEXT pCertContext = NULL;
    BOOL fReturn = FALSE;
    BOOL fResult;
    DWORD dwSize;

    __try {
        *pCounterSignerInfo = NULL;

        // Loop through unathenticated attributes for
        // szOID_RSA_counterSign OID.
        for (DWORD n = 0; n < pSignerInfo->UnauthAttrs.cAttr; n++) {
            if (lstrcmpA(pSignerInfo->UnauthAttrs.rgAttr[n].pszObjId,
                         szOID_RSA_counterSign) == 0) {
                // Get size of CMSG_SIGNER_INFO structure.
                fResult = CryptDecodeObject(ENCODING,
                                            PKCS7_SIGNER_INFO,
                                            pSignerInfo->UnauthAttrs.rgAttr[n].rgValue[0].pbData,
                                            pSignerInfo->UnauthAttrs.rgAttr[n].rgValue[0].cbData,
                                            0,
                                            NULL,
                                            &dwSize);

                if (!fResult) {
                    _tprintf(_T("CryptDecodeObject failed with %x\n"),
                             GetLastError());
                    __leave;
                }

                // Allocate memory for CMSG_SIGNER_INFO.
                *pCounterSignerInfo = (PCMSG_SIGNER_INFO)LocalAlloc(LPTR, dwSize);

                if (!*pCounterSignerInfo) {
                    _tprintf(_T("Unable to allocate memory for timestamp info.\n"));
                    __leave;
                }

                // Decode and get CMSG_SIGNER_INFO structure
                // for timestamp certificate.
                fResult = CryptDecodeObject(ENCODING,
                                            PKCS7_SIGNER_INFO,
                                            pSignerInfo->UnauthAttrs.rgAttr[n].rgValue[0].pbData,
                                            pSignerInfo->UnauthAttrs.rgAttr[n].rgValue[0].cbData,
                                            0,
                                            (PVOID) * pCounterSignerInfo,
                                            &dwSize);

                if (!fResult) {
                    _tprintf(_T("CryptDecodeObject failed with %x\n"),
                             GetLastError());
                    __leave;
                }

                fReturn = TRUE;
                break; // Break from for loop.
            }
        }
    } __finally {
        // Clean up.
        if (pCertContext != NULL) CertFreeCertificateContext(pCertContext);
    }

    return fReturn;
}

} // namespace cactus


#endif // __CACTUS_CACTUS_VERIFY_H__
