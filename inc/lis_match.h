//
// Created by xyc on 18-1-23.
//

#ifndef LIS_MATCH_H
#define LIS_MATCH_H

#define FIND_MATCH_SQL "SELECT number from `radar_matches` where number = %d"

#define INSERT_MATCH_SQL \
"INSERT INTO radar_matches (`number`,`score`,`stream_url`,`type`,`visible`,\
`suspended`,`status`,`sport_id`,`tournament_id`,`home_team_id`,`home_team_name`,\
`away_team_id`,`away_team_name`,`outright_name`,`start_time`,`end_time`)\
VALUES(%d,%s,%s,%d,%d,%d,%d,%d,%d,%d,%s,%d,%s,%s,%s,%s)"

#define UPDATE_MATCH_SQL \
"UPDATE radar_matches set score = %s, stream_url = %s, \
type = %d, visible = %d,suspended = %d, status = %d, \
sport_id = %d, tournament_id = %d, home_team_id = %d, \
home_team_name = %s, away_team_id = %d, away_team_name = %s, \
outright_name = %s, start_time = %s, end_time = %s where number = %d"

/* 保存match数据的宏 */
#define SAVE_MATCH_MYSQL(match, ptr_conn)                         \
	do                                                            \
	{                                                             \
		int ret;                                                  \
		match_t *ptr_m = match;                                   \
		ret = himysql(ptr_conn, FIND_ONE_SQL, ptr_m->number);     \
		himysql_fetch_row(ptr_conn);                              \
		if (!ptr_conn->row)                                       \
		{                                                         \
			ret = himysql(ptr_conn, INSERT_SQL,                   \
						  ptr_m->number,                          \
						  ptr_m->score,                           \
						  ptr_m->stream_url,                      \
						  ptr_m->type,                            \
						  ptr_m->visible,                         \
						  ptr_m->suspended,                       \
						  ptr_m->status,                          \
						  ptr_m->sport_id,                        \
						  ptr_m->tournament_id,                   \
						  ptr_m->home_team_id,                    \
						  ptr_m->home_team_name,                  \
						  ptr_m->away_team_id,                    \
						  ptr_m->away_team_name,                  \
						  ptr_m->outright_name,                   \
						  ptr_m->start_time,                      \
						  ptr_m->end_time);                       \
			if (ret != HM_OK)                                     \
			{                                                     \
				wlog(LOG_ERROR, "%s\n", himysql_error(ptr_conn)); \
			}                                                     \
		}                                                         \
		else                                                      \
		{                                                         \
			ret = himysql(ptr_conn, UPDATE_SQL,                   \
						  ptr_m->score,                           \
						  ptr_m->stream_url,                      \
						  ptr_m->type,                            \
						  ptr_m->visible,                         \
						  ptr_m->suspended,                       \
						  ptr_m->status,                          \
						  ptr_m->sport_id,                        \
						  ptr_m->tournament_id,                   \
						  ptr_m->home_team_id,                    \
						  ptr_m->home_team_name,                  \
						  ptr_m->away_team_id,                    \
						  ptr_m->away_team_name,                  \
						  ptr_m->outright_name,                   \
						  ptr_m->start_time,                      \
						  ptr_m->end_time,                        \
						  ptr_m->number);                         \
			if (ret != HM_OK)                                     \
			{                                                     \
				wlog(LOG_ERROR, "%s\n", himysql_error(ptr_conn)); \
			}                                                     \
		}                                                         \
	} while (0);

typedef struct _radar_matches {
    int number;
    char score[16];
    char stream_url[256];
    short type;
    short visible;
    short suspended;
    short status;
    short sport_id;
    int tournament_id;
    int home_team_id;
    char home_team_name[80];
    int away_team_id;
    char away_team_name[80];
    char outright_name[80];
    char start_time[32];
    char end_time[32];
} match_t;

void save_match_to_db(himysql_t *, void *);

void save_matchfull(listener_t *);

#endif /* LIS_MATCH_H */
