//
// Created by xyc on 18-1-23.
//

#ifndef LIS_CORE_H
#define LIS_CORE_H

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stddef.h>
#include <stdarg.h>
#include <time.h>

#include <regex.h>
#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/epoll.h>
#include <sys/inotify.h>

#include <hiredis.h>
#include "cJSON.h"
#include "himysql.h"
#include "lis.h"
#include "lis_start.h"
#include "lis_log.h"
#include "lis_exit.h"
#include "lis_read.h"
#include "lis_match.h"
#include "lis_cache.h"
#include "lis_league.h"
#include "lis_market.h"
#include "lis_notify.h"
#include "lis_interval.h"
#include "lis_daemon.h"

#define lis_int int
#define lis_int64 long int
#define lis_u_int unsigned int
#define lis_u_int64 unsigned long int
#define lis_str char *

#endif /* LIS_CORE_H */
