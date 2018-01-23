#include "lis_core.h"
#include "lis_config.h"

static int new_mysql_connect(himysql_t **);

static int new_redis_connect(redisContext **);

static int new_const_files_ref(listener_t *);

static int
new_mysql_connect(himysql_t **hm)
{
    (*hm) = himysql_init(DB_HOST, DB_PORT, DB_NAME, DB_USER_NAME, DB_USER_PASS, NULL, 0x0);
    if ((*hm) == NULL) {
        return -1;
    }
    return 0;
}

static int
new_redis_connect(redisContext **c)
{
    (*c) = redisConnect(REDIS_HOST, REDIS_PORT);
    
    if ((*c) == NULL || (*c)->err) {
        if ((*c)) {
            printf("%s: %s\n", "Error:", (*c)->errstr);
            return -1;
            
        } else {
            printf("%s\n", "Can't allocate redis context");
            return -1;
        }
    }
    return 0;
}

static int
new_const_files_ref(listener_t * lis)
{
    /* 初始化常亮文件结构 */
    /* 该结构主要用来做为引用计数 为定时器是否进行冷热数据交换提供判定条件 */
    lis->const_file = (const_file_t **)malloc(sizeof(const_file_t *) * 4);
    if (lis->const_file == NULL) {
        return -1;
    }
    lis->const_file[LEAGUE_FILE_INDEX] = (const_file_t *)malloc(sizeof(const_file_t));
    lis->const_file[LEAGUE_FILE_INDEX]->key = (void *)"league";
    lis->const_file[LEAGUE_FILE_INDEX]->ref = 0;
    lis->const_file_size++;
    
    lis->const_file[MATCH_FULL_FILE_INDEX] = (const_file_t *)malloc(sizeof(const_file_t));
    lis->const_file[MATCH_FULL_FILE_INDEX]->key = (void *)"matchfull";
    lis->const_file[MATCH_FULL_FILE_INDEX]->ref = 0;
    lis->const_file_size++;
    
    lis->const_file[MARKET_FULL_FILE_INDEX] = (const_file_t *)malloc(sizeof(const_file_t));
    lis->const_file[MARKET_FULL_FILE_INDEX]->key = (void *)"marketfull";
    lis->const_file[MARKET_FULL_FILE_INDEX]->ref = 0;
    lis->const_file_size++;
    
    lis->const_file[LEAGUE_FULL_FILE_INDEX] = (const_file_t *)malloc(sizeof(const_file_t));
    lis->const_file[LEAGUE_FULL_FILE_INDEX]->key = (void *)"leaguefull";
    lis->const_file[LEAGUE_FULL_FILE_INDEX]->ref = 0;
    lis->const_file_size++;
    return 0;
}

listener_t * new_listener()
{
    listener_t *lis = (listener_t *)malloc(sizeof(listener_t));
    
    if (lis == NULL) {
#ifdef DEBUG
        printf("Error listener malloc error.\n");
#endif
        return NULL;
    }
    
    if ((lis->inotify_fd = inotify_init()) == -1) {
#ifdef DEBUG
        printf("Error inotify_init error.\n");
#endif
        
        free(lis);
        return NULL;
    }
    
    /* 创建监听文件描述符 */
    if ((lis->dir_fd = open(LISTENER_ROOT_PATH, O_RDONLY)) == -1) {
#ifdef DEBUG
        printf("Error: The %s directory not exists.\n",LISTENER_ROOT_PATH);
#endif
        
        free(lis);
        return NULL;
    }
    
    /* 初始化互斥量 */
    if (pthread_mutex_init(&lis->mutex_lock,NULL) != 0) {
#ifdef DEBUG
        printf("Error: pthread_mutex_init error.\n");
#endif
        
        free(lis);
        return NULL;
    }
    
    /* 创建mysql连接 */
    if (new_mysql_connect(&lis->hm) == -1) {
#ifdef DEBUG
        printf("Error: mysql connect error\n");
#endif
        free(lis);
        return NULL;
    }
    
    /* 创建redis连接 */
    if (new_redis_connect(&lis->redis_conn) == -1) {
#ifdef DEBUG
        printf("Error: new_redis_connect error.");
#endif
        
        free(lis);
        return NULL;
    }
    
    /* 创建常量文件计数结构 */
    if (new_const_files_ref(lis) == -1) {
#ifdef DEBUG
        printf("Error: new_const_files_ref error.\n");
#endif
        
        free(lis);
        return NULL;
    }
    return lis;
}

