#pragma once

#include <string>
#include <list>
#include <vector>
#include <memory>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <utility>
#include <set>
#include <cstring>
#include <cstdio>


class IExportable {
public:
	virtual std::string ToJson() = 0;
	virtual std::string ToXml() = 0;
	virtual std::string ToCsv() = 0;
	virtual std::string ToText() = 0;
};

enum OutputFormat {
	TEXT,
	JSON,
	XML,
	CSV
};

std::string const base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

class helper
{

public:
	helper() {}
	~helper() {}

	template<typename T>
	static  T* GetStructFromStream(POLE::Stream* stream) {
		size_t size = sizeof(T);
		unsigned char* bytes = new unsigned char[size];

		POLE::uint64 read = stream->read(bytes, size);
		T*s = (T*) bytes;

		return s;
	}

	template<class T>
	static std::string vector_join(const std::vector<T>& v, const std::string& token, const bool useQuotes = false) {
		std::ostringstream result;
		for (typename std::vector<T>::const_iterator i = v.begin(); i != v.end(); i++) {
			if (i != v.begin()) result << token;
			if (useQuotes)
				result << "\"" << *i << "\"";
			else
				result << *i;
		}
		return result.str();
	}

	static std::string JsonEscape(const std::string &source) {
		std::string result;
		for (const char* c = source.c_str(); *c != 0; ++c) {
			switch (*c) {
			case '\"':
				result += "\\\"";
				break;
			case '\\':
				result += "\\\\";
				break;
			case '\b':
				result += "\\b";
				break;
			case '\f':
				result += "\\f";
				break;
			case '\n':
				result += "\\n";
				break;
			case '\r':
				result += "\\r";
				break;
			case '\t':
				result += "\\t";
				break;
			default:
				if (iscntrl(*c)) {
					std::ostringstream oss;
					oss << "\\u" << std::hex << std::uppercase << std::setfill('0')
						<< std::setw(4) << static_cast<int>(*c);
					result += oss.str();
				}
				else {
					result += *c;
				}
				break;
			}
		}
		return result;
	}

	static unsigned short GetItem2Byte(const unsigned char* data, unsigned int index) {
		return (data[index + 1] << 8) | data[index + 0];
	}

	static unsigned int GetItem4Byte(const unsigned char* data, unsigned int index) {
		return
			(((unsigned long long)data[index + 3]) << 32) |
			(((unsigned int)data[index + 2]) << 16) |
			(((unsigned int)data[index + 1]) << 8) |
			data[index + 0];
	}

	static std::string GetItemString(const unsigned char* data, unsigned int index, unsigned int length) {
		std::string str((char*)&data[index], (size_t)length);
		return str;
	}

	static std::wstring GetItemStringW(const unsigned char* data, unsigned int index, unsigned int length) {
		std::wstring str(reinterpret_cast<wchar_t*>(data[index]), (size_t)length / sizeof(wchar_t));
		return str;
	}

	static inline bool is_base64(unsigned char c) {
		return (isalnum(c) || (c == '+') || (c == '/'));
	}

	static std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
		std::string ret;
		int i = 0;
		int j = 0;
		unsigned char char_array_3[3];
		unsigned char char_array_4[4];

		while (in_len--) {
			char_array_3[i++] = *(bytes_to_encode++);
			if (i == 3) {
				char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
				char_array_4[3] = char_array_3[2] & 0x3f;

				for (i = 0; (i < 4); i++)
					ret += base64_chars[char_array_4[i]];
				i = 0;
			}
		}

		if (i)
		{
			for (j = i; j < 3; j++)
				char_array_3[j] = '\0';

			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (j = 0; (j < i + 1); j++)
				ret += base64_chars[char_array_4[j]];

			while ((i++ < 3))
				ret += '=';

		}

		return ret;

	}
	static std::string base64_decode(std::string const& encoded_string) {
		size_t in_len = encoded_string.size();
		size_t i = 0;
		size_t j = 0;
		int in_ = 0;
		unsigned char char_array_4[4], char_array_3[3];
		std::string ret;

		while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
			char_array_4[i++] = encoded_string[in_]; in_++;
			if (i == 4) {
				for (i = 0; i < 4; i++)
					char_array_4[i] = static_cast<unsigned char>(base64_chars.find(char_array_4[i]));

				char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
				char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

				for (i = 0; (i < 3); i++)
					ret += char_array_3[i];
				i = 0;
			}
		}

		if (i) {
			for (j = i; j < 4; j++)
				char_array_4[j] = 0;

			for (j = 0; j < 4; j++)
				char_array_4[j] = static_cast<unsigned char>(base64_chars.find(char_array_4[j]));

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
		}

		return ret;
	}
};

class OleSummary : public IExportable {
public:
	std::string FullName;
	unsigned long long Size;

	virtual std::string ToJson() {
		std::ostringstream str;
		str << "{ \"path\" : \"" << helper::JsonEscape(this->FullName) << "\", \"size\" : \"" << this->Size << "\"}";
		return str.str();
	}
	virtual std::string ToXml() {
		std::ostringstream str;
		str << "<item>";
		str << "<path>" << this->FullName << "</path>";
		str << "<size>" << this->Size << "</size>";
		str << "</item>";
		return str.str();
	}
	virtual std::string ToText() {
		std::ostringstream str;
		str << this->FullName << "\t" << this->Size;
		return str.str();
	}
	virtual std::string ToCsv() {
		std::ostringstream str;
		str << this->FullName << "," << this->Size;
		return str.str();
	}
};

class ExtensionInfo : public IExportable {
public:
	std::string Extension;
	unsigned short Version;
	std::string VersionName;
public:
	virtual std::string ToJson() {
		std::ostringstream str;
		str << "{ \"extension\" : \"" << this->Extension << "\", \"version\" : \"" << this->Version << "\", \"name\" : \"" << this->VersionName << "\"}";
		return str.str();
	}
	virtual std::string ToXml() {
		std::ostringstream str;
		str << "<item>";
		str << "<extension>" << this->Extension << "</extension>";
		str << "<version>" << this->Version << "</version>";
		str << "<name>" << this->VersionName << "</name>";
		str << "</item>";
		return str.str();
	}
	virtual std::string ToText() {
		std::ostringstream str;
		str << this->Extension << "\t" << this->Version << "\t" << this->VersionName;
		return str.str();
	}
	virtual std::string ToCsv() {
		std::ostringstream str;
		str << this->Extension << "," << this->Version << "," << this->VersionName;
		return str.str();
	}
};
