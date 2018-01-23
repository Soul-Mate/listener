#include "lis_core.h"
#include "lis_config.h"

/**
 * 程序终止后的清理函数
 * @param status 程序退出时的状态
 * @param arg    调用进程传递的参数
 */
void exit_cleanup(int status, void *arg)
{
    listener_t *l = (listener_t *)arg;
    
    /* 关闭文件描述符 */
    close(l->dir_fd);
    
    /* 删除互斥量 */
    pthread_mutex_destroy(&l->mutex_lock);
    
    /* 关闭mysql连接 */
    mysql_close(l->hm->conn);
    free(l->hm->host);
    free(l->hm->db);
    free(l->hm->user);
    free(l->hm->passwd);
    free(l->hm->charset);
    free(l->hm->sql);
    free(l->hm);
    
    /* 关闭redis连接 */
    redisFree(l->redis_conn);
    
    /* 释放常亮文件计数结构内存 */
    int i;
    
    for (i = 0; i < l->const_file_size; i++) {
        free(l->const_file[i]);
    }
    
    free(l);
}