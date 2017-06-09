#include "TestCase.h"
#include "CTime.h"
#include "slog.h"
#include "AVLTree.h"
#include "ErrMsg.h"
#include <bitset>
#include <iostream>
#include <fstream>


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

    printf("二维数组行数:%ld\n", sizeof(szArrayCity)/sizeof(szArrayCity[0])); // 13
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
    static const int sample_case_id = 2;
    switch(sample_case_id)
    {
    case 0:
        {
            char buffer11[] = "0,,80A,102010168861,2000760,0,201705,10138,6,6;"
                "1,1993,80A,102010168861,2000760,0,201705,10138,16,69;"
                "2,1993,80A,102010168861,2000760,0,201705,10138,116,69;";
            char * pToken = NULL;
    		char * pSavePtr = NULL;
            int nSeq = 0;

            pToken = strtok_r(buffer11, ";", &pSavePtr);
            while(pToken != NULL)
            {
                nSeq = 0;
    			pToken = strtok(pToken, ",");
    			while(pToken != NULL)
    			{
                    nSeq++;
                    switch (nSeq)
                    {
                    case 2: printf("\t--> %s\n", pToken);break;
                    case 3: printf("\t--> %s\n", pToken);break;
                    case 4: printf("\t--> %s\n", pToken);break;
                    case 5: printf("\t--> %s\n", pToken);break;
    				case 8: printf("\t--> %s\n", pToken);break;
    				case 10:printf("\t--> %s\n", pToken);break;
                    default:break;
                    }
    				pToken = strtok(NULL, ",");
    			}
                
                pToken = strtok_r(pSavePtr, ";", &pSavePtr);
            }
        }
        break;
    case 1:
        {
            enum{BITS_COUNT = 16};
            bitset<BITS_COUNT> bitBucket(0xFFFF);
            printf("bitBucket[%s]\n", bitBucket.to_string().c_str());

            for(int i = 0; i < BITS_COUNT; i += 2)
            {
                bitBucket.set(i, false);
            }
            printf("bitBucket[%s]\n", bitBucket.to_string().c_str());

            printf("bitBucket[%s]有%d位1\n", bitBucket.to_string().c_str(), (int)bitBucket.count());

            if(bitBucket.test(BITS_COUNT-1))
            {
                printf("[%s]第%u位为1, 共[%d]位\n", bitBucket.to_string().c_str(), BITS_COUNT - 1, (int)bitBucket.size());
            }
        }
        break;
    case 2:
        {
            int nSeq = 0;
    		char * pToken = NULL;
    		char * pSavePtr = NULL;
            char buffer[] = "0,1994,80A,102010168861,2000760,0,201705,10138,6,6;"
                "1,1993,80A,102010168861,2000760,0,201705,10138,16,69;"
                "2,1993,80A,102010168861,2000760,0,201705,10138,116,69;";
            
    		printf("buffer:%s\n", buffer);

            for(pToken = buffer; ; pToken = pSavePtr)  
            {
                nSeq = 0;
                pToken = strtok_r(pToken, ";", &pSavePtr);
                if(NULL == pToken)
                    break;

                printf("pToken:%s\n", pToken);
                
                for( ; ; pToken = NULL)
                {
                    pToken = strtok(pToken, ",");
                    if(NULL == pToken)
                        break;
                        
                    nSeq++;
                    switch(nSeq)
                    {
                    case 2: printf("\t--> %s\n", pToken);break;
                    case 3: printf("\t--> %s\n", pToken);break;
                    case 4: printf("\t--> %s\n", pToken);break;
                    case 5: printf("\t--> %s\n", pToken);break;
    				case 8: printf("\t--> %s\n", pToken);break;
    				case 10:printf("\t--> %s\n", pToken);break;
                    default:break;
                    }
                }
            }
        }
        break;
    case 3:
        {
            int i;
            char * buffer = new (std::nothrow) char[128];
            char szString[] = "\"2,2,2,2,\"";
            int len = strlen(szString);
            cout << "1 len = " << len << endl;
            strncpy(buffer, szString, len);
            buffer[len] = '\0';
            cout << "@@$buffer = " << buffer << endl;

            len = strlen(buffer);
            cout << " 2 len = " << len << endl;
        	for(i = 0; i < len; i++)
        	{
        		if(!strchr("\"", buffer[i]))
        			break;
        	}
            cout << " 3 i = " << i << endl;
        	if(i < len)
        	{
                memmove(buffer,buffer+i,len-i+1);
        	}
            cout << "##$buffer = " << buffer << endl;
            
            delete [] buffer;
        }
        break;
    case 99:
        {
        }
        break;
    default:
        break;
    }

    return kTestPass;
}

