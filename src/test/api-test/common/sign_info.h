#ifndef sign_info_h__
#define sign_info_h__

/**
 * @file
 * @brief 数字签名信息获取
 * @see http://support.microsoft.com/kb/323809/zh-cn
 */


#include <Windows.h>
#include <wincrypt.h>
#include <wintrust.h>
#include <atlstr.h>

namespace cactus
{


/**
 * @brief 数字签名信息读取接口
 */
class ZLSignInfo
{
public:
    BOOL Load(LPCTSTR lpFilePath);

    ///> 签名者
    CString GetNameOfSigner() const;
    ///> 颁发者
    CString GetNameOfIssuer() const;
    ///> 签名时间
    SYSTEMTIME GetSigningTime() const;
    ///> 证书序列号
    CString GetSerialNumber() const;

    ///> 判断签名是否有时间戳(即时间戳在指定年限内)
    static BOOL IsDigitalSignatureHasTimestamp(LPCWSTR lpFilePath, WORD wMinYear = 2000, WORD wMaxYear = 2100);

private:
    void _Clear();
    static void              _GetFileSignerHandle(LPCTSTR lpFilePath, HCRYPTMSG& hCryptMsg, HCERTSTORE& hCertStore);
    static PCMSG_SIGNER_INFO _GetMsgSignerInfoOfFile(HCRYPTMSG hCryptMsg);
    static PCMSG_SIGNER_INFO _GetMsgSignerInfoOfTimestamp(PCMSG_SIGNER_INFO pMsgSignerInfoOfFile);
    static SYSTEMTIME        _ReadSigningTime(PCMSG_SIGNER_INFO pFileSignerInfo);
    static CString           _ReadSerialNumber(PCCERT_CONTEXT pCertContext);
    static CString           _ReadSignerName(PCCERT_CONTEXT pCertContext);
    static CString           _ReadIssuerName(PCCERT_CONTEXT pCertContext);
    static CString           _SimpleCertContextReader(PCCERT_CONTEXT pCertContext, DWORD dwType, DWORD dwFlags);
    static void              _ReleaseMsgSignerInfoPtr(PCMSG_SIGNER_INFO p);
    static PCCERT_CONTEXT    _ReadCertInfo(HCERTSTORE hCertStore, const PCMSG_SIGNER_INFO pMsgSignerInfoOfFile);

private:
    CString     m_sSignerName;    // 使用者
    CString     m_sIssuerName;    // 颁发者
    SYSTEMTIME  m_tSigningTime;   // 签名时间
    CString     m_sSerialNumber;  // 证书序列号
};

// 以下是实现部分


} // end of namespace zl


#endif // sign_info_h__
