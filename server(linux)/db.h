
/* db.h */
#include <mysql.h> // mysql����
#include <stdio.h>
#include <malloc.h>
#include <string.h>


enum RETURNTYPE
{
    FALSE,
    TRUE
};

//��ʼ�����ݿ�
enum RETURNTYPE InitDatabase(MYSQL **_mysql);

//�жϸ��û��ǲ����״ε�½
enum RETURNTYPE JudgeFirstLog(MYSQL *_mysql, char *_username);

//�ж��û������Ƿ���ȷ 1-��ȷ 2-������� 3-�û��������� 4-�״ε�½��Ҫ����[ǰ�᣺�û�����������ȷ]
int JudgeUser(MYSQL *_mysql, char *_username, char *_keyword);

//�������ݿ�username������Ϊ_keywork
enum RETURNTYPE UpdateSecret(MYSQL *_mysql, char *_username, char *_keyword);

//��ǰ�����û������û�
enum RETURNTYPE AddOnlineUser(MYSQL *_mysql, char *_username,int id);

//��ǰ�����û������û�
enum RETURNTYPE DelOnlineUser(MYSQL *_mysql, char *_username);

//���ص�ǰ�����û����ļ���
char *GetOnlineUsername(MYSQL *_mysql, int _id);

//���ص�ǰ�����û���ID��
int GetOnlineId(MYSQL *_mysql, char *_username);

//���ص�ǰ���������û�
char *GetAllUsers(MYSQL *_mysql);

//�õ�ÿһ��������ӽ�����ͻ�����������Ϣ
int GetSendMessage(MYSQL* _mysql,char* username,char*** _return);

//������ӽ����������ӽ��̷��Ͱ�װ�õ�֡
void SetMessageToDB(MYSQL* _mysql,char* fromuser,char* touser,char* msg);

char *GetAllOnlineUsers(MYSQL *_mysql);