#ifndef LIS_CONFIG_H
#define LIS_CONFIG_H

/* 需要监听的常量文件对应的索引 */
#define LEAGUE_FILE_INDEX 0
#define MATCH_FULL_FILE_INDEX 1
#define MARKET_FULL_FILE_INDEX 2
#define LEAGUE_FULL_FILE_INDEX 3

/* 需要监听的根目录 */
#define LISTENER_ROOT_PATH "/home/www/"

/*需要监听的常量文件*/
#define LEAGUE_FILE "/home/www/league.json"
#define MATCH_FULL_FILE "/home/www/matchfull.json"
#define MARKET_FULL_FILE "/home/www/marketfull.json"
#define LEAGUE_FULL_FILE "/home/www/leaguesfull.json"

/* 守护进程pid保存路径 */
#define LOCK_FILE "/var/lock/listener.pid"


/* 守护进程pid访问模式 */
#define LOCK_MODE (S_IRUSR | S_IWUSR |S_IRGRP | S_IROTH)

#define REDIS_HOST "127.0.0.1"
#define REDIS_PORT 6379

#define DB_HOST "192.168.0.109"
#define DB_PORT 3306
#define DB_NAME "test"
#define DB_USER_NAME "root"
#define DB_USER_PASS "root"


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

#endif /* LIS_CONFIG_H  */
