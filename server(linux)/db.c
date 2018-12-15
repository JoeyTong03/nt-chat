//mysql_demo.cpp

#include <mysql.h> // mysql特有
#include <stdio.h>

enum RETURNTYPE
{
    TRUE,
    FALSE
};

//初始化数据库
enum RETURNTYPE InitDatabase(MYSQL** _mysql)
{
    /* 初始化 mysql 变量，失败返回NULL */
    if ((*_mysql = mysql_init(NULL)) == NULL)
    {
        printf("mysql_init failed\n");
        return -1;
    }

    /* 连接数据库，失败返回NULL
       1、mysqld没运行
       2、没有指定名称的数据库存在 */
    if (mysql_real_connect(*_mysql, "localhost", "root", "123456", "GroupChat", 0, NULL, 0) == NULL)
    {
        printf("mysql_real_connect failed(%s)", mysql_error(*_mysql));
        return FALSE;
    }

    /* 设置字符集，否则读出的字符乱码，即使/etc/my.cnf中设置也不行 */
    mysql_set_character_set(*_mysql, "gbk");

    return TRUE;
}

//判断用户密码是否正确
enum RETURNTYPE JudgeUser(MYSQL* _mysql, char* _username, char* _keyword)
{
    MYSQL_RES *result;
    MYSQL_ROW row;

    /* 进行查询，成功返回0，不成功非0
    1、查询字符串存在语法错误
    2、查询不存在的数据表 */
    char sql[200];
    sprintf(
        sql,
        "select count(username) from user where username='%s' and keyword='%s'",
        _username,
        _keyword);

    if (mysql_query(_mysql, sql))
    {
        printf("mysql_query_connect failed(%s)", mysql_error(_mysql));
        return -1;
    }

    /* 将查询结果存储起来，出现错误则返回NULL
       注意：查询结果为NULL，不会返回NULL */
    if ((result = mysql_store_result(_mysql)) == NULL)
    {
        printf("mysql_store_result failed");
        return -1;
    }

    /* 打印当前查询到的记录的数量 */
    //printf("select return %d records\n",mysql_num_rows(result));

    /* 循环读取所有满足条件的记录
       1、返回的列顺序与select指定的列顺序相同，从row[0]开始
       2、不论数据库中是什么类型，C中都当作是字符串来进行处理，如果有必要，需要自己进行转换
       3、根据自己的需要组织输出格式 */
    int resultnum = 0;
    while ((row = mysql_fetch_row(result)) != NULL)
    {
        resultnum = atoi(row[0]);
    }

    /* 释放result */
    mysql_free_result(result);

    if (resultnum == 1)
        return TRUE;
    else
        return FALSE;

}

int main(int argc, char *argv[])
{
    MYSQL* mysql;

    InitDatabase(&mysql);

    char username[16]="MiaoMiaoYang1";
    char keyword[12]="123456";
    if(JudgeUser(mysql,username,keyword)==TRUE)
    {
        printf("successful");
    }
    


    /* 关闭整个连接 */
    mysql_close(mysql);

    return 0;
}
