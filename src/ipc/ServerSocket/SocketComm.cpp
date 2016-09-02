///////////////////////////////////////////////////////////////////////////////
//  File:       SocketComm.cpp
//  Version:    1.4
//
//  Author:     Ernest Laurentin
//  E-mail:     elaurentin@netzero.net
//
//  Implementation of the CSocketComm and associated classes.
//
//  This code may be used in compiled form in any way you desire. This
//  file may be redistributed unmodified by any means PROVIDING it is
//  not sold for profit without the authors written consent, and
//  providing that this notice and the authors name and all copyright
//  notices remains intact.
//
//  This file is provided "as is" with no expressed or implied warranty.
//  The author accepts no liability for any damage/loss of business that
//  this c++ class may cause.
//
//  Version history
//
//  1.0 - Initial release.
//  1.1 - Add support for Smart Addressing mode
//  1.2 - Fix various issues with address list (in UDP mode)
//  1.3 - Fix bug when sending message to broadcast address
//  1.4 - Add UDP multicast support
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include <process.h>
#include <crtdbg.h>
#include "SocketComm.h"

const DWORD DEFAULT_TIMEOUT = 100L;

///////////////////////////////////////////////////////////////////////////////
// SockAddrIn Struct

///////////////////////////////////////////////////////////////////////////////
// Copy
SockAddrIn& SockAddrIn::Copy(const SockAddrIn& sin)
{
    memcpy(this, &sin, Size());
    return *this;
}

///////////////////////////////////////////////////////////////////////////////
// IsEqual
bool SockAddrIn::IsEqual(const SockAddrIn& sin) const
{
    // Is it Equal? - ignore 'sin_zero'
    return (memcmp(this, &sin, Size() - sizeof(sin_zero)) == 0);
}

///////////////////////////////////////////////////////////////////////////////
// IsGreater
bool SockAddrIn::IsGreater(const SockAddrIn& sin) const
{
    // Is it Greater? - ignore 'sin_zero'
    return (memcmp(this, &sin, Size() - sizeof(sin_zero)) > 0);
}

///////////////////////////////////////////////////////////////////////////////
// IsLower
bool SockAddrIn::IsLower(const SockAddrIn& sin) const
{
    // Is it Lower? - ignore 'sin_zero'
    return (memcmp(this, &sin, Size() - sizeof(sin_zero)) < 0);
}

///////////////////////////////////////////////////////////////////////////////
// CreateFrom
bool SockAddrIn::CreateFrom(LPCTSTR sAddr, LPCTSTR sService, int nFamily /*=AF_INET*/)
{
    Clear();
    sin_addr.s_addr = htonl(CSocketComm::GetIPAddress(sAddr));
    sin_port = htons(CSocketComm::GetPortNumber(sService));
    sin_family = nFamily;
    return !IsNull();
}


///////////////////////////////////////////////////////////////////////////////
// Construct & Destruct
CSocketComm::CSocketComm() :
    m_bServer(false), m_bSmartAddressing(false), m_bBroadcast(false),
    m_hComm(INVALID_HANDLE_VALUE), m_hThread(NULL), m_hMutex(NULL)
{
}

CSocketComm::~CSocketComm()
{
    StopComm();
}

///////////////////////////////////////////////////////////////////////////////
// Members
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// IsOpen
bool CSocketComm::IsOpen() const
{
    return (INVALID_HANDLE_VALUE != m_hComm);
}


///////////////////////////////////////////////////////////////////////////////
// IsStart
bool CSocketComm::IsStart() const
{
    return (NULL != m_hThread);
}


///////////////////////////////////////////////////////////////////////////////
// IsServer
bool CSocketComm::IsServer() const
{
    return m_bServer;
}


///////////////////////////////////////////////////////////////////////////////
// IsBroadcast
bool CSocketComm::IsBroadcast() const
{
    return m_bBroadcast;
}


///////////////////////////////////////////////////////////////////////////////
// IsSmartAddressing
bool CSocketComm::IsSmartAddressing() const
{
    return m_bSmartAddressing;
}


///////////////////////////////////////////////////////////////////////////////
// GetSocket
SOCKET CSocketComm::GetSocket() const
{
    return (SOCKET) m_hComm;
}


