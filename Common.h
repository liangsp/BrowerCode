/*!
 * @file      Common.h
 * @brief     常用功能小程序类
 *
 * Copyright (c) 2015 chinasoft Tech.Co.,Ltd
 *
 * @author    lsp74391
 * @date      2015/6/1
 * @version   1.0.0
 *
*******************************************************************************
 */
#ifndef __COMMON_H__
#define __COMMON_H__

class Common
{
public:
    static char *UpperString(char *pszString);
    static char *LowerString(char *pszString);
    static char *LeftTrim(char *pszString, int length);
    static char *RightTrim(char *pszString, int length);
    static char *Trim(char *pszString, int length);
    static char *LeftTruncate(char *pszString, const char *pszDelim);
    static char *RightTruncate(char *pszString, const char *pszDelim);
    static char *Truncate(char *pszString, const char *pszDelim);
};

#endif//!__COMMON_H__
