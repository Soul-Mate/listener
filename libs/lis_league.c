#include "lis_core.h"
#include "lis_config.h"

static int
create_league_tournament_for_mysql(himysql_t *hm, league_tournament_t *league_tournament);

static int
update_league_tournament_for_mysql(himysql_t *hm, league_tournament_t *league_tournament);

static int
parse_league_json(himysql_t *hm, cJSON *json, league_t *p_league);

static int
update_or_create_league_for_mysql(himysql_t *hm,league_t *p_league);

static int
create_league_tournament_for_mysql(himysql_t *hm, league_tournament_t *league_tournament)
{
    int status;
    
    /* 插入数据 */
    status = himysql(hm,INSERT_LEAGUE_TOURNAMENT_SQL,league_tournament->number,
                     league_tournament->name,league_tournament->league_number);
    
    if (status != HM_OK) {
#ifdef DEBUG
        printf("Error: %s. in %s, line: %d\n",
               himysql_error(hm),__FILE__,__LINE__);
#endif

#ifdef RUN
        lis_log(LIS_LOG_ERR,"%s. in %s, line: %d\n",
                himysql_error(hm),__FILE__,__LINE__);
#endif
        return -1;
    }
    return 0;
}

static int
update_league_tournament_for_mysql(himysql_t *hm, league_tournament_t *league_tournament)
{
    int status;
    status = himysql(hm,"SELECT number from `radar_league_tournaments` where number = %u",
                     league_tournament->number);
    
    /* query error */
    if (status != HM_OK) {
#ifdef DEBUG
        printf("Error: %s. in %s, line: %d\n",
               himysql_error(hm),__FILE__,__LINE__);
#endif

#ifdef RUN
        lis_log(LIS_LOG_ERR,"%s. in %s, line: %d\n",
                himysql_error(hm),__FILE__,__LINE__);
#endif
        return -1;
        
    } else {
        himysql_fetch_row(hm);
        
        if (hm->row == NULL) {
            /* query null 时新增数据 */
            return create_league_tournament_for_mysql(hm, league_tournament);
            
        } else {
            /* 更新数据 */
            status = himysql(hm,UPDATE_LEAGUE_TOURNAMENT_SQL,
                             league_tournament->name,league_tournament->league_number,league_tournament->number);
            
            if (status != HM_OK) {
#ifdef DEBUG
                printf("Error: %s. in %s, line: %d\n",
                       himysql_error(hm),__FILE__,__LINE__);
#endif

#ifdef RUN
                lis_log(LIS_LOG_ERR,"%s. in %s, line: %d\n",
                        himysql_error(hm),__FILE__,__LINE__);
#endif
                return -1;
            }
        }
    }
    return 0;
}

