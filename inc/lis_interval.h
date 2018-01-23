#ifndef LIS_INTERVAL_H
#define LIS_INTERVAL_H

/* 定时器到期时线程处理函数 */
void cache_routine(union sigval);

int set_cache_interval(listener_t *);

#endif /* LIS_INTERVAL_H */
