#ifndef HELPER_HEXDUMP_H
#define HELPER_HEXDUMP_H

#include <string>
#include <vector>

std::string toHexArray(const std::vector<char>& oBuffer);
bool hexdumpToFile(const std::wstring& oInput, const std::wstring& oOutput);

#endif // HELPER_HEXDUMP_H