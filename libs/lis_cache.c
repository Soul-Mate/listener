#include "lis_core.h"
#include "lis_config.h"

/*写入数据*/
int redis_set(void *conn, void *k, void *v)
{
    redisReply *reply;
    redisContext *redis_conn;

    redis_conn = (redisContext *)conn;
    k = (char *)k;
    v = (char *)v;
    reply = redisCommand(redis_conn, "SET %s %s", k, v);

    if (reply->type != REDIS_REPLY_STATUS) {
#ifdef DEBUG
        printf("Error: %s\n",redis_conn->errstr);
#endif
        return -1;
    }
    freeReplyObject(reply);
    return 0;
}

/* 取出数据 */
void *redis_get(void *conn, void *k)
{
    redisReply *reply;
    redisContext *redis_conn;
    redis_conn = (redisContext *)conn;
    
    k = (char *)k;
    reply = redisCommand(redis_conn,"GET %s", k);
    
    if (reply->type != REDIS_REPLY_STRING ) {
#ifdef DEBUG
        printf("Error: %s\n",redis_conn->errstr);
#endif
        return NULL;
    }
    return (void *)reply;
}

int cache_file_value (int fd, int index, listener_t *lis, cache_set set)
{
    struct stat stat_buf;				/* 文件属性 */
    
    if (fstat(fd,&stat_buf) != 0) {
        return -1;
    }
    
    char read_buf[stat_buf.st_size + 1];
    
    if (read(fd, read_buf, (size_t) (stat_buf.st_size + 1)) < 0) {
        return -1;
    }
    
    pthread_mutex_lock(&lis->mutex_lock);
    set(lis->redis_conn, lis->const_file[index]->key, (void *)read_buf);
    lis->const_file[index]->ref = 1;
    pthread_mutex_unlock(&lis->mutex_lock);
    close(fd);
    return 0;
}

void cache_to_db_for_index(void *redis_conn, void *hm, int const index, char * const key)
{
    redisReply *reply;
    redis_conn = (redisContext *)redis_conn;
    hm = (himysql_t *)hm;
    switch (index) {
        case LEAGUE_FILE_INDEX:
            reply = redis_get(redis_conn,key);
            save_league_to_db(hm,(void *)reply->str);
            break;
        case LEAGUE_FULL_FILE_INDEX:
            reply = redis_get(redis_conn,key);
            save_league_to_db(hm,(void *)reply->str);
            break;
        case MARKET_FULL_FILE_INDEX:
            reply = redis_get(redis_conn,key);
            save_market_to_db(hm,(void *)reply->str);
            break;
        case MATCH_FULL_FILE_INDEX:
            reply = redis_get(redis_conn,key);
            save_match_to_db(hm,(void *)reply->str);
            break;
    }
    freeReplyObject(reply);
}

//void cache_to_db_for_index(void *redis_conn, void *hm, int const index, char * const key)
//{
//    redisReply *reply;
//    redis_conn = (redisContext *)redis_conn;
//    hm = (himysql_t *)hm;
//    switch (index) {
//        case LEAGUE_FILE_INDEX:
//            reply = redis_get(redis_conn,key);
//            save_league_to_db(hm,(void *)reply->str);
//            break;
//        case LEAGUE_FULL_FILE_INDEX:
//            reply = redis_get(redis_conn,key);
//            save_league_to_db(hm,(void *)reply->str);
//            break;
//        case MARKET_FULL_FILE_INDEX:
//            reply = redis_get(redis_conn,key);
//            save_market_to_db(hm,(void *)reply->str);
//            break;
//        case MATCH_FULL_FILE_INDEX:
//            reply = redis_get(redis_conn,key);
//            save_match_to_db(hm,(void *)reply->str);
//            break;
//    }
//    freeReplyObject(reply);
//}