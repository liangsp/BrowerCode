#include <stdint.h>
#include <stdio.h>
#include "TestCase.h"

int main(int argc,char** argv)
{
    CTestCase oCase;
    oCase.Run();

#if 0
    myThread  *obj = new (std::nothrow) myThread();
    obj->Start();
    
    //obj->Sleep(20000);
    obj->Join();

    delete obj;
#endif

    return 0;

}

