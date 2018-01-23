#include "lis_core.h"
#include "lis_config.h"

int lis_log(int level,const char *format, ...)
{
    FILE *f;
    
    CREATE_LIS_LOG_FILE(level,f);
    
    if (f == NULL) {
        return -1;
    }
    
    va_list arg;
    va_start(arg,format);
    time_t t = time(NULL);
    struct tm* tm_ptr = localtime(&t);
    fprintf(f, "%04d-%02d-%02d %02d:%02d:%02d ",
            tm_ptr->tm_year + 1900, tm_ptr->tm_mon + 1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
    vfprintf(f, format, arg);
    va_end(arg);
    fflush(f);
    fclose(f);
    return 0;
}