///////////////////////////////////////////////////////////////////////////////
// LockList
void CSocketComm::LockList()
{
    if (NULL != m_hMutex)
        WaitForSingleObject(m_hMutex, INFINITE);
}


///////////////////////////////////////////////////////////////////////////////
// UnlockList
void CSocketComm::UnlockList()
{
    if (NULL != m_hMutex)
        ReleaseMutex(m_hMutex);
}


///////////////////////////////////////////////////////////////////////////////
// AddToList
void CSocketComm::AddToList(const SockAddrIn& saddr_in)
{
    LockList();
    m_AddrList.insert(m_AddrList.end(), saddr_in);
    UnlockList();
}

///////////////////////////////////////////////////////////////////////////////
// RemoveFromList
void CSocketComm::RemoveFromList(const SockAddrIn& saddr_in)
{
    LockList();
    m_AddrList.remove(saddr_in);
    UnlockList();
}

///////////////////////////////////////////////////////////////////////////////
// SetServerState
void CSocketComm::SetServerState(bool bServer)
{
    if (!IsStart())
        m_bServer = bServer;
}


///////////////////////////////////////////////////////////////////////////////
// SetSmartAddressing : Address is included with message
void CSocketComm::SetSmartAddressing(bool bSmartAddressing)
{
    if (!IsStart())
        m_bSmartAddressing = bSmartAddressing;
}

///////////////////////////////////////////////////////////////////////////////
// OnDataReceived
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//              This function is PURE Virtual, you MUST overwrite it.  This is
//              called every time new data is available.
// PARAMETERS:
///////////////////////////////////////////////////////////////////////////////
void CSocketComm::OnDataReceived(const LPBYTE lpBuffer, DWORD dwCount)
{
}


///////////////////////////////////////////////////////////////////////////////
// OnEvent
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//              This function reports events & errors
// PARAMETERS:
//      UINT uEvent: can be one of the event value EVT_(events)
//      LPVOID lpvData: Event data if any
///////////////////////////////////////////////////////////////////////////////
void CSocketComm::OnEvent(UINT uEvent, LPVOID lpvData)
{
}

///////////////////////////////////////////////////////////////////////////////
// GetPortNumber
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//              Returns a port number based on service name or port number string
// PARAMETERS:
//  LPCTSTR strServiceName: Service name or port string
///////////////////////////////////////////////////////////////////////////////
USHORT CSocketComm::GetPortNumber(LPCTSTR strServiceName)
{
    LPSERVENT   lpservent;
    USHORT      nPortNumber = 0;

    if (_istdigit(strServiceName[0])) {
        nPortNumber = (USHORT) _ttoi(strServiceName);
    } else {
#ifdef _UNICODE
        char pstrService[HOSTNAME_SIZE];
        WideCharToMultiByte(CP_ACP, 0, strServiceName, -1, pstrService, sizeof(pstrService), NULL, NULL);
#else
        LPCTSTR pstrService = strServiceName;
#endif

        // Convert network byte order to host byte order
        if ((lpservent = getservbyname(pstrService, NULL)) != NULL)
            nPortNumber = ntohs(lpservent->s_port);
    }

    return nPortNumber;
}


///////////////////////////////////////////////////////////////////////////////
// GetIPAddress
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//      Returns an IP address.
//          - It tries to convert the string directly
//          - If that fails, it tries to resolve it as a hostname
// PARAMETERS:
//  LPCTSTR strHostName: host name to get IP address
///////////////////////////////////////////////////////////////////////////////
ULONG CSocketComm::GetIPAddress(LPCTSTR strHostName)
{
    LPHOSTENT   lphostent;
    ULONG       uAddr = INADDR_NONE;
    TCHAR       strLocal[HOSTNAME_SIZE] = { 0 };

    // if no name specified, get local
    if (NULL == strHostName) {
        GetLocalName(strLocal, sizeof(strLocal));
        strHostName = strLocal;
    }

#ifdef _UNICODE
    char strHost[HOSTNAME_SIZE] = { 0 };
    WideCharToMultiByte(CP_ACP, 0, strHostName, -1, strHost, sizeof(strHost), NULL, NULL);
#else
    LPCTSTR strHost = strHostName;
#endif
    // Check for an Internet Protocol dotted address string
    uAddr = inet_addr(strHost);

    if ((INADDR_NONE == uAddr) && (strcmp(strHost, "255.255.255.255"))) {
        // It's not an address, then try to resolve it as a hostname
        if (lphostent = gethostbyname(strHost))
            uAddr = *((ULONG *) lphostent->h_addr_list[0]);
    }

    return ntohl(uAddr);
}


