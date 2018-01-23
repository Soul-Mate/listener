#include "lis_core.h"
#include "lis_config.h"

static int lock_file(int);

static int lock_file(int fd)
{
    struct flock f1;
    f1.l_type = F_WRLCK;
    f1.l_start = 0;
    f1.l_whence = SEEK_SET;
    f1.l_len = 0;
    return fcntl(fd,F_SETLK,&f1);
}


int daemonize()
{
    int pid;
    
    char buf[16];
    
    int fd,fd_0,fd_1,fd_2;
    
    struct sigaction sa;
    
    umask(0);
    
    if ((pid = fork()) < 0) {
        lis_log(LIS_LOG_ERR,"Error: daemon fork error. file: %s, line: %d\n",
             __FILE__, __LINE__);
        exit(1);
        
    } else if(pid != 0) {
        exit(0);
    }
    
    
    setsid();
    
    sigemptyset(&sa.sa_mask);
    
    sa.sa_handler = SIG_IGN;
    
    if (sigaction(1,&sa,NULL) < 0) {
        lis_log(LIS_LOG_ERR,"Error: daemon sigaction error. file: %s, line: %d\n",
             __FILE__, __LINE__);
        exit(1);
    }
    
    /* 这里进行第二次fork是为了让守护进程不是会话首进程.防止他获取终端 */
    if ((pid = fork()) < 0) {
        lis_log(LIS_LOG_ERR,"Error: daemon fork error. file: %s, line: %d\n",
             __FILE__, __LINE__);
        exit(1);
    
    } else if(pid != 0) {
        exit(0);
    }
    
    if (chdir("/") < 0) {
        lis_log(LIS_LOG_ERR,"Error: daemon chdir error. file: %s, line: %d\n",
             __FILE__, __LINE__);
        exit(1);
    }
    
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    close(STDIN_FILENO);

    fd_0 = open("/dev/null",O_RDWR);
    fd_1 = dup(0);
    fd_2 = dup(0);

    if (fd_0 != 0 || fd_1 != 1 || fd_2 != 2) {
        syslog(LOG_ERR,"unexpected file descriptors %d %d %d",fd_0,fd_1,fd_2);
        exit(1);
    }
    
    /* 单实例守护进程 */
    fd = open(LOCK_FILE,O_RDWR|O_CREAT,LOCK_MODE);

    if (fd < 0) {
        syslog(LOG_ERR,"can't open %s: %s",LOCK_FILE,strerror(errno));
        exit(1);
    }
    
    if (lock_file(fd) < 0) {
        if (errno == EACCES || errno == EAGAIN) {
            close(fd);
            return 0;
        }
        syslog(LOG_ERR,"can't lock %s: %s",LOCK_FILE,strerror(errno));
        exit(1);
    }
    
    ftruncate(fd,0);
    sprintf(buf,"%ld",(long)getpid());
    write(fd,buf,strlen(buf)+1);
    return 0;
}
