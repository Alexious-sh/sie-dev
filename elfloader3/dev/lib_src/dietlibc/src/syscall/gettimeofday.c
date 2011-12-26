
#include <time.h>
#include <sys/time.h>
#include <swilib.h>

char mon_days[] =
{
    31,
    28, /*(в високосном году Ч 29)*/
    31,
    30,
    31,
    30,
    31,
    31,
    30,
    31,
    30,
    31,
};



int GetDay(TDate *td)
{
    if(!td) return -1;
    int yday = 0;
    for (int i=0; i<td->month-1; ++i)
    {
        yday += mon_days[i];
    }

    return yday+td->day-1;
}


int gettimeofday (struct timeval *tv, struct timezone *tz)
{
    TTime tt;
    TDate td;

    GetDateTime(&td, &tt);

    if (NULL != tv)
    {
        struct tm t;
        t.tm_sec  = tt.sec;
        t.tm_min  = tt.min;
        t.tm_hour = tt.hour;
        t.tm_mday = td.day;
        t.tm_mon  = td.month-1;
        t.tm_year = td.year-1900;
        t.tm_wday = GetWeek(&td);
        t.tm_yday = GetDay(&td);
        t.tm_isdst = 0;
        t.tm_gmtoff = 10800;
        t.tm_zone = "EEST";

        tv->tv_sec = mktime(&t);
    }

    if (NULL != tz)
    {
        tz->tz_dsttime = 0;
        tz->tz_minuteswest = -180;
    }

    return 0;
}


int settimeofday (const struct timeval *tv, const struct timezone *tz)
{
    TTime tt;
    TDate td;

    if (NULL != tv)
    {
        struct tm t;
        tt.sec   = t.tm_sec;
        tt.min = t.tm_min;
        tt.hour  = t.tm_hour;
        td.day   = t.tm_mday;
        td.month = t.tm_mon+1;
        td.year  = t.tm_year+1900;

        SetDateTime(&td, &tt);
    }

    if (NULL != tz)
    {
        /* а шо тут делать? */
    }

    return 0;
}

