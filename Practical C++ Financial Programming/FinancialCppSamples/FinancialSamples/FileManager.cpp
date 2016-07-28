//
//  FileManager.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "FileManager.h"

#define BOOST_NO_CXX11_SCOPED_ENUMS

#include <boost/filesystem/operations.hpp>

using namespace boost::filesystem;

FileManager::FileManager(const std::string &basePath)
: m_basePath(basePath)
{
}

FileManager::FileManager(const FileManager &v)
: m_basePath(v.m_basePath)
{

}

FileManager::~FileManager()
{

}

FileManager &FileManager::operator=(const FileManager &v)
{
    if (this != &v)
    {
        m_basePath = v.m_basePath;
    }
    return *this;
}

void FileManager::removeFiles()
{
    std::vector<std::string> files = getDirectoryContents();
    for (unsigned i=0; i<files.size(); ++i)
    {
        path aPath(files[i]);

        if (is_regular_file(aPath))
        {
            std::cout << " path " << files[i] << " is not a regular file " << std::endl;
        }
        else
        {
            remove(aPath);
        }
    }
}

void FileManager::listContents()
{
    std::vector<std::string> files = getDirectoryContents();
    for (unsigned i=0; i<files.size(); ++i)
    {
        path aPath(files[i]);
        if (is_regular_file(aPath))
        {
            std::cout << aPath.string() << std::endl;
        }
    }
}

std::vector<std::string> FileManager::getDirectoryContents()
{
    std::vector<std::string> results;
    path aPath(m_basePath);
    if (!is_directory(aPath))
    {
        std::cout << " incorrect path was used " << std::endl;
    }
    else
    {
        auto iterator = directory_iterator(aPath);

        std::vector <path> contents;
        copy(directory_iterator(aPath), directory_iterator(), back_inserter(contents));

        for (unsigned i=0; i<contents.size(); ++i)
        {
            results.push_back(contents[i].string());
        }
    }
    return results;
}

void FileManager::copyToTempDirectory(const std::string &tmpDir)
{
    const path tmpPath(tmpDir);
    path aPath(m_basePath);
    if (!is_directory(aPath))
    {
        std::cout << " incorrect path was used " << std::endl;
        return;
    }
    std::cout << " copying the following files: " << std::endl;
    this->listContents();

    std::vector<std::string> contents = getDirectoryContents();
    for (auto it = directory_iterator(aPath); it != directory_iterator(); ++it)
    {
        if (is_regular_file(it->path()))
        {
            copy_file(it->path(), tmpPath);
        }
    }
}

int main_filem()
{
    // create a FileManager object for the /tmp directory
    //
    FileManager fm("/tmp/");
    std::vector<std::string> contents = fm.getDirectoryContents();
    std::cout << "entries: " << std::endl;
    for (std::string entry : contents)
    {
        std::cout << entry << std::endl;
    }
    return 0;
}
