#include "TestCase.h"
#include "CTime.h"
#include "slog.h"
#include "AVLTree.h"
#include "ErrMsg.h"
#include <bitset>


using namespace lspublic;
using namespace std;


#ifndef SIGN
    #define SIGN(value) (value) > (0) ? (1) : ((value) == (0) ? (0) : (-1))
#endif


class CCity
{
public:
    CCity(const char * pszCityName = NULL)
    {
        if(pszCityName)
        {
            memset(szCityBuf, 0x00, sizeof(szCityBuf));
            snprintf(szCityBuf, sizeof(szCityBuf), "%s", pszCityName);
        }
    }
    
    virtual ~CCity() 
    { 
        memset(szCityBuf, 0x00, sizeof(szCityBuf)); 
    }
    
    char * GetCityName()
    {
        return &szCityBuf[0];
    }

    int Compare(const CCity & other) const
    {
        int nRet = strncmp(szCityBuf, other.szCityBuf, sizeof(szCityBuf));
        return SIGN(nRet);
    }
    
private:
    CCity() {}
    char szCityBuf[128];
};



eTestRet CTestCase::TestCTime()
{
	long usetime = 0L;
	char szBuffer[128] = "";
	
	CLoggerMgr::GetInstance()->SetLogLevel(LL_DEBUG);
	
	CTime::BeginTime();
	
	CTime::format("%Y-%m-%d %H:%M:%S", szBuffer, sizeof(szBuffer));
	TL_MSG("Time: 【%s】", szBuffer);
	
	CTime::GetCurrentTime(szBuffer, sizeof(szBuffer));
	TL_MSG("Current Time: 【%s】", szBuffer);
	
	CTime::GetCurTimeMicSecond(szBuffer, sizeof(szBuffer));
	TL_MSG("Current Time MicSecond: 【%s】", szBuffer);
	
	TL_MSG("Current month: 【%d】", CTime::GetMonth());
	TL_MSG("Current month: 【%d】", CTime::GetMonth(szBuffer));
		
	TL_MSG("Current day: 【%d】", CTime::GetDay());
	TL_MSG("Current day: 【%d】", CTime::GetDay(szBuffer));
		
    TL_MSG("Current week day: 【%d】", CTime::GetWeekDay());
    	
    for(long index = 0; index < 20; index++)
    {
    	usetime++;
    	TL_DEBUG("count: 【%ld】", usetime);
    }
    
    usetime = 0;	
    usetime = CTime::EndTime();

    double duration = CTime::ComputeDuration("20120527153501", "20170527153600");
    printf("duration:%f\n", duration);
    	
    TL_MSG("usetime: 【%ld】ms", usetime);
    
	return kTestPass;
}

eTestRet CTestCase::TestAVLTree()
{
    CAVLTree<CCity>   treeCity;
    CAVLNode<CCity> * pAVLNode = NULL;

    char szArrayCity[][64] = {
            "London",
            "Paris",
            "Moskau",
            "Hamburg",
            "Mexico",
            "Berlin",
            "Rom",
            "Chengdu",
            "XiaMen",
            "HangZhou",
            "NanJing",
            "YinChuan",
            "LaSa"
        };

    printf("二维数组行数:%ld\n", sizeof(szArrayCity)/sizeof(szArrayCity[0]));
    for(unsigned i = 0; i < sizeof(szArrayCity)/sizeof(szArrayCity[0]); ++i)
    {
        CCity tmp(szArrayCity[i]);
        CCity *pCity = treeCity.SearchObject(tmp);
        if(NULL == pCity)
        {
            pCity = new(std::nothrow) CCity(szArrayCity[i]);
            pAVLNode = treeCity.Insert(pCity);
        }
        else
        {
            //!< TODO: dosomething
        }
    }

    CAVLTreeIterator<CCity> iter(treeCity);
    for(; iter; ++iter)
    {
        CCity *pCity = iter.GetData();
        printf("%s\n", pCity->GetCityName());
    }

    printf("nodes: %d\n", treeCity.GetRoot()->NodesInTree());
    printf("depth: %d\n", treeCity.GetDepth());
    printf("NODES: %ld\n", treeCity.NodesInTree());
    
    return kTestPass;
}


