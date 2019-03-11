# nt-chat

- Operator System Supported ：Windows7,8,10
- Develop Environment:
  - Client：Windows + Qt（C++）
  - Server:  CentOS7 + C

## Client

### Usage

Double click `client/exe/QtClient.exe`

#### log-in

<div align=center><img width="500" height="500" src="https://github.com/JoeyTong03/nt-chat/blob/master/img/log-in.png"/></div>

#### friend list

When a friend is not on-line, there will show a label "off-line" . And when a friend newly up-line, all users who are his friends will get a message about it.

<div align=center><img width="500" height="500" src="https://github.com/JoeyTong03/nt-chat/blob/master/img/list.png"/></div>

#### Communicate window

When he/she is on-line, you can send your message or file(no bigger than 1GB) to him/her.

<div align=center><img width="600" height="600" src="https://github.com/JoeyTong03/nt-chat/blob/master/img/communicate-window.png"/></div>

## Server

### Usage

1. Copy directory `server` to the server (Linux)
2. Under this directory, execute the command line `make` to compile the resource code, then there will be a executable file  `server`
3. Run server