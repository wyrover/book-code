// console_template.cpp : Defines the entry point for the console application.
//

//#include "targetver.h"
#include <msgpack.hpp>
#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cassert>
#include <tracetool/tracetool.h>


bool file_get_contents(const std::wstring& filename, std::vector<unsigned char>& buffer)
{
    std::ifstream file(filename.c_str(), std::ios_base::binary);

    if (file) {
        file.seekg(0, std::ios_base::end);
        std::streamsize size = file.tellg();

        if (size > 0) {
            file.seekg(0, std::ios_base::beg);
            buffer.resize(static_cast<std::size_t>(size));
            file.read((char*)&buffer[0], size);
        }

        return true;
    } else {
        return false;
    }
}


int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, _T(""));
    std::wcout.imbue(std::locale("")); 
	TRACE_REGKEY_W(L"HKEY_LOCAL_MACHINE", L"SOFTWARE\\Microsoft\\Windows");
	TRACE_SEND_A("test");
    
    /*std::vector<unsigned char> v1;
    v1.push_back('h');
    v1.push_back('e');
    v1.push_back('\0');
    std::stringstream ss;
    msgpack::pack(ss, v1);
    msgpack::pack(ss, "world");
    msgpack::pack(ss, "hello");
    msgpack::pack(ss, v1);	
    ss.seekg(0);
    FILE *f = fopen("msgpack-test-data.dat", "wb+");
    fwrite(ss.str().data(), sizeof(&ss.str().data()[0]), ss.str().size(), f);
    fflush(f);
    fclose(f);*/
    std::vector<unsigned char> out_data;

    if (file_get_contents(L"msgpack-test-data.dat", out_data)) {
        //std::cout << "v3:" << &v3[0] << std::endl;
        msgpack::zone zone_;
        std::size_t offset = 0;
        msgpack::object obj1 = msgpack::unpack(zone_, (const char*)&out_data[0], out_data.size(), offset);
        std::vector<unsigned char> data1 = obj1.as<std::vector<unsigned char> >();
        std::cout << "数据长度:" << data1.size() << " 数据内容:" << &data1[0] << std::endl;
        std::string const& string2 = msgpack::unpack(zone_, (const char*)&out_data[0], out_data.size(), offset).as<std::string>();
        std::cout << "数据长度:" << string2.size() << " 数据内容:" << string2 << std::endl;
        std::string const& string3 = msgpack::unpack(zone_, (const char*)&out_data[0], out_data.size(), offset).as<std::string>();
        std::cout << "数据长度:" << string3.size() << " 数据内容:" << string3 << std::endl;

        msgpack::object obj2 = msgpack::unpack(zone_, (const char*)&out_data[0], out_data.size(), offset);
        std::vector<unsigned char> data4 = obj2.as<std::vector<unsigned char> >();
        std::cout << "数据长度:" << data4.size() << " 数据内容:" << &data4[0] << std::endl;
    }

   
	
	//_tsystem(_T("pause"));
    return 0;
}

