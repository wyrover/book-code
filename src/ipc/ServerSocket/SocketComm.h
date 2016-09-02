///////////////////////////////////////////////////////////////////////////////
// FILE : SocketComm.h
// Header file for CSocketComm class
// CSocketComm
//     Generic class for Socket Communication
///////////////////////////////////////////////////////////////////////////////

#ifndef _SOCKETCOMM_H_
#define _SOCKETCOMM_H_
#include <list>

#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32")

// Event value
#define EVT_CONSUCCESS      0x0000  // Connection established
#define EVT_CONFAILURE      0x0001  // General failure - Wait Connection failed
#define EVT_CONDROP         0x0002  // Connection dropped
#define EVT_ZEROLENGTH      0x0003  // Zero length message

#ifndef BUFFER_SIZE
    #define BUFFER_SIZE     MAX_PATH
#endif
#define HOSTNAME_SIZE   MAX_PATH
#define STRING_LENGTH   40


struct SockAddrIn : public SOCKADDR_IN {
public:
    SockAddrIn()
    {
        Clear();
    }
    SockAddrIn(const SockAddrIn& sin)
    {
        Copy(sin);
    }
    ~SockAddrIn() { }
    SockAddrIn& Copy(const SockAddrIn& sin);
    void    Clear()
    {
        memset(this, 0, sizeof(SOCKADDR_IN));
    }
    bool    IsEqual(const SockAddrIn& sin) const;
    bool    IsGreater(const SockAddrIn& sin) const;
    bool    IsLower(const SockAddrIn& pm) const;
    bool    IsNull() const
    {
        return ((sin_addr.s_addr == 0L) && (sin_port == 0));
    }
    ULONG   GetIPAddr() const
    {
        return sin_addr.s_addr;
    }
    short   GetPort() const
    {
        return sin_port;
    }
    bool    CreateFrom(LPCTSTR sAddr, LPCTSTR sService, int nFamily = AF_INET);
    SockAddrIn& operator=(const SockAddrIn& sin)
    {
        return Copy(sin);
    }
    bool    operator==(const SockAddrIn& sin)
    {
        return IsEqual(sin);
    }
    bool    operator!=(const SockAddrIn& sin)
    {
        return !IsEqual(sin);
    }
    bool    operator<(const SockAddrIn& sin)
    {
        return IsLower(sin);
    }
    bool    operator>(const SockAddrIn& sin)
    {
        return IsGreater(sin);
    }
    bool    operator<=(const SockAddrIn& sin)
    {
        return !IsGreater(sin);
    }
    bool    operator>=(const SockAddrIn& sin)
    {
        return !IsLower(sin);
    }
    operator LPSOCKADDR()
    {
        return (LPSOCKADDR)(this);
    }
    size_t  Size() const
    {
        return sizeof(SOCKADDR_IN);
    }
    void    SetAddr(SOCKADDR_IN* psin)
    {
        memcpy(this, psin, Size());
    }
};

typedef std::list<SockAddrIn> CSockAddrList;

struct stMessageProxy {
    SockAddrIn address;
    BYTE byData[BUFFER_SIZE];
};

class CSocketComm
{
public:
    CSocketComm();
    virtual ~CSocketComm();

    bool IsOpen() const;    // Is Socket valid?
    bool IsStart() const;   // Is Thread started?
    bool IsServer() const;  // Is running in server mode
    bool IsBroadcast() const; // Is UDP Broadcast active
    bool IsSmartAddressing() const; // Is Smart Addressing mode support
    SOCKET GetSocket() const;   // return socket handle
    void SetServerState(bool bServer);  // Run as server mode if true
    void SetSmartAddressing(bool bSmartAddressing); // Set Smart addressing mode
    bool GetSockName(SockAddrIn& saddr_in); // Get Socket name - address
    bool GetPeerName(SockAddrIn& saddr_in); // Get Peer Socket name - address
    bool AddMembership(LPCTSTR strAddress);
    bool DropMembership(LPCTSTR strAddress);
    void AddToList(const SockAddrIn& saddr_in); // Add an address to the list
    void RemoveFromList(const SockAddrIn& saddr_in);    // Remove an address from the list
    void CloseComm();       // Close Socket
    bool WatchComm();       // Start Socket thread
    void StopComm();        // Stop Socket thread

    // Create a socket - Server side (support for multiple adapters)
    bool CreateSocketEx(LPCTSTR strHost, LPCTSTR strServiceName, int nFamily, int nType, UINT uOptions /* = 0 */);
    // Create a Socket - Server side
    bool CreateSocket(LPCTSTR strServiceName, int nProtocol, int nType, UINT uOptions = 0);
    // Create a socket, connect to (Client side)
    bool ConnectTo(LPCTSTR strDestination, LPCTSTR strServiceName, int nProtocol, int nType);

// Event function - override to get data
    virtual void OnDataReceived(const LPBYTE lpBuffer, DWORD dwCount);
    virtual void OnEvent(UINT uEvent, LPVOID lpvData);
// Run function - override to implement a new behaviour
    virtual void Run();

// Data function
    DWORD ReadComm(LPBYTE lpBuffer, DWORD dwSize, DWORD dwTimeout);
    DWORD WriteComm(const LPBYTE lpBuffer, DWORD dwCount, DWORD dwTimeout);

    // Utility functions
    static SOCKET WaitForConnection(SOCKET sock); // Wait For a new connection (Server side)
    static bool ShutdownConnection(SOCKET sock);  // Shutdown a connection
    static USHORT GetPortNumber(LPCTSTR strServiceName);    // Get service port number
    static ULONG GetIPAddress(LPCTSTR strHostName);     // Get IP address of a host
    static bool GetLocalName(LPTSTR strName, UINT nSize);   // GetLocalName
    static bool GetLocalAddress(LPTSTR strAddress, UINT nSize); // GetLocalAddress
// SocketComm - data
protected:
    HANDLE      m_hComm;        // Serial Comm handle
    HANDLE      m_hThread;      // Thread Comm handle
    bool        m_bServer;      // Server mode (true)
    bool        m_bSmartAddressing; // Smart Addressing mode (true) - many listeners
    bool        m_bBroadcast;   // Broadcast mode
    CSockAddrList m_AddrList;   // Connection address list for broadcast
    HANDLE      m_hMutex;       // Mutex object
// SocketComm - function
protected:
    // Synchronization function
    void LockList();            // Lock the object
    void UnlockList();          // Unlock the object

    static UINT WINAPI SocketThreadProc(LPVOID pParam);

private:
};

#endif // _SOCKETCOMM_H_
