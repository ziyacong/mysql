#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "cgic.h"

char * headname = "head.html";
char * footname = "footer.html";
int cgiMain()
{
	FILE * fd;
	char ch;
	fprintf(cgiOut, "Content-type:text/html;charset=utf-8\n\n");
	if(!(fd = fopen(headname, "r"))){
		fprintf(cgiOut, "Cannot open file, %s\n", headname);
		return -1;
	}
	ch = fgetc(fd);

	while(ch != EOF){
		fprintf(cgiOut, "%c", ch);
		ch = fgetc(fd);
	}
	fclose(fd);
	
/*	fprintf(cgiOut, "<head><meta charset=\"utf-8\"/><title>查询结果</title>\
			<style>table {width:400px; margin: 50px auto; border: 1px solid gray; border-collapse: collapse; border-spacing: none; text-align:center;}\
			tr,td,th{border: 1px solid gray;}\
			</style>\
			</head>");*/

	fprintf(cgiOut, "<head><meta charset=\"utf-8\"><title>查询结果</title>\
		    <link rel=\"stylesheet\" href=\"/stu/public/css/bootstrap.min.css\">\
		</head>");


	int ret;
	MYSQL *db;
	char sql[128] = "\0";
	char stu="stu";


  sprintf(sql, "select * from stu");



	//初始化
	db = mysql_init(NULL);
	if (db == NULL)
	{
		fprintf(cgiOut,"mysql_init fail:%s\n", mysql_error(db));
		return -1;
	}

	//连接数据库
	db = mysql_real_connect(db, "127.0.0.1", "root", "123456", "stu",  3306, NULL, 0);
	if (db == NULL)
	{
		fprintf(cgiOut,"mysql_real_connect fail:%s\n", mysql_error(db));
		mysql_close(db);
		return -1;
	}


	if ((ret = mysql_real_query(db, sql, strlen(sql) + 1)) != 0)
	{
		fprintf(cgiOut,"mysql_real_query fail:%s\n", mysql_error(db));
		mysql_close(db);
		return -1;
	}

	MYSQL_RES *res;
	res = mysql_store_result(db);
	if (res == NULL)
	{
		fprintf(cgiOut,"mysql_store_result fail:%s\n", mysql_error(db));
		return -1;
	}

	fprintf(cgiOut, "<div class=\"container\"> <h1 class=\"text-center\">查询结果</h1>");

	fprintf(cgiOut,"<table class=\"table table-striped table-bordered\"><tr>");
	int i = 0;

	unsigned int fields;
	fields = mysql_num_fields(res);

	MYSQL_FIELD *mysql_filed;
	mysql_filed = mysql_fetch_fields(res);
	for (i = 0; i < fields ; i++)
	{
		fprintf(cgiOut, "<th>%s</th>", mysql_filed[i].name);
	}
	fprintf(cgiOut,"</tr>");

	//访问每一条记录的值
	MYSQL_ROW  row;
	unsigned long  *len;

	while ((row = mysql_fetch_row(res)) != NULL)
	{
		fprintf(cgiOut,"<tr>");
		len = mysql_fetch_lengths(res);
		for (i = 0; i < fields ; i++)
		{
			fprintf(cgiOut,"<td>%.*s</td>", (int)len[i], row[i]);
		}
		fprintf(cgiOut,"</tr>");
	}
	fprintf(cgiOut,"</table></div>");



	mysql_close(db);
	return 0;
}
