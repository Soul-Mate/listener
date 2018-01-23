#ifndef LIS_CACHE_H
#define LIS_CACHE_H

/* 定义设置缓存函数 */
typedef int (*cache_set)(void *, void *, void *);

/* 定义获取缓存函数 */
typedef void* (*cache_get)(void *, void *, void *);

/* redis 写入数据 */
int redis_set(void *, void *, void *);

/* redis 取出数据 */
void *redis_get(void *conn, void *k);

int cache_file_value (int, int, listener_t *, cache_set);

/* cache数据持久化到db */
void cache_to_db_for_index(void *redis_conn, void *hm, int const index, char * const key);

#endif /* LIS_CACHE_H */