eTestRet CTestCase::TestClient()
{
	char buffer[13] = "";
	int nRet = snprintf(buffer, sizeof(buffer), "%s", "liangshiping");
	printf("nRet = %d, buffer = %s\n", nRet, buffer);
	
	char buffer1[12] = "";
	nRet = snprintf(buffer1, sizeof(buffer1), "%s", "liangshiping");
	printf("nRet = %d, buffer1 = %s\n", nRet, buffer1);
	
	char buffer2[11] = "";
	nRet = snprintf(buffer2, sizeof(buffer2), "%s", "liangshiping");
	printf("nRet = %d, buffer2 = %s\n", nRet, buffer2);
	
	
	union myUnion
	{
		int n;
		char buf[13];
		long l;
	};
	
	myUnion record;
	printf("myUnion.n = %d\n", (int)sizeof(record.n));
	printf("myUnion.buf = %d\n", (int)sizeof(record.buf));
	printf("myUnion.l = %d\n", (int)sizeof(record.l));
	printf("myUnion = %d\n", (int)sizeof(union myUnion));
	{
	  //#define BUFFER_SIZE 64
      //char buffer[BUFFER_SIZE] = "";
      //wchar_t wcstring[] = L"Liangshiping";
 
      //size_t nRet = wcstombs(buffer, wcstring, BUFFER_SIZE);
      //printf("Characters converted = %u\n", nRet);
      //printf("Multibyte character = %s\n\n", buffer);
	}

    CErrMsg cErrmsg;
    cErrmsg.SetLastErrMsg("%s%d", "jkkkkkkkk,UUU", 999);
    printf("%s\n", cErrmsg.GetLastErrMsg());
	
	return kTestPass;
}

eTestRet CTestCase::TestSomeCode()
{
    if(0)
    {
        char buffer11[] = "#0|1993||102010168861|2000760|0|201705|10138|6|6";
        char * pToken = NULL;

        printf("%s\n", buffer11);
        pToken = strtok(buffer11, "|");
        while(pToken != NULL)
        {
            printf("%s\n", pToken);
            pToken = strtok(NULL, "|");
        }
    }
    else if(1)
    {
        enum{BITS_COUNT = 16};
        bitset<BITS_COUNT> bitBucket(0xFFFF);
        printf("bitBucket[%s]\n", bitBucket.to_string().c_str());

        for(int i = 0; i < BITS_COUNT; i += 2)
        {
            bitBucket.set(i, false);
        }
        printf("bitBucket[%s]\n", bitBucket.to_string().c_str());

        printf("bitBucket[%s]有%u位1\n", bitBucket.to_string().c_str(), bitBucket.count());

        if(bitBucket.test(BITS_COUNT-1))
        {
            printf("[%s]第%u位为1, 共[%u]位\n", bitBucket.to_string().c_str(), BITS_COUNT - 1, bitBucket.size());
        }
    }

    return kTestPass;
}



int CTestCase::Init()
{
    CCaseElem oElem(TestCTime, "Test func CTime::GetCurrentTime");
    s_vecCase.push_back(oElem);

    oElem.Set(TestAVLTree, "Test AVL Tree");
    s_vecCase.push_back(oElem);

    oElem.Set(TestClient, "Test class TestClient");
    s_vecCase.push_back(oElem);

    oElem.Set(TestSomeCode, "Test Some Code");
    s_vecCase.push_back(oElem);
    
    return 0;
}




void myThread::Run()
{
    printf("################RUN##################!\n");
	int ncnt = 0;
	do
	{
		ncnt ++;
	    printf("HELLO THREAD!\n");
        Sleep(1000);
        
	}while(ncnt < 10);

    
    return;
}



