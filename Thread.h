/*!
 * @file    Thread.h
 * @brief   线程封装
 *
 * Copyright (c) 2015 chinasoft Tech.Co.,Ltd
 *
 * @author	  lsp74391 
 * @date      2017/4/28
 * @version   1.0.0
 *
*******************************************************************************
 */
#ifndef __THREAD_INC__
#define __THREAD_INC__

#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>
#include <pthread.h>
#include "ErrMsg.h"


class CThread
{
public:
    CThread()
    {
        m_nFlags = 0;
        m_lThreadId = 0;
        m_nStatus = PS_UNSTART;
        SetThreadName("DEFAULT THREAD");
    }

    CThread(const char * pszName)
    {
        m_nFlags = 0;
        m_lThreadId = 0;
        m_nStatus = PS_UNSTART;
        SetThreadName(pszName);
    }
    
    
    virtual ~CThread()
    {
        printf("pthread_self():%lu, m_lThreadId:%lu\n", pthread_self(), m_lThreadId);
        if (IsRunning() && !(m_nFlags & THR_CONSIGND))
        {
            printf("Stop()\n");
            Stop();
        }

        printf("~CThread()\n");
    }

    void SetThreadName(const char * pszName)
    {
        (void)snprintf(m_szThreadName, sizeof(m_szThreadName), "%s", pszName);
    }

    const char* GetThreadName() const
    {
        return &m_szThreadName[0];
    }

    pthread_t GetPid() const
    {
        return m_lThreadId;
    }

    bool IsRunning()
    {
        return (m_nStatus != PS_UNSTART);
    }

    /**
     * @brief CThread错误信息提供函数
     *
     * 提供CThread错误信息描述
     *
     * @param void
     * @return const char *
     * @retval 非NULL 最近一次发生错误操作的错误信息
     */
    const char *GetLastErrMsg()
    {
        return m_cErrmsg.GetLastErrMsg();
    }

    // Creates a new thread using the specified operation
    virtual int Start(void *(func)(void *args), \
                      void *args = NULL, \
                      int flags = CThread::THR_JOINABLE, \
                      int stacksize = -1)
    {
        flags = flags | THR_CONSIGND;
        if (Activate(func, args, flags, stacksize) != 0)
            return -1;
        return 0;
    }

    virtual int Start(int flags = THR_JOINABLE, int stacksize = -1)
    {
        flags = flags & ~THR_CONSIGND;
        if (Activate(CThread::RunAs, (void *)this, flags, stacksize) != 0)
            return -1;
        return 0;
    }

    // Terminates this thread if it is running
    virtual void Stop()
    {
        
        if (!IsRunning())
            return;
        
        if (pthread_self() == m_lThreadId)
        {
            m_cErrmsg.SetLastErrMsg("CThread::Stop() Error:Cann't stop thread-self.\n");
            return;
        }

        //int32_t ret = pthread_kill(m_Tid, SIGSTOP); //SIGKILL);
        int ret = pthread_cancel(m_lThreadId);
        if (0 == ret)
        {
            m_nStatus = PS_UNSTART;
        }
        m_cErrmsg.SetLastErrMsg("CThread::Stop() Error:%s.\n", strerror(ret));
        return;
    }

    /**
     * @brief CThread join线程函数(可以继承实现)
     *
     * CThread等待线程函数(可以继承实现)
     *
     * @return int
     * @retval 0 操作成功
     * @retval 非0 操作失败
     */
    /**
     * Blocks until this thread exits.
     * @return 0 Success
     */
    virtual int Join()
    {
        if (!(m_nFlags & THR_JOINABLE))
        {
            return -1;
        }
        else
        {
            return pthread_join(m_lThreadId, NULL);
        }
    }

