//
// Created by xyc on 18-1-23.
//

#ifndef LIS_LEAGUE_H
#define LIS_LEAGUE_H

#define INSERT_LEAGUE_SQL \
"INSERT INTO radar_leagues (`number`, `name`, `sport_id`) \
VALUES(%d,%s,%d)"

#define UPDATE_LEAGUE_SQL \
"UPDATE `radar_leagues` set `name` = %s, `sport_id` = %d where `number` = %d"

#define INSERT_LEAGUE_TOURNAMENT_SQL \
"INSERT INTO radar_league_tournaments (`number`,`name`,`league_number`) \
VALUES(%d,%s,%d)"

#define UPDATE_LEAGUE_TOURNAMENT_SQL \
"UPDATE `radar_league_tournaments` set `name` = %s,`league_number` = %d where `number` = %d"

typedef struct _radar_league_tournament_table {
    int number;
    char name[256];
    int league_number;
}league_tournament_t; /* 联赛:比赛表 */

typedef struct _radar_league_table {
    int number;
    char name[256];
    int sport_id;
    int league_tournament_size;
    league_tournament_t **league_tournament;
}league_t; /* 联赛表 */

void save_league_to_db(himysql_t *, void *);

void save_leaguesfull(listener_t *);

void save_league(listener_t *);

#endif /* LIS_LEAGUE_H */
