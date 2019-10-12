#include "server.h"
#include "client.h"
#include "myhead.h"
sqlite3 *db;
user_info user_query;                                   //接收查询用户信息回调函数的结构体
struct GROUP_CHAT group_query;                          //接收查询群聊信息回调函数的结构体
void init_database();
void insert_user(user_info user_insert);
void view_user(char *user_name,user_info *user_data);
void view_group(int group_id,struct GROUP_CHAT *group_chat);
void insert_group(struct GROUP_CHAT group_insert);
int main(int argc,char *argv[])
{
    int ret;
    ret = sqlite3_open("myQQ.db",&db);
    if(ret != 0)
    {
        fprintf(stderr,"cannot open myQQ.db:%s\n",sqlite3_errmsg(db));
        exit(0);
    }
    
    user_info a;
    memset(&a,0,sizeof(user_info));
    init_database();
    strcpy(a.user_name,"linshuhao");
    strcpy(a.user_passwd,"jay2345");
    strcpy(a.user_phone,"18565632814");
    a.friend_list[0] = a.user_id;
    a.group_chat[0].group_id = 1;
    strcpy(a.group_chat[0].group_name,"first groupchat");
    a.group_chat[0].member_id[0] = a.user_id;
    a.group_chat[0].member_num = 1;
    insert_user(a);
    insert_group(a.group_chat[0]);
    view_user(a.user_name,&a);
    return 0;
}

/*初始化数据库,包含user_info和group_info两个数据表*/
void init_database()
{
    char sql_cmd[300];
    sprintf(sql_cmd,"create table user_info(");
    strcat(sql_cmd,"user_id INTEGER PRIMARY KEY autoincrement,");
    strcat(sql_cmd,"user_name CHAR(20) NOT NULL,");
    strcat(sql_cmd,"user_passwd CHAR(20) NOT NULL,");
    strcat(sql_cmd,"user_phone CHAR(11) NOT NULL,");
    strcat(sql_cmd,"user_friend CHAR(50) NULL,");
    strcat(sql_cmd,"user_groupchat CHAR(50) NULL");
    strcat(sql_cmd,");");
    sqlite3_exec(db,sql_cmd,NULL,NULL,NULL);

    memset(sql_cmd,0,sizeof(sql_cmd));
    sprintf(sql_cmd,"create table group_info(");
    strcat(sql_cmd,"group_id INTEGER PRIMARY KEY autoincrement,");
    strcat(sql_cmd,"group_name CHAR(20) NOT NULL,");
    strcat(sql_cmd,"member_num INTEGER NOT NULL,");
    strcat(sql_cmd,"member_id CHAR(50) NOT NULL");
    strcat(sql_cmd,");");
    sqlite3_exec(db,sql_cmd,NULL,NULL,NULL);
}

/*增加一条用户信息*/
void insert_user(user_info user_insert)
{
    int i,j;
    char sql_cmd[500] = "insert into user_info(user_id,user_name,user_passwd,user_phone,user_friend,user_groupchat) values(";
    char user_friend[50] = {};
    char user_group_id[50] = {};

    /*将int类型的用户id转换为字符串形式存储在数据库中,例如ID=12转为字符串"12",多个ID中间以"-"分开*/
    for(i = 0,j = 0;user_insert.friend_list[i] != 0;i ++)
    {
        if(user_insert.friend_list[i] < 10)
        {
            user_friend[j ++] = user_insert.friend_list[i] + 0x30;
        }
        else if(user_insert.friend_list[i] < 100)
        {
            user_friend[j ++] = user_insert.friend_list[i] / 10 + 0x30;
            user_friend[j ++] = user_insert.friend_list[i] % 10 + 0x30;
        }
        if(user_insert.friend_list[i + 1] != 0)
            user_friend[j ++] = '-';
    }
    for(i = 0,j = 0;user_insert.group_chat[i].group_id != 0;i ++)
    {
        if(user_insert.group_chat[i].group_id < 10)
        {
            user_group_id[j ++] = user_insert.group_chat[i].group_id + 0x30;
        }
        else if(user_insert.group_chat[i].group_id < 100)
        {
            user_group_id[j ++] = user_insert.group_chat[i].group_id / 10 + 0x30;
            user_group_id[j ++] = user_insert.group_chat[i].group_id % 10 + 0x30;
        }
        if(user_insert.group_chat[i + 1].group_id != 0)
        user_group_id[j ++] = '-';
    }
    strcat(sql_cmd,"NULL");
    strcat(sql_cmd,",'");
    strcat(sql_cmd,user_insert.user_name);
    strcat(sql_cmd,"','");
    strcat(sql_cmd,user_insert.user_passwd);
    strcat(sql_cmd,"','");
    strcat(sql_cmd,user_insert.user_phone);
    strcat(sql_cmd,"','");
    strcat(sql_cmd,user_friend);
    strcat(sql_cmd,"','");
    strcat(sql_cmd,user_group_id);
    strcat(sql_cmd,"');");
    sqlite3_exec(db,sql_cmd,NULL,NULL,NULL);
}

