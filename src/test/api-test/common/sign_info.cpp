#include "sign_info.h"

namespace cactus
{

CString ZLSignInfo::_ReadSerialNumber(PCCERT_CONTEXT pCertContext)
{
    CString sSerialNumber;

    if (pCertContext) {
        DWORD dwSize = pCertContext->pCertInfo->SerialNumber.cbData;

        for (DWORD n = 0; n < dwSize; n++) {
            sSerialNumber.AppendFormat(_T("%02x"), pCertContext->pCertInfo->SerialNumber.pbData[dwSize - (n + 1)]);
        }
    }

    return sSerialNumber;
}

CString ZLSignInfo::_ReadSignerName(PCCERT_CONTEXT pCertContext)
{
    return _SimpleCertContextReader(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0);
}

CString ZLSignInfo::_ReadIssuerName(PCCERT_CONTEXT pCertContext)
{
    return _SimpleCertContextReader(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, CERT_NAME_ISSUER_FLAG);
}

CString ZLSignInfo::_SimpleCertContextReader(PCCERT_CONTEXT pCertContext, DWORD dwType, DWORD dwFlags)
{
    CString sReturn;
    TCHAR*  pBuf = NULL;

    if (pCertContext) {
        // 取大小
        DWORD dwSize = ::CertGetNameString(pCertContext, dwType, dwFlags, NULL, NULL, 0);

        if (dwSize <= 0) {
            goto Exit0;
        }

        // 申请内存
        pBuf = (LPTSTR)::LocalAlloc(LPTR, dwSize * sizeof(TCHAR));

        if (NULL == pBuf) {
            goto Exit0;
        }

        // 取值
        ::CertGetNameString(pCertContext, dwType, dwFlags, NULL, pBuf, dwSize);

        if (NULL == pBuf) {
            goto Exit0;
        }

        sReturn = pBuf;
    }

Exit0:

    if (pBuf) {
        ::LocalFree((HLOCAL)pBuf);
    }

    return sReturn;
}

SYSTEMTIME ZLSignInfo::_ReadSigningTime(PCMSG_SIGNER_INFO pMsgSignerInfoOfTimestamp)
{
    SYSTEMTIME st = {0};

    for (DWORD n = 0; n < pMsgSignerInfoOfTimestamp->AuthAttrs.cAttr; ++n) {
        if (lstrcmpA(szOID_RSA_signingTime, pMsgSignerInfoOfTimestamp->AuthAttrs.rgAttr[n].pszObjId) == 0) {
            FILETIME ftSignInfo = {0};
            DWORD dwSize = sizeof(FILETIME);

            if (::CryptDecodeObject(
                    X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
                    szOID_RSA_signingTime,
                    pMsgSignerInfoOfTimestamp->AuthAttrs.rgAttr[n].rgValue[0].pbData,
                    pMsgSignerInfoOfTimestamp->AuthAttrs.rgAttr[n].rgValue[0].cbData,
                    0,
                    (PVOID)&ftSignInfo,
                    &dwSize)) {
                FILETIME ftTmp = {0};
                ::FileTimeToLocalFileTime(&ftSignInfo, &ftTmp);
                ::FileTimeToSystemTime(&ftTmp, &st);
            }

            break;
        }
    }

    return st;
}

BOOL ZLSignInfo::Load(LPCTSTR lpFilePath)
{
    _Clear();
    HCRYPTMSG         hCryptMsg = NULL;
    HCERTSTORE        hCertStore = NULL;
    PCCERT_CONTEXT    pCertContext = NULL;
    PCMSG_SIGNER_INFO pMsgSignerInfoOfFile = NULL;
    PCMSG_SIGNER_INFO pMsgSignerInfoOfTimestamp = NULL;
    _GetFileSignerHandle(lpFilePath, hCryptMsg, hCertStore);

    if (NULL == hCryptMsg || NULL == hCertStore)
        return FALSE;

    if (hCryptMsg) {
        pMsgSignerInfoOfFile = _GetMsgSignerInfoOfFile(hCryptMsg);
    }

    if (pMsgSignerInfoOfFile) {
        pMsgSignerInfoOfTimestamp = _GetMsgSignerInfoOfTimestamp(pMsgSignerInfoOfFile);
    }

    if (pMsgSignerInfoOfTimestamp) {
        m_tSigningTime = _ReadSigningTime(pMsgSignerInfoOfTimestamp);
    }

    pCertContext    = _ReadCertInfo(hCertStore, pMsgSignerInfoOfFile);
    m_sSignerName   = _ReadSignerName(pCertContext);
    m_sIssuerName   = _ReadIssuerName(pCertContext);
    m_sSerialNumber = _ReadSerialNumber(pCertContext);

    if (hCryptMsg)                 ::CryptMsgClose(hCryptMsg);

    if (hCertStore)                ::CertCloseStore(hCertStore, 0);

    if (pCertContext)              ::CertFreeCertificateContext(pCertContext);

    if (pMsgSignerInfoOfFile)      ::LocalFree((HLOCAL)pMsgSignerInfoOfFile);

    if (pMsgSignerInfoOfTimestamp) ::LocalFree((HLOCAL)pMsgSignerInfoOfTimestamp);

    return TRUE;
}

CString ZLSignInfo::GetNameOfIssuer() const
{
    return m_sIssuerName;
}

SYSTEMTIME ZLSignInfo::GetSigningTime() const
{
    return m_tSigningTime;
}

CString ZLSignInfo::GetSerialNumber() const
{
    return m_sSerialNumber;
}

BOOL ZLSignInfo::IsDigitalSignatureHasTimestamp(LPCWSTR lpFilePath, WORD wMinYear, WORD wMaxYear)
{
    ZLSignInfo signer;

    if (signer.Load(lpFilePath)) {
        SYSTEMTIME st = signer.GetSigningTime();

        if (st.wYear >= wMinYear && st.wYear <= wMaxYear) {
            return TRUE;
        }
    }

    return FALSE;
}

void ZLSignInfo::_Clear()
{
    m_sSignerName.Empty();
    m_sIssuerName.Empty();
    m_sSerialNumber.Empty();
    memset(&m_tSigningTime, 0, sizeof(SYSTEMTIME));
}

void ZLSignInfo::_GetFileSignerHandle(LPCTSTR lpFilePath, HCRYPTMSG& hCryptMsg, HCERTSTORE& hCertStore)
{
    hCryptMsg  = NULL;
    hCertStore = NULL;

    if (::PathFileExists(lpFilePath)) {
        if (FALSE == ::CryptQueryObject(
                CERT_QUERY_OBJECT_FILE,
                lpFilePath,
                CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED,
                CERT_QUERY_FORMAT_FLAG_BINARY,
                0,
                NULL,
                NULL,
                NULL,
                &hCertStore,
                &hCryptMsg,
                NULL)) {
            hCryptMsg  = NULL;
            hCertStore = NULL;
        }
    }
}

PCMSG_SIGNER_INFO ZLSignInfo::_GetMsgSignerInfoOfTimestamp(PCMSG_SIGNER_INFO pMsgSignerInfoOfFile)
{
    PCMSG_SIGNER_INFO pMsgSignerInfoOfTimestamp = NULL;

    for (DWORD n = 0; n < pMsgSignerInfoOfFile->UnauthAttrs.cAttr; n++) {
        if (lstrcmpA(pMsgSignerInfoOfFile->UnauthAttrs.rgAttr[n].pszObjId, szOID_RSA_counterSign) == 0) {
            BOOL bRet = FALSE;
            // 取大小
            DWORD dwSize = 0;
            bRet = ::CryptDecodeObject(
                       X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
                       PKCS7_SIGNER_INFO,
                       pMsgSignerInfoOfFile->UnauthAttrs.rgAttr[n].rgValue[0].pbData,
                       pMsgSignerInfoOfFile->UnauthAttrs.rgAttr[n].rgValue[0].cbData,
                       0,
                       NULL,
                       &dwSize);

            if (!bRet || dwSize <= 0) {
                goto Exit0;
            }

            // 申请内存
            pMsgSignerInfoOfTimestamp = (PCMSG_SIGNER_INFO)::LocalAlloc(LPTR, dwSize);

            if (NULL == pMsgSignerInfoOfTimestamp) {
                goto Exit0;
            }

            // 取值
            bRet = ::CryptDecodeObject(
                       X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
                       PKCS7_SIGNER_INFO,
                       pMsgSignerInfoOfFile->UnauthAttrs.rgAttr[n].rgValue[0].pbData,
                       pMsgSignerInfoOfFile->UnauthAttrs.rgAttr[n].rgValue[0].cbData,
                       0,
                       pMsgSignerInfoOfTimestamp,
                       &dwSize);

            if (!bRet) {
                goto Exit0;
            }
        }
    }

Exit0:
    return pMsgSignerInfoOfTimestamp;
}

PCMSG_SIGNER_INFO ZLSignInfo::_GetMsgSignerInfoOfFile(HCRYPTMSG hCryptMsg)
{
    PCMSG_SIGNER_INFO pMsgSignerInfoOfFile = NULL;

    if (hCryptMsg) {
        BOOL bRet = FALSE;
        // 取大小
        DWORD dwMsgSignerInfoSize = 0;
        bRet = ::CryptMsgGetParam(hCryptMsg, CMSG_SIGNER_INFO_PARAM, 0, NULL, &dwMsgSignerInfoSize);

        if (!bRet || 0 == dwMsgSignerInfoSize) {
            goto Exit0;
        }

        // 申请内存
        pMsgSignerInfoOfFile = (PCMSG_SIGNER_INFO)::LocalAlloc(LPTR, dwMsgSignerInfoSize);

        if (NULL == pMsgSignerInfoOfFile) {
            goto Exit0;
        }

        // 取值
        bRet = ::CryptMsgGetParam(hCryptMsg, CMSG_SIGNER_INFO_PARAM, 0, (void*)pMsgSignerInfoOfFile, &dwMsgSignerInfoSize);

        if (!bRet) {
            goto Exit0;
        }
    }

Exit0:
    return pMsgSignerInfoOfFile;
}

PCCERT_CONTEXT ZLSignInfo::_ReadCertInfo(HCERTSTORE hCertStore, const PCMSG_SIGNER_INFO pMsgSignerInfoOfFile)
{
    PCCERT_CONTEXT pCertContext = NULL;

    if (hCertStore && pMsgSignerInfoOfFile) {
        CERT_INFO stCertInfo;
        stCertInfo.Issuer       = pMsgSignerInfoOfFile->Issuer;
        stCertInfo.SerialNumber = pMsgSignerInfoOfFile->SerialNumber;
        pCertContext = ::CertFindCertificateInStore(hCertStore,
                       X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
                       0,
                       CERT_FIND_SUBJECT_CERT,
                       (PVOID)&stCertInfo,
                       NULL);
    }

    return pCertContext;
}

CString ZLSignInfo::GetNameOfSigner() const
{
    return m_sSignerName;
}

}