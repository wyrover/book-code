//
//  FileManager.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__FileManager__
#define __FinancialSamples__FileManager__

#include <string>
#include <vector>

class FileManager {
public:
    FileManager(const std::string &basePath);
    FileManager(const FileManager &);
    ~FileManager();
    FileManager &operator=(const FileManager &);

    void removeFiles();
    std::vector<std::string> getDirectoryContents();
    void listContents();
    void copyToTempDirectory(const std::string &prefix);

private:
    std::string m_basePath;
};


#endif /* defined(__FinancialSamples__FileManager__) */
