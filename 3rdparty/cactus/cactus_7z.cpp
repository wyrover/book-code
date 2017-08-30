
#include <cactus/cactus_7z.h>
#include <cactus/PropVariant.h>
#include <cactus/Guid.h>
#include <vector>


bool ConvertToFormatInfo(
    CPropVariant &vGUID,
    CPropVariant &vUpdate,
    CPropVariant &vExtension,
    CPropVariant &vSignature,
    CPropVariant &vName,
    FormatInfoInternal *pInfo
)
{
    if ((vGUID.vt != VT_BSTR) ||
        (vUpdate.vt != VT_BOOL) ||
        (vName.vt != VT_BSTR))
        return false;

    memcpy(&pInfo->uid, vGUID.bstrVal, sizeof(GUID));
    pInfo->bUpdate = (vUpdate.boolVal == VARIANT_TRUE);
    int nLength;

    if (vExtension.vt == VT_BSTR) {
        nLength = SysStringLen(vExtension.bstrVal);
        pInfo->lpDefaultExt = (char*)malloc(nLength + 1);
        memset(pInfo->lpDefaultExt, 0, nLength + 1);
        WideCharToMultiByte(CP_OEMCP, 0, vExtension.bstrVal, -1, pInfo->lpDefaultExt, nLength, NULL, NULL);
        char *p = strchr(pInfo->lpDefaultExt, ' ');

        if (p)
            *p = 0;
    } else
        pInfo->lpDefaultExt = NULL;

    if (vSignature.vt == VT_BSTR) {
        nLength = SysStringByteLen(vSignature.bstrVal);
        pInfo->lpSignature = (char*)malloc(nLength);
        pInfo->nSignatureLength = nLength;
        memcpy(pInfo->lpSignature, vSignature.bstrVal, nLength);
    } else
        pInfo->lpSignature = NULL;

    nLength = SysStringLen(vName.bstrVal);
    pInfo->lpName = (char*)malloc(nLength + 1 + strlen(" archive [7z]"));
    memset(pInfo->lpName, 0, nLength + 1);
    WideCharToMultiByte(CP_OEMCP, 0, vName.bstrVal, -1, pInfo->lpName, nLength, NULL, NULL);
    strcat(pInfo->lpName, " archive [7z]");
    return true;
}

static HRESULT GetCoderClass(GetMethodPropertyFunc getMethodProperty, UInt32 index,
                             PROPID propId, CLSID &clsId, bool &isAssigned)
{
    CPropVariant prop;
    isAssigned = false;
    RINOK(getMethodProperty(index, propId, &prop));

    if (prop.vt == VT_BSTR) {
        isAssigned = true;
        clsId = *(const GUID *)prop.bstrVal;
    } else if (prop.vt != VT_EMPTY)
        return E_FAIL;

    return S_OK;
}

namespace NMethodPropID
{
enum EEnum {
    kID,
    kName,
    kDecoder,
    kEncoder,
    kInStreams,
    kOutStreams,
    kDescription,
    kDecoderIsAssigned,
    kEncoderIsAssigned
};
}

SevenZipModule::SevenZipModule()
{
    m_hModule = NULL;
}

bool SevenZipModule::Initialize(LPCTSTR lpFileName)
{
    bool bResult = false;
    m_pInfo = NULL;
    m_hModule = LoadLibraryEx(
                    lpFileName,
                    NULL,
                    LOAD_WITH_ALTERED_SEARCH_PATH
                );

    if (m_hModule) {
        m_pfnCreateObject = (CREATEOBJECT)GetProcAddress(m_hModule, "CreateObject");
        m_pfnGetHandlerProperty = (GETHANDLERPROPERTY)GetProcAddress(m_hModule, "GetHandlerProperty");
        m_pfnGetHandlerProperty2 = (GETHANDLERPROPERTY2)GetProcAddress(m_hModule, "GetHandlerProperty2");
        m_pfnGetNumberOfFormats = (GETNUMBEROFFORMATS)GetProcAddress(m_hModule, "GetNumberOfFormats");
        GetMethodPropertyFunc GetMethodProperty = (GetMethodPropertyFunc)GetProcAddress(m_hModule, "GetMethodProperty");
        UInt32 numMethods = 1;
        GetNumberOfMethodsFunc getNumberOfMethodsFunc = (GetNumberOfMethodsFunc)GetProcAddress(m_hModule, "GetNumberOfMethods");

        if (getNumberOfMethodsFunc != NULL) {
            getNumberOfMethodsFunc(&numMethods);
        }

        for (UINT32 i = 0; i < numMethods; i++) {
            CDllCodecInfo info;
            info.LibIndex = 0;
            info.CodecIndex = i;
            GetCoderClass(GetMethodProperty, i, NMethodPropID::kEncoder, info.Encoder, info.EncoderIsAssigned);
            GetCoderClass(GetMethodProperty, i, NMethodPropID::kDecoder, info.Decoder, info.DecoderIsAssigned);
            m_codecs.push_back(info);
        }

        // old
        CPropVariant vGUID, vUpdate, vExtension, vSignature, vName;

        if (m_pfnCreateObject &&
            (m_pfnGetHandlerProperty || (m_pfnGetHandlerProperty2 && m_pfnGetNumberOfFormats))) {
            if (m_pfnGetHandlerProperty2 && m_pfnGetNumberOfFormats) {
                if (m_pfnGetNumberOfFormats(&m_nNumberOfFormats) == S_OK) {
                    m_pInfo = (FormatInfoInternal*)malloc(m_nNumberOfFormats * sizeof(FormatInfoInternal));
                    bResult = true;

                    for (unsigned int i = 0; i < m_nNumberOfFormats; i++) {
                        if ((m_pfnGetHandlerProperty2(i, NArchive::kClassID, &vGUID) != S_OK) ||
                            (m_pfnGetHandlerProperty2(i, NArchive::kUpdate, &vUpdate) != S_OK) ||
                            (m_pfnGetHandlerProperty2(i, NArchive::kExtension, &vExtension) != S_OK) ||
                            (m_pfnGetHandlerProperty2(i, NArchive::kStartSignature, &vSignature) != S_OK) ||
                            (m_pfnGetHandlerProperty2(i, NArchive::kName, &vName) != S_OK) ||
                            !ConvertToFormatInfo(vGUID, vUpdate, vExtension, vSignature, vName, &m_pInfo[i])) {
                            bResult = false;
                            break;
                        }
                    }
                }
            } else {
                m_nNumberOfFormats = 1;
                m_pInfo = (FormatInfoInternal*)malloc(m_nNumberOfFormats * sizeof(FormatInfoInternal));

                if ((m_pfnGetHandlerProperty(NArchive::kClassID, &vGUID) != S_OK) ||
                    (m_pfnGetHandlerProperty(NArchive::kUpdate, &vUpdate) != S_OK) ||
                    (m_pfnGetHandlerProperty(NArchive::kExtension, &vExtension) != S_OK) ||
                    (m_pfnGetHandlerProperty(NArchive::kStartSignature, &vSignature) != S_OK) ||
                    (m_pfnGetHandlerProperty(NArchive::kName, &vName) != S_OK) ||
                    !ConvertToFormatInfo(vGUID, vUpdate, vExtension, vSignature, vName, &m_pInfo[0]))
                    bResult = false;
            }
        }
    }

    return bResult;
}


