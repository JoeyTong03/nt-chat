//mysql_demo.cpp

#include <mysql.h> // mysql特有
#include <stdio.h>
#include <malloc.h>
#include <string.h>

enum RETURNTYPE
{
    FALSE,
    TRUE
};

//初始化数据库
enum RETURNTYPE InitDatabase(MYSQL **_mysql)
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

//判断该用户是不是首次登陆
enum RETURNTYPE JudgeFirstLog(MYSQL *_mysql, char *_username)
{
    MYSQL_RES *result;
    MYSQL_ROW row;

    /* 进行查询，成功返回0，不成功非0
    1、查询字符串存在语法错误
    2、查询不存在的数据表 */
    char sql[200];
    sprintf(
        sql,
        "select firstlog from user where username='%s'",
        _username);

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

    /* 循环读取所有满足条件的记录
       1、返回的列顺序与select指定的列顺序相同，从row[0]开始
       2、不论数据库中是什么类型，C中都当作是字符串来进行处理，如果有必要，需要自己进行转换
       3、根据自己的需要组织输出格式 */
    int resultnum = 0;
    while ((row = mysql_fetch_row(result)) != NULL)
    {
        resultnum = atoi(row[0]);
    }

    if (resultnum == 1)
    {
        //首次登陆 - 将数据库写
        char tmpsql[200];
        sprintf(
            tmpsql,
            "update user set firstlog=false where username='%s';",
            _username);

        if (mysql_query(_mysql, tmpsql))
        {
            printf("mysql_query_connect failed(%s)", mysql_error(_mysql));
            return -1;
        }

        /* 释放result */
        mysql_free_result(result);

        return TRUE;
    }
    else
    {
        /* 释放result */
        mysql_free_result(result);
        return FALSE;
    }
}

//判断用户密码是否正确 1-正确 2-密码错误 3-用户名不存在 4-首次登陆需要改密[前提：用户名与密码正确]
int JudgeUser(MYSQL *_mysql, char *_username, char *_keyword)
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

    if (resultnum == 1)
    {
        /* 用户名和密码正确，判断是不是首次登陆，需不需要改密 */
        if (JudgeFirstLog(_mysql, _username) == TRUE)
        {
            /* 释放result */
            mysql_free_result(result);

            //首次登陆，需要改密
            return 4;
        }
        else
        {
            /* 释放result */
            mysql_free_result(result);

            return 1;
        }
    }
    else
    {
        /* 用户名和密码其中一方错误 - 判断是哪个错了*/
        sprintf(
            sql,
            "select count(username) from user where username='%s'",
            _username);

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
        resultnum = 0;
        while ((row = mysql_fetch_row(result)) != NULL)
        {
            resultnum = atoi(row[0]);
        }

        if (resultnum == 1)
        {
            //用户名是存在的 - 密码出错了
            /* 释放result */
            mysql_free_result(result);
            return 2;
        }
        else
        {
            //用户名不存在
            mysql_free_result(result);
            return 3;
        }
    }
}

//更新数据库username的密码为_keywork
enum RETURNTYPE UpdateSecret(MYSQL *_mysql, char *_username, char *_keyword)
{
    char sql[200];
    sprintf(
        sql,
        "update user set keyword='%s' where username='%s'; ",
        _keyword,
        _username);

    if (mysql_query(_mysql, sql))
    {
        printf("mysql_query_connect failed(%s)", mysql_error(_mysql));
        return FALSE;
    }

    return TRUE;
}

//当前在线用户增加用户
enum RETURNTYPE AddOnlineUser(MYSQL *_mysql, char *_username,int id)
{
    /* 进行查询，成功返回0，不成功非0
    1、查询字符串存在语法错误
    2、查询不存在的数据表 */
    char sql[200];
    sprintf(
        sql,
        "insert into onlineuser(username,id) values ('%s',%d); ",
        _username,
        id    
    );

    if (mysql_query(_mysql, sql))
    {
        printf("mysql_query_connect failed(%s)", mysql_error(_mysql));
        return FALSE;
    }

    return TRUE;
}

//当前在线用户减少用户
enum RETURNTYPE DelOnlineUser(MYSQL *_mysql, char *_username)
{
    /* 进行查询，成功返回0，不成功非0
    1、查询字符串存在语法错误
    2、查询不存在的数据表 */
    char sql[200];
    sprintf(
        sql,
        "delete from onlineuser where username='%s'; ",
        _username);

    if (mysql_query(_mysql, sql))
    {
        printf("mysql_query_connect failed(%s)", mysql_error(_mysql));
        return FALSE;
    }

    return TRUE;
}

//返回当前在线用户的文件名
char *GetOnlineUsername(MYSQL *_mysql, int _id)
{
    MYSQL_RES *result;
    MYSQL_ROW row;

    /* 进行查询，成功返回0，不成功非0
    1、查询字符串存在语法错误
    2、查询不存在的数据表 */
    char sql[200];
    sprintf(
        sql,
        "select username from onlineuser where id=%d;",
        _id);

    if (mysql_query(_mysql, sql))
    {
        printf("mysql_query_connect failed(%s)", mysql_error(_mysql));
        return NULL;
    }

    /* 将查询结果存储起来，出现错误则返回NULL
       注意：查询结果为NULL，不会返回NULL */
    if ((result = mysql_store_result(_mysql)) == NULL)
    {
        printf("mysql_store_result failed");
        return NULL;
    }

    /* 循环读取所有满足条件的记录
       1、返回的列顺序与select指定的列顺序相同，从row[0]开始
       2、不论数据库中是什么类型，C中都当作是字符串来进行处理，如果有必要，需要自己进行转换
       3、根据自己的需要组织输出格式 */
    char *buf = (char *)malloc(16 * sizeof(char));
    while ((row = mysql_fetch_row(result)) != NULL)
    {
        sprintf(buf, "%s", row[0]);
    }

    /* 释放result */
    mysql_free_result(result);

    return buf;
}

