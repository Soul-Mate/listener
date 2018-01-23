#include "lis_core.h"
#include "lis_config.h"

/* 增量match文件正则规则 */
static char *pattern_match_incr_file = "match[0-9]+\\.json$";

/* 增量market文件正则规则  */
static char *pattern_market_incr_file = "market[0-9]+\\.json$";

/* 常量文件 matchfull.json */
static char *matches_full = "matchfull.json";

/* 常量文件 leaguesfull.json */
static char *leagues_full = "leaguesfull.json";

/* 常量文件 marketfull.json */
static char *market_full = "marketfull.json";

/* 常量文件 league.json */
static char *league = "league.json";

/* listener_t 结构体 */
static listener_t *listener;

/**
 * 启动监听器
 * @param listener_t *l
 */
void start_inotify(listener_t *lis)
{
	listener = lis;
	
	uint32_t 	mask;
	
	mask = IN_CREATE | IN_MODIFY | IN_MOVE_SELF | IN_MOVED_FROM | IN_MOVED_TO;

	if (inotify_add_watch(lis->inotify_fd, LISTENER_ROOT_PATH, mask) == -1) {
#ifdef DEBUG
      printf("Error: inotify_add_watch %s: %s. in %s, line: %d\n",
             LISTENER_ROOT_PATH,strerror(errno),__FILE__,__LINE__);
#endif

#ifdef RUN
        lis_log(LIS_LOG_ERR,"Error: inotify_add_watch %s: %s. in %s, line: %d\n",
                LISTENER_ROOT_PATH,strerror(errno),__FILE__,__LINE__);
#endif
		
		exit(1);
	}

	for (;;) {
        read_inotify_event(lis);	/* 读取inotify事件 */
    }
}

/**
 * 读取监听器的事件
 * @param l
 */
void read_inotify_event(listener_t *l)
{
	int  n;
    char *p;
    ssize_t read_num;
	size_t ev_buf_len;
	struct inotify_event *event;
 
	/* 计算读取事件的buf大小 默认个数 DEFAULT_INOTIFY_COUNT */
	ev_buf_len = INOTIFY_BUF_LEN(DEFAULT_INOTIFY_COUNT);
	
    char read_buf[ev_buf_len];
	
    read_num = read(l->inotify_fd, read_buf, ev_buf_len);
	n = 0;
	p = read_buf;
	
    do { /* 处理多个事件 */
		event = (struct inotify_event *)p; /* 获取事件 */
		// TODO 使用线程池+异步IO
        parse_routine(&event->name); /* 解析事件 */
		int tmp_len = sizeof(struct inotify_event) + event->len; /* 计算单个事件的长度 */
		n += tmp_len;
		p += tmp_len;
	} while (n < read_num);
}

/**
 * 解析文件的线程
 * @param arg
 * @return
 */
