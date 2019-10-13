#include "database.h"
#include "client.h"
#include "server.h"
#include "myhead.h"
/*初始化数据库,包含user_info和group_info两个数据表*/
void init_database(sqlite3 *db)
{
    char sql_cmd[300];
    int ret;
    ret = sqlite3_open("myQQ.db",&db);
    if(ret != 0)
    {
        fprintf(stderr,"cannot open myQQ.db:%s\n",sqlite3_errmsg(db));
        exit(0);
    }
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

/*增加一条用户信息,需要先对用户名进行判断,如果已存在相同用户名则返回VALUE_FALSE,否则返回VALUE_TRUE*/
int insert_user(sqlite3 *db,user_info *user_insert)
{
    int i,j;
    char sql_cmd[500] = "insert into user_info(user_id,user_name,user_passwd,user_phone,user_friend,user_groupchat) values(";
    char user_friend[50] = {};
    char user_group_id[50] = {};
    sqlite3_open("myQQ.db",&db);
    if(user_insert == NULL)
        return VALUE_FALSE;
    // /*该用户名已存在,无法再次添加*/
    // user_info *temp = (user_info *)malloc(sizeof(user_info));
    // memset(&temp,0,sizeof(temp));
    // if(view_user(db,user_insert->user_name,temp) != VALUE_FALSE)
    //     return VALUE_FALSE;

    /*将int类型的用户好友id转换为字符串形式存储在数据库中,例如ID=12转为字符串"12",多个ID中间以"-"分开*/
    for(i = 0,j = 0;user_insert->friend_list[i] != 0;i ++)
    {
        if(user_insert->friend_list[i] < 10)
        {
            user_friend[j ++] = user_insert->friend_list[i] + 0x30;
        }
        else if(user_insert->friend_list[i] < 100)
        {
            user_friend[j ++] = user_insert->friend_list[i] / 10 + 0x30;
            user_friend[j ++] = user_insert->friend_list[i] % 10 + 0x30;
        }
        if(user_insert->friend_list[i + 1] != 0)
            user_friend[j ++] = '-';
    }
    for(i = 0,j = 0;user_insert->groupid[i] != 0;i ++)
    {
        if(user_insert->groupid[i] < 10)
        {
            user_group_id[j ++] = user_insert->groupid[i] + 0x30;
        }
        else if(user_insert->groupid[i] < 100)
        {
            user_group_id[j ++] = user_insert->groupid[i] / 10 + 0x30;
            user_group_id[j ++] = user_insert->groupid[i] % 10 + 0x30;
        }
        if(user_insert->groupid[i + 1] != 0)
        user_group_id[j ++] = '-';
    }
    strcat(sql_cmd,"NULL");
    strcat(sql_cmd,",'");
    strcat(sql_cmd,user_insert->user_name);
    strcat(sql_cmd,"','");
    strcat(sql_cmd,user_insert->user_passwd);
    strcat(sql_cmd,"','");
    strcat(sql_cmd,user_insert->user_phone);
    strcat(sql_cmd,"','");
    strcat(sql_cmd,user_friend);
    strcat(sql_cmd,"','");
    strcat(sql_cmd,user_group_id);
    strcat(sql_cmd,"');");
    sqlite3_exec(db,sql_cmd,NULL,NULL,NULL);
    return VALUE_TRUE;
}

/*增加一条群聊信息*/
void insert_group(sqlite3 *db,struct GROUP_CHAT group_insert)
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

/*根据用户名,查看一条用户信息,返回值为VALUE_TRUE表示查询到该数据,返回值为VALUE_FALSE表示未找到该数据*/
int view_user(sqlite3 *db,char *user_name,user_info *user_data)
{
    int i;
    char sql_cmd[200];
    sqlite3_open("myQQ.db",&db);
    user_info *user_info_struct = (user_info *)malloc(sizeof(user_info));
    // memset(user_info_struct,0,sizeof(user_info));
    sprintf(sql_cmd,"select * from user_info where user_name = '%s';",user_name);
    sqlite3_exec(db,sql_cmd,sqlite3_usercallback,(void *)user_info_struct,NULL);
    if(strcmp(user_info_struct->user_name,user_name) != 0)
    {
        printf("mei you\n");
        return VALUE_FALSE;
    }
    printf("you\n");
    user_data->user_id = user_info_struct->user_id;
    strcpy(user_data->user_name,user_info_struct->user_name);
    strcpy(user_data->user_passwd,user_info_struct->user_passwd);
    strcpy(user_data->user_phone,user_info_struct->user_phone);
    for(i = 0;i < MAX_FRIEND_NUM;i ++)
        user_data->friend_list[i] = user_info_struct->friend_list[i];
    for(i = 0;i < MAX_GROUP_CHAT_NUM;i ++)
        user_data->groupid[i] = user_info_struct->groupid[i];
    if(user_info_struct->user_id != 0)
        return VALUE_TRUE;
    else
        return VALUE_FALSE;
}

/*根据群聊ID,查看一条群聊信息*/
void view_group(sqlite3 *db,int group_id,struct GROUP_CHAT *group_chat)
{
    int i;
    char sql_cmd [50] = {};
    group_info *group_info_struct = (group_info *)malloc(sizeof(group_info));
    sprintf(sql_cmd,"select * from group_info where group_id = '%s';",itoa(group_id));
    sqlite3_exec(db,sql_cmd,sqlite3_groupcallback,(void *)group_info_struct,NULL);
    memset(group_chat,0,sizeof(struct GROUP_CHAT));
    group_chat->groupid = group_info_struct->groupid;
    strcpy(group_chat->group_name,group_info_struct->group_name);
    group_chat->member_num = group_info_struct->member_num;
    for(i = 0;i < MAX_GROUP_CHAT_USER_NUM;i ++)
        group_chat->member_id[i] = group_info_struct->member_id[i];
}

/*查找用户信息回调函数*/
int sqlite3_usercallback(void *user_info_struct,int columns,char **data,char **item)
{
    int i = 0;
    int j = 0;
    int k = 0;

    user_info * user_callback = (user_info *)user_info_struct;
    // user_callback->user_id = atoi(data[0]);
    // printf("%s\n",data[1]);
    user_callback->user_name[0] = 's';
    user_callback->user_name[1] = 's';
    user_callback->user_name[2] = 's';



    // strcpy(user_callback->user_name,data[1]);

    // strcpy(user_callback->user_passwd,data[2]);
    // strcpy(user_callback->user_phone,data[3]);

    // char buff[10] = {};
    // if(data[4][0] == 0)
    //     memset(user_callback->friend_list,0,sizeof(user_callback->friend_list));
    // else
    // {
    //     while(data[4][i] != 0)
    //     {
    //         if(data[4][i] == '-')                   //遇到'-'则停止并将字符串中的内容转为int放入结构体中
    //         {
    //             user_callback->friend_list[j ++] = atoi(buff);
    //             memset(buff,0,sizeof(buff));
    //             k = 0;
    //             i ++;
    //         }
    //         buff[k] = data[4][i];                   //依次将字符串放入buff中
    //         i ++;
    //         k ++;
    //     }
    //     user_callback->friend_list[j ++] = atoi(buff);
    // }
    
    // i = 0;
    // j = 0;
    // k = 0;
    // memset(buff,0,sizeof(buff));
    // if(data[5][0] == 0)
    //     memset(user_callback->groupid,0,sizeof(user_callback->groupid));
    // else
    // {
    //     while(data[5][i] != 0)
    //     {
    //         if(data[5][i] == '-')                   //遇到'-'则停止并将字符串中的内容转为int放入结构体中
    //         {
    //             user_callback->groupid[j ++] = atoi(buff);
    //             memset(buff,0,sizeof(buff));
    //             k = 0;
    //             i ++;
    //         }
    //         buff[k] = data[5][i];                   //依次将字符串放入buff中
    //         i ++;
    //         k ++;
    //     }
    //     user_callback->groupid[j ++] = atoi(buff);
    // }
    return 0;
}

/*查找群聊信息回调函数*/
int sqlite3_groupcallback(void *group_info_struct,int columns,char **data,char **item)
{
    int i = 0;
    int j = 0;
    int k = 0;
    char buff[10] = {};
    group_info *group_callback = (group_info *)group_info_struct;
    memset(&group_callback,0,sizeof(group_info));
    group_callback->groupid = atoi(data[0]);
    strcpy(group_callback->group_name,data[1]);
    group_callback->member_num = atoi(data[2]);

    memset(buff,0,sizeof(buff));
    if(data[3][0] == 0)
        memset(group_callback->member_id,0,sizeof(group_callback->member_id));
    else
    {
        while(data[3][i] != 0)
        {
            if(data[3][i] == '-')                   //遇到'-'则停止并将字符串中的内容转为int放入结构体中
            {
                group_callback->member_id[j ++] = atoi(buff);
                memset(buff,0,sizeof(buff));
                k = 0;
                i ++;
            }
            buff[k] = data[3][i];                   //依次将字符串放入buff中
            i ++;
            k ++;
        }
        group_callback->member_id[j ++] = atoi(buff);
    }
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