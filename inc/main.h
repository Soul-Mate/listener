#ifndef LISTENER_MAIN_H
#define LISTENER_MAIN_H
#define	MAX_LINE	4096
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stddef.h>
#include <stdarg.h>
#include <time.h>

#include <regex.h>
#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/epoll.h>
#include <sys/inotify.h>

#include <hiredis.h>
#include "himysql.h"
#include "cJSON.h"

/*需要监听的目录*/
#ifndef LISTENER_PATH
#define LISTENER_PATH "/home/www/"
#endif

/*文件名称最大长度 */
#ifndef FILE_NAMX_MAX
#define FILE_NAMX_MAX 256
#endif 

/* 需要监听的常量文件对应的索引 */
#define LEAGUE_FILE_INDEX 0x0
#define MATCH_FULL_FILE_INDEX 0x1
#define MARKET_FULL_FILE_INDEX 0x2
#define LEAGUE_FULL_FILE_INDEX 0x3


/*需要监听的常量文件*/
#define LEAGUE_FILE "/home/www/league.json"
#define MATCH_FULL_FILE "/home/www/matchfull.json"
#define MARKET_FULL_FILE "/home/www/marketfull.json"
#define LEAGUE_FULL_FILE "/home/www/leaguesfull.json"

/* 守护进程pid保存路径 */
#define LOCK_FILE "/var/lock/listener.pid"
/* 守护进程pid访问模式 */
#define LOCK_MODE (S_IRUSR | S_IWUSR |S_IRGRP | S_IROTH)


/* 常量文件 */
typedef struct _const_files {
	void *key;
	int ref;
}const_file_t;

/*listener结构*/
typedef struct _listener_t {
	int inotify_fd; /* inotify fd */
	int dir_fd;  /* 监听目录fd */
	int const_file_size; /* 常亮文件计数结构个数 */
	const_file_t **const_file; /* 常亮文件计数结构 */
	pthread_mutex_t mutex_lock; /* 互斥锁 */
	himysql_t *hm; /* mysql连接资源 */
	redisContext *redis_conn; /* redis连接资源 */
} listener_t;

void *parse_file(void *);

#endif /*LISTENER_MAIN_H*/
