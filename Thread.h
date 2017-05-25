/*!
 * @file    Thread.h
 * @brief   �̷߳�װ
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
     * @brief CThread������Ϣ�ṩ����
     *
     * �ṩCThread������Ϣ����
     *
     * @param void
     * @return const char *
     * @retval ��NULL ���һ�η�����������Ĵ�����Ϣ
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
     * @brief CThread join�̺߳���(���Լ̳�ʵ��)
     *
     * CThread�ȴ��̺߳���(���Լ̳�ʵ��)
     *
     * @return int
     * @retval 0 �����ɹ�
     * @retval ��0 ����ʧ��
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
     * @brief CThread�ṩ˯�߹��ܺ���
     *
     * CThread�ṩ˯�߹��ܺ���
     *
     * @param msec [in] ָ��˯��ʱ��
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
    ** @brief     CThread�����̺߳���(�ɼ̳�ʵ��)
    ** @param     void
    ** @return    void
    */
    virtual void Run()
    {
        
    }

private:
    /**
     * @brief CThreadָ�������������̺߳���
     *
     * CThreadָ�������������̺߳���
     *
     * @param func [in] ���߳����е��߳���
     * @param args [in] ���߳��߳��崫�����
     * @param flags [in] ���̴߳����ı�־
     * @param size [in] ���̴߳���ָ������Ķ�ջ�ߴ�
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
            pthread_attr_setstacksize(&attr, 10*1024*1024); //Ĭ��10M
        
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
     * @brief CThread�߳��庯��
     *
     * CThread�߳��庯��
     *
     * @param args [in] �߳��崫�����
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

            //��linux����������׳��쳣��coredump
            throw;
        }
        
        thread->m_nStatus = PS_UNSTART;
        
        return NULL;
    }
    

public:
    enum
    { 
        LEN_THREAD_NAME = 0xFF,  //!< ����ö��

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
    int         m_nFlags;                           //!< �̶߳����־��Ա����
    int         m_nStatus;                          //!< �̶߳���״̬��Ա����
    pthread_t   m_lThreadId;                        //!< �̶߳���Id��Ա����
    char        m_szThreadName[LEN_THREAD_NAME];    //!< �̶߳������Ƴ�Ա����
    CErrMsg     m_cErrmsg;                          //!< ������Ϣ�����Ա����
};

#endif//__THREAD_INC__

