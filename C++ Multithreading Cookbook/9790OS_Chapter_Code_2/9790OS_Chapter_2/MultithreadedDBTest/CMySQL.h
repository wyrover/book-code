#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

class CMySQL
{
public:
    static CMySQL* CreateInstance(char* szHostName, char* szDatabase, char* szUserId, char* szPassword);
    static void ReleaseInstance();
    bool ConnectInstance();
    bool DisconnectInstance();
    bool ReadData(char* szQuery, char* szResult, size_t uBufferLenght);
    bool WriteData(char* szQuery, char* szResult, size_t uBufferLenght);
private:
    CMySQL(char* szHostName, char* szDatabase, char* szUserId, char* szPassword);
    ~CMySQL();
    char* szHostName;
    char* szDatabase;
    char* szUserId;
    char* szPassword;
    MYSQL* mysqlConnection;
    static CMySQL* mySqlInstance;
};
