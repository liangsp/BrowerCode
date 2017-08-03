#include "Common.h"
#include <cstring>
#include <cctype>
#include <new> //! std::nothrow


static const char LINE_FEED             = (char)0x0a; //! 换行('\n')
static const char LF = LINE_FEED;
static const char CARRIAGE_RETURN       = (char)0x0d; //! 回车('\r')
static const char CR = CARRIAGE_RETURN;
static const char SPACES                = (char)0x20; //!< 空格(' ')
static const char SP = SPACES;
static const char HORIZONTAL_TEXT       = (char)0x09; //!< 横向制表('\t')
static const char HT = HORIZONTAL_TEXT;


char *Common::UpperString(char *pszString)
{
    if(NULL == pszString)
    {
        return NULL;
    }

    char *ptr = pszString;
    while(*ptr)
    {
        *ptr = toupper(*ptr);
        ptr++;
    }

    return pszString;
}


char *Common::LowerString(char *pszString)
{
    if(NULL == pszString)
    {
        return NULL;
    }
    
    char *ptr = pszString;
    while(*ptr)
    {
        *ptr = tolower(*ptr);
        ptr++;
    }

    return pszString;
}


char *Common::LeftTrim(char *pszString, int length)
{
    if(NULL == pszString || *pszString == '\0' || length == 0)
    {
        return NULL;
    }

    char *pTemp = new(std::nothrow) char[length + 1];
    if(NULL == pTemp)
    {
        return NULL;
    }

    memset(pTemp, 0x00, length + 1);
    memcpy(pTemp, pszString, length);

    char *start = pTemp;
    while(SP == (*start) || HT == (*start))
    {
        start ++;
    }

    memcpy(pszString, start, strlen(start));
    pszString[strlen(start)] = '\0';

    delete [] pTemp;

    return pszString;
}


char *Common::RightTrim(char *pszString, int length)
{
    if(NULL == pszString || *pszString == '\0' || length == 0)
    {
        return NULL;
    }

    char *end = NULL;
    end = pszString + length - 1;

    while(SP == *end || HT == *end)
    {
        *end = '\0';
        if(end == pszString)
        {
            break;
        }
        end--;
    }

    return pszString;
}


char *Common::Trim(char *pszString, int length)
{
    char *s = NULL;
    s = LeftTrim(pszString, length);
    s = RightTrim(pszString, strlen(s));

    return s;
}


char *Common::LeftTruncate(char *pszString, const char *pszDelim)
{
    if(NULL == pszString || NULL == pszDelim)
    {
        return NULL;
    }

    int index = 0;
    int len = strlen(pszString);
    for(index = 0; index < len; ++index)
    {
        //! 字符不在字符串pszDelim中, 则跳出
        if(!strchr(pszDelim, pszString[index]))
        {
            break;
        }
    }

    if(index < len)
    {
        memmove(pszString, pszString + index, len - index + 1);
    }

    return pszString;
}


char *Common::RightTruncate(char *pszString, const char *pszDelim)
{
    if(NULL == pszString || NULL == pszDelim)
    {
        return NULL;
    }
    
    int index = 0;
    int len = strlen(pszString);
    for(index = len - 1; index >= 0; --index)
    {
        //! 字符不在字符串pszDelim中, 则跳出
        if(!strchr(pszDelim, pszString[index]))
        {
            break;
        }
    }

    pszString[index+1] = '\0';

    return pszString;
}


char *Common::Truncate(char *pszString, const char *pszDelim)
{
    if(NULL == pszString || NULL == pszDelim)
    {
        return NULL;
    }

    (void)LeftTruncate(pszString, pszDelim);
    
    return RightTruncate(pszString, pszDelim);
}



