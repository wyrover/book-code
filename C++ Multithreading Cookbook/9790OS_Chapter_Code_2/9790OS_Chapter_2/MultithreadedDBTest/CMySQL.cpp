#include "stdafx.h"
#include "CMySQL.h"

CMySQL* CMySQL::mySqlInstance = NULL;

CMySQL* CMySQL::CreateInstance(char* szHostName, char* szDatabase, char* szUserId, char* szPassword)
{
    if (mySqlInstance) {
        return mySqlInstance;
    }

    return new CMySQL(szHostName, szDatabase, szUserId, szPassword);
}

void CMySQL::ReleaseInstance()
{
    if (mySqlInstance) {
        delete mySqlInstance;
    }
}

CMySQL::CMySQL(char* szHostName, char* szDatabase, char* szUserId, char* szPassword)
{
    size_t length = 0;
    this->szHostName = new char[length = strlen(szHostName) + 1];
    strcpy_s(this->szHostName, length, szHostName);
    this->szDatabase = new char[length = strlen(szDatabase) + 1];
    strcpy_s(this->szDatabase, length, szDatabase);
    this->szUserId = new char[length = strlen(szUserId) + 1];
    strcpy_s(this->szUserId, length, szUserId);
    this->szPassword = new char[length = strlen(szPassword) + 1];
    strcpy_s(this->szPassword, length, szPassword);
}

CMySQL::~CMySQL()
{
    delete szHostName;
    delete szDatabase;
    delete szUserId;
    delete szPassword;
}

bool CMySQL::ConnectInstance()
{
    MYSQL* mysqlLink = NULL;

    try {
        mysqlConnection = mysql_init(NULL);
        mysqlLink = mysql_real_connect(mysqlConnection, szHostName, szUserId, szPassword, szDatabase, 3306, NULL, 0);
    } catch (...) {
        mysqlConnection = 0;
        return false;
    }

    return mysqlLink ? true : false;
}

bool CMySQL::DisconnectInstance()
{
    try {
        mysql_close(mysqlConnection);
        return true;
    } catch (...) {
        return false;
    }
}

bool CMySQL::ReadData(char* szQuery, char* szResult, size_t uBufferLenght)
{
    int mysqlStatus = 0;
    MYSQL_RES* mysqlResult = NULL;
    MYSQL_ROW mysqlRow = NULL;
    my_ulonglong numRows = 0;
    unsigned numFields = 0;

    try {
        mysqlStatus = mysql_query(mysqlConnection, szQuery);

        if (mysqlStatus) {
            return false;
        } else {
            mysqlResult = mysql_store_result(mysqlConnection);
        }

        if (mysqlResult) {
            numRows = mysql_num_rows(mysqlResult);
            numFields = mysql_num_fields(mysqlResult);
        }

        mysqlRow = mysql_fetch_row(mysqlResult);

        if (mysqlRow) {
            if (!mysqlRow[0]) {
                mysql_free_result(mysqlResult);
                return false;
            }
        } else {
            mysql_free_result(mysqlResult);
            return false;
        }

        size_t szResultLength = strlen(mysqlRow[0]) + 1;
        strcpy_s(szResult, szResultLength > uBufferLenght ? uBufferLenght : szResultLength, mysqlRow[0]);

        if (mysqlResult) {
            mysql_free_result(mysqlResult);
            mysqlResult = NULL;
        }
    } catch (...) {
        return false;
    }

    return true;
}

bool CMySQL::WriteData(char* szQuery, char* szResult, size_t uBufferLenght)
{
    try {
        int mysqlStatus = mysql_query(mysqlConnection, szQuery);

        if (mysqlStatus) {
            size_t szResultLength = strlen("Failed!") + 1;
            strcpy_s(szResult, szResultLength > uBufferLenght ? uBufferLenght : szResultLength, "Failed!");
            return false;
        }
    } catch (...) {
        size_t szResultLength = strlen("Exception!") + 1;
        strcpy_s(szResult, szResultLength > uBufferLenght ? uBufferLenght : szResultLength, "Exception!");
        return false;
    }

    size_t szResultLength = strlen("Success") + 1;
    strcpy_s(szResult, szResultLength > uBufferLenght ? uBufferLenght : szResultLength, "Success");
    return true;
}
