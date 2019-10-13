#ifndef _DATABASE_H
#define _DATABASE_H

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
#include <sqlite3.h>
#include "server.h"
#include "client.h"

void init_database(sqlite3 *db);
int insert_user(sqlite3 *db,user_info *user_insert);
void insert_group(sqlite3 *db,struct GROUP_CHAT group_insert);
int view_user(sqlite3 *db,char *user_name,user_info *user_data);
void view_group(sqlite3 *db,int group_id,struct GROUP_CHAT *group_chat);
int sqlite3_usercallback(void *user_info_struct,int columns,char **data,char **item);
int sqlite3_groupcallback(void *group_info_struct,int columns,char **data,char **item);
char *itoa(int num);

#endif