///////////////////////////////////////////////////////////////////////////////
// GetLocalName
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//              Get local computer name.  Something like: "mycomputer.myserver.net"
// PARAMETERS:
//  LPTSTR strName: name of the computer is returned here
//  UINT nSize: size max of buffer "strName"
///////////////////////////////////////////////////////////////////////////////
bool CSocketComm::GetLocalName(LPTSTR strName, UINT nSize)
{
    if (strName != NULL && nSize > 0) {
        char strHost[HOSTNAME_SIZE] = { 0 };

        // get host name, if fail, SetLastError is set
        if (SOCKET_ERROR != gethostname(strHost, sizeof(strHost))) {
            struct hostent* hp;
            hp = gethostbyname(strHost);

            if (hp != NULL) {
                strncpy(strHost, hp->h_name, HOSTNAME_SIZE);
            }

            // check if user provide enough buffer
            if (strlen(strHost) > nSize) {
                SetLastError(ERROR_INSUFFICIENT_BUFFER);
                return false;
            }

            // Unicode conversion
#ifdef _UNICODE
            return (0 != MultiByteToWideChar(CP_ACP, 0, strHost, -1, strName, nSize));
#else
            _tcscpy(strName, strHost);
            return true;
#endif
        }
    } else
        SetLastError(ERROR_INVALID_PARAMETER);

    return false;
}


///////////////////////////////////////////////////////////////////////////////
// GetLocalAddress
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//              Get TCP address of local computer in dot format ex: "127.0.0.0"
// PARAMETERS:
//  LPTSTR strAddress: pointer to hold address string, must be long enough
//  UINT nSize: maximum size of this buffer
///////////////////////////////////////////////////////////////////////////////
bool CSocketComm::GetLocalAddress(LPTSTR strAddress, UINT nSize)
{
    // Get computer local address
    if (strAddress != NULL && nSize > 0) {
        char strHost[HOSTNAME_SIZE] = { 0 };

        // get host name, if fail, SetLastError is called
        if (SOCKET_ERROR != gethostname(strHost, sizeof(strHost))) {
            struct hostent* hp;
            hp = gethostbyname(strHost);

            if (hp != NULL && hp->h_addr_list[0] != NULL) {
                // IPv4: Address is four bytes (32-bit)
                if (hp->h_length < 4)
                    return false;

                // Convert address to . format
                strHost[0] = 0;
                // IPv4: Create Address string
                sprintf(strHost, "%u.%u.%u.%u",
                        (UINT)(((PBYTE) hp->h_addr_list[0])[0]),
                        (UINT)(((PBYTE) hp->h_addr_list[0])[1]),
                        (UINT)(((PBYTE) hp->h_addr_list[0])[2]),
                        (UINT)(((PBYTE) hp->h_addr_list[0])[3]));

                // check if user provide enough buffer
                if (strlen(strHost) > nSize) {
                    SetLastError(ERROR_INSUFFICIENT_BUFFER);
                    return false;
                }

                // Unicode conversion
#ifdef _UNICODE
                return (0 != MultiByteToWideChar(CP_ACP, 0, strHost, -1, strAddress, nSize));
#else
                _tcscpy(strAddress, strHost);
                return true;
#endif
            }
        }
    } else
        SetLastError(ERROR_INVALID_PARAMETER);

    return false;
}


///////////////////////////////////////////////////////////////////////////////
// WaitForConnection
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//              Wait for a network connection.  Only for connection type of socket
//              This function may fail, in this case it returns "INVALID_SOCKET"
// PARAMETERS:
//  SOCKET sock: a socket capable of receiving new connection (TCP: SOCK_STREAM)
///////////////////////////////////////////////////////////////////////////////
SOCKET CSocketComm::WaitForConnection(SOCKET sock)
{
    // Accept an incoming connection - blocking
    // no information about remote address is returned
    return accept(sock, 0, 0);
}


