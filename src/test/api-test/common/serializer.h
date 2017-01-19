#include <Windows.h>
#include <vector>
#include <map>
#include <atlstr.h>

class Serializer
{
	const BYTE*  m_buf;
	size_t       m_size;
	std::vector<BYTE> m_storage;

public:

	Serializer();
	Serializer(const CString& hex);
	Serializer(const BYTE* buf, size_t size);

	CString GetHex() const;

	bool IsReading() const { return m_buf != NULL; }

	Serializer& SerSimpleType(BYTE* ptr, size_t size);
	Serializer& operator << (BOOL& val) { return SerSimpleType((BYTE*)&val, sizeof(val)); }
	Serializer& operator << (USHORT& val) { return SerSimpleType((BYTE*)&val, sizeof(val)); }
	Serializer& operator << (DWORD& val) { return SerSimpleType((BYTE*)&val, sizeof(val)); }
	Serializer& operator << (HANDLE& val) { return SerSimpleType((BYTE*)&val, sizeof(val)); }
	Serializer& operator << (CStringA& val);
	Serializer& operator << (CStringW& val);
};

//Serializer& operator << (Serializer& ser, MINIDUMP_EXCEPTION_INFORMATION& val);
//Serializer& operator << (Serializer& ser, Config& cfg);
//Serializer& operator << (Serializer& ser, Params& param);