static int
update_or_create_league_for_mysql(himysql_t *hm,league_t *p_league)
{
    int i;
    
    int status;
    status = himysql(hm,"SELECT `number` from `radar_leagues` where `number` = %d",p_league->number);
    /* query error */
    if (status != HM_OK) {
#ifdef DEBUG
        printf("Error: %s. in %s, line: %d\n",
               himysql_error(hm),__FILE__,__LINE__);
#endif

#ifdef RUN
        lis_log(LIS_LOG_ERR,"%s. in %s, line: %d\n",
                himysql_error(hm),__FILE__,__LINE__);
#endif
        return -1;
        
    } else {
        himysql_fetch_row(hm);
        /* query结果为空 */
        if (hm->row == NULL) {
            /* 插入数据 */
            status = himysql(hm,INSERT_LEAGUE_SQL,
                             p_league->number,p_league->name,p_league->sport_id);
            
            if (status != HM_OK) {
#ifdef DEBUG
                printf("Error: %s. in %s, line: %d\n",
                       himysql_error(hm),__FILE__,__LINE__);
#endif

#ifdef RUN
                lis_log(LIS_LOG_ERR,"%s. in %s, line: %d\n",
                        himysql_error(hm),__FILE__,__LINE__);
#endif
                return -1;
                
            } else {
                if (p_league->league_tournament_size != 0) {
                    
                    /* 插入league_tournament数据 */
                    for (i = 0; i < p_league->league_tournament_size; ++i) {
                        
                        if (create_league_tournament_for_mysql(hm, p_league->league_tournament[i]) != 0) {
#ifdef DEBUG
                            printf("insert league_tournament data error: league_number = %d, in file :%s, line :%d\n",
                                   p_league->number,__FILE__,__LINE__);
#endif

#ifdef RUN
                            lis_log(LIS_LOG_ERR,"insert league_tournament data error: league_number = %d, in file :%s, line :%d\n",
                                    p_league->number,__FILE__, __LINE__);
#endif
                            
                            
                            free(p_league->league_tournament[i]);
                            
                        } else {
                            free(p_league->league_tournament[i]);
                        }
                    }
                }
            }
            
        } else {
            /* 更新数据 */
            status = himysql(hm,UPDATE_LEAGUE_SQL,
                             p_league->name,p_league->sport_id,p_league->number);
            
            if (status != HM_OK) {
#ifdef DEBUG
                printf("Error: %s. in %s, line: %d\n",
                       himysql_error(hm),__FILE__,__LINE__);
#endif

#ifdef RUN
                lis_log(LIS_LOG_ERR,"%s. in %s, line: %d\n",
                        himysql_error(hm),__FILE__,__LINE__);
#endif
                return -1;
                
            } else {
                /* 更新league_tournament数据 */
                for (i = 0; i < p_league->league_tournament_size; ++i) {
                    
                    if (update_league_tournament_for_mysql(hm, p_league->league_tournament[i]) != 0) {
#ifdef DEBUG
                        printf("insert league_tournament data error: league_number = %d, in file :%s, line :%d\n",
                               p_league->number,__FILE__,__LINE__);
#endif

#ifdef RUN
                        lis_log(LIS_LOG_ERR,"insert league_tournament data error: league_number = %d, in file :%s, line :%d\n",
                                p_league->number,__FILE__, __LINE__);
#endif
                        
                        free(p_league->league_tournament[i]);
                        
                    } else {
                        free(p_league->league_tournament[i]);
                    }
                }
            }
        }
    }
    return 0;
}

static int
parse_league_json(himysql_t *hm, cJSON *json, league_t *p_league)
{
    /**
     * 数据源中的一些字段有可能是字符 也有可能是整形
     * 需要对两种情况进行判定
     */
    
    /* id */
    cJSON *number = cJSON_GetObjectItemCaseSensitive(json,"Id");
    if (cJSON_IsNumber(number)) {
        p_league->number = number->valueint;
        
    } else if(cJSON_IsString(number)) {
        p_league->number = atoi(number->valuestring);
        
    } else {
        cJSON_Delete(number);
        return -1;
    }
    
    /* 名称 */
    cJSON *name = cJSON_GetObjectItemCaseSensitive(json,"CategoryName");
    if (cJSON_IsString(name) && name->valuestring != NULL) {
        strcpy(p_league->name,name->valuestring);
        
    } else {
        strcpy(p_league->name,"");
    }
    
    /* sport id */
    cJSON *sport_id = cJSON_GetObjectItemCaseSensitive(json,"SportId");
    if (cJSON_IsNumber(sport_id)) {
        p_league->sport_id = sport_id->valueint;
        
    } else if(cJSON_IsString(sport_id)) {
        p_league->sport_id = atoi(sport_id->valuestring);
        
    } else{
        p_league->sport_id = 0;
    }
    
    /* tournaments 集合 */
    cJSON *tournaments = cJSON_GetObjectItemCaseSensitive(json,"Tournaments");
    if (cJSON_IsArray(tournaments)) {
        int i,len;
        i = 0;
        len = cJSON_GetArraySize(tournaments);
        
        /* 空的数据集 */
        if (len == 0) {
            p_league->league_tournament_size = 0;
            goto end;
            
        } else {
            cJSON *item;
            p_league->league_tournament_size = len;
            p_league->league_tournament = (league_tournament_t **)malloc(
                    sizeof(league_tournament_t *) * len);
            
            /* 迭代数据集 */
            cJSON_ArrayForEach(item,tournaments) {
                /* 迭代完成 */
                if (i == len) {
                    goto end;
                }
                
                /* 为每个league_tournament_t结构体分配内存 */
                p_league->league_tournament[i] = (league_tournament_t *)malloc(
                        sizeof(league_tournament_t));
                
                /* tournament id */
                cJSON *tournament_number =  cJSON_GetObjectItemCaseSensitive(item,"Id");
                if (cJSON_IsNumber(tournament_number)) {
                    p_league->league_tournament[i]->number = tournament_number->valueint;
                    
                } else if(cJSON_IsString(tournament_number)
                          && tournament_number != NULL)
                {
                    p_league->league_tournament[i]->number = atoi(tournament_number->valuestring);
                }
                
                /* tournament name*/
                cJSON *tournament_name =  cJSON_GetObjectItemCaseSensitive(item,"Name");
                if (cJSON_IsString(tournament_name)
                    && tournament_name->valuestring != NULL)
                {
                    strcpy(p_league->league_tournament[i]->name,tournament_name->valuestring);
                } else {
                    strcpy(p_league->league_tournament[i]->name,"");
                }
                
                /* tournament -> league number */
                cJSON *tournament_league_number = cJSON_GetObjectItemCaseSensitive(item,"CategoryId");
                if (cJSON_IsNumber(tournament_league_number)) {
                    p_league->league_tournament[i]->league_number = tournament_league_number->valueint;
                    
                } else if(cJSON_IsString(tournament_league_number)
                          && tournament_league_number->valuestring != NULL)
                {
                    p_league->league_tournament[i]->league_number = atoi(
                            tournament_league_number->valuestring);
                } else {
                    p_league->league_tournament[i]->league_number = 0;
                }
                i++;
            }
        }
        
    } else {
        p_league->league_tournament_size = 0;
        goto end;
    }
end:
    // 写入mysql
    update_or_create_league_for_mysql(hm,p_league);
    return 0;
}

