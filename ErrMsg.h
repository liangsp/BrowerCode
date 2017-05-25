/*!
 * @file    ErrMsg.h
 * @brief   错误信息设置
 *   GetLastErrMsg: 获取最后设置的错误信息
 *   SetLastErrMsg: 设置最最后错误信息
 *
 * Copyright (c) 2015 chinasoft Tech.Co.,Ltd
 *
 * @author	  lsp74391 
 * @date      2017/4/26
 * @version   1.0.0
 *
*******************************************************************************
 */
#ifndef __ERR_MSG_INC__
#define __ERR_MSG_INC__

#include <cstdio>  //!<  vsnprintf 
#include <cstdarg> //!<  va_start  va_arg  va_end


class CErrMsg
{
public:
    CErrMsg() {}
    virtual ~CErrMsg() {}
    
    const char* GetLastErrMsg() { return &szErrMsg[0]; }
    
    void SetLastErrMsg(const char *pszErrormsg, ...)
    {
        if(pszErrormsg)
        {
            va_list ap;
            va_start(ap, pszErrormsg);
            vsnprintf(szErrMsg, sizeof(szErrMsg), pszErrormsg, ap);
            va_end(ap);
        }
    }
    
private:
    enum{MAX_ERR_MSG_SIZE = 1024};
    char szErrMsg[MAX_ERR_MSG_SIZE];
};

#endif//__ERR_MSG_INC__