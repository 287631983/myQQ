#include "client.h"
#include "myhead.h"

int main(int argc,char *argv[])
{
    //1.显示主界面并得到用户的输入指令
    int func_num;
    printf("                欢迎使用聊天系统!请按照提示输入相关序号\n");
LOGIN:
    printf("        1.登录              2.注册新用户                3.找回密码\n");
    scanf("%d",&func_num);
    switch(func_num)
    {
        case 1:
        {
            user_login();
            break;
        }
        case 2:
        {
            user_register();
            break;
        }
        case 3:
        {
            user_find();
            break;
        }
        default:
        {
            printf("输入错误!请重新输入:\n");
            goto LOGIN;
            break;
        }
    }
    return VALUE_TRUE;
}

/*用户登录程序*/
int user_login()
{
    int fd;
    int ret;
    struct sockaddr_in cin;
    /*与服务器建立TCP连接,用来验证输入的信息是否正确*/
    fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd < 0)
    {
        perror("user_login socket");
        exit(1);
    }
    memset(&cin,0,sizeof(cin));
    cin.sin_addr.s_addr = inet_addr(SERVER_IP);
    cin.sin_family = AF_INET;
    cin.sin_port = htons(SERVER_LOGIN_PORT);
    ret = connect(fd,(const struct sockaddr *)&cin,sizeof(cin));
    if(ret < 0)
    {
        perror("user_login connect");
        exit(1);
    }
    user_info user_login;
    printf("请输入您的用户名:\n");
INPUT_USER_NAME:    
    scanf("%s",user_login.user_name);
    if(verify_user_info(0,user_login) == VALUE_FALSE)
    {
        printf("用户名不存在!请重新输入\n");
        goto INPUT_USER_NAME;
    }
    
    printf("请输入密码:\n");
INPUT_USER_PASSWD:    
    scanf("%s",user_login.user_passwd);
    if(verify_user_info(1,user_login) == VALUE_FALSE)
    {
        printf("密码错误!请重新输入\n");
        goto INPUT_USER_PASSWD;
    }

    /*从服务器下载用户的所有信息*/
    return VALUE_TRUE;
}

int user_register()
{
    int fd;
    int ret;
    struct sockaddr_in cin;
    /*与服务器建立TCP连接,用来验证输入的信息是否正确*/
    fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd < 0)
    {
        perror("user_register socket");
        exit(1);
    }
    memset(&cin,0,sizeof(cin));
    cin.sin_addr.s_addr = inet_addr(SERVER_IP);
    cin.sin_family = AF_INET;
    cin.sin_port = htons(SERVER_REGISTER_PORT);
    ret = connect(fd,(const struct sockaddr *)&cin,sizeof(cin));
    if(ret < 0)
    {
        perror("user_register connect");
        exit(1);
    }

    char user_input[20] = {};
    user_info user_register;
    printf("请输入您的账号:\n");
INPUT_USER_NAME:
    memset(user_register.user_name,0,sizeof(user_register.user_name));
    scanf("%s",user_register.user_name);
    // if(verify_user_info(0,user_register) == VALUE_TRUE)
    // {
    //     printf("账号已存在,请重新输入\n");
    //     goto INPUT_USER_NAME;
    // }
INPUT_USER_PASSWD:
    /*输入密码*/
    printf("请输入您的密码:\n");
    memset(user_input,0,sizeof(user_input));
    scanf("%s",user_register.user_passwd);
    printf("请再次输入您的密码:\n");
    scanf("%s",user_input);
    if(strcmp(user_register.user_passwd,user_input) != 0)
    {
        printf("两次密码不一致,请重新输入!:\n");
        memset(user_register.user_passwd,0,sizeof(user_register.user_passwd));
        goto INPUT_USER_PASSWD;
    }

    /*输入手机号*/
    printf("请输入您的手机号:\n");
INPUT_USER_PHONE:
    scanf("%s",user_register.user_phone);
    // if(verify_user_info(2,user_register) == VALUE_TRUE)
    // {
    //     printf("手机号已被使用,请重新输入\n");
    //     goto INPUT_USER_PHONE;
    // }

    /*{ "command": "verify", "class": "login name", "content": "287631983" }*/
    struct json_object *register_message = json_object_new_object();
    struct json_object *register_head = json_object_new_string(CMD_REGISTER_HEAD);
    struct json_object *register_name = json_object_new_string(user_register.user_name);
    struct json_object *register_passwd = json_object_new_string(user_register.user_passwd);
    struct json_object *register_phone = json_object_new_string(user_register.user_phone);

    json_object_object_add(register_message,CMD_HEAD,register_head);
    json_object_object_add(register_message,CMD_REGISTER_NAME,register_name);
    json_object_object_add(register_message,CMD_REGISTER_PASSWD,register_passwd);
    json_object_object_add(register_message,CMD_REGISTER_PHONE,register_phone);
    const char *register_str = json_object_to_json_string(register_message);
    ret = send(fd,register_str,strlen(register_str),0);
    if(ret < 0)
    {
        perror("user_register send user info");
        exit(1);
    }
    printf("注册成功!\n");
    close(fd);
    return VALUE_TRUE;
}

int user_find()
{
    int fd;
    int ret;
    struct sockaddr_in cin;
    /*与服务器建立TCP连接*/
    fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd < 0)
    {
        perror("user find socket");
        exit(1);
    }
    memset(&cin,0,sizeof(cin));
    cin.sin_addr.s_addr = inet_addr(SERVER_IP);
    cin.sin_family = AF_INET;
    cin.sin_port = htons(SERVER_RETRIEVE_PORT);

    ret = connect(fd,(const struct sockaddr *)&cin,sizeof(cin));
    if(ret < 0)
    {
        perror("user find connect");
        exit(1);
    }

    user_info user_input;
    printf("请输入您的用户名:\n");
