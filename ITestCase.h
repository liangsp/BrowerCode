/*!
 * @file    ITestCase.h
 * @brief   $end$
 *
 *
 * @author    liangshiping
 * @date      2016/8/8
 * @version   1.0.0
 *
*******************************************************************************
 */
#ifndef _ITEST_CASE_
#define _ITEST_CASE_

#include <stdint.h>
#include <vector>
#include <cstring>
#include <cstdio>


enum eTestRet
{
    kUnknow,
    kTestPass,
    kTestFail
};


typedef enum eTestRet(*pTestFunc)(void);

/**
typedef struct TagTestCase
{
    const char *description;
    pTestFunc function;
    bool bRun;
}TestCase;
**/

class CCaseElem
{
public:
    CCaseElem(pTestFunc func, const char *pDesc, bool bRun = true)
    {
        m_pFunc = func;
        m_pszDesc = pDesc;
        m_bRun = bRun;
    }

    void Set(pTestFunc func, const char *desc, bool run = true)
    {
        m_pFunc = func;
        m_pszDesc = desc;
        m_bRun = run;
    }

    void SetDesc(const char *pszVal)
    {
        m_pszDesc = pszVal;
    }
    
    void SetRuntable(bool bVal)
    {
        m_bRun = bVal;
    }
    
    void SetFunc(pTestFunc val)
    {
        m_pFunc = val;
    }

    const char *GetDesc() const
    {
        return m_pszDesc;
    }
    
    bool  IsRuntable() const
    {
        return m_bRun;
    }
    
    pTestFunc GetFunc() const
    {
        return m_pFunc;
    }

private:
    CCaseElem(); //!< not supported

private:
    const char  *m_pszDesc;
    bool m_bRun;
    pTestFunc m_pFunc;
};

class ITestCase
{
public:
    virtual ~ITestCase() {}

    int virtual Init() = 0;

    void Run(void)
    {
        if(0 != Init())
        {
            return ;
        }

        const int paddingsz = 60;
        char padding[paddingsz + 1];
        memset(padding, ' ', paddingsz);

        for(TVecCase::const_iterator it = s_vecCase.begin(); it != s_vecCase.end(); ++it)
        {
            if(false == it->IsRuntable())
            {
                continue;
            }
            enum eTestRet eRet = it->GetFunc()();

            int len = strlen(it->GetDesc());
            if(len > paddingsz)
            {
                len = paddingsz;
            }
            padding[paddingsz - len] = '\0';
            fprintf(stdout, "TestCase:%s:%s", it->GetDesc(), padding);
            padding[paddingsz - len] = ' ';

            fflush(stdout);
            switch(eRet)
            {
                case kUnknow:
                    fprintf(stdout, "[UNKNOW]\n");
                    break;
                case kTestPass:
                    fprintf(stdout, "[OK]\n");
                    break;
                case kTestFail:
                    fprintf(stdout, "[FAILED]\n");
                    break;
                default:
                    fprintf(stdout, "[UNKNOW]\n");
                    break;
            }
            fflush(stdout);
        }

    }

protected:
    typedef std::vector<CCaseElem> TVecCase;
    TVecCase s_vecCase;
};

#endif
