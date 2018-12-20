

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
-- 用户表
-- ----------------------------
drop Table if exists user;
create Table user(
    username varchar(16) not null,      /* 用户名，最大16字节 */
    keyword  varchar(12) not null,      /* 密码，最大12字节 */
    firstlog boolean default false,      /* 首次登陆 - 默认true */
    infonum  INT not null default 100,  /* 回看信息条数 */
    PRIMARY KEY (username)
)character set=gbk;

insert into user (username,keyword) values
('MiaoMiaoYang','123456'),
('zhengxuanci' ,'123456'),
('tongjiayan'  ,'123456');

-- ----------------------------
-- Table structure for `onlineuser`
-- 当前用户表
-- ----------------------------
drop Table if exists onlineuser;
create Table onlineuser(
    username varchar(16) not null,  /* 用户名，最大16字节 */
    id int not null,                /* 用户ID */
    PRIMARY KEY (username),
    UNIQUE (id)
)character set=gbk;