//返回当前在线用户的ID号
int GetOnlineId(MYSQL *_mysql, char *_username)
{
    MYSQL_RES *result;
    MYSQL_ROW row;

    /* 进行查询，成功返回0，不成功非0
    1、查询字符串存在语法错误
    2、查询不存在的数据表 */
    char sql[200];
    sprintf(
        sql,
        "select id from onlineuser where username='%s';",
        _username
    );

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

    /* 循环读取所有满足条件的记录
       1、返回的列顺序与select指定的列顺序相同，从row[0]开始
       2、不论数据库中是什么类型，C中都当作是字符串来进行处理，如果有必要，需要自己进行转换
       3、根据自己的需要组织输出格式 */
    int buf;
    while ((row = mysql_fetch_row(result)) != NULL)
    {
        buf=atoi(row[0]);
    }

    /* 释放result */
    mysql_free_result(result);

    return buf;
}

//返回当前所有在线用户
char *GetAllUsers(MYSQL *_mysql)
{
    MYSQL_RES *result;
    MYSQL_ROW row;

    /* 进行查询，成功返回0，不成功非0
    1、查询字符串存在语法错误
    2、查询不存在的数据表 */
    char sql[200];
    sprintf(
        sql,
        "select * from onlineuser;");

    if (mysql_query(_mysql, sql))
    {
        printf("mysql_query_connect failed(%s)", mysql_error(_mysql));
        return NULL;
    }

    /* 将查询结果存储起来，出现错误则返回NULL
       注意：查询结果为NULL，不会返回NULL */
    if ((result = mysql_store_result(_mysql)) == NULL)
    {
        printf("mysql_store_result failed");
        return NULL;
    }

    /* 循环读取所有满足条件的记录
       1、返回的列顺序与select指定的列顺序相同，从row[0]开始
       2、不论数据库中是什么类型，C中都当作是字符串来进行处理，如果有必要，需要自己进行转换
       3、根据自己的需要组织输出格式 */
    char *buf = NULL;
    int size = 0;
    while ((row = mysql_fetch_row(result)) != NULL)
    {
        if (buf == NULL)
        {
            size = strlen(row[0]) + 2;
            buf = (char *)malloc(size * sizeof(char));
            sprintf(buf, "@%s", row[0]);
        }
        else
        {
            char *tmpbuf = NULL;
            size += strlen(row[0]) + 2;
            tmpbuf = (char *)malloc(size * sizeof(char));
            sprintf(tmpbuf, "%s@%s", buf, row[0]);
            free(buf);
            buf = tmpbuf;
        }
    }

    /* 释放result */
    mysql_free_result(result);

    return buf;
}

int main(int argc, char *argv[])
{
    MYSQL *mysql;

    InitDatabase(&mysql);

    char username1[16] = "MiaoMiaoYang";
    char username2[16] = "MiaoMiaoYan";
    char username3[16] = "MiaoMiaoYag";
    char username4[16] = "MiaoMiaoYng";
    char username5[16] = "MiaoMiaoang";
    char keyword1[12] = "123456";
    char keyword2[12] = "mkamskksa";

    // printf("首次登陆-4:%d\n", JudgeUser(mysql, username1, keyword1));
    // printf("密码错误-2:%d\n", JudgeUser(mysql, username1, keyword2));
    // printf("用户消失-3:%d\n", JudgeUser(mysql, username2, keyword1));
    // printf("正    确-1:%d\n", JudgeUser(mysql, username1, keyword1));

    //    UpdateSecret(mysql,username1,keyword2);

    AddOnlineUser(mysql, username1,1);
    AddOnlineUser(mysql, username2,2);
    AddOnlineUser(mysql, username3,3);
    AddOnlineUser(mysql, username4,4);
    AddOnlineUser(mysql, username5,5);

    printf("%d\n",GetOnlineId(mysql,username1));
    printf("%s\n",GetOnlineUsername(mysql,2));

    // char *buf = GetAllUsers(mysql);
    // if (buf != NULL)
    // {
    //     printf("%s\n", buf);
    //     free(buf);
    // }
    // else
    // {
    //     printf("empty\n");
    // }

    // DelOnlineUser(mysql, username1);
    // DelOnlineUser(mysql, username2);
    // DelOnlineUser(mysql, username3);
    // DelOnlineUser(mysql, username4);
    // DelOnlineUser(mysql, username5);

    // buf = GetAllUsers(mysql);
    // if (buf != NULL)
    // {
    //     printf("%s\n", buf);
    //     free(buf);
    // }

    /* 关闭整个连接 */
    mysql_close(mysql);

    return 0;
}
