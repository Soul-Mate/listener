#ifndef LIS_NOTIFY_H
#define LIS_NOTIFY_H

#define FILE_NAME_MAX 255

#define DEFAULT_INOTIFY_COUNT 10

/* 计算inotify长度 */
#define INOTIFY_BUF_LEN(_INOTIFY_EVENT_NUM_) (_INOTIFY_EVENT_NUM_ * (sizeof (struct inotify_event) + FILE_NAME_MAX + 1))

/* 启动文件监听 */
void start_inotify(listener_t *);

/* 读取inotify事件 */
void read_inotify_event(listener_t *);

/* 解析文件 */
void *parse_routine(void *);

#endif /* LIS_NOTIFY_H */
