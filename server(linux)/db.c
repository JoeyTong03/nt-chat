//mysql_demo.cpp

#include "db.h"
#include <stdint.h>

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
enum RETURNTYPE AddOnlineUser(MYSQL *_mysql, char *_username, int id)
{
    /* ���в�ѯ���ɹ�����0�����ɹ���0
    1����ѯ�ַ��������﷨����
    2����ѯ�����ڵ����ݱ� */
    char sql[200];
    sprintf(
        sql,
        "insert into onlineuser(username,id) values ('%s',%d); ",
        _username,
        id);

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
    int buf;
    while ((row = mysql_fetch_row(result)) != NULL)
    {
        buf = atoi(row[0]);
    }

    /* �ͷ�result */
    mysql_free_result(result);

    return buf;
}

//��װ�������ݿ��msg
void PackMsg(char _msg[], int _len)
{
    int i = 0;
    for (i = 0; i < _len; i++)
        if ((_msg)[i] == 0)
            (_msg)[i] = '`';
}

void UnpackMsg(char *_msg, int _len)
{
    int i = 0;
    for (i = 0; i < _len; i++)
        if ((_msg)[i] == '`')
            (_msg)[i] = 0;
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
            size = strlen(row[0]) + 3;
            buf = (char *)malloc(size * sizeof(char));
            sprintf(buf, "@%s\0", row[0]);
        }
        else
        {
            char *tmpbuf = NULL;
            size += strlen(row[0]) + 3;
            tmpbuf = (char *)malloc(size * sizeof(char));
            sprintf(tmpbuf, "%s@%s\0", buf, row[0]);
            free(buf);
            buf = tmpbuf;
        }
    }

    char *final = (char *)malloc(sizeof(char) * (size + 1));
    sprintf(final, "%s#", buf);
    free(buf);

    /* �ͷ�result */
    mysql_free_result(result);

    return final;
}

//�õ�ÿһ��������ӽ�����ͻ�����������Ϣ
int GetSendMessage(MYSQL *_mysql, char *_username, char ***_return)
{
    MYSQL_RES *result;
    MYSQL_ROW row;

    /* ���в�ѯ���ɹ�����0�����ɹ���0
    1����ѯ�ַ��������﷨����
    2����ѯ�����ڵ����ݱ� */
    char sql[200];
    sprintf(
        sql,
        "select msg from cMessage where to_user='%s' and read_flag=true;",
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

    /* ������ݿ�������ѯ�������ݵ����� - �����ռ� */
    int resultnum = (int)mysql_num_rows(result);
    (*_return) = (char **)malloc(sizeof(char *) * resultnum); //����ָ�������ά����Ŀռ�
    int i = 0;
    for (i = 0; i < resultnum; i++)
    {
        (*_return)[i] = (char *)malloc(sizeof(char) * 1024);
        //����ָ������һά����Ŀռ䣬ÿ��һά����Ŀռ�Ϊ1024�ֽ� - ��Ӧ���ݿ�max 1024
    }

    /* ѭ����ȡ�������������ļ�¼
       1�����ص���˳����selectָ������˳����ͬ����row[0]��ʼ
       2���������ݿ�����ʲô���ͣ�C�ж��������ַ��������д�������б�Ҫ����Ҫ�Լ�����ת��
       3�������Լ�����Ҫ��֯�����ʽ */

    //printf("\n ----------------- \n ���ݿ�����в��� \n");

    i = 0;
    while ((row = mysql_fetch_row(result)) != NULL && i < resultnum)
    {

        //�����ж�һ��֡�Ĵ�С
        uint16_t len = 0;
        char tmp_buf[2] = {row[0][2], row[0][3]};
        UnpackMsg(tmp_buf, 2);
        memcpy(&len, tmp_buf, 2 * sizeof(char));

        // memcpy(&len, &(row[0][3]), 2);
        // Str2int2(row[0], len);

        memcpy((*_return)[i], row[0], len * sizeof(char));

        UnpackMsg((*_return)[i], len);

        // Str2int2((*_return)[i], len);

        i++;

        /* * * * *
     *�����ݿ�����Ѿ���ɵ���Щ����     * 
     * * * * */

        char update_sql[200];
        sprintf(
            update_sql,
            "update cMessage set read_flag=false where to_user='%s' and msg='%s' and read_flag=true",
            _username,
            row[0]);

        if (mysql_query(_mysql, update_sql))
        {
            printf("mysql_query_connect failed(%s)", mysql_error(_mysql));
            return -1;
        }
    }

    /* �ͷ�result */
    mysql_free_result(result);

    return resultnum;
}

//������ӽ����������ӽ��̷��Ͱ�װ�õ�֡
void SetMessageToDB(MYSQL *_mysql, char *fromuser, char *touser, char *msg)
{
    /* ���в�ѯ���ɹ�����0�����ɹ���0
    1����ѯ�ַ��������﷨����
    2����ѯ�����ڵ����ݱ� */

    //������ѯ����ǰ�벿��
    char sql[200];
    sprintf(
        sql,
        "insert into cMessage(from_user,to_user,msg) values ('%s','%s','%s')",
        fromuser,
        touser,
        msg);

    if (mysql_query(_mysql, sql))
    {
        printf("mysql_query_connect failed(%s)", mysql_error(_mysql));
        return;
    }
};

#define TESTDB
#ifdef TESTDB

void Str2int2(char *buf, int length)
{
    int i;
    for (i = 0; i < length; i++)
    {
        if ((buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= '0' && buf[i] <= '9') || (buf[i] == '@'))
            printf("%c ", buf[i]);
        else
            printf("%x ", (int)buf[i]);
        // if(i!=0&&i%4==0)
        //     printf(" ");
    }
    printf("%c", '\n');
}

int main(int argc, char *argv[])
{
    MYSQL *mysql;

    InitDatabase(&mysql);

    /*******
     * 
     * char username1[16] = "MiaoMiaoYang";
     * char username2[16] = "MiaoMiaoYan";
     * char username3[16] = "MiaoMiaoYag";
     * char username4[16] = "MiaoMiaoYng";
     * char username5[16] = "MiaoMiaoang";
     * char keyword1[12] = "123456";
     * char keyword2[12] = "mkamskksa";
     * 
     * UpdateSecret(mysql, username1, keyword2);
     * 
     * printf("�״ε�½-4:%d\n", JudgeUser(mysql, username1, keyword1));
     * printf("�������-2:%d\n", JudgeUser(mysql, username1, keyword2));
     * printf("�û���ʧ-3:%d\n", JudgeUser(mysql, username2, keyword1));
     * printf("��    ȷ-1:%d\n", JudgeUser(mysql, username1, keyword1));
     * 
     * UpdateSecret(mysql,username1,keyword2);
     * 
     * AddOnlineUser(mysql, username1,1);
     * AddOnlineUser(mysql, username2,2);
     * AddOnlineUser(mysql, username3,3);
     * AddOnlineUser(mysql, username4,4);
     * AddOnlineUser(mysql, username5,5);
     * 
     * printf("%d\n",GetOnlineId(mysql,username1));
     * printf("%s\n",GetOnlineUsername(mysql,2));
     * 
     * char *buf = GetAllUsers(mysql);
     * if (buf != NULL)
     * {
     *     printf("%s\n", buf);
     *     free(buf);
     * }
     * else
     * {
     *     printf("empty\n");
     * }
     * 
     * 
     * DelOnlineUser(mysql, username1);
     * DelOnlineUser(mysql, username2);
     * DelOnlineUser(mysql, username3);
     * DelOnlineUser(mysql, username4);
     * DelOnlineUser(mysql, username5);
     * 
     * buf = GetAllUsers(mysql);
     * if (buf != NULL)
     * {
     *     printf("%s\n", buf);
     *     free(buf);
     * }
*/

    char from[] = "MiaoMiaoYang";
    char to[] = "zhengxuanci";

    char msg1[20] = "0000hhhhhhh1";
    char msg2[20] = "0000hhhhhhh2";
    char msg3[20] = "0000hhhhhhh3";
    char msg4[20] = "0000hhhhhhh4";
    char msg5[20] = "0000hhhhhhh5";

    uint16_t num = 12;
    memcpy(&(msg1[2]), &num, 2);
    memcpy(&(msg2[2]), &num, 2);
    memcpy(&(msg3[2]), &num, 2);
    memcpy(&(msg4[2]), &num, 2);
    memcpy(&(msg5[2]), &num, 2);

    PackMsg(msg1, num);
    PackMsg(msg2, num);
    PackMsg(msg3, num);
    PackMsg(msg4, num);
    PackMsg(msg5, num);

    SetMessageToDB(mysql, from, to, msg1);
    SetMessageToDB(mysql, from, to, msg2);
    SetMessageToDB(mysql, from, to, msg3);
    SetMessageToDB(mysql, from, to, msg4);
    SetMessageToDB(mysql, from, to, msg5);

    printf("\n-------------------------\n��һ�β�ѯ\n");

    char **get = NULL;
    int gnum = GetSendMessage(mysql, to, &get);
    printf("num:%d\n", gnum);

    int i = 0;
    for (i = 0; i < gnum; i++)
    {
        Str2int2(get[i], num);
    }

    printf("\n-------------------------\n��2�β�ѯ\n");
    get = NULL;

    gnum = GetSendMessage(mysql, to, &get);
    printf("num:%d\n", gnum);

    for (i = 0; i < gnum; i++)
    {
        Str2int2(get[i], num);
    }

    /* �ر��������� */
    mysql_close(mysql);

    return 0;
}

#endif