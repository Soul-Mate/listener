#include "lis_core.h"
#include "lis_market.h"

static int find_market_for_mysql(himysql_t *, int);

static int create_market_for_mysql(himysql_t *, market_t *);

static int update_market_for_mysql(himysql_t *, market_t *);

static int update_or_create_market_for_mysql(himysql_t *, market_t *);

static int parse_market_json(himysql_t *, cJSON *, market_t *);

static void free_market(market_t *);

/* 根据编号查询market */
static int
find_market_for_mysql(himysql_t *hm, int number)
{
    int status;
    status = himysql(hm,"SELECT `number` from radar_markets where number = %d",number);
    
    /**
     * 返回值
     * -1 sql查询错误
     * 0 无记录
     * 1 查询到记录
     */
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
        return hm->row == NULL? 0 : 1;
    }
}

/* 插入market */
static int
create_market_for_mysql(himysql_t *hm, market_t *p_market)
{
    int status;
    
    status = himysql(hm,INSERT_MARKET_SQL,p_market->number,p_market->name,
                     p_market->match_id,p_market->suspended,p_market->status,
                     p_market->is_live,p_market->visible,p_market->odds);
    
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

/* 更新market */
static int
update_market_for_mysql(himysql_t *hm, market_t *p_market)
{
    int status;
    
    status = himysql(hm,UPDATE_MARKET_SQL,p_market->name,p_market->match_id,
                     p_market->suspended,p_market->status,p_market->is_live,
                     p_market->visible,p_market->odds,p_market->number);
    
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

/* 更新或插入market数据 */
static int
update_or_create_market_for_mysql(himysql_t *hm, market_t *p_market)
{
    int status;
    
    /* 查询失败 */
    if ((status = find_market_for_mysql(hm,p_market->number)) == -1) {
        free_market(p_market);
        return -1;
        
        /* 不存在 插入数据 */
    } else if (status == 0) {
        status =  create_market_for_mysql(hm,p_market);
        
        /* 存在 更新数据 */
    } else {
        status = update_market_for_mysql(hm,p_market);
    }
    free_market(p_market);
    return status;
}
/* 解析 market 中的 json 数据 */
static int
parse_market_json(himysql_t *hm, cJSON *json, market_t *p_market)
{
    cJSON *number = cJSON_GetObjectItemCaseSensitive(json,"Id");
    if (cJSON_IsNumber(number)) {
        p_market->number = number->valueint;
        
    } else if(cJSON_IsString(number) && number->valuestring != NULL) {
        p_market->number = atoi(number->valuestring);
        
    } else {
        return -1;
    }
    
    cJSON *name = cJSON_GetObjectItemCaseSensitive(json,"Name");
    if (cJSON_IsString(name) && name->valuestring != NULL) {
        p_market->name = (char *)malloc(strlen(name->valuestring) + 0x8);
        strcpy(p_market->name,name->valuestring);
        
    } else {
        p_market->name = malloc(1);
        strcpy(p_market->name,"");
    }
    
    
    cJSON *match_id = cJSON_GetObjectItemCaseSensitive(json,"MatchId");
    if (cJSON_IsNumber(match_id)) {
        p_market->match_id = match_id->valueint;
        
    } else if(cJSON_IsString(match_id) && match_id->valuestring != NULL) {
        p_market->match_id = atoi(match_id->valuestring);
        
    } else {
        p_market->match_id = 0;
    }
    
    cJSON *suspended =  cJSON_GetObjectItemCaseSensitive(json,"Suspended");
    if (cJSON_IsTrue(suspended)) {
        p_market->suspended = 1;
        
    } else if(cJSON_IsFalse(suspended)) {
        p_market->suspended = 0;
        
    } else {
        p_market->suspended = -1;
    }
    
    cJSON *status = cJSON_GetObjectItemCaseSensitive(json,"Status");
    if (cJSON_IsNumber(status)) {
        p_market->status = (short) status->valueint;
    } else {
        p_market->status = -1;
    }
    
    cJSON *is_live = cJSON_GetObjectItemCaseSensitive(json,"IsLive");
    if (cJSON_IsTrue(is_live)) {
        p_market->is_live = 1;
        
    } else if(cJSON_IsFalse(is_live)) {
        p_market->is_live = 0;
        
    } else {
        p_market->is_live = -1;
    }
    
    cJSON *visible = cJSON_GetObjectItemCaseSensitive(json,"Visible");
    if (cJSON_IsTrue(visible)) {
        p_market->visible = 1;
        
    } else if(cJSON_IsFalse(visible)) {
        p_market->visible = 0;
        
    } else {
        p_market->visible = -1;
    }
    
    cJSON *odds = cJSON_GetObjectItemCaseSensitive(json,"Odds");
    char *odds_str = cJSON_PrintUnformatted(odds);
    if (odds_str != NULL) {
        p_market->odds = (char *)malloc(strlen(odds_str) + 1);
        strcpy(p_market->odds,odds_str);
        free(odds_str);
        
    } else {
        p_market->odds = malloc(1);
        strcpy(p_market->odds,"");
    }
    
    update_or_create_market_for_mysql(hm,p_market);
    return 0;
}

static void free_market(market_t *p_market)
{
    free(p_market->odds);
    free(p_market->name);
}

void save_market_to_db(himysql_t *hm, void *arg)
{
    char *buf = (char *)arg;
    
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
        }
        
        goto end;
    }
    
    /* 数组 */
    if (cJSON_IsArray(json)) {
        int i,len;
        cJSON *item;
        
        i = 0;
        len = cJSON_GetArraySize(json);
        
        /* 根据json的大小动态创建结构体 */
        market_t markets[len];
        
        cJSON_ArrayForEach(item,json) {
            if (i == len) {
                goto end;
            }
            parse_market_json(hm,item,&markets[i]);
            i++;
        }
        
    } else {
        market_t market;
        parse_market_json(hm,json,&market);
        goto end;
    }
    
end:
    cJSON_Delete(json);
    
}


void save_marketfull(listener_t *lis)
{
    char *buf;
    ssize_t read_num;
    
    read_num = read_file(MARKET_FULL_FILE,(void *)&buf);
    
    if (read_num > 0) {
        save_market_to_db(lis->hm, (void *)buf); /* 保存到数据库 */
        
        /* 保存到内存数据库 */
        if (redis_set(lis->redis_conn, (void *)"marketfull", (void *)buf) == -1) {
            lis_log(LIS_LOG_ERR,"Error: redis_set error for key marketfull. in %s,line: %d\n",
                    __FILE__,__LINE__);
        }
        
        free(buf);
    }
}

