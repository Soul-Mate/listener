#include "lis_core.h"
#include "lis_config.h"

/* 定时器线程 */
void cache_routine(union sigval sv)
{
    int i;
    listener_t *l = (listener_t *)sv.sival_ptr;
    pthread_mutex_lock(&l->mutex_lock);
    for (i = 0; i < l->const_file_size; i++) {
        /* 触发计数条件 */
        if (l->const_file[i]->ref != 0) {
#ifdef RUN
            lis_log(LIS_LOG_RUN,"%s: 热数据写入到mysql\n",(char *)l->const_file[i]->key);
#endif
            cache_to_db_for_index(l->redis_conn,l->hm,i,l->const_file[i]->key);
            /* 进行数据持久化处理 */
            l->const_file[i]->ref = 0;
        }
    }
    pthread_mutex_unlock(&l->mutex_lock);
}

int set_cache_interval(listener_t *l)
{
    timer_t timer_id;
    struct sigevent evp;
    struct itimerspec timer_spec;
    
    /* 使用线程作为信号的处理方式 */
    evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_notify_function = cache_routine;
    evp.sigev_notify_attributes = NULL;
    evp.sigev_value.sival_ptr = (void *)l;
    if (timer_create(CLOCK_REALTIME,&evp,&timer_id) != 0) {
        return -1;
    }
    timer_spec.it_interval.tv_sec = 3600;
    timer_spec.it_interval.tv_nsec = 0;
    timer_spec.it_value.tv_sec = 3600;
    timer_spec.it_value.tv_nsec = 0;
    if (timer_settime(timer_id,0,&timer_spec,NULL) != 0) {
        return -1;
    }
    return 0;
}