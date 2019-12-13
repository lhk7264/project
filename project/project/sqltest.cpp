#include<mysql.h>
#include <stdio.h>
#pragma comment(lib,"libmySQL.lib")

using namespace std;

void main()
{
	printf("mysql : %s\n", mysql_get_client_info());
}