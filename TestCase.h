#ifndef _CTEST_CASE_H_
#define _CTEST_CASE_H_

#include "ITestCase.h"
#include "Thread.h"
#include <string>


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

    static eTestRet TestConfig();

    static int Parse2Vector(const char * pszString, std::vector<std::string> &vecStringList, const char *delim);

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

