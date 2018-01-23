//
// Created by xyc on 18-1-23.
//

#include "lis_core.h"
#include "lis_config.h"


int main(int argc ,const char *argv[])
{
    char *buf = NULL;
    ssize_t read_num;
    
    if (argc != 2 || strcmp(argv[1],"--help") == 0) {
        printf("input file path\n");
        exit(0);
    }
    
    read_num = read_file(argv[1],(void *)&buf);
    printf("read num:%ld\n",read_num);
    printf("read: \n%s\n",buf);
    free(buf);
    return 0;
}