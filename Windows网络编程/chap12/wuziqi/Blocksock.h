// blocksock.h

// needs winsock.h in the precompiled headers

typedef const struct sockaddr* LPCSOCKADDR;

class CBlockingSocketException : public CException
{
	DECLARE_DYNAMIC(CBlockingSocketException)
public:
// Constructor
	CBlockingSocketException(char* pchMessage);

public:
	~CBlockingSocketException() {}
	virtual BOOL GetErrorMessage(LPTSTR lpstrError, UINT nMaxError,
		PUINT pnHelpContext = NULL);
private:
	int m_nError;
	CString m_strMessage;
};

extern void LogBlockingSocketException(LPVOID pParam, char* pch, CBlockingSocketException* pe);

class CSockAddr : public sockaddr_in {
public:
	// constructors
	CSockAddr()
		{ sin_family = AF_INET;
		  sin_port = 0;
		  sin_addr.s_addr = 0; } // Default
	CSockAddr(const SOCKADDR& sa) { memcpy(this, &sa, sizeof(SOCKADDR)); }
	CSockAddr(const SOCKADDR_IN& sin) { memcpy(this, &sin, sizeof(SOCKADDR_IN)); }
	CSockAddr(const ULONG ulAddr, const USHORT ushPort = 0) // parms are host byte ordered
		{ sin_family = AF_INET;
		  sin_port = htons(ushPort);
	      sin_addr.s_addr = htonl(ulAddr); }
	CSockAddr(const char* pchIP, const USHORT ushPort = 0) // dotted IP addr string
		{ sin_family = AF_INET;
		  sin_port = htons(ushPort);
		  sin_addr.s_addr = inet_addr(pchIP); } // already network byte ordered
	// Return the address in dotted-decimal format
	CString DottedDecimal()
		{ return inet_ntoa(sin_addr); } // constructs a new CString object
	// Get port and address (even though they're public)
	USHORT Port() const
		{ return ntohs(sin_port); }
	ULONG IPAddr() const
		{ return ntohl(sin_addr.s_addr); }
	// operators added for efficiency
	const CSockAddr& operator=(const SOCKADDR& sa)
		{ memcpy(this, &sa, sizeof(SOCKADDR));
		  return *this; }
	const CSockAddr& operator=(const SOCKADDR_IN& sin)
		{ memcpy(this, &sin, sizeof(SOCKADDR_IN));
		  return *this; }
	operator SOCKADDR()
		{ return *((LPSOCKADDR) this); }
	operator LPSOCKADDR()
		{ return (LPSOCKADDR) this; }
	operator LPSOCKADDR_IN()
		{ return (LPSOCKADDR_IN) this; }
};

// member functions truly block and must not be used in UI threads
// use this class as an alternative to the MFC CSocket class
class CBlockingSocket : public CObject
{
	DECLARE_DYNAMIC(CBlockingSocket)
public:
	SOCKET m_hSocket;
	CBlockingSocket() { m_hSocket = NULL; }
	void Cleanup();
	void Create(int nType = SOCK_STREAM);
	void Close();
	void Bind(LPCSOCKADDR psa);
	void Listen();
	void Connect(LPCSOCKADDR psa);
	BOOL Accept(CBlockingSocket& s, LPSOCKADDR psa);
	int Send(const char* pch, const int nSize, const int nSecs);
	int Write(const char* pch, const int nSize, const int nSecs);
	int Receive(char* pch, const int nSize, const int nSecs);
	int SendDatagram(const char* pch, const int nSize, LPCSOCKADDR psa, 
		const int nSecs);
	int ReceiveDatagram(char* pch, const int nSize, LPSOCKADDR psa, 
		const int nSecs);
	void GetPeerAddr(LPSOCKADDR psa);
	void GetSockAddr(LPSOCKADDR psa);
	static CSockAddr GetHostByName(const char* pchName, 
		const USHORT ushPort = 0);
	static const char* GetHostByAddr(LPCSOCKADDR psa);
	operator SOCKET()
		{ return m_hSocket; }
};

class CHttpBlockingSocket : public CBlockingSocket
{
public:
	DECLARE_DYNAMIC(CHttpBlockingSocket)
	enum {nSizeRecv = 1000}; // max receive buffer size (> hdr line length)
	CHttpBlockingSocket();
	~CHttpBlockingSocket();
	int ReadHttpHeaderLine(char* pch, const int nSize, const int nSecs);
	int ReadHttpResponse(char* pch, const int nSize, const int nSecs);
private:
	char* m_pReadBuf; // read buffer
	int m_nReadBuf; // number of bytes in the read buffer
};