eTestRet CTestCase::TestStreamCode()
{
    long begin = 0L;
    long end = 0L;
    char szFileinPath[] = "/app/bjabmdev/lsp/dev/obj/Makefile";
    char szFileOutPath[] = "/app/bjabmdev/lsp/dev/obj/outfile";

   
    ifstream fin(szFileinPath);
    if(!fin.is_open())
    {
        cout << szFileinPath << " open failed!" << endl;
    }

    ofstream fout;
    fout.open(szFileOutPath);
    if(!fout.is_open())
    {
        cout << szFileOutPath << " open failed!" << endl;
    }


    begin = fin.tellg();
    fin.seekg(0, ios::end);
    end = fin.tellg();
    cout << "#1 file size: " << end - begin << "byte" << endl;

    filebuf *inbuf = fin.rdbuf();
    long size = inbuf->pubseekoff(0, ios::end);
	
	inbuf->pubseekpos(0); //rewind
    char *pDataBuf = new (std::nothrow) char[size];
    std::streamsize nReadCount = inbuf->sgetn(pDataBuf, size);
    cout << "nReadCount = " << nReadCount << ", size = " << size << endl;
    if(nReadCount == size)
    {
        //cout << pDataBuf << endl;
    }
    
    fout << "#THIS is TEST&^%\r\n";
    fout.write(pDataBuf, size);
    fout.close();
	
	
//	fin.seekg(0, ios::beg);
//    while(!fin.eof())
//    {
//        fin.getline(buffer, sizeof(buffer));
//        cout << buffer << endl;
//    }

    fin.close();
    fout.close();

    delete[] pDataBuf;


    return kTestPass;
}
    
eTestRet CTestCase::TestStringCode()
{
    string::size_type pos = 0;
    string::size_type begin = 0;
	string::size_type backup;
    string subStr;
    string item;

    
    char buffer[] = "0,,80A,102010168861,2000760,0,201705,10138,6,6;"
                    "1,1993,80A,102010168861,2000760,0,201705,10138,16,69;";

	string mystr(buffer);
    cout << "string[" << mystr.size() << "]:" << mystr << endl;

    do{
        pos = mystr.find(";", begin);
        if(pos == string::npos)
        {
            break;
        }
        
        subStr = mystr.substr(begin, pos - begin);
        begin = pos + 1;
        cout << "SubStr:" << subStr << endl;

        //保存下次查找起始位置
        backup = begin;
		begin = 0;
		do{
			pos = subStr.find(",", begin);
			if(pos == string::npos)
       		{
                cout << "\t@last:" << subStr.substr(begin) << endl;
            	break;
        	}
            
            item = subStr.substr(begin, pos - begin);
            
            if(item.compare("80A") == 0)
            {
                cout << "\t80A --> 80C" << endl;
                item.replace(item.begin(), item.end(), "80C");
                item += " 80A";
				item += " 80B";
                cout << "\t@:" << item << endl;
            }
            else 
            {
                cout << "\t@:" << item << endl;
            }
			
			begin = pos + 1;
            
		}while(1);

        //恢复查找位置
        begin = backup;
        
    }while(1);
	
	return kTestPass;
}

int CTestCase::Init()
{
    CCaseElem oElem(TestCTime, "Test func CTime::GetCurrentTime");
    oElem.SetRuntable(false);
    s_vecCase.push_back(oElem);
   

    oElem.Set(TestAVLTree, "Test AVL Tree");
    oElem.SetRuntable(false);
    oElem.SetRuntable(true);
    s_vecCase.push_back(oElem);


    oElem.Set(TestClient, "Test class TestClient");
    oElem.SetRuntable(false);
    s_vecCase.push_back(oElem);


    oElem.Set(TestSomeCode, "Test Some Code");
    s_vecCase.push_back(oElem);


    oElem.Set(TestStreamCode, "Test file stream Code");
    s_vecCase.push_back(oElem);


    oElem.Set(TestStringCode, "Test string Code");
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



