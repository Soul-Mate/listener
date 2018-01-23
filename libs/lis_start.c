#include "lis_core.h"
#include "lis_config.h"

static int create_log_dir();

static int create_log_dir()
{
    int status;
    status = mkdir(LIS_LOG_PATH, 0755);
    return status == -1? errno == EEXIST? 0 : -1 : -1;
}


void start_listener(listener_t *lis)
{
    if (on_exit(exit_cleanup, lis) != 0){
#ifdef DEBUG
        printf("Error: on_exit for exit_cleanup\n");
#endif
        free(lis);
        exit(1);
    }
    
    if (create_log_dir() == -1) {
#ifdef DEBUG
        printf("Error: create_log_dir\n");
#endif
        exit(1);
    }
    
    save_league(lis);
    
    // save_leaguesfull(lis);
    
    // save_matchfull(lis);
    
    // save_marketfull(lis);
    
    
    if (set_cache_interval(lis) != 0) {
#ifdef DEBUG
        printf("Error: set_cache_interval\n");
#endif
        exit(1);
    }

    // daemonize();
    
    start_inotify(lis);
}