#ifndef pipe_h__
#define pipe_h__

#include <Windows.h>
#include <string>

namespace cactus
{



class pipe
{
private:
    HANDLE hPipe;
    std::wstring pipePath;

public:
    pipe(std::wstring pipeName = L"mimikatz\\kiwi", std::wstring serveur = L".");
    virtual ~pipe(void);

    bool closePipe();

    bool readFromPipe(std::wstring &laReponse);
    bool writeToPipe(const std::wstring &leMessage);

    bool createServer();
    bool createClient();

    bool isConnected();
};

}

#endif // pipe_h__
