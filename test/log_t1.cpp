#include "../log.h"
#include <stdlib.h>

int main()
{
    log_init(".",APP_TRACE, NULL);
    LOG_ERROR("%d\n", 123);
}
