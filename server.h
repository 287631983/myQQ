#ifndef _SERVER_H
#define _SERVER_H
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
char *itoa(int num);
int sqlite3_usercallback(void *ret,int columns,char **data,char **item);
int sqlite3_groupcallback(void *ret,int columns,char **data,char **item);
#endif