void *parse_routine(void *arg)
{
	int fd;
	
	int status;
	
	regex_t reg;
	
	char *file = (char *)arg;

	
	if (strcmp(file, matches_full) == 0) { /* matchfull.json */
  
		fd = openat(listener->dir_fd, file, O_RDONLY);
		
		if (fd < 0) {
#ifdef DEBUG
            printf("Error: can't openat %s file. in %s,line: %d\n",
                   file,__FILE__,__LINE__);
#endif

#ifdef RUN
            lis_log(LIS_LOG_ERR,"Error: can't openat %s file. in %s,line: %d\n",
                    file,__FILE__,__LINE__);
#endif
            return NULL;
        }
        
        if (cache_file_value(fd,MATCH_FULL_FILE_INDEX,listener,redis_set) != 0) {
#ifdef DEBUG
            printf("Error: can't cache file: %s. in %s,line: %d\n",
                   file,__FILE__,__LINE__);
#endif

#ifdef RUN
            lis_log(LIS_LOG_ERR,"Error: can't cache file: %s. in %s,line: %d\n",
                    file,__FILE__,__LINE__);
#endif
            return NULL;
        }
	}
	
	if (strcmp(file, market_full) == 0) { /* marketfull.json */
  
		fd = openat(listener->dir_fd, file, O_RDONLY);
		
		if (fd < 0) {
#ifdef DEBUG
            printf("Error: can't openat %s file. in %s,line: %d\n",
                   file,__FILE__,__LINE__);
#endif

#ifdef RUN
            lis_log(LIS_LOG_ERR,"Error: can't openat %s file. in %s,line: %d\n",
                    file,__FILE__,__LINE__);
#endif
            return NULL;
        }
        
        if (cache_file_value(fd,MARKET_FULL_FILE_INDEX,listener,redis_set) != 0) {
#ifdef DEBUG
            printf("Error: can't cache file: %s. in %s,line: %d\n",
                   file,__FILE__,__LINE__);
#endif

#ifdef RUN
            lis_log(LIS_LOG_ERR,"Error: can't cache file: %s. in %s,line: %d\n",
                    file,__FILE__,__LINE__);
#endif
            return NULL;
        }
	}

	if (strcmp(file, leagues_full) == 0) { /* leaguesfull.json */
  
		fd = openat(listener->dir_fd, file, O_RDONLY);
		
		if (fd < 0) {
#ifdef DEBUG
            printf("Error: can't openat %s file. in %s,line: %d\n",
                   file,__FILE__,__LINE__);
#endif

#ifdef RUN
            lis_log(LIS_LOG_ERR,"Error: can't openat %s file. in %s,line: %d\n",
                    file,__FILE__,__LINE__);
#endif
            return NULL;
		}
        if (cache_file_value(fd,LEAGUE_FULL_FILE_INDEX,listener,redis_set) != 0) {
#ifdef DEBUG
            printf("Error: can't cache file: %s. in %s,line: %d\n",
                   file,__FILE__,__LINE__);
#endif

#ifdef RUN
            lis_log(LIS_LOG_ERR,"Error: can't cache file: %s. in %s,line: %d\n",
                    file,__FILE__,__LINE__);
#endif
            return NULL;
        }
	}

    if (strcmp(file, league) == 0) { /* league.json */
        
        fd = openat(listener->dir_fd, file, O_RDONLY);
        
        if (fd < 0) {
#ifdef DEBUG
            printf("Error: can't openat %s file. in %s,line: %d\n",
                   file,__FILE__,__LINE__);
#endif

#ifdef RUN
            lis_log(LIS_LOG_ERR,"Error: can't openat %s file. in %s,line: %d\n",
                    file,__FILE__,__LINE__);
#endif
            return NULL;
        }
        if (cache_file_value(fd,LEAGUE_FILE_INDEX,listener,redis_set) != 0) {
#ifdef DEBUG
            printf("Error: can't cache file: %s. in %s,line: %d\n",
                   file,__FILE__,__LINE__);
#endif

#ifdef RUN
            lis_log(LIS_LOG_ERR,"Error: can't cache file: %s. in %s,line: %d\n",
                    file,__FILE__,__LINE__);
#endif
            return NULL;
        }
    }
    
	/* match增量文件 */
	regcomp(&reg, pattern_match_incr_file, REG_NOSUB | REG_EXTENDED);
	status = regexec(&reg, file, 0, NULL, 0);
	
	if (status != REG_NOMATCH) {
#ifdef DEBUG
		printf("match file: %s. in %s,line: %d\n",
               file,__FILE__,__LINE__);
#endif

#ifdef RUN
        lis_log(LIS_LOG_RUN,"match file: %s. in %s,line: %d\n",
               file,__FILE__,__LINE__);
#endif
  
		char *file_value;
		
		fd = openat(listener->dir_fd, file, O_RDONLY);
		
		if (fd < 0) {
#ifdef DEBUG
            printf("Error: can't openat %s file. in %s,line: %d\n",
                   file,__FILE__,__LINE__);
#endif

#ifdef RUN
            lis_log(LIS_LOG_ERR,"Error: can't openat %s file. in %s,line: %d\n",
                    file,__FILE__,__LINE__);
#endif
			return NULL;
		}
        
        if (fread_file(fd, (void **) &file_value) > 0) {
            
            // TODO 出错情况
            save_match_to_db(listener->hm,file_value);
            
#ifdef DEBUG
            printf("write file: %s to db\n",file);
#endif

#ifdef RUN
            lis_log(LIS_LOG_RUN,"write file: %s to db\n",file);
#endif
            /* 删除文件 */
            if (unlinkat(listener->dir_fd,file,0) != 0) {
#ifdef DEBUG
                printf("can't remove %s file. in %s, line: %d\n",
                       file,__FILE__,__LINE__);
#endif

#ifdef RUN
                lis_log(LIS_LOG_ERR,"can't remove %s file. in %s, line: %d\n",
                        file,__FILE__,__LINE__);
#endif
            } else {
#ifdef DEBUG
                printf("remove %s file. in %s, line: %d\n",
                       file,__FILE__,__LINE__);
#endif

#ifdef RUN
                lis_log(LIS_LOG_RUN,"remove %s file. in %s, line: %d\n",
                        file,__FILE__,__LINE__);
#endif
            }
        }
    }
    
    
	/* market增量文件 */
	regcomp(&reg, pattern_market_incr_file, REG_NOSUB | REG_EXTENDED);
 
	status = regexec(&reg, file, 0, NULL, 0);
	
	if (status != REG_NOMATCH) {
#ifdef DEBUG
        printf("match file: %s. in %s,line: %d\n",
               file,__FILE__,__LINE__);
#endif

#ifdef RUN
        lis_log(LIS_LOG_RUN,"match file: %s. in %s,line: %d\n",
                file,__FILE__,__LINE__);
#endif
  
		char *file_value;
		
		fd = openat(listener->dir_fd, file, O_RDONLY);
        
        if (fd < 0) {
#ifdef DEBUG
            printf("Error: can't openat %s file. in %s,line: %d\n",
                   file,__FILE__,__LINE__);
#endif

#ifdef RUN
            lis_log(LIS_LOG_ERR,"Error: can't openat %s file. in %s,line: %d\n",
                    file,__FILE__,__LINE__);
#endif
            return NULL;
        }
        
        if (fread_file(fd, (void **) &file_value) > 0) {
            
            // TODO 出错情况
            save_market_to_db(listener->hm,file_value);

#ifdef DEBUG
            printf("write file: %s to db\n",file);
#endif

#ifdef RUN
            lis_log(LIS_LOG_RUN,"write file: %s to db\n",file);
#endif
            /* 删除文件 */
            if (unlinkat(listener->dir_fd,file,0) != 0) {
#ifdef DEBUG
                printf("can't remove %s file. in %s, line: %d\n",
                       file,__FILE__,__LINE__);
#endif

#ifdef RUN
                lis_log(LIS_LOG_ERR,"can't remove %s file. in %s, line: %d\n",
                        file,__FILE__,__LINE__);
#endif
            } else {
#ifdef DEBUG
                printf("remove %s file. in %s, line: %d\n",
                       file,__FILE__,__LINE__);
#endif

#ifdef RUN
                lis_log(LIS_LOG_RUN,"remove %s file. in %s, line: %d\n",
                        file,__FILE__,__LINE__);
#endif
            }
        }
	}
	return NULL;
}