/*增加一条群聊信息*/
void insert_group(struct GROUP_CHAT group_insert)
{
    char sql_cmd[500] = "insert into group_info(group_id,group_name,member_num,member_id) values(";
    char member_id_str[50] = {};
    int i,j;
    for(i = 0,j = 0;group_insert.member_id[i] != 0;i ++)
    {
        if(group_insert.member_id[i] < 10)
        {
            member_id_str[j ++] = group_insert.member_id[i] + 0x30;
        }
        else if(group_insert.member_id[i] < 100)
        {
            member_id_str[j ++] = group_insert.member_id[i] / 10 + 0x30;
            member_id_str[j ++] = group_insert.member_id[i] % 10+ 0x30;
        }
        if(group_insert.member_id[i + 1] != 0)
            member_id_str[j ++] = '-';
    }
    strcat(sql_cmd,"NULL");
    strcat(sql_cmd,",'");
    strcat(sql_cmd,group_insert.group_name);
    strcat(sql_cmd,"','");
    strcat(sql_cmd,itoa(group_insert.member_num));
    strcat(sql_cmd,"','");
    strcat(sql_cmd,member_id_str);
    strcat(sql_cmd,"'");
    strcat(sql_cmd,");");
    sqlite3_exec(db,sql_cmd,NULL,NULL,NULL);
}

static int callback(void *data, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}
/*根据用户名,查看一条用户信息*/
void view_user(char *user_name,user_info *user_data)
{
    int i;
    // char **data;
    char sql_cmd[300] = {};
    sprintf(sql_cmd,"select * from user_info where user_name = '%s'",user_name);
  //  strcat(sql_cmd,user_name);
  //  strcat(sql_cmd,"';");
    // sprintf(sql_cmd,"select user_name from user_info;");
    sqlite3_exec(db,sql_cmd,sqlite3_usercallback,NULL,NULL);
// sqlite3_exec(db,sql_cmd,callback,0,0);
    // int x11,y11;
    // printf("%d  %d\n",x11,y11);
    // char *errmsg;
    // sqlite3_open("myQQ.db",&db);
    // sqlite3_get_table(db,sql_cmd,&data,&x11,&y11,&errmsg);
    //printf("%s\n",errmsg);

    // printf("%d  %d\n",x11,y11);
    // printf("%s\n",data[0]);
    // printf("%s\n",data[1]);
    // printf("%s\n",data[2]);
    // while(1);
    memset(user_data,0,sizeof(user_info));
    user_data->user_id = user_query.user_id;
    strcpy(user_data->user_name,user_query.user_name);
    strcpy(user_data->user_passwd,user_query.user_passwd);
    strcpy(user_data->user_phone,user_query.user_phone);
    for(i = 0;i < MAX_FRIEND_NUM;i ++)
        user_data->friend_list[i] = user_query.friend_list[i];
    for(i = 0;i < MAX_GROUP_CHAT_NUM;i ++)
        user_data->group_chat[i].group_id = user_query.group_chat[i].group_id;


    /*查询用户的每个群聊信息并存放到用户信息结构体*/
    for(i = 0;user_data->group_chat[i].group_id != 0;i ++)
        view_group(user_data->group_chat[i].group_id,&(user_data->group_chat[i]));

    printf("user id:%d\n",user_data->user_id);
    printf("user name:%s\n",user_data->user_name);
    printf("user phone:%s\n",user_data->user_phone);
    printf("user passwd:%s\n",user_data->user_passwd);
    printf("group id:%d\n",user_data->group_chat[0].group_id);
    printf("group name:%s\n",user_data->group_chat[0].group_name);
    printf("group name:%d\n",user_data->group_chat[0].member_num);
    printf("group name:%s\n",user_data->group_chat[0].member_id);
}

