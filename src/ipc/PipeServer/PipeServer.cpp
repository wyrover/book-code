// PipeServer.cpp : Defines the entry point for the console application.
//


#include <io.h>
#include <fcntl.h>
#include "PipeConst.h"
#include "CPipeServer.h"

int _tmain(int argc, _TCHAR* argv[])
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    std::wcout << _T("---------------------Pipe Server--------------------") << std::endl;
    std::wstring sPipeName(PIPENAME);
    CPipeServer* pServer = new CPipeServer(sPipeName);
    ::WaitForSingleObject(pServer->GetThreadHandle(), INFINITE);
    delete pServer;
    pServer = NULL;
    return 0;
}

