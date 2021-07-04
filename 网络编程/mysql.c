#include<stdio.h>
#include<stdlib.h>
#include<mysql/mysql.h>
void my_err(char *string,int line)
{
    printf("%s errno in %d line\n",string,line);
    exit(1);
}
int
main(void)
{
    MYSQL mysql;
    if(mysql_init(&mysql)==NULL)
        my_err("mysql_init",__LINE__);
    //初始化数据库
    if(mysql_library_init(0,NULL,NULL)!=0)
        my_err("mysql_library_init",__LINE__);
    //连接数据库
    if(mysql_real_connect(&mysql,"127.0.0.1","root","123456","my_chat",0,NULL,0)==NULL)
        my_err("mysql_real_connect",__LINE__);
    //设置中文字符
    mysql_set_character_set(&mysql,"utf8");
    if(mysql_query(&mysql,"select *from user_data where account = 1000000")!=0)
        my_err("mysql_query",__LINE__);
    MYSQL_RES *res = mysql_store_result(&mysql);
    //获取了表头的列数
    int num = mysql_num_fields(res);
    printf("%d\n",num);
    MYSQL_FIELD *filed=NULL;
    //获取表头
    while((filed = mysql_fetch_field(res)))
        printf("%-10s",filed->name);
    MYSQL_ROW row;
    //获取数据
    while(row = mysql_fetch_row(res))
    {   for(int i =0;i<num;i++)
        {
            printf("%-20s",row[i]);
        }
        printf("\n");
    }   

    return 0;
}