///////////////////////////////////////////////////////////////////////////////
// ShutdownConnection
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//              Shutdown a connection and close socket.  This will force all
//              transmission/reception to fail.
// PARAMETERS:
//  SOCKET sock: Socket to close
///////////////////////////////////////////////////////////////////////////////
bool CSocketComm::ShutdownConnection(SOCKET sock)
{
    shutdown(sock, SD_BOTH);
    return (0 == closesocket(sock));
}


///////////////////////////////////////////////////////////////////////////////
// GetSockName
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//              retrieves the local name for a socket
// PARAMETERS:
//  SockAddrIn& saddr_in: object to store address
///////////////////////////////////////////////////////////////////////////////
bool CSocketComm::GetSockName(SockAddrIn& saddr_in)
{
    if (IsOpen()) {
        int namelen = saddr_in.Size();
        return (SOCKET_ERROR != getsockname(GetSocket(), saddr_in, &namelen));
    }

    return false;
}


///////////////////////////////////////////////////////////////////////////////
// GetPeerName
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//              retrieves the name of the peer to which a socket is connected
// PARAMETERS:
//  SockAddrIn& saddr_in: object to store address
///////////////////////////////////////////////////////////////////////////////
bool CSocketComm::GetPeerName(SockAddrIn& saddr_in)
{
    if (IsOpen()) {
        int namelen = saddr_in.Size();
        return (SOCKET_ERROR != getpeername(GetSocket(), saddr_in, &namelen));
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////
// AddMembership
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//              Add membership to a multicast address
// PARAMETERS:
//  LPCTSTR strAddress: ip address for membership
///////////////////////////////////////////////////////////////////////////////
bool CSocketComm::AddMembership(LPCTSTR strAddress)
{
    if (IsOpen()) {
        int nType = 0;
        int nOptLen = sizeof(int);
        SOCKET sock = (SOCKET) m_hComm;

        if (SOCKET_ERROR != getsockopt(sock, SOL_SOCKET, SO_TYPE, (char*)&nType, &nOptLen)) {
            if (nType == SOCK_DGRAM) {
                int nTTL = 5;

                if (SOCKET_ERROR != setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (const char*)&nTTL, sizeof(nTTL))) {
                    ip_mreq mreq;
                    mreq.imr_multiaddr.s_addr = htonl(CSocketComm::GetIPAddress(strAddress));
                    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
                    return (SOCKET_ERROR != setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*)&mreq, sizeof(mreq)));
                }
            }
        }
    }

    return false;
}


///////////////////////////////////////////////////////////////////////////////
// DropMembership
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//              Remove membership from a multicast address
// PARAMETERS:
//  LPCTSTR strAddress: ip address for membership
///////////////////////////////////////////////////////////////////////////////
bool CSocketComm::DropMembership(LPCTSTR strAddress)
{
    if (IsOpen()) {
        int nType = 0;
        int nOptLen = sizeof(int);
        SOCKET sock = (SOCKET) m_hComm;

        if (SOCKET_ERROR != getsockopt(sock, SOL_SOCKET, SO_TYPE, (char*)&nType, &nOptLen)) {
            if (nType == SOCK_DGRAM) {
                ip_mreq mreq;
                mreq.imr_multiaddr.s_addr = htonl(CSocketComm::GetIPAddress(strAddress));
                mreq.imr_interface.s_addr = htonl(INADDR_ANY);
                return (SOCKET_ERROR != setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (const char*)&mreq, sizeof(mreq)));
            }
        }
    }

    return false;
}


