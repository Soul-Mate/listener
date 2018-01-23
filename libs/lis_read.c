#include "lis_core.h"
#include "lis_config.h"


ssize_t read_file(const char * file, void **buf)
{
    int fd;
    
    ssize_t read_num;
    
    struct stat stat_buf;
    
    fd = open(file, O_RDONLY);
    
    if (fd == -1) {
#ifdef DEBUG
        printf("Error: file: %s not found. in %s line: %d\n",
               file, __FILE__, __LINE__);
#endif
        return -1;
    }
    
    if (fstat(fd, &stat_buf) == -1) {
#ifdef DEBUG
        printf("Error: fstat. in %s line: %d\n",
               __FILE__, __LINE__);
#endif
        return-1;
    }
    
    (*buf) = malloc((size_t) (stat_buf.st_size));
    
    if ((*buf) == NULL) {
        return -1;
    }
    
    for(;;) {
        
        read_num = read(fd, (*buf), (size_t) stat_buf.st_size);
        
        if(read_num == -1) {
            
            if (errno == EINTR) {
                continue;
            }
            
        } else if(read_num == 0) {
            free((*buf));
            return 0;
            
        } else {
            break;
        }
    }
    return read_num;
}


char *get_file_value(const char *file)
{
    int fd;
    
    char *buf;
    
    ssize_t read_num;
    
    struct stat stat_buf;
    
    fd = open(file, O_RDONLY);
    
    if (fd == -1) {
#ifdef DEBUG
        printf("Error: file: %s not found. in %s line: %d\n",
               file, __FILE__, __LINE__);
#endif
        return NULL;
    }
    
    if (fstat(fd, &stat_buf) == -1) {
#ifdef DEBUG
        printf("Error: fstat. in %s line: %d\n",
               __FILE__, __LINE__);
#endif
        return NULL;
    }
    
    buf = malloc((size_t) (stat_buf.st_size));
    
    if (buf == NULL) {
        return NULL;
    }
    
    for(;;) {
        
        read_num = read(fd, buf, (size_t) stat_buf.st_size);
        
        if(read_num == -1) {
            
            if (errno == EINTR) {
                continue;
            }
            
        } else if(read_num == 0) {
            free(buf);
            return 0;
            
        } else {
            break;
        }
    }
    return buf;
}

ssize_t fread_file(int fd, void **buf)
{
    ssize_t read_num;
    
    struct stat stat_buf;
    
    (*buf) = (char *)malloc((size_t) (stat_buf.st_size + 1));
    
    if ((*buf) == NULL) {
        return -1;
    }
    
    for(;;) {
        
        read_num = read(fd, (*buf), (size_t) stat_buf.st_size);
        
        if(read_num == -1) {
            
            if (errno == EINTR) {
                continue;
            }
            
        } else if(read_num == 0) {
            free((*buf));
            return 0;
            
        } else {
            break;
        }
    }
    return read_num;
}