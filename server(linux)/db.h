
/* db.h */
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
enum RETURNTYPE InitDatabase(MYSQL **_mysql);

//判断该用户是不是首次登陆
enum RETURNTYPE JudgeFirstLog(MYSQL *_mysql, char *_username);

//判断用户密码是否正确 1-正确 2-密码错误 3-用户名不存在 4-首次登陆需要改密[前提：用户名与密码正确]
int JudgeUser(MYSQL *_mysql, char *_username, char *_keyword);

//更新数据库username的密码为_keywork
enum RETURNTYPE UpdateSecret(MYSQL *_mysql, char *_username, char *_keyword);

//当前在线用户增加用户
enum RETURNTYPE AddOnlineUser(MYSQL *_mysql, char *_username,int id);

//当前在线用户减少用户
enum RETURNTYPE DelOnlineUser(MYSQL *_mysql, char *_username);

//返回当前在线用户的文件名
char *GetOnlineUsername(MYSQL *_mysql, int _id);

//返回当前在线用户的ID号
int GetOnlineId(MYSQL *_mysql, char *_username);

//返回当前所有在线用户
char *GetAllUsers(MYSQL *_mysql);

//得到每一条服务端子进程向客户端所发的消息
int GetSendMessage(MYSQL* _mysql,char* username,char*** _return);

//服务端子进程向其他子进程发送包装好的帧
void SetMessageToDB(MYSQL* _mysql,char* fromuser,char* touser,char* msg);

char *GetAllOnlineUsers(MYSQL *_mysql);