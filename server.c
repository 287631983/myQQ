#include "server.h"
#include "client.h"
#include "myhead.h"
#include "database.h"
sqlite3 *db = NULL;

int main(int argc,char *argv[])
{
    int ret;
    init_database(db);

user_info *user_data;
    view_user(db,"sss",user_data);
while(1);

    ret = fork();
    if(ret == 0)
    {
        process_register();
    }
    else
    {
        // ret = fork();
        // if(ret == 0)
        // {

        // }
        // else
        // {
            
        // }
    }
}

user_info * register_message(char *message)
{
    user_info *user_struct = (user_info *)malloc(sizeof(user_info));
    struct json_object *message_recv = json_tokener_parse(message); 
    struct json_object *js_head = json_object_object_get(message_recv,CMD_HEAD);

    struct json_object *js_user_name = json_object_object_get(message_recv,CMD_REGISTER_NAME);
    struct json_object *js_user_passwd = json_object_object_get(message_recv,CMD_REGISTER_PASSWD);
    struct json_object *js_user_phone = json_object_object_get(message_recv,CMD_REGISTER_PHONE);
    const char *head = json_object_get_string(js_head);
    const char *user_name = json_object_get_string(js_user_name);
    const char *user_passwd = json_object_get_string(js_user_passwd);
    const char *user_phone = json_object_get_string(js_user_phone);
    memset(user_struct,0,sizeof(*user_struct));
    if(strcmp(head,CMD_REGISTER_HEAD) != 0)
        return NULL;
    strcpy(user_struct->user_name,user_name);
    strcpy(user_struct->user_passwd,user_passwd);
    strcpy(user_struct->user_phone,user_phone);
    return user_struct;
}

void process_register()
{
    char recv_message[500] = {};
    int ret;
    int sock_register,sock_register_cli;
    struct sockaddr_in sin_register;
    /*建立注册套接字*/
    sock_register = socket(AF_INET,SOCK_STREAM,0);
    if(sock_register == -1)
    {
        perror("socket");
        exit(1);
    }
    /*绑定注册套接字*/
    int optval = 1;
    setsockopt(sock_register,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));
    memset(&sin_register,0,sizeof(sin_register));
    sin_register.sin_addr.s_addr = inet_addr(SERVER_IP);
    sin_register.sin_family = AF_INET;
    sin_register.sin_port = htons(SERVER_REGISTER_PORT);
    ret = bind(sock_register,(const struct sockaddr *)&sin_register,sizeof(sin_register));
    if(ret < 0)
    {
        perror("register bind");
        exit(1);
    }

    /*监听注册套接字*/
    ret = listen(sock_register,5);
    if(ret < 0)
    {
        perror("register listen");
        exit(1);
    }

    /*建立注册连接*/
    sock_register_cli = accept(sock_register,NULL,NULL);
    if(ret < 0)
    {
        perror("register accept");
        exit(1);
    }
    while(1)
    {
        memset(recv_message,0,sizeof(recv_message));
        ret = read(sock_register_cli,recv_message,sizeof(recv_message));
        if(ret <= 0)
        {
            break;
        }
        else
        {
            insert_user(db,register_message(recv_message));
        }
    }

    close(sock_register);
    exit(0);
}

void process_login()
{
    char recv_message[500] = {};
    int ret;
    int sock_login,sock_login_cli;
    struct sockaddr_in sin_login;
    /*建立注册套接字*/
    sock_login = socket(AF_INET,SOCK_STREAM,0);
    if(sock_login == -1)
    {
        perror("socket");
        exit(1);
    }
    /*绑定注册套接字*/
    int optval = 1;
    setsockopt(sock_login,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));
    memset(&sin_login,0,sizeof(sin_login));
    sin_login.sin_addr.s_addr = inet_addr(SERVER_IP);
    sin_login.sin_family = AF_INET;
    sin_login.sin_port = htons(SERVER_LOGIN_PORT);
    ret = bind(sock_login,(const struct sockaddr *)&sin_login,sizeof(sin_login));
    if(ret < 0)
    {
        perror("login bind");
        exit(1);
    }

    /*监听注册套接字*/
    ret = listen(sock_login,5);
    if(ret < 0)
    {
        perror("login listen");
        exit(1);
    }

    /*建立注册连接*/
    sock_login_cli = accept(sock_login,NULL,NULL);
    if(ret < 0)
    {
        perror("login accept");
        exit(1);
    }
    while(1)
    {
        memset(recv_message,0,sizeof(recv_message));
        ret = read(sock_login_cli,recv_message,sizeof(recv_message));
        if(ret <= 0)
        {
            break;
        }
        else
        {
            
        }
    }

    close(sock_login);
    exit(0);
}
