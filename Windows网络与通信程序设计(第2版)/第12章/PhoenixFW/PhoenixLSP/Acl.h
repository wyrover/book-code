///////////////////////////////////////////////
// Acl.h文件





#include "../common/PMacRes.h"

#include "../common/TypeStruct.h"



// 为每个套节字创建一个Session		check access list  CPCheckAccess
 
class CAcl
{
public:
	CAcl();
	~CAcl();

	// 套节字的创建与关闭，对应着Session的创建与关闭
	void CheckSocket(SOCKET s, int af, int type, int protocol);
	void CheckCloseSocket(SOCKET s);
	void CheckBind(SOCKET s, const struct sockaddr *addr);

	// 查看会话是否允许（会话的远程服务信息）
	int CheckAccept(SOCKET s, SOCKET sNew, sockaddr FAR *addr);
	int CheckConnect(SOCKET s, const struct sockaddr FAR *addr);

	int	CheckSendTo(SOCKET s, const SOCKADDR *pTo);
	int	CheckRecvFrom(SOCKET s, SOCKADDR *pFrom);

private:
	// 设置会话的属性
	void SetSession(SESSION *pSession, 
		USHORT usRemotePort, ULONG ulRemoteIP, UCHAR ucDirection);
	// 向应用程序通知一个会话
	void NotifySession(SESSION *pSession, int nCode);

	int GetAccessInfo(SESSION *pSession);
	int GetAccessFromWorkMode(SESSION *pSession);


	// 在规则文件中查找指定程序的过滤规则
	int FindRule(TCHAR *szAppName, int nStart);


	// 返回索引
	int CreateSession(SOCKET s, int nProtocol);

	void DeleteSession(SOCKET s);

	void InitializeSession(SESSION *pSession);

	int FindSession(SOCKET s);






	SESSION *m_pSession;
	int m_nSessionCount;
	int m_nSessionMaxCount;

	static BOOL QueryAccess();
	static BOOL IsLocalIP(DWORD dwIP);
};