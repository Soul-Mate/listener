#include "lis_core.h"
#include "lis_config.h"

static int find_match_for_mysql(himysql_t *, const int);

static int create_match_for_mysql(himysql_t *, match_t *);

static int update_match_for_mysql(himysql_t *, match_t *);

static int update_or_create_for_mysql(himysql_t *, match_t *);

static int parse_match_json(himysql_t *, cJSON *, match_t *);


static int
find_match_for_mysql(himysql_t *hm, const int number)
{
    int status;
    
    status = himysql(hm,FIND_MATCH_SQL,number);
    
    if (status != HM_OK) {
        printf("%s\n",himysql_error(hm));
        return -1;
    }
    
    himysql_fetch_row(hm);
    return hm->row == NULL? 0 : 1;
}

static int
create_match_for_mysql(himysql_t *hm, match_t *p_match)
{
    int status;
    status = himysql(hm, INSERT_MATCH_SQL,
                     p_match->number,
                     p_match->score,
                     p_match->stream_url,
                     p_match->type,
                     p_match->visible,
                     p_match->suspended,
                     p_match->status,
                     p_match->sport_id,
                     p_match->tournament_id,
                     p_match->home_team_id,
                     p_match->home_team_name,
                     p_match->away_team_id,
                     p_match->away_team_name,
                     p_match->outright_name,
                     p_match->start_time,
                     p_match->end_time);
    
    if (status != HM_OK) {
        return -1;
    }
    return 0;
}

static int
update_match_for_mysql(himysql_t *hm, match_t *p_match)
{
    int status;
    
    status = himysql(hm, UPDATE_MATCH_SQL,
                     p_match->score,
                     p_match->stream_url,
                     p_match->type,
                     p_match->visible,
                     p_match->suspended,
                     p_match->status,
                     p_match->sport_id,
                     p_match->tournament_id,
                     p_match->home_team_id,
                     p_match->home_team_name,
                     p_match->away_team_id,
                     p_match->away_team_name,
                     p_match->outright_name,
                     p_match->start_time,
                     p_match->end_time,
                     p_match->number);
    
    if (status != HM_OK) {
        printf("%s\n",himysql_error(hm));
        return -1;
    }
    return 0;
}

static int
update_or_create_for_mysql(himysql_t *hm, match_t *p_match)
{
    int status;
    
    /* 查询错误 */
    if ((status = find_match_for_mysql(hm,p_match->number)) == -1) {
        return -1;
        
        /* 插入数据 */
    } else if (status == 0) {
        status =  create_match_for_mysql(hm,p_match);
        
        /* 更新数据 */
    } else {
        status = update_match_for_mysql(hm,p_match);
    }
    return status;
}