///////////////////////////////////////////////////////////////////////////////
// CreateSocketEx
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//              This function creates a new socket for connection (SOCK_STREAM)
//              or an connectionless socket (SOCK_DGRAM).  A connectionless
//              socket should not call "accept()" since it cannot receive new
//              connection.  This is used as SERVER socket
// PARAMETERS:
//  LPCTSTR strHost: Hostname or adapter IP address
//  LPCTSTR strServiceName: Service name or port number
//  int nFamily: address family to use (set to AF_INET)
//  int nType: type of socket to create (SOCK_STREAM, SOCK_DGRAM)
//  UINT uOptions: other options to use
///////////////////////////////////////////////////////////////////////////////
bool CSocketComm::CreateSocketEx(LPCTSTR strHost, LPCTSTR strServiceName, int nFamily, int nType, UINT uOptions /* = 0 */)
{
    // Socket is already opened
    if (IsOpen())
        return false;

    // Create a Socket that is bound to a specific service provide
    // nFamily: (AF_INET)
    // nType: (SOCK_STREAM, SOCK_DGRAM)
    SOCKET sock = socket(nFamily, nType, IPPROTO_IP);

    if (INVALID_SOCKET != sock) {
        if (uOptions & SO_REUSEADDR) {
            // Inform Windows Sockets provider that a bind on a socket should not be disallowed
            // because the desired address is already in use by another socket
            BOOL optval = TRUE;

            if (SOCKET_ERROR == setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *) &optval, sizeof(BOOL))) {
                closesocket(sock);
                return false;
            }
        }

        if (nType == SOCK_DGRAM) {
            if (uOptions & SO_BROADCAST) {
                // Inform Windows Sockets provider that broadcast messages are allowed
                BOOL optval = TRUE;

                if (SOCKET_ERROR == setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *) &optval, sizeof(BOOL))) {
                    closesocket(sock);
                    return false;
                }
            }
        }

        // Associate a local address with the socket
        SockAddrIn sockAddr;
        sockAddr.CreateFrom(strHost, strServiceName, nFamily);

        if (SOCKET_ERROR == bind(sock, sockAddr, sockAddr.Size())) {
            closesocket(sock);
            return false;
        }

        // Listen to the socket, only valid for connection socket
        if (SOCK_STREAM == nType) {
            if (SOCKET_ERROR == listen(sock, SOMAXCONN)) {
                closesocket(sock);
                return false;
            }
        }

        // Success, now we may save this socket
        m_hComm = (HANDLE) sock;
    }

    return (INVALID_SOCKET != sock);
}

///////////////////////////////////////////////////////////////////////////////
// CreateSocket
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//              This function creates a new socket for connection (SOCK_STREAM)
//              or an connectionless socket (SOCK_DGRAM).  A connectionless
//              socket should not call "accept()" since it cannot receive new
//              connection.  This is used as SERVER socket
// PARAMETERS:
//  LPCTSTR strServiceName: Service name or port number
//  int nFamily: address family to use (set to AF_INET)
//  int nType: type of socket to create (SOCK_STREAM, SOCK_DGRAM)
//  UINT uOptions: other options to use
///////////////////////////////////////////////////////////////////////////////
bool CSocketComm::CreateSocket(LPCTSTR strServiceName, int nFamily, int nType, UINT uOptions /* = 0 */)
{
    return CreateSocketEx(NULL, strServiceName, nFamily, nType, uOptions);
}


///////////////////////////////////////////////////////////////////////////////
// ConnectTo
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//              Establish connection with a server service or port
// PARAMETERS:
//  LPCTSTR strDestination: hostname or address to connect (in .dot format)
//  LPCTSTR strServiceName: Service name or port number
//  int nFamily: address family to use (set to AF_INET)
//  int nType: type of socket to create (SOCK_STREAM, SOCK_DGRAM)
///////////////////////////////////////////////////////////////////////////////
bool CSocketComm::ConnectTo(LPCTSTR strDestination, LPCTSTR strServiceName, int nFamily, int nType)
{
    // Socket is already opened
    if (IsOpen())
        return false;

    // Create a Socket that is bound to a specific service provide
    // nFamily: (AF_INET)
    // nType: (SOCK_STREAM, SOCK_DGRAM)
    SOCKET sock = socket(nFamily, nType, 0);

    if (INVALID_SOCKET != sock) {
        // Associate a local address with the socket
        SockAddrIn sockAddr;

        if (false == sockAddr.CreateFrom(NULL, TEXT("0"), nFamily)) {
            closesocket(sock);
            return false;
        }

        if (SOCKET_ERROR == bind(sock, sockAddr, sockAddr.Size())) {
            closesocket(sock);
            return false;
        }

        // Now get destination address & port
        sockAddr.CreateFrom(strDestination, strServiceName);

        // try to connect - if fail, server not ready
        if (SOCKET_ERROR == connect(sock, sockAddr, sockAddr.Size())) {
            closesocket(sock);
            return false;
        }

        // Success, now we may save this socket
        m_hComm = (HANDLE) sock;
    }

    return (INVALID_SOCKET != sock);
}


