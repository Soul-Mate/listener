//
// Created by xyc on 18-1-23.
//

#ifndef LIS_MARKET_H
#define LIS_MARKET_H

#define INSERT_MARKET_SQL \
"INSERT INTO `radar_markets` (`number`,`name`,`match_id`,`suspended`,\
`status`,`is_live`,`visible`,`odds`) VALUES(%d,%s,%d,%d,%d,%d,%d,%s)"

#define UPDATE_MARKET_SQL \
"UPDATE `radar_markets` set `name` = %s, `match_id` = %d, `suspended` = %d,\
`status` = %d, `is_live` = %d, `visible` = %d, `odds` = %s where `number` = %d"

typedef struct _radar_market_t {
    int number;
    char *name;
    int match_id;
    short suspended;
    short status;
    short is_live;
    short visible;
    char *odds;
}market_t;

void save_market_to_db(himysql_t *, void*);

void save_marketfull(listener_t *);

#endif /* LIS_MARKET_H */
