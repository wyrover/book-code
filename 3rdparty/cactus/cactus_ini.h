/*!
* \file cactus_ini.h
* \brief Ini操作声明 
* 
* Ini操作声明 
* 
* \author wangyang 
* \date 2015/01/30 
* \version 1.0
*/

#ifndef __CACTUS_CACTUS_INI_H__
#define __CACTUS_CACTUS_INI_H__

#include <cactus/cactus.h>
#include <map>
#include <atlstr.h>

namespace cactus
{


enum emIniEncoding
{
    emIniEncodingAnsi = 1,
    emIniEncodingUtf8,
    emIniEncodingUnicode,
};

class CACTUS_API KIniWrap
{
public:
    KIniWrap();
    ~KIniWrap();

    BOOL LoadFile(LPCTSTR szFileName, int emIniEncode = emIniEncodingUnicode);
    BOOL SaveFile(LPCTSTR szFileName, int emIniEncode);
    BOOL Parse(LPCWSTR szString);
    void Clear(void);

    int Read(LPCWSTR szAppName, LPCWSTR szKeyName, int nDefault);
    BOOL Write(LPCWSTR szAppName, LPCWSTR szKeyName, int nValue);

    LPCWSTR Read(LPCWSTR szAppName, LPCWSTR szKeyName, LPCWSTR szDefault);
    BOOL Write(LPCWSTR szAppName, LPCWSTR szKeyName, LPCWSTR szValue);

    DWORD Read(LPCTSTR szAppName, LPCTSTR szKeyName, LPBYTE pBuffer, DWORD dwSize);
    BOOL Write(LPCTSTR szAppName, LPCTSTR szKeyName, LPBYTE pBuffer, DWORD dwSize);

    const std::map<CStringW, std::map<CStringW, CStringW>>& ReadAll(void);

    BOOL ToString(CStringW& strBuffer);

protected:
    BOOL LoadFileToBuffer(LPCTSTR szFileName, LPVOID* ppBuffer, DWORD& dwSize);
    void ReleaseBuffer(LPVOID pBuffer);
    BOOL GetLine(LPCWSTR szBuffer, CStringW& strLine);
    BOOL ProcessLine(CStringW& strLine);
    BOOL ParseLine(CStringW& strLine, CStringW& strAppName, CStringW& strKeyName, CStringW& strValue);
    BOOL AddItem(CStringW& strAppName, CStringW& strKeyName, LPCWSTR szValue);
    BOOL BinaryToString(LPBYTE pBuffer, DWORD dwSize, CStringW& strBinStr);
    BOOL StringToBinary(CStringW& strBinStr, LPBYTE pBuffer, DWORD dwSize);

private:
    CStringW m_strCurrentAppName;
    std::map<CStringW, std::map<CStringW, CStringW>> m_mapIniData;
};
} // namespace cactus

#include <cactus/simpleini/SimpleIni.h>

#endif // __CACTUS_CACTUS_INI_H__
