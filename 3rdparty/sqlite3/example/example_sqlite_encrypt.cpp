#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;

    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");
    return 0;
}

static char* passwd = "helloworld";

int main(int argc, char* argv[])
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int  rc;
    char *sql;
    /* Open database */
    rc = sqlite3_open("test.db", &db);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    } else {
        fprintf(stdout, "Opened database successfully\n");
    }

    rc = sqlite3_key(db, passwd, strlen(passwd));

    if (rc) {
        fprintf(stderr, "Can't encrypt database: %s\n", sqlite3_errmsg(db));
        exit(0);
    } else {
        fprintf(stdout, "Encrypt database successfully\n");
    }

    /* Create SQL statement */
    sql = "CREATE TABLE IF NOT EXISTS COMPANY ("  \
          "ID INT PRIMARY KEY     NOT NULL," \
          "NAME           TEXT    NOT NULL," \
          "AGE            INT     NOT NULL," \
          "ADDRESS        CHAR(50)," \
          "SALARY         REAL );"; //Create table
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }

    sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
          "VALUES (1, 'Paul', 32, 'California', 20000.00); " \
          "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
          "VALUES (2, 'Allen', 25, 'Texas', 15000.00); "     \
          "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
          "VALUES (3, 'Teddy', 23, 'Norway', 20000.00);" \
          "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
          "VALUES (4, 'Mark', 25, 'Rich-Mond', 65000.00);";  //Insert data
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Records created successfully\n");
    }

    /* Create SQL statement */
    sql = "SELECT * from COMPANY"; //Select data
    /* Execute SQL statement */
    const char* data = "Callback function called";
    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Operation done successfully\n");
    }

    sqlite3_close(db);
    return 0;
}