static int
parse_match_json(himysql_t *hm, cJSON *item, match_t *p_match)
{
    
    /* id */
    cJSON *number = cJSON_GetObjectItemCaseSensitive(item, "Id");
    if (cJSON_IsNumber(number)) {
        p_match->number = number->valueint;
        
    } else if (cJSON_IsString(number) && number->valuestring != NULL) {
        p_match->number = atoi(number->valuestring);
        
    } else {
        return -1;
    }
    
    /* score */
    cJSON *score = cJSON_GetObjectItemCaseSensitive(item, "Score");
    if (cJSON_IsString(score) && score->valuestring != NULL) {
        strcpy(p_match->score, score->valuestring);
        
    } else {
        strcpy(p_match->score, "");
    }
    
    /* stream url */
    cJSON *stream_url = cJSON_GetObjectItemCaseSensitive(item, "StreamURL");
    if (cJSON_IsString(stream_url) && stream_url->valuestring != NULL) {
        strcpy(p_match->stream_url, stream_url->valuestring);
    } else {
        strcpy(p_match->stream_url,"");
    }
    
    cJSON *type = cJSON_GetObjectItemCaseSensitive(item, "Type");
    if (cJSON_IsNumber(type)) {
        p_match->type = (short) type->valueint;
        
    } else if (cJSON_IsString(type) && type->valuestring != NULL) {
        p_match->type = (short) atoi(type->valuestring);
        
    } else {
        p_match->type = (short) -1;
    }
    
    cJSON *visible = cJSON_GetObjectItemCaseSensitive(item, "Visible");
    if (cJSON_IsTrue(visible)) {
        p_match->visible = 1;
        
    } else if (cJSON_IsFalse(visible)) {
        p_match->visible = 0;
        
    } else {
        p_match->visible = -1;
    }
    
    cJSON *suspended = cJSON_GetObjectItemCaseSensitive(item, "Suspended");
    if (cJSON_IsTrue(suspended)) {
        p_match->suspended = 1;
        
    } else if (cJSON_IsFalse(suspended)) {
        p_match->suspended = 0;
        
    } else {
        p_match->suspended = -1;
    }
    
    cJSON *status = cJSON_GetObjectItemCaseSensitive(item, "Status");
    if (cJSON_IsNumber(status)) {
        p_match->status = (short) status->valueint;
        
    } else if (cJSON_IsString(status) && status->valuestring != NULL) {
        p_match->status = (short) atoi(status->valuestring);
        
    } else {
        p_match->status = -1;
    }
    
    cJSON *sport_id = cJSON_GetObjectItemCaseSensitive(item, "SportId");
    if (cJSON_IsNumber(sport_id)) {
        p_match->sport_id = (short) sport_id->valueint;
        
    } else if (cJSON_IsString(sport_id) && sport_id->valuestring != NULL) {
        p_match->sport_id = (short) atoi(sport_id->valuestring);
        
    } else {
        p_match->sport_id = -1;
    }
    
    cJSON *tournament_id = cJSON_GetObjectItemCaseSensitive(item, "TournamentId");
    if (cJSON_IsNumber(tournament_id)) {
        p_match->tournament_id = tournament_id->valueint;
        
    } else if (cJSON_IsString(tournament_id) && tournament_id->valuestring != NULL) {
        p_match->tournament_id = atoi(tournament_id->valuestring);
        
    } else {
        p_match->tournament_id = -1;
    }
    
    cJSON *home_team_id = cJSON_GetObjectItemCaseSensitive(item, "HomeTeamId");
    if (cJSON_IsNumber(home_team_id)) {
        p_match->home_team_id = home_team_id->valueint;
        
    } else if (cJSON_IsString(home_team_id) && home_team_id->valuestring != NULL) {
        p_match->home_team_id = atoi(home_team_id->valuestring);
        
    } else {
        p_match->home_team_id = -1;
    }
    
    cJSON *home_team_name = cJSON_GetObjectItemCaseSensitive(item, "HomeTeamName");
    if (cJSON_IsString(home_team_name) && home_team_name->valuestring != NULL) {
        strcpy(p_match->home_team_name, home_team_name->valuestring);
    } else {
        strcpy(p_match->home_team_name,"");
    }
    
    cJSON *away_team_id = cJSON_GetObjectItemCaseSensitive(item, "AwayTeamId");
    if (cJSON_IsNumber(away_team_id)) {
        p_match->away_team_id = away_team_id->valueint;
        
    } else if (cJSON_IsString(away_team_id) && away_team_id->valuestring != NULL) {
        p_match->away_team_id = atoi(away_team_id->valuestring);
        
    } else {
        p_match->away_team_id = -1;
    }
    
    cJSON *away_team_name = cJSON_GetObjectItemCaseSensitive(item, "AwayTeamName");
    if (cJSON_IsString(away_team_name) && away_team_name->valuestring != NULL) {
        strcpy(p_match->away_team_name, away_team_name->valuestring);
    } else {
        strcpy(p_match->away_team_name,"");
    }
    
    cJSON *outright_name = cJSON_GetObjectItemCaseSensitive(item, "OutrightName");
    if (cJSON_IsString(outright_name) && outright_name->valuestring != NULL) {
        strcpy(p_match->outright_name, outright_name->valuestring);
    } else {
        strcpy(p_match->outright_name,"");
    }
    
    cJSON *start_time = cJSON_GetObjectItemCaseSensitive(item, "StartTime");
    if (cJSON_IsString(start_time) && start_time->valuestring != NULL) {
        strcpy(p_match->start_time, start_time->valuestring);
    } else {
        strcpy(p_match->start_time,"");
    }
    
    cJSON *end_time = cJSON_GetObjectItemCaseSensitive(item, "EndTime");
    if (cJSON_IsString(end_time) && end_time->valuestring != NULL) {
        strcpy(p_match->end_time, end_time->valuestring);
    } else {
        strcpy(p_match->end_time,"");
    }
    
    return update_or_create_for_mysql(hm, p_match);
}


void save_match_to_db(himysql_t *hm, void *arg)
{
    char *buf = (char *)arg;
    cJSON *json = cJSON_Parse(buf);
    if (json == NULL) {
        goto end;
    }
    
    /* json数组 */
    if (cJSON_IsArray(json)) {
        int i,len;
        cJSON *item;
        i = 0;
        len = cJSON_GetArraySize(json);
        match_t matches[len];
        cJSON_ArrayForEach(item, json) {
            if (i == len) {
                goto end;
            }
            
            parse_match_json(hm, item, &matches[i]);
            i++;
        }
    } else  {
        match_t match;
        parse_match_json(hm, json, &match);
    }
end:
    cJSON_Delete(json);
}


void save_matchfull(listener_t *lis)
{
    char *buf;
    ssize_t read_num;
    
    read_num = read_file(MATCH_FULL_FILE,buf);
    
    if (read_num > 0) {
        save_match_to_db(lis->hm, (void *)buf); /* 保存到数据库 */
        
        /* 保存到内存数据库 */
        if (redis_set(lis->redis_conn, (void *)"matchfull", (void *)buf) == -1) {
            lis_log(LIS_LOG_ERR,"Error: redis_set error for key matchfull. in %s,line: %d\n",
                    __FILE__,__LINE__);
        }
        
        free(buf);
    }
}