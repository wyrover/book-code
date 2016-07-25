1 把mysql.txt中的脚本拷到SQL Server 的查询分析器中， 执行，产生数据库ASC

2 执行ASC.reg 把数据库参数写入注册表中  位于HKEY_CURRENT_USER\Software\iXRayASC\Option\DBConnection下

3 这里是假定登录数据库的用户名为：sa  密码为：WZY，如果与假定的不一致请修改注册表HKEY_CURRENT_USER\Software\iXRayASC\Option\DBConnection下的LogNm 和 LogPWD；不一致时还需修改程序ASC(my)中的数据库连接字符串，具体如下：BOOL CASCApp::SetGlobleConnect()
{
	CString connectStr = "driver={SQL Server};Server=127.0.0.1;DATABASE=ASC;UID=sa;PWD=WZY";
。。。
}
修改其中的UID 和PWD

4 按附录配置好openssh服务，并建立公钥认证

5 因监听端程序没有提供，要模拟程序运行，请把 \data\log 目录下的数据拷到监听端，手工上传到邮件处理中心对应的log目录下。为了测试方便,也可把\data\log 目录下的数据直接拷到\home\username\log目录下进行测试.
