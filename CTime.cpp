#include "CTime.h"
#include <cstdio>      ///< snprintf
#include <cstdlib>     ///< atoi
#include <cstring>     ///< strncpy
#include <time.h>      ///< time_t   struct tm
#include <sys/time.h>  ///< gettimeofday

static struct timeval g_previousTv;

static struct tm * __getTmPtr()
{
	time_t rawtime;
	struct tm *timeinfo;
	
	time(&rawtime);
	timeinfo = localtime (&rawtime);
	
	return timeinfo;
}

int CTime::format(const char* fmt, char* time, int length)
{
	struct tm *timeinfo = 0;
	timeinfo = __getTmPtr();
	
	return (int)strftime(time, length, fmt, timeinfo);
}

char* CTime::GetCurrentTime(char *szCurDate, int length)
{
	struct tm *timeinfo = 0;
	timeinfo = __getTmPtr();
	
	/// º¯ÊýÔ­ÐÍ
	/// size_t strftime (char* ptr, size_t maxsize, const char* format, const struct tm* timeptr );
	strftime(szCurDate, length, "%Y%m%d%H%M%S", timeinfo);
	
	return szCurDate;
}

char* CTime::GetCurTimeMicSecond(char *szCurDate, int length)
{
	time_t rawtime;
	struct tm *timeinfo;
	struct timeval tvNow;
	
	gettimeofday(&tvNow, 0);
	
	rawtime = tvNow.tv_sec;
	timeinfo = localtime (&rawtime);
	
	snprintf(szCurDate, length, "%04d%02d%02d %02d:%02d:%02d.%06ld", \
	          timeinfo->tm_year+1900, \
	          timeinfo->tm_mon+1, \
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
	
	strncpy(buffer, szDate+4, 2);
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
	
	strncpy(buffer, szDate+6, 2);
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

void CTime::BeginTime()
{
	gettimeofday(&g_previousTv, 0);
}

long CTime::EndTime()
{
	struct timeval nowtv;
	gettimeofday(&nowtv, 0);
	
	return (long)(1000*(nowtv.tv_sec-g_previousTv.tv_sec)+(nowtv.tv_usec-g_previousTv.tv_usec)/1000);
}
