#ifndef _CLIENT_H
#define _CLIENT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include "/usr/local/include/json/json.h"
#define SERVER_IP                   "192.168.5.73"
#define SERVER_LOGIN_PORT           10001               //服务器登录端口号
#define SERVER_REGISTER_PORT        10002               //服务器注册端口号
#define SERVER_RETRIEVE_PORT        10003               //服务器找回账号端口号
#define SERVER_USER_INFO_PORT       10004               //用户相关信息交互端口号
#define SERVER_GROUP_PORT           10005               //用户群聊端口号

#define MAX_FRIEND_NUM              10                  //最大好友数量
#define MAX_GROUP_CHAT_USER_NUM     10                  //一个群聊内最大人数
#define MAX_GROUP_CHAT_NUM          10                  //单个用户最多保存的群聊个数

#define DEBUG 1

struct GROUP_CHAT
{
    char group_id;                                      //群聊id,从1开始累加
    char group_name[20];                                //群聊名称
    char member_num;                                    //群聊成员数量
    char member_id[MAX_GROUP_CHAT_USER_NUM];            //群聊内部的用户名单,填充用户ID
};

typedef struct USER_INFO
{
    char user_name[20];                                 //用户登录名
    char user_passwd[20];                               //用户登录密码
    char user_id;                                       //用户ID识别用户身份,不可重复,从1开始累加
    char user_phone[11];                                //用户手机号，用于找回密码或修改密码
    char friend_list[MAX_FRIEND_NUM];                   //用户好友列表,填充用户id,最大
    struct GROUP_CHAT group_chat[MAX_GROUP_CHAT_NUM];   //用户加入的群聊相关信息
}user_info;

int user_login(void);
int user_register(void);
int user_find(void);
int verify_user_info(char verify_type,user_info user_input);
#endif