#include "lis_core.h"
#include "lis_config.h"


int main(int argc, char *argv[])
{
	/* 非root用户 */
//	if (getuid() != 0) {
//		printf("Error: Permission denied\n");
//		exit(0);
//	}
    
    strcpy(argv[0],"listenerd");
    
	listener_t *listener = new_listener();
	
	if (listener == NULL) {
		exit(0);
	}
 
	start_listener(listener);
	
    return 0;
}
