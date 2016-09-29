#ifndef __CACTUS_CACTUS_WMI_H__
#define __CACTUS_CACTUS_WMI_H__

#include <Windows.h>
#include <comdef.h>
#include <wbemidl.h>
#include <string>
#include <atlstr.h>

namespace cactus
{
class CWmiUtilities
{
public:
    CWmiUtilities();
    virtual ~CWmiUtilities();

private:
    //禁用拷贝构造
    CWmiUtilities(CWmiUtilities& objWmiUtilities);
    //禁用“=”号
    CWmiUtilities& operator=(CWmiUtilities& objWmiUtilities);

private:
    BOOL PutInstance();

public:
    //连接到WMI
    BOOL ConnectWmi(const wchar_t* csWmiNameSpace);

    //执行WQL语句
    BOOL ExecQuery(const wchar_t* csWQL);

    BOOL BeginEnumClassObject(const wchar_t* className);

    //枚举下一个IWbemClassObject实例,第一次调用得到第一个实例
    BOOL Next();

    //尝试枚举下一个IWbemClassObject实例,与Next()相同，照顾自己的RecordSet编程经验
    //一般用于只返回一条实例记录的情况
    BOOL IsEof();


    template <typename T>
    T GetInt(std::wstring item)
    {
        VARIANT prop;
        VariantInit(&prop);
        pls_->Get(bstr_t(item.c_str()), 0, &prop, 0, 0);
        T value = prop.intVal;
        VariantClear(&prop);
        return value;
    }

    template <typename T>
    T GetUInt(std::wstring item)
    {
        VARIANT prop;
        VariantInit(&prop);
        pls_->Get(bstr_t(item.c_str()), 0, &prop, 0, 0);
        T value = prop.uintVal;
        VariantClear(&prop);
        return value;
    }


	HRESULT GetValue(const wchar_t* name, CComVariant* value);
	HRESULT GetValue(const wchar_t* name, CString* value);
	HRESULT GetValue(const wchar_t* name, bool* value);
	HRESULT GetValue(const wchar_t* name, int* value);
	HRESULT GetValue(const wchar_t* name, UINT32* value);


    std::wstring GetString(std::wstring);

    //从WMI实例中设置或读取一个string属性
    BOOL GetStringProperty(const wchar_t* csName, std::wstring& csValue);
    BOOL PutStringProperty(const wchar_t* csName, const wchar_t* csValue);

    //从WMI实例中设置或读取一个uint32属性
    BOOL GetUINT32Property(const wchar_t* csName, UINT32& uInt32Value);
    BOOL PutUINT32Property(const wchar_t* csName, UINT32 uInt32Value);

    //从WMI实例中设置或读取一个boolean属性
    BOOL GetBOOLProperty(const wchar_t* csName, BOOL& bValue);
    BOOL PutBOOLProperty(const wchar_t* csName, BOOL bValue);

private:
    HRESULT _hResult;
    IWbemLocator* loc_;
    IWbemServices* svc_;
    IEnumWbemClassObject* pEnum_;
    IWbemClassObject* pls_;

};
} // namespace cactus

#endif // __CACTUS_CACTUS_WMI_H__
