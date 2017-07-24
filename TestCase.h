#ifndef _CTEST_CASE_H_
#define _CTEST_CASE_H_

#include "ITestCase.h"
#include "Thread.h"


class CTestCase: public ITestCase
{
public:
    static eTestRet TestCTime();

    static eTestRet TestAVLTree();

    static eTestRet TestClient();

    static eTestRet TestSomeCode();

    static eTestRet TestStreamCode();

    static eTestRet TestStringCode();

    static eTestRet TestSample();

    int Init();
};



class myThread : public CThread
{
public:
    myThread() {}
    ~myThread() {}

    void Run();
};


#endif

