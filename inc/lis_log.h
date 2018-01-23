
#ifndef LIS_LOG_H
#define LIS_LOG_H

#include "lis_core.h"


#ifndef LIS_LOG_ERR
#define LIS_LOG_ERR 0
#endif /* LIS_LOG_ERR */

#ifndef LIS_LOG_RUN
#define LIS_LOG_RUN 1
#endif /* LIS_LOG_RUN */


#ifndef LIS_LOG_EXT /* 日志文件扩展名称 */
#define LIS_LOG_EXT ".log"
#endif /* LIS_LOG_EXT */


#ifndef LIS_LOG_RUN_PREFIX /* 运行时日志名称前缀 */
#define LIS_LOG_RUN_PREFIX "listener-run"
#endif /* LIS_LOG_RUN_PREFIX */


#ifndef LIS_LOG_ERR_PREFIX /* 错误日志名称前缀 */
#define LIS_LOG_ERR_PREFIX "listener-error"
#endif /* LIS_LOG_ERR_PREFIX */

/* 日志路径 */
#ifndef LIS_LOG_PATH
#define LIS_LOG_PATH "/var/log/caike/"
#endif /* LIS_LOG_PATH */

#ifndef CREATE_LIS_LOG_FILE /* 创建日志文件 */
#define CREATE_LIS_LOG_FILE(__LEVEL__,__F__)do {\
    size_t len;\
	size_t ext_len;\
    lis_str prefix;\
    struct tm *ptr_tm;\
    ext_len = strlen(LIS_LOG_EXT);\
	switch(__LEVEL__) {\
		case LIS_LOG_ERR:\
			len = strlen(LIS_LOG_ERR_PREFIX) + ext_len;\
			prefix = LIS_LOG_ERR_PREFIX;\
			break;\
		default:\
			len = strlen(LIS_LOG_RUN_PREFIX) + ext_len;\
			prefix = LIS_LOG_RUN_PREFIX;\
	}\
	char file[len + 40];\
	time_t t = time(NULL);\
	ptr_tm = gmtime(&t);\
	sprintf(file,"%s%s%d%d%d%s",\
		LIS_LOG_PATH,prefix,\
		ptr_tm->tm_year + 1900, ptr_tm->tm_mon + 1, ptr_tm->tm_mday,LIS_LOG_EXT);\
    FILE *fp = fopen(file,"a+");\
	__F__ = fp;\
}while(0);
#endif /* CREATE_LIS_LOG_FILE */

int lis_log(int, const char *, ...);

int lis_err_log(const char *,...);

int lis_run_log(const char *,...);

#endif /* LIS_LOG_H */
