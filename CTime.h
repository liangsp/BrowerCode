#ifndef __CTIME_INC__
#define __CTIME_INC__


class CTime
{
public:
    /*
    ** @brief GetCurrentTime  取当前时间, 14位日期 YYYYMMDDHH24MISS
    **
    ** @param fmt  eg: %Y%m%d%H%M%S
    ** @param time
    ** @param length
    **
    ** @return ret code
    **
    ** @see strftime <ctime>
    ** http://www.cplusplus.com/reference/ctime/strftime/
    **
    */
    static int format(const char *fmt, char *time, int length);

    /*
    ** @brief GetCurrentTime  取当前时间, 14位日期 YYYYMMDDHH24MISS
    **
    ** @param szCurDate
    ** @param length
    **
    ** @return 当前时间
    **
    */
    static char *GetCurrentTime(char *szCurDate, int length);

    /*
    ** @brief GetCurrentTime  取当前时间, YYYYMMDD HH24:MI:SS:123456到微秒级
    **
    ** @param szCurDate
    ** @param length
    **
    ** @return 当前时间
    **
    */
    static char *GetCurTimeMicSecond(char *szCurDate, int length);

    /*
    ** @brief GetMonth  取当前月份
    **
    ** @return 当前月份
    **
    */
    static int GetMonth();

    /*
    ** @brief GetMonth  取日期字符串中月份
    **
    ** @param szCurDate
    **
    ** @return 当前月份
    **
    */
    static int GetMonth(char *szDate);

    /*
    ** @brief GetMonth  取当前日期
    **
    ** @return 当前day
    **
    */
    static int GetDay();

    /*
    ** @brief GetMonth  取日期字符串中天
    **
    ** @param szDate
    **
    ** @return 当前日期
    **
    */
    static int GetDay(char *szDate);

    /*
    ** @brief GetWeekDay  取当前日期的星期(1-7)
    **
    ** @return 当前日期的星期
    **
    */
    static int GetWeekDay();

    static bool ConvertStringToTm(const char *pszTimeFmt, struct tm *ptm);

    static double ComputeDuration(const char *pszBegTime, const char *pszEndTime);

    /*
    ** @brief BeginTime  开始计时
    **
    */
    static void BeginTime();

    /*
    ** @brief EndTime  计时结束
    **
    ** @return 返回毫秒
    */
    static long EndTime();
};

#endif//__CTIME_INC__