///////////////////////////////////////////////////////////////////////////////
// CloseComm
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//      Close Socket Communication
// PARAMETERS:
//      None
///////////////////////////////////////////////////////////////////////////////
void CSocketComm::CloseComm()
{
    if (IsOpen()) {
        ShutdownConnection((SOCKET)m_hComm);
        m_hComm = INVALID_HANDLE_VALUE;
        m_bBroadcast = false;
    }
}


///////////////////////////////////////////////////////////////////////////////
// WatchComm
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//      Starts Socket Communication Working thread
// PARAMETERS:
//      None
///////////////////////////////////////////////////////////////////////////////
bool CSocketComm::WatchComm()
{
    if (!IsStart()) {
        if (IsOpen()) {
            HANDLE hThread;
            UINT uiThreadId = 0;
            hThread = (HANDLE)_beginthreadex(NULL,  // Security attributes
                                             0,    // stack
                                             SocketThreadProc,   // Thread proc
                                             this,   // Thread param
                                             CREATE_SUSPENDED,   // creation mode
                                             &uiThreadId);   // Thread ID

            if (NULL != hThread) {
                //SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
                ResumeThread(hThread);
                m_hThread = hThread;
                return true;
            }
        }
    }

    return false;
}


///////////////////////////////////////////////////////////////////////////////
// StopComm
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//      Close Socket and Stop Communication thread
// PARAMETERS:
//      None
///////////////////////////////////////////////////////////////////////////////
void CSocketComm::StopComm()
{
    // Close Socket
    if (IsOpen()) {
        CloseComm();
    }

    // Kill Thread
    if (IsStart()) {
        SleepEx(DEFAULT_TIMEOUT, TRUE);

        if (WaitForSingleObject(m_hThread, 1000L) == WAIT_TIMEOUT)
            TerminateThread(m_hThread, 1L);

        CloseHandle(m_hThread);
        m_hThread = NULL;
    }

    // Clear Address list
    if (!m_AddrList.empty()) {
        m_AddrList.clear();
    }

    // Destroy Synchronization objects
    if (NULL != m_hMutex) {
        CloseHandle(m_hMutex);
        m_hMutex = NULL;
    }
}


