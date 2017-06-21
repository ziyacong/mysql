#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
	fprintf(cgiOut, "Content-type:text/html;charset=utf-8\n\n");

	char Cid[16] = "\0";
	char id[20] = "\0";
	char chengji[20] = "\0";

	int status = 0;

	status = cgiFormString("Cid",  Cid, 16);
	if (status != cgiFormSuccess)
	{
		fprintf(cgiOut, "get Cid error!\n");
		return 1;
	}

	status = cgiFormString("id",  id, 20);
	if (status != cgiFormSuccess)
	{
		fprintf(cgiOut, "get id error!\n");
		return 1;
	}

	status = cgiFormString("chengji", chengji, 20);
	if (status != cgiFormSuccess)
	{
		fprintf(cgiOut, "get chengji error!\n");
		return 1;
	}



	//fprintf(cgiOut, "name = %s, age = %s, stuId = %s\n", name, age, stuId);

	int ret;
	char sql[128] = "\0";
	MYSQL *db;

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



	 // strcpy(sql, "");
	// ,create table major(subid int(16) not null primary key,col varchar(40) not null,sub varchar(40) not null)"
	//create table stu(id int not null primary key, name varchar(20) not null, age int not null,sex varchar(4) not null,sub varchar(40) not null,subid int(16) not null)
	//if ((ret = mysql_real_query(db, sql, strlen(sql) + 1)) != 0)
	//{
		//if (ret != 1)
	//	{
	//		fprintf(cgiOut,"mysql_real_query fail:%s\n", mysql_error(db));
	//		mysql_close(db);
	//		return -1;
	//	}
	//}



	sprintf(sql, "insert into Score values(%d,  %d ,'%s')",atoi(id),atoi(Cid),chengji);
	if (mysql_real_query(db, sql, strlen(sql) + 1) != 0)
	{
		fprintf(cgiOut, "%s\n", mysql_error(db));
		mysql_close(db);
		return -1;
	}

	fprintf(cgiOut, "add student ok!\n");
	mysql_close(db);
	return 0;
}