void save_league_to_db(himysql_t *hm, void *arg)
{
    /* 读取json文件的缓冲区数据 */
    char *buf = (char *)arg;
    
    /* 解析为json格式 */
    cJSON *json = cJSON_Parse(buf);
    
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        
        if (error_ptr != NULL) {

#ifdef DEBUG
            printf("Error: %s in %s,line: %d\n",
                   error_ptr,__FILE__,__LINE__);
#endif

#ifdef RUN
            lis_log(LIS_LOG_ERR,"%s, in %s,line: %d",
                    error_ptr,__FILE__,__LINE__);
#endif
            goto end;
        }
        
    }
    
    if (cJSON_IsArray(json)) {
        int i,len;
        cJSON *item;
        
        i = 0;
        len = cJSON_GetArraySize(json);
        
        /* 根据json的大小动态创建结构体 */
        league_t leagues[len];
        
        cJSON_ArrayForEach(item,json) {
            if (i == len) {
                goto end;
            }
            
            /* 解析json数据 */
            parse_league_json(hm,item,&leagues[i]);
            i++;
        }
        
    } else {
        league_t league;
        parse_league_json(hm,json,&league);
        goto end;
    }
end:
    cJSON_Delete(json);
}

void save_leaguesfull(listener_t *lis)
{
    char *buf;
    ssize_t read_num;
    
    read_num = read_file(LEAGUE_FULL_FILE,buf);
    
    if (read_num > 0) {
        save_league_to_db(lis->hm, (void *)buf); /* 保存到数据库 */
        
        /* 保存到内存数据库 */
        if (redis_set(lis->redis_conn, (void *)"leaguefull", (void *)buf) == -1) {
            lis_log(LIS_LOG_ERR,"Error: redis_set error for key leaguefull. in %s,line: %d\n",
                    __FILE__,__LINE__);
        }
        
        free(buf);
    }
}

void save_league(listener_t *lis)
{
    char *buf;
    
    buf = get_file_value(LEAGUE_FILE);
    printf("%s\n", buf);
    if (buf != NULL) {
        save_league_to_db(lis->hm, (void *)buf); /* 保存到数据库 */
        
        /* 保存到内存数据库 */
        if (redis_set(lis->redis_conn, (void *)"league", (void *)buf) == -1) {
            lis_log(LIS_LOG_ERR,"Error: redis_set error for key league. in %s,line: %d\n",
                    __FILE__,__LINE__);
        }
        
        free(buf);
    }
}