//mysql_demo.cpp


#include "db.h"


//��ʼ�����ݿ�
enum RETURNTYPE InitDatabase(MYSQL **_mysql)
{
    /* ��ʼ�� mysql ������ʧ�ܷ���NULL */
    if ((*_mysql = mysql_init(NULL)) == NULL)
    {
        printf("mysql_init failed\n");
        return -1;
    }

    /* �������ݿ⣬ʧ�ܷ���NULL
       1��mysqldû����
       2��û��ָ�����Ƶ����ݿ���� */
    if (mysql_real_connect(*_mysql, "localhost", "root", "123456", "GroupChat", 0, NULL, 0) == NULL)
    {
        printf("mysql_real_connect failed(%s)", mysql_error(*_mysql));
        return FALSE;
    }

    /* �����ַ���������������ַ����룬��ʹ/etc/my.cnf������Ҳ���� */
    mysql_set_character_set(*_mysql, "gbk");

    return TRUE;
}

//�жϸ��û��ǲ����״ε�½
enum RETURNTYPE JudgeFirstLog(MYSQL *_mysql, char *_username)
{
    MYSQL_RES *result;
    MYSQL_ROW row;

    /* ���в�ѯ���ɹ�����0�����ɹ���0
    1����ѯ�ַ��������﷨����
    2����ѯ�����ڵ����ݱ� */
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

    /* ����ѯ����洢���������ִ����򷵻�NULL
       ע�⣺��ѯ���ΪNULL�����᷵��NULL */
    if ((result = mysql_store_result(_mysql)) == NULL)
    {
        printf("mysql_store_result failed");
        return -1;
    }

    /* ѭ����ȡ�������������ļ�¼
       1�����ص���˳����selectָ������˳����ͬ����row[0]��ʼ
       2���������ݿ�����ʲô���ͣ�C�ж��������ַ��������д�������б�Ҫ����Ҫ�Լ�����ת��
       3�������Լ�����Ҫ��֯�����ʽ */
    int resultnum = 0;
    while ((row = mysql_fetch_row(result)) != NULL)
    {
        resultnum = atoi(row[0]);
    }

    if (resultnum == 1)
    {
        //�״ε�½ - �����ݿ�д
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

        /* �ͷ�result */
        mysql_free_result(result);

        return TRUE;
    }
    else
    {
        /* �ͷ�result */
        mysql_free_result(result);
        return FALSE;
    }
}

//�ж��û������Ƿ���ȷ 1-��ȷ 2-������� 3-�û��������� 4-�״ε�½��Ҫ����[ǰ�᣺�û�����������ȷ]
int JudgeUser(MYSQL *_mysql, char *_username, char *_keyword)
{
    MYSQL_RES *result;
    MYSQL_ROW row;

    /* ���в�ѯ���ɹ�����0�����ɹ���0
    1����ѯ�ַ��������﷨����
    2����ѯ�����ڵ����ݱ� */
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

    /* ����ѯ����洢���������ִ����򷵻�NULL
       ע�⣺��ѯ���ΪNULL�����᷵��NULL */
    if ((result = mysql_store_result(_mysql)) == NULL)
    {
        printf("mysql_store_result failed");
        return -1;
    }

    /* ��ӡ��ǰ��ѯ���ļ�¼������ */
    //printf("select return %d records\n",mysql_num_rows(result));

    /* ѭ����ȡ�������������ļ�¼
       1�����ص���˳����selectָ������˳����ͬ����row[0]��ʼ
       2���������ݿ�����ʲô���ͣ�C�ж��������ַ��������д�������б�Ҫ����Ҫ�Լ�����ת��
       3�������Լ�����Ҫ��֯�����ʽ */
    int resultnum = 0;
    while ((row = mysql_fetch_row(result)) != NULL)
    {
        resultnum = atoi(row[0]);
    }

    if (resultnum == 1)
    {
        /* �û�����������ȷ���ж��ǲ����״ε�½���費��Ҫ���� */
        if (JudgeFirstLog(_mysql, _username) == TRUE)
        {
            /* �ͷ�result */
            mysql_free_result(result);

            //�״ε�½����Ҫ����
            return 4;
        }
        else
        {
            /* �ͷ�result */
            mysql_free_result(result);

            return 1;
        }
    }
    else
    {
        /* �û�������������һ������ - �ж����ĸ�����*/
        sprintf(
            sql,
            "select count(username) from user where username='%s'",
            _username);

        if (mysql_query(_mysql, sql))
        {
            printf("mysql_query_connect failed(%s)", mysql_error(_mysql));
            return -1;
        }

        /* ����ѯ����洢���������ִ����򷵻�NULL
       ע�⣺��ѯ���ΪNULL�����᷵��NULL */
        if ((result = mysql_store_result(_mysql)) == NULL)
        {
            printf("mysql_store_result failed");
            return -1;
        }

        /* ��ӡ��ǰ��ѯ���ļ�¼������ */
        //printf("select return %d records\n",mysql_num_rows(result));

        /* ѭ����ȡ�������������ļ�¼
       1�����ص���˳����selectָ������˳����ͬ����row[0]��ʼ
       2���������ݿ�����ʲô���ͣ�C�ж��������ַ��������д�������б�Ҫ����Ҫ�Լ�����ת��
       3�������Լ�����Ҫ��֯�����ʽ */
        resultnum = 0;
        while ((row = mysql_fetch_row(result)) != NULL)
        {
            resultnum = atoi(row[0]);
        }

        if (resultnum == 1)
        {
            //�û����Ǵ��ڵ� - ���������
            /* �ͷ�result */
            mysql_free_result(result);
            return 2;
        }
        else
        {
            //�û���������
            mysql_free_result(result);
            return 3;
        }
    }
}

//�������ݿ�username������Ϊ_keywork
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

//��ǰ�����û������û�
enum RETURNTYPE AddOnlineUser(MYSQL *_mysql, char *_username,int id)
{
    /* ���в�ѯ���ɹ�����0�����ɹ���0
    1����ѯ�ַ��������﷨����
    2����ѯ�����ڵ����ݱ� */
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

//��ǰ�����û������û�
enum RETURNTYPE DelOnlineUser(MYSQL *_mysql, char *_username)
{
    /* ���в�ѯ���ɹ�����0�����ɹ���0
    1����ѯ�ַ��������﷨����
    2����ѯ�����ڵ����ݱ� */
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

//���ص�ǰ�����û����ļ���
char *GetOnlineUsername(MYSQL *_mysql, int _id)
{
    MYSQL_RES *result;
    MYSQL_ROW row;

    /* ���в�ѯ���ɹ�����0�����ɹ���0
    1����ѯ�ַ��������﷨����
    2����ѯ�����ڵ����ݱ� */
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

    /* ����ѯ����洢���������ִ����򷵻�NULL
       ע�⣺��ѯ���ΪNULL�����᷵��NULL */
    if ((result = mysql_store_result(_mysql)) == NULL)
    {
        printf("mysql_store_result failed");
        return NULL;
    }

    /* ѭ����ȡ�������������ļ�¼
       1�����ص���˳����selectָ������˳����ͬ����row[0]��ʼ
       2���������ݿ�����ʲô���ͣ�C�ж��������ַ��������д�������б�Ҫ����Ҫ�Լ�����ת��
       3�������Լ�����Ҫ��֯�����ʽ */
    char *buf = (char *)malloc(16 * sizeof(char));
    while ((row = mysql_fetch_row(result)) != NULL)
    {
        sprintf(buf, "%s", row[0]);
    }

    /* �ͷ�result */
    mysql_free_result(result);

    return buf;
}

//���ص�ǰ�����û���ID��
int GetOnlineId(MYSQL *_mysql, char *_username)
{
    MYSQL_RES *result;
    MYSQL_ROW row;

    /* ���в�ѯ���ɹ�����0�����ɹ���0
    1����ѯ�ַ��������﷨����
    2����ѯ�����ڵ����ݱ� */
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

    /* ����ѯ����洢���������ִ����򷵻�NULL
       ע�⣺��ѯ���ΪNULL�����᷵��NULL */
    if ((result = mysql_store_result(_mysql)) == NULL)
    {
        printf("mysql_store_result failed");
        return -1;
    }

    /* ѭ����ȡ�������������ļ�¼
       1�����ص���˳����selectָ������˳����ͬ����row[0]��ʼ
       2���������ݿ�����ʲô���ͣ�C�ж��������ַ��������д�������б�Ҫ����Ҫ�Լ�����ת��
       3�������Լ�����Ҫ��֯�����ʽ */
    int buf;
    while ((row = mysql_fetch_row(result)) != NULL)
    {
        buf=atoi(row[0]);
    }

    /* �ͷ�result */
    mysql_free_result(result);

    return buf;
}

//���ص�ǰ���������û�
char *GetAllUsers(MYSQL *_mysql)
{
    MYSQL_RES *result;
    MYSQL_ROW row;

    /* ���в�ѯ���ɹ�����0�����ɹ���0
    1����ѯ�ַ��������﷨����
    2����ѯ�����ڵ����ݱ� */
    char sql[200];
    sprintf(
        sql,
        "select * from onlineuser;");

    if (mysql_query(_mysql, sql))
    {
        printf("mysql_query_connect failed(%s)", mysql_error(_mysql));
        return NULL;
    }

    /* ����ѯ����洢���������ִ����򷵻�NULL
       ע�⣺��ѯ���ΪNULL�����᷵��NULL */
    if ((result = mysql_store_result(_mysql)) == NULL)
    {
        printf("mysql_store_result failed");
        return NULL;
    }

    /* ѭ����ȡ�������������ļ�¼
       1�����ص���˳����selectָ������˳����ͬ����row[0]��ʼ
       2���������ݿ�����ʲô���ͣ�C�ж��������ַ��������д�������б�Ҫ����Ҫ�Լ�����ת��
       3�������Լ�����Ҫ��֯�����ʽ */
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

    /* �ͷ�result */
    mysql_free_result(result);

    return buf;
}

// int main(int argc, char *argv[])
// {
    // MYSQL *mysql;

    // InitDatabase(&mysql);

    // char username1[16] = "MiaoMiaoYang";
    // char username2[16] = "MiaoMiaoYan";
    // char username3[16] = "MiaoMiaoYag";
    // char username4[16] = "MiaoMiaoYng";
    // char username5[16] = "MiaoMiaoang";
    // char keyword1[12] = "123456";
    // char keyword2[12] = "mkamskksa";

    // printf("�״ε�½-4:%d\n", JudgeUser(mysql, username1, keyword1));
    // printf("�������-2:%d\n", JudgeUser(mysql, username1, keyword2));
    // printf("�û���ʧ-3:%d\n", JudgeUser(mysql, username2, keyword1));
    // printf("��    ȷ-1:%d\n", JudgeUser(mysql, username1, keyword1));

    //    UpdateSecret(mysql,username1,keyword2);

    // AddOnlineUser(mysql, username1,1);
    // AddOnlineUser(mysql, username2,2);
    // AddOnlineUser(mysql, username3,3);
    // AddOnlineUser(mysql, username4,4);
    // AddOnlineUser(mysql, username5,5);

    // printf("%d\n",GetOnlineId(mysql,username1));
    // printf("%s\n",GetOnlineUsername(mysql,2));

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

    /* �ر��������� */
    // mysql_close(mysql);

    // return 0;
// }