    /**
     * @brief CThread提供睡眠功能函数
     *
     * CThread提供睡眠功能函数
     *
     * @param msec [in] 指定睡眠时间
     * @return void
     */
    static void Sleep(unsigned long msec)
    {
#ifndef WIN32
        fd_set fdsck;
        FD_ZERO(&fdsck);

        struct timeval timeOut;
        timeOut.tv_sec = msec / 1000;
        timeOut.tv_usec = (msec % 1000) * 1000;
        select(0, &fdsck, &fdsck, &fdsck, &timeOut);
#else
        ::Sleep(msec);
#endif //WIN32
    }

protected:
    /////////////////////////////////////////////////////////////////////////////
    //TODO: Overrides this method to add in user-special missions.
    /////////////////////////////////////////////////////////////////////////////
    /*!
    ** @name      Run 
    ** @brief     CThread运行线程函数(可继承实现)
    ** @param     void
    ** @return    void
    */
    virtual void Run()
    {
        
    }

private:
    /**
     * @brief CThread指定参数创建新线程函数
     *
     * CThread指定参数创建新线程函数
     *
     * @param func [in] 新线程运行的线程体
     * @param args [in] 新线程线程体传入参数
     * @param flags [in] 新线程创建的标志
     * @param size [in] 新线程创建指定分配的堆栈尺寸
     * @return void
     */
    int Activate(void *(func)(void *), void *args, int flags, int size)
    {
        if (IsRunning())
        {
            m_cErrmsg.SetLastErrMsg("CThread::Activate() Error:Thread is running already.\n");
            return -1;
        }

        pthread_attr_t attr;
        pthread_attr_init(&attr);
        if (size > 0)
            pthread_attr_setstacksize(&attr, size*1024*1024); 
        else
            pthread_attr_setstacksize(&attr, 10*1024*1024); //默认10M
        
        m_nFlags = flags;
        m_nStatus = PS_RUNNING;
        if (pthread_create(&m_lThreadId, &attr, func, args) != 0)
        {
            m_cErrmsg.SetLastErrMsg(strerror(errno));
            return -1;
        }

        pthread_attr_destroy(&attr);

        // Detach it from current thread to avoid memory leak
        if (!(m_nFlags & THR_JOINABLE))
        {
            pthread_detach(m_lThreadId);
        }
        return 0;
    }

    /**
     * @brief CThread线程体函数
     *
     * CThread线程体函数
     *
     * @param args [in] 线程体传入参数
     * @return void *
     */
    /////////////////////////////////////////////
    //The static method is used while creating a new thread.
    static void *RunAs(void *args)
    {
        CThread *thread = (CThread *)args;
        
        // Execute the operation overrided by subclasses
        try
        {
            thread->Run();
        }
        catch (...)
        {
            //printf("%s[%ld] captured an unknown exception and exit!\n", thread->GetName(), thread->GetPid());
            printf("thread captured an unknown exception and exit!\n");

            //在linux如果不重新抛出异常会coredump
            throw;
        }
        
        thread->m_nStatus = PS_UNSTART;
        
        return NULL;
    }
    

public:
    enum
    { 
        LEN_THREAD_NAME = 0xFF,  //!< 长度枚举

        //!< @brief: Status definition
        PS_UNSTART  = 0,
        PS_STOP     = 0,
        PS_RUNNING  = 1,
        PS_IDLE     = 2,
        PS_BUSY     = 3,
        PS_SUSPEND  = 4,

        //!< @brief: Flags Definition
        THR_JOINABLE = 1,
        THR_NEW_LWP  = 2,
        THR_CONSIGND = 8,
    };

private:
    int         m_nFlags;                           //!< 线程对象标志成员变量
    int         m_nStatus;                          //!< 线程对象状态成员变量
    pthread_t   m_lThreadId;                        //!< 线程对象Id成员变量
    char        m_szThreadName[LEN_THREAD_NAME];    //!< 线程对象名称成员变量
    CErrMsg     m_cErrmsg;                          //!< 错误消息输出成员变量
};

#endif//__THREAD_INC__

