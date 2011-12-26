
#include <time.h>



time_t time(time_t *tm)
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    return tm ? *tm = tv.tv_sec : tv.tv_sec;
}
