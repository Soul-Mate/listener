#include "lis_core.h"
#include "lis_config.h"

int main(int argc ,const char *argv[])
{
    listener_t *listener = new_listener();
    save_marketfull(listener);
    return 0;
}