///////////////////////////////////////////////////////////////////////////////
// ReadComm
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//      Reads the Socket Communication
// PARAMETERS:
//      LPBYTE lpBuffer: buffer to place new data
//      DWORD dwSize: maximum size of buffer
//      DWORD dwTimeout: timeout to use in millisecond
///////////////////////////////////////////////////////////////////////////////
DWORD CSocketComm::ReadComm(LPBYTE lpBuffer, DWORD dwSize, DWORD dwTimeout)
{
    _ASSERTE(IsOpen());
    _ASSERTE(lpBuffer != NULL);

    if (lpBuffer == NULL || dwSize < 1L)
        return 0L;

    fd_set  fdRead  = { 0 };
    TIMEVAL stTime;
    TIMEVAL *pstTime = NULL;

    if (INFINITE != dwTimeout) {
        stTime.tv_sec = dwTimeout / 1000;
        stTime.tv_usec = (dwTimeout % 1000) * 1000;
        pstTime = &stTime;
    }

    SOCKET s = (SOCKET) m_hComm;

    // Set Descriptor
    if (!FD_ISSET(s, &fdRead))
        FD_SET(s, &fdRead);

    // Select function set read timeout
    DWORD dwBytesRead = 0L;
    int res = select(s + 1, &fdRead, NULL, NULL, pstTime);

    if (res > 0) {
        if (IsBroadcast() || IsSmartAddressing()) {
            SockAddrIn sockAddr;
            sockAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
            int nLen = sockAddr.Size();
            int nOffset = IsSmartAddressing() ? nLen : 0; // use offset for Smart addressing

            if (dwSize < (DWORD) nOffset) { // error - buffer to small
                SetLastError(ERROR_INVALID_USER_BUFFER);
                return -1L;
            }

            LPSTR lpszData = (LPSTR)(lpBuffer + nOffset);
            res = recvfrom(s, lpszData, dwSize - nOffset, 0, sockAddr, &nLen);
            // clear 'sin_zero', we will ignore them with 'SockAddrIn' anyway!
            memset(&sockAddr.sin_zero, 0, sizeof(sockAddr.sin_zero));
            // Lock the list...
            LockList();
            m_AddrList.remove(sockAddr);

            if (res >= 0) {
                // insert unique address
                m_AddrList.insert(m_AddrList.end(), sockAddr);

                if (IsSmartAddressing()) {
                    memcpy(lpBuffer, &sockAddr, sockAddr.Size());
                    res += sockAddr.Size();
                }
            } else if (WSAGetLastError() == WSAECONNRESET && m_AddrList.size() == 1) {
                // recvfrom doesn't always return the connection address for last connection
                m_AddrList.clear();
            }

            UnlockList(); // unlock this object addresses-list
        } else {
            res = recv(s, (LPSTR)lpBuffer, dwSize, 0);
        }
    }

    dwBytesRead = (DWORD)((res > 0) ? (res) : (-1L));
    return dwBytesRead;
}


///////////////////////////////////////////////////////////////////////////////
// WriteComm
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//      Writes data to the Socket Communication
// PARAMETERS:
//      const LPBYTE lpBuffer: data to write
//      DWORD dwCount: maximum characters to write
//      DWORD dwTimeout: timeout to use in millisecond
///////////////////////////////////////////////////////////////////////////////
DWORD CSocketComm::WriteComm(const LPBYTE lpBuffer, DWORD dwCount, DWORD dwTimeout)
{
    _ASSERTE(IsOpen());
    _ASSERTE(NULL != lpBuffer);

    // Accept 0 bytes message
    if (!IsOpen() || NULL == lpBuffer)
        return 0L;

    fd_set  fdWrite  = { 0 };
    TIMEVAL stTime;
    TIMEVAL *pstTime = NULL;

    if (INFINITE != dwTimeout) {
        stTime.tv_sec = dwTimeout / 1000;
        stTime.tv_usec = (dwTimeout % 1000) * 1000;
        pstTime = &stTime;
    }

    SOCKET s = (SOCKET) m_hComm;

    // Set Descriptor
    if (!FD_ISSET(s, &fdWrite))
        FD_SET(s, &fdWrite);

    // Select function set write timeout
    DWORD dwBytesWritten = 0L;
    int res = select(s + 1, NULL, &fdWrite, NULL, pstTime);

    if (res > 0) {
        // Send message to peer or broadcast it
        bool bSmartAddressing = IsSmartAddressing();

        if (IsBroadcast() || bSmartAddressing) {
            // use offset for Smart addressing
            int nOffset = bSmartAddressing ? sizeof(SOCKADDR_IN) : 0;

            if (bSmartAddressing) {
                if (dwCount < sizeof(SOCKADDR_IN)) { // error - buffer to small
                    SetLastError(ERROR_INVALID_USER_BUFFER);
                    return -1L;
                }

                // read socket address from buffer
                SockAddrIn sockAddr;
                sockAddr.SetAddr((PSOCKADDR_IN) lpBuffer);

                // Get Address and send data
                if (sockAddr.sin_addr.s_addr != htonl(INADDR_BROADCAST)) {
                    LPSTR lpszData = (LPSTR)(lpBuffer + nOffset);
                    res = sendto(s, lpszData, dwCount - nOffset, 0, sockAddr, sockAddr.Size());
                    dwBytesWritten = (DWORD)((res >= 0) ? (res) : (-1));
                    return dwBytesWritten;
                } else {
                    // NOTE: broadcast will broadcast only to our peers
                    // Broadcast send to all connected-peers
                    LockList(); // Lock this object addresses-list
                    CSockAddrList::iterator iter = m_AddrList.begin();

                    for (; iter != m_AddrList.end();) {
                        // Fix v1.3 - nOffset was missing
                        sockAddr = (*iter);
                        res = sendto(s, (LPCSTR)&lpBuffer[nOffset], dwCount - nOffset, 0, sockAddr, iter->Size());

                        if (res < 0) {
                            CSockAddrList::iterator deladdr = iter;
                            ++iter; // get next
                            m_AddrList.erase(deladdr);
                        } else
                            ++iter; // get next
                    }

                    UnlockList(); // unlock this object addresses-list
                }
            }

            // always return success - UDP
            res = (int) dwCount - nOffset;
        } else // Send to peer-connection
            res = send(s, (LPCSTR)lpBuffer, dwCount, 0);
    }

    dwBytesWritten = (DWORD)((res >= 0) ? (res) : (-1L));
    return dwBytesWritten;
}


///////////////////////////////////////////////////////////////////////////////
// Run
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//      This function runs the main thread loop
//      this implementation can be overloaded.
//      This function calls CSocketComm::OnDataReceived() (Virtual Function)
// PARAMETERS:
// NOTES:
//      You should not wait on the thread to end in this function or overloads
///////////////////////////////////////////////////////////////////////////////
void CSocketComm::Run()
{
    stMessageProxy stMsgProxy;
    DWORD   dwBytes  = 0L;
    DWORD   dwTimeout = INFINITE;
    LPBYTE  lpData  = (LPBYTE)&stMsgProxy;
    DWORD   dwSize  = sizeof(stMsgProxy);
    bool bSmartAddressing = IsSmartAddressing();

    if (!bSmartAddressing) {
        lpData = stMsgProxy.byData;
        dwSize = sizeof(stMsgProxy.byData);
    }

    // Should we run as server mode
    if (IsServer() && !bSmartAddressing) {
        if (!IsBroadcast()) {
            SOCKET sock = (SOCKET) m_hComm;
            sock = WaitForConnection(sock);

            // Get new connection socket
            if (sock != INVALID_SOCKET) {
                ShutdownConnection((SOCKET) m_hComm);
                m_hComm = (HANDLE) sock;
                OnEvent(EVT_CONSUCCESS, NULL);   // connect
            } else {
                // Do not send event if we are closing
                if (IsOpen())
                    OnEvent(EVT_CONFAILURE, NULL);   // wait fail

                return;
            }
        }
    } else {
        GetPeerName(stMsgProxy.address);
    }

    while (IsOpen()) {
        // Blocking mode: Wait for event
        dwBytes = ReadComm(lpData, dwSize, dwTimeout);

        // Error? - need to signal error
        if (dwBytes == (DWORD) - 1L) {
            // Do not send event if we are closing
            if (IsOpen()) {
                if (bSmartAddressing) {
                    RemoveFromList(stMsgProxy.address);
                }

                OnEvent(EVT_CONDROP, &stMsgProxy.address);   // lost connection
            }

            // special case for UDP, alert about the event but do not stop
            if (bSmartAddressing)
                continue;
            else
                break;
        }

        // Chars received?
        if (bSmartAddressing && dwBytes == sizeof(SOCKADDR_IN)) {
            OnEvent(EVT_ZEROLENGTH, NULL);
        } else if (dwBytes > 0L) {
            OnDataReceived(lpData, dwBytes);
        }

        //Sleep(0);
    }
}


///////////////////////////////////////////////////////////////////////////////
// SocketThreadProc
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//     Socket Thread function.  This function is the main thread for socket
//     communication - Asynchronous mode.
// PARAMETERS:
//     LPVOID pParam : Thread parameter - a CSocketComm pointer
// NOTES:
///////////////////////////////////////////////////////////////////////////////
UINT WINAPI CSocketComm::SocketThreadProc(LPVOID pParam)
{
    CSocketComm* pThis = reinterpret_cast<CSocketComm*>(pParam);
    _ASSERTE(pThis != NULL);
    pThis->Run();
    return 1L;
} // end SocketThreadProc