/*根据群聊ID,查看一条群聊信息*/
void view_group(int group_id,struct GROUP_CHAT *group_chat)
{
    int i;
    char *sql_cmd = "select group_id from group_info where group_id = '";
    strcat(sql_cmd,itoa(group_id));
    strcat(sql_cmd,"';");
    sqlite3_exec(db,sql_cmd,sqlite3_groupcallback,NULL,NULL);
    memset(group_chat,0,sizeof(struct GROUP_CHAT));
    group_chat->group_id = group_query.group_id;
    strcpy(group_chat->group_name,group_query.group_name);
    group_chat->member_num = group_query.member_num;
    for(i = 0;i < MAX_GROUP_CHAT_USER_NUM;i ++)
        group_chat->member_id[i] = group_query.member_id[i];
}

/*查找用户信息回调函数*/
int sqlite3_usercallback(void *ret,int columns,char **data,char **item)
{
    int i;
    memset(&user_query,0,sizeof(user_query));
    user_query.user_id = atoi(data[0]);
    strcpy(user_query.user_name,data[1]);
    strcpy(user_query.user_passwd,data[2]);
    strcpy(user_query.user_phone,data[3]);

    for(i = 0;data[4][i] != 0;i ++)
    {
        user_query.friend_list[i] = data[4][i];
    }
    // for(i = 0;data[5][i] != 0;i ++)
    //     user_query.group_chat[i].group_id = data[5][i];
    // for(i = 0;i < columns;i ++)
    //     printf("%s=%s\n",item[i],data[i]);
    //     while(1);
    return 0;
}

/*查找群聊信息回调函数*/
int sqlite3_groupcallback(void *ret,int columns,char **data,char **item)
{
    int i;
    memset(&user_query,0,sizeof(user_query));
    group_query.group_id = atoi(data[0]);
    strcpy(group_query.group_name,data[1]);
    group_query.member_num = atoi(data[2]);
    for(i = 0;data[3][i] != 0;i ++)
        group_query.member_id[i] = data[3][i];
    return 0;
}

char *itoa(int num)
{
    if(num < 10)
    {
        char *str = (char *)malloc(sizeof(char) * 2);
        str[0] = num + 0x30;
        str[1] = 0;
        return str;
    }
    else if(num < 100)
    {
        char *str = (char *)malloc(sizeof(char) * 3);
        str[0] = num / 10 + 0x30;
        str[1] = num % 10 + 0x30;
        str[2] = 0;
        return str;
    }
    else if(num < 1000)
    {
        char *str = (char *)malloc(sizeof(char) * 4);
        str[0] = num / 100 + 0x30;
        str[1] = num % 100 / 10 + 0x30;
        str[2] = num % 10 + 0x30;
        str[3] = 0;
        return str;
    }
    else if(num < 1000)
    {
        char *str = (char *)malloc(sizeof(char) * 5);
        str[0] = num / 1000 + 0x30;
        str[1] = num % 1000 / 100 + 0x30;
        str[2] = num % 100 / 10 + 0x30;
        str[3] = num % 10 + 0x30;
        str[4] = 0;
        return str;
    }
    else
    {
        char *str = (char *)malloc(sizeof(char) * 6);
        str[0] = num / 10000 + 0x30;
        str[1] = num % 10000 / 1000 + 0x30;
        str[2] = num % 1000 / 100 + 0x30;
        str[3] = num % 100 / 10 + 0x30;
        str[4] = num % 10 + 0x30;
        str[5] = 0;
        return str;
    }
}