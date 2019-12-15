#include <mysql.h>
#include <iostream>
#include <string.h>
#include <stdio.h>

#pragma comment(lib,"libmySQL.lib")
#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "s10633"
#define DB_NAME "carnumber"
#define CHOP(x) x[strlen(x) - 1] = ' '
using namespace std;

int main() {
    MYSQL* connection = NULL, conn;
    MYSQL_RES* sql_result;
    MYSQL_ROW   sql_row;
    int       query_stat;

  
    char CARID[11];
    char carnumber[45];
    char query[255];

    mysql_init(&conn);
   
    connection = mysql_real_connect(&conn, DB_HOST,
        DB_USER, DB_PASS,
        DB_NAME, 3306,
        (char*)NULL, 0);

    if (connection == NULL)
    {
        fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
        return 1;
    }

    query_stat = mysql_query(connection, "SELECT * FROM carnumber_capture");

    if (query_stat != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
        return 1;
    }

    sql_result = mysql_store_result(connection);
   
    printf("이:");
    fgets(CARID, 12, stdin);
    CHOP(CARID);

    printf("번호:");
    fgets(carnumber, 12, stdin);
    CHOP(carnumber);

    sprintf_s(query, "insert into carnumber_capture values " "('%s','%s')",CARID,carnumber);
    query_stat = mysql_query(connection, query);
   
    if (query_stat != 0)
    {
        fprintf(stderr, "에러 : %s", mysql_error(&conn));
        return 1;
    }
    mysql_close(connection);
}
