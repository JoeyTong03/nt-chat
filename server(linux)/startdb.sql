

/*
Navicat MySQL Data Transfer

Source Server         : test
Source Host           : localhost:3306
Source Database       : QQ

Target Server Type    : MYSQL

Date: 2018-12-15 23:46:11
*/

-- ----------------------------
-- Database structure for `GroupChat`
-- ----------------------------
drop Database if exists GroupChat;
create Database GroupChat;
use GroupChat;

-- ----------------------------
-- Table structure for `user`
-- �û���
-- ----------------------------
drop Table if exists user;
create Table user(
    username varchar(16) not null,      /* �û��������16�ֽ� */
    keyword  varchar(12) not null,      /* ���룬���12�ֽ� */
    firstlog boolean default false,      /* �״ε�½ - Ĭ��true */
    infonum  INT not null default 100,  /* �ؿ���Ϣ���� */
    PRIMARY KEY (username)
)character set=gbk;

insert into user (username,keyword) values
('MiaoMiaoYang','123456'),
('zhengxuanci' ,'123456'),
('tongjiayan'  ,'123456');

-- ----------------------------
-- Table structure for `onlineuser`
-- ��ǰ�û���
-- ----------------------------
drop Table if exists onlineuser;
create Table onlineuser(
    username varchar(16) not null,  /* �û��������16�ֽ� */
    id int not null,                /* �û�ID */
    PRIMARY KEY (username),
    UNIQUE (id)
)character set=gbk;

-- ----------------------------
-- Table structure for `cMessage`
-- ��Ϣ��
-- ----------------------------
drop Table if exists cMessage;
create Table cMessage(
    from_user varchar(16),          /* ��Ϣ�ķ��ͷ� */
    to_user varchar(16) not null,   /* ��Ϣ�Ľ��շ� */
    msg varchar(1024),              /* ���͵���Ϣ */
    read_flag boolean default true  /* ���շ��Ƿ��Ѿ����ܵ�������Ϣ��true-û�н���,false-�Ѿ����� */
)character set=gbk;
