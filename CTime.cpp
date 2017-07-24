#include "CTime.h"
#include <cstdio>      ///< snprintf
#include <cstdlib>     ///< atoi
#include <cstring>     ///< strncpy
#include <time.h>      ///< time_t   struct tm
#include <sys/time.h>  ///< gettimeofday

static struct timeval g_previousTv;

static struct tm *__getTmPtr()
{
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    return timeinfo;
}

int CTime::format(const char *fmt, char *time, int length)
{
    struct tm *timeinfo = 0;
    timeinfo = __getTmPtr();

    return (int)strftime(time, length, fmt, timeinfo);
}

char *CTime::GetCurrentTime(char *szCurDate, int length)
{
    struct tm *timeinfo = 0;
    timeinfo = __getTmPtr();

    /// º¯ÊýÔ­ÐÍ
    /// size_t strftime (char* ptr, size_t maxsize, const char* format, const struct tm* timeptr );
    strftime(szCurDate, length, "%Y%m%d%H%M%S", timeinfo);

    return szCurDate;
}

char *CTime::GetCurTimeMicSecond(char *szCurDate, int length)
{
    time_t rawtime;
    struct tm *timeinfo;
    struct timeval tvNow;

    gettimeofday(&tvNow, 0);

    rawtime = tvNow.tv_sec;
    timeinfo = localtime(&rawtime);

    snprintf(szCurDate, length, "%04d%02d%02d %02d:%02d:%02d.%06ld", \
             timeinfo->tm_year + 1900, \
             timeinfo->tm_mon + 1, \
             timeinfo->tm_mday, \
             timeinfo->tm_hour, \
             timeinfo->tm_min, \
             timeinfo->tm_sec, \
             tvNow.tv_usec);

    return szCurDate;
}

int CTime::GetMonth()
{
    struct tm *timeinfo = 0;
    timeinfo = __getTmPtr();

    return timeinfo->tm_mon + 1;
}

int CTime::GetMonth(char *szDate)
{
    char buffer[3];

    strncpy(buffer, szDate + 4, 2);
    buffer[2] = '\0';

    return atoi(buffer);
}

int CTime::GetDay()
{
    struct tm *timeinfo = 0;
    timeinfo = __getTmPtr();

    return timeinfo->tm_mday;
}

int CTime::GetDay(char *szDate)
{
    char buffer[3];

    strncpy(buffer, szDate + 6, 2);
    buffer[2] = '\0';

    return atoi(buffer);
}

int CTime::GetWeekDay()
{
    struct tm *timeinfo = 0;
    timeinfo = __getTmPtr();

    char buffer[8] = "";
    strftime(buffer, sizeof(buffer), "%u", timeinfo);

    return atoi(buffer);
}

bool CTime::ConvertStringToTm(const char *pszTimeFmt, struct tm *ptm)
{
    char szBuffer[7];
    struct tm tmStaTime;
    time_t lt;

    /* Get year */
    memcpy(szBuffer, pszTimeFmt, 4);
    szBuffer[4] = 0;
    tmStaTime.tm_year = atoi(szBuffer) - 1900;

    /* Get month */
    memcpy(szBuffer, pszTimeFmt + 4, 2);
    szBuffer[2] = 0;
    tmStaTime.tm_mon = atoi(szBuffer) - 1;

    /* Get day */
    memcpy(szBuffer, pszTimeFmt + 6, 2);
    szBuffer[2] = 0;
    tmStaTime.tm_mday = atoi(szBuffer);

    /* Get hour */
    memcpy(szBuffer, pszTimeFmt + 8, 2);
    szBuffer[2] = 0;
    tmStaTime.tm_hour = atoi(szBuffer);

    /* Get minute */
    memcpy(szBuffer, pszTimeFmt + 10, 2);
    szBuffer[2] = 0;
    tmStaTime.tm_min = atoi(szBuffer);

    /* Get second */
    memcpy(szBuffer, pszTimeFmt + 12, 2);
    szBuffer[2] = 0;
    tmStaTime.tm_sec = atoi(szBuffer);

    tmStaTime.tm_isdst = -1; /* daylight savings time */

    lt = mktime(&tmStaTime);

    if(lt == -1)
    {
        return false;
    }

#ifdef WIN32
    tmStaTime = *localtime(&lt);
#else
    localtime_r(&lt, &tmStaTime);
#endif

    *ptm = tmStaTime;

    return true;
}

double CTime::ComputeDuration(const char *pszBegTime, const char *pszEndTime)
{
    struct tm cBegTm;
    struct tm cEndTm;

    ConvertStringToTm(pszBegTime, &cBegTm);
    ConvertStringToTm(pszEndTime, &cEndTm);

    //return mktime(&cEndTm) - mktime(&cBegTm);
    return difftime(mktime(&cEndTm), mktime(&cBegTm));
}


void CTime::BeginTime()
{
    gettimeofday(&g_previousTv, 0);
}

long CTime::EndTime()
{
    struct timeval nowtv;
    gettimeofday(&nowtv, 0);

    return (long)(1000 * (nowtv.tv_sec - g_previousTv.tv_sec) + (nowtv.tv_usec - g_previousTv.tv_usec) / 1000);
}