SevenZipModule::~SevenZipModule()
{
    if (m_pInfo) {
        for (unsigned int i = 0; i < m_nNumberOfFormats; i++) {
            free(m_pInfo[i].lpDefaultExt);
            free(m_pInfo[i].lpSignature);
        }

        free(m_pInfo);
    }

    FreeLibrary(m_hModule);
    m_hModule = NULL;
}

SevenZipArchive::SevenZipArchive(const SevenZipModule *pModule)
{
    m_pModule = pModule;
    m_pArchive = NULL;
    m_pInFile = NULL;
    m_bOpened = false;
}

SevenZipArchive::~SevenZipArchive()
{
}

bool __stdcall SevenZipArchive::pOpenArchive(const char *lpFileName, int nOpMode)
{
    m_lpFileName = lpFileName;

    if (m_bOpened)
        pCloseArchive();

    m_pInFile = new CInFile(lpFileName);

    if (m_pInFile->Open()) {
        HRESULT hr = E_FAIL;
        unsigned __int64 max = 1 << 22;
        BOOL bFounded = FALSE;
        CArchiveOpenCallback *pCallback = new CArchiveOpenCallback(this);

        for (int i = 0; i < m_pModule->m_nNumberOfFormats && !bFounded; ++i) {
            hr = m_pModule->m_pfnCreateObject(&m_pModule->m_pInfo[i].uid, &IID_IInArchive, (void**)&m_pArchive);

            if (SUCCEEDED(hr)) {
                GUID g = m_pModule->m_pInfo[i].uid;
                //ATLTRACE("%d\t%02x\n", i, g.Data4[5]);
                m_pInFile->Seek(0, STREAM_SEEK_SET, NULL);
                hr = m_pArchive->Open(m_pInFile, &max, pCallback);

                if (hr == S_OK) {
                    bFounded = TRUE;
                    break;
                }
            }
        }

        // extract
        if (bFounded) {
            if (m_pArchive->GetNumberOfItems((unsigned int*)&m_nItemsNumber) == S_OK) {
                m_nItemsNumber--;
                m_bOpened = true;
                delete pCallback;
                return true;
            }

            m_pArchive->Close();
            m_pArchive->Release();
            m_pArchive = NULL;
        }

        delete pCallback;
        m_pInFile->Release();
        m_pInFile = NULL;
    }

    return false;
}

void __stdcall SevenZipArchive::pCloseArchive()
{
    if (m_pArchive) {
        m_pArchive->Close();
        m_pArchive->Release();
        m_pArchive = NULL;
    }

    if (m_pInFile) {
        m_pInFile->Release();
        m_pInFile = NULL;
    }
}

unsigned __int64 VariantToInt64(CPropVariant *value)
{
    switch (value->vt) {
    case VT_UI1:
        return value->bVal;

    case VT_UI2:
        return value->uiVal;

    case VT_UI4:
        return value->ulVal;

    case VT_UI8:
        return (unsigned __int64)value->uhVal.QuadPart;

    default:
        return 0;
    }
}

int __cdecl compare(const void *p1, const void *p2)
{
    int i1 = *(int*)p1;
    int i2 = *(int*)p2;

    if (i1 > i2)
        return 1;

    if (i1 < i2)
        return -1;

    return 0;
}


bool __stdcall SevenZipArchive::pExtract(const char *lpDestPath)
{
    UINT nItemsNumber = 0;
    m_pArchive->GetNumberOfItems(&nItemsNumber);

    if (nItemsNumber == 0)
        return false;

    bool bResult = false;
    int lastitem = 0;
    CArchiveExtractCallback *pCallback = new CArchiveExtractCallback(this, lpDestPath);
    unsigned int *indices = (unsigned int*)malloc(nItemsNumber * sizeof(unsigned int));

    for (int i = 0; i < nItemsNumber; i++) {
        indices[lastitem] = i;
        lastitem++;
    }

    if (m_pArchive->Extract(indices, (unsigned int)lastitem, 0, pCallback) == S_OK)
        bResult = true;

    m_files = pCallback->m_files;
    delete pCallback;
    delete indices;
    return bResult;
}