INPUT_USER_NAME:    
    scanf("%s",user_input.user_name);
    if(verify_user_info(0,user_input) != VALUE_TRUE)
    {
        printf("用户名不存在,请重新输入\n");
        goto INPUT_USER_NAME;
    }
    
    printf("请输入您的手机号:\n");
INPUT_USER_PHONE:
    scanf("%s",user_input.user_phone);
    if(verify_user_info(0,user_input) != VALUE_TRUE)
    {
        printf("手机号不存在,请重新输入\n");
        goto INPUT_USER_PHONE;
    }

    char buf[20] = {};
INPUT_PASSWD:
    printf("请输入您的新密码:\n");
    scanf("%s",user_input.user_passwd);
    printf("请再次输入您的密码:\n");
    memset(buf,0,sizeof(buf));
    scanf("%s",buf);
    if(strcmp(user_input.user_passwd,buf) != 0)
    {
        printf("两次输入的密码不相同,请重新输入\n");
        goto INPUT_PASSWD;
    }
    printf("密码修改成功!\n");
    return VALUE_TRUE;
}

/*通过服务器验证用户信息*/
/*verify_type:  0 验证用户名是否正确
                1 验证用户密码是否正确
                2验证用户手机号是否正确
return value:   0 服务器检测到相关数据
                1 服务器未找到相关数据*/
int verify_user_info(char verify_type,user_info user_input)
{
    int fd;
    int ret;
    struct sockaddr_in cin;
    fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd < 0)
    {
        perror("socket");
        return VALUE_FALSE;
    }
    memset(&cin,0,sizeof(cin));
    cin.sin_addr.s_addr = inet_addr(SERVER_IP);
    cin.sin_family = AF_INET;
    cin.sin_port = htons(SERVER_LOGIN_PORT);
    ret = connect(fd,(const struct sockaddr *)&cin,sizeof(cin));
    if(ret < 0)
    {
        perror("verify info connect");
        exit(1);
    }
    
    switch(verify_type)
    {
        case 0:
        {
            char return_value;
            /*{ "command": "verify", "class": "login name", "content": "287631983" }*/
            struct json_object *verify_message = json_object_new_object();
            struct json_object *verify_head = json_object_new_string(CMD_VERIFY_HEAD);
            struct json_object *verify_class = json_object_new_string(CMD_VERIFY_LOGIN_NAME);
            struct json_object *verify_content = json_object_new_string(user_input.user_name);
            json_object_object_add(verify_message,CMD_HEAD,verify_head);
            json_object_object_add(verify_message,CMD_VERIFY_CLASS,verify_class);
            json_object_object_add(verify_message,CMD_VERIFY_CONTENT,verify_content);
            const char *verify_str = json_object_to_json_string(verify_message);
            ret = send(fd,verify_str,sizeof(verify_str),0);
            if(ret < 0)
            {
                perror("verify info send");
                exit(1);
            }
            ret = recv(fd,&return_value,1,0);
            if(ret < 0)
            {
                perror("verify info recv");
                exit(1);
            }
            return return_value;
            break;
        }
        case 1:
        {
            char return_value;
            /*{ "command": "verify", "class": "login password", "content": "jay2345" }*/
            struct json_object *verify_message = json_object_new_object();
            struct json_object *verify_head = json_object_new_string(CMD_VERIFY_HEAD);
            struct json_object *verify_class = json_object_new_string(CMD_VERIFY_LOGIN_PASSWD);
            struct json_object *verify_content = json_object_new_string(user_input.user_passwd);
            json_object_object_add(verify_message,CMD_HEAD,verify_head);
            json_object_object_add(verify_message,CMD_VERIFY_CLASS,verify_class);
            json_object_object_add(verify_message,CMD_VERIFY_CONTENT,verify_content);
            const char *verify_str = json_object_to_json_string(verify_message);
            ret = send(fd,verify_str,sizeof(verify_str),0);
            if(ret < 0)
            {
                perror("verify info send");
                exit(1);
            }
            ret = recv(fd,&return_value,1,0);
            if(ret < 0)
            {
                perror("verify info recv");
                exit(1);
            }
            return return_value;
            break;
        }
        case 2:
        {
            char return_value;
            /*{ "command": "verify", "class": "login phone", "content": "18987652415" }*/
            struct json_object *verify_message = json_object_new_object();
            struct json_object *verify_head = json_object_new_string(CMD_VERIFY_HEAD);
            struct json_object *verify_class = json_object_new_string(CMD_VERIFY_LOGIN_PHONE);
            struct json_object *verify_content = json_object_new_string(user_input.user_phone);
            json_object_object_add(verify_message,CMD_HEAD,verify_head);
            json_object_object_add(verify_message,CMD_VERIFY_CLASS,verify_class);
            json_object_object_add(verify_message,CMD_VERIFY_CONTENT,verify_content);
            const char *verify_str = json_object_to_json_string(verify_message);
            ret = send(fd,verify_str,sizeof(verify_str),0);
            if(ret < 0)
            {
                perror("verify info send");
                exit(1);
            }
            ret = recv(fd,&return_value,1,0);
            if(ret < 0)
            {
                perror("verify info recv");
                exit(1);
            }
            return return_value;
            break;
        }
        default:break;
    }
    return VALUE_FALSE;
}