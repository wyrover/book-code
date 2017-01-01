#include <vector>
#include <iomanip>
#include <fstream>



std::string toHexArray(const std::vector<char>& oBuffer)
{
    std::string s;
    char const *sep = ", ";
    char str[12];
    str[0] = 0;

	for (int i = 0; i < oBuffer.size(); ++i) {
		sprintf(&str[strlen(str)], "0x%02X", (unsigned char)oBuffer[i]);
		s += str;
		strcpy(str, sep);
	}

    return s;
}

bool hexdumpToFile(const std::wstring& oInput, const std::wstring& oOutput)
{
    std::fstream fl(oInput.c_str(), std::ios::in | std::ios::binary);
    std::fstream txt(oOutput.c_str(), std::ios::out);
    char fbuffer[16];

    while (!fl.eof()) {
        std::vector<char> buffer;
		std::string s;
        fl.read(fbuffer, sizeof(fbuffer));
        int n = fl.gcount();

        for (int i = 0; i < n; i++)
            buffer.push_back(fbuffer[i]);
		
		s.append(toHexArray(buffer));		 
		s.append(",");     
        txt << s.c_str() << std::endl;
    }

    fl.close();
    txt.close();
    return true;
}