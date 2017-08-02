#include "TestCase.h"
#include "CTime.h"
#include "slog.h"
#include "AVLTree.h"
#include "ErrMsg.h"
#include "IAlloc.h"
#include "ConfigFile.h"
#include <bitset>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <algorithm>
#include <functional>




using namespace lspublic;
using namespace std;


#ifndef SIGN
    #define SIGN(value) (value) > (0) ? (1) : ((value) == (0) ? (0) : (-1))
#endif


class CCity
{
public:
    CCity(const char *pszCityName = NULL)
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

    char *GetCityName()
    {
        return &szCityBuf[0];
    }

    int Compare(const CCity &other) const
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
    CAVLNode<CCity> *pAVLNode = NULL;

    char szArrayCity[][64] =
    {
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

    printf("二维数组行数:%ld\n", sizeof(szArrayCity) / sizeof(szArrayCity[0])); // 13
    for(unsigned i = 0; i < sizeof(szArrayCity) / sizeof(szArrayCity[0]); ++i)
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
    static const int sample_case_id = 5;
    switch(sample_case_id)
    {
        case 0:
        {
            char buffer11[] = "0,,80A,102010168861,2000760,0,201705,10138,6,6;"
                              "1,1993,80A,102010168861,2000760,0,201705,10138,16,69;"
                              "2,1993,80A,102010168861,2000760,0,201705,10138,116,69;";
            char *pToken = NULL;
            char *pSavePtr = NULL;
            int nSeq = 0;

            pToken = strtok_r(buffer11, ";", &pSavePtr);
            while(pToken != NULL)
            {
                nSeq = 0;
                pToken = strtok(pToken, ",");
                while(pToken != NULL)
                {
                    nSeq++;
                    switch(nSeq)
                    {
                        case 2:
                            printf("\t--> %s\n", pToken);
                            break;
                        case 3:
                            printf("\t--> %s\n", pToken);
                            break;
                        case 4:
                            printf("\t--> %s\n", pToken);
                            break;
                        case 5:
                            printf("\t--> %s\n", pToken);
                            break;
                        case 8:
                            printf("\t--> %s\n", pToken);
                            break;
                        case 10:
                            printf("\t--> %s\n", pToken);
                            break;
                        default:
                            break;
                    }
                    pToken = strtok(NULL, ",");
                }

                pToken = strtok_r(pSavePtr, ";", &pSavePtr);
            }
        }
        break;
        case 1:
        {
            enum {BITS_COUNT = 16};
            bitset<BITS_COUNT> bitBucket(0xFFFF);
            printf("bitBucket[%s]\n", bitBucket.to_string().c_str());

            for(int i = 0; i < BITS_COUNT; i += 2)
            {
                bitBucket.set(i, false);
            }
            printf("bitBucket[%s]\n", bitBucket.to_string().c_str());

            printf("bitBucket[%s]有%d位1\n", bitBucket.to_string().c_str(), (int)bitBucket.count());

            if(bitBucket.test(BITS_COUNT - 1))
            {
                printf("[%s]第%u位为1, 共[%d]位\n", bitBucket.to_string().c_str(), BITS_COUNT - 1, (int)bitBucket.size());
            }
        }
        break;
        case 2:
        {
            int nSeq = 0;
            char *pToken = NULL;
            char *pSavePtr = NULL;
            char buffer[] = "0,1994,80A,102010168861,2000760,0,201705,10138,6,6;"
                            "1,1993,80A,102010168861,2000760,0,201705,10138,16,69;"
                            "2,1993,80A,102010168861,2000760,0,201705,10138,116,699;";

            printf("buffer:%s\n", buffer);

            for(pToken = buffer; ; pToken = pSavePtr)
            {
                nSeq = 0;
                pToken = strtok_r(pToken, ";", &pSavePtr);
                if(NULL == pToken)
                {
                    break;
                }

                printf("strlen(pToken) = [%d], pToken:%s\n", (int)strlen(pToken), pToken);
                printf("strlen(pSavePtr) = [%d], pSavePtr:[%s]\n", (int)strlen(pSavePtr), pSavePtr);

                for(; ; pToken = NULL)
                {
                    pToken = strtok(pToken, ",");
                    if(NULL == pToken)
                    {
                        printf("\n");
                        break;
                    }

                    nSeq++;
                    switch(nSeq)
                    {
                        case 2:
                            printf("\t--> %s\n", pToken);
                            break;
                        case 3:
                            printf("\t--> %s\n", pToken);
                            break;
                        case 4:
                            printf("\t--> %s\n", pToken);
                            break;
                        case 5:
                            printf("\t--> %s\n", pToken);
                            break;
                        case 8:
                            printf("\t--> %s\n", pToken);
                            break;
                        case 10:
                            printf("\t--> %s\n", pToken);
                            break;
                        default:
                            break;
                    }
                }
            }
        }
        break;
        case 3:
        {
            int i;
            char *buffer = new(std::nothrow) char[128];
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
                {
                    break;
                }
            }
            cout << " 3 i = " << i << endl;
            if(i < len)
            {
                memmove(buffer, buffer + i, len - i + 1);
            }
            cout << "##$buffer = " << buffer << endl;

            delete [] buffer;
        }
        break;
        case 4:
        {
            int nSeq = 0;
            char *pToken = NULL;
            char *ptr = NULL;
            char buffer[] = ";0,1994,80A,102010168861,2000760,0,201705,10138,16,69;"
                            ";1,1993,80A,102010168861,2000760,0,201705,10138,26,699; "
                            ";2,1993,80A,102010168861,2000760,0,201705,10138,36,6999;;";

            printf("buffer:%s\n", buffer);

            for(ptr = buffer; '\0' != *ptr;)
            {
                //跳过待分解字符串开始的所有分界符
                ptr += strspn(ptr, ";");
                if('\0' == *ptr)
                {
                    break;
                }

                pToken = ptr;
                ptr = strstr(ptr, ";");
                //ptr = strpbrk(pToken, ";"); // 使用 strpbrk 函数
                if(NULL == ptr)
                {
                    break;
                }

                *ptr = '\0';
                ++ptr;

                printf("strlen(pToken)   = [%d], pToken:[%s]\n", (int)strlen(pToken), pToken);
                printf("ptr - pToken - 1 = [%d], pToken:[%s]\n", (int)(ptr - pToken - 1), pToken);

                for(nSeq = 0; ; pToken = NULL)
                {
                    pToken = strtok(pToken, ",");
                    if(NULL == pToken)
                    {
                        printf("\n");
                        break;
                    }

                    nSeq++;
                    switch(nSeq)
                    {
                        case 2:
                            printf("\t--> %s\n", pToken);
                            break;
                        case 3:
                            printf("\t--> %s\n", pToken);
                            break;
                        case 4:
                            printf("\t--> %s\n", pToken);
                            break;
                        case 5:
                            printf("\t--> %s\n", pToken);
                            break;
                        case 8:
                            printf("\t--> %s\n", pToken);
                            break;
                        case 10:
                            printf("\t--> %s\n", pToken);
                            break;
                        default:
                            break;
                    }
                }
            }
        }
        break;
        case 5:
        {
            int count = 0;
            DIR *pDir = NULL;
            struct dirent *pDirent = NULL;
            char szFileName[128] = "";
            char path[] = "/app/bjabmdev/lsp/dev/obj";

            pDir = opendir(path);
            if(pDir == NULL)
            {
                printf("opendir failed!\n");
                return kTestFail;
            }

            while((pDirent = readdir(pDir)) != NULL)
            {
                strcpy(szFileName, pDirent->d_name);
                if(strcmp(szFileName, ".") == 0 || strcmp(szFileName, "..") == 0 || 4 == pDirent->d_type)
                {
                    printf("d_name[%s] is . or .. or dir\n", pDirent->d_name);
                    continue;
                }
                count++;
                printf("[%d]file name:%s\n", count, szFileName);
            }
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
    char szFileinPath[BUFSIZ >> 3] = "/app/bjabmdev/lsp/dev/obj/Makefile";
    char szFileOutPath[BUFSIZ >> 3] = "/app/bjabmdev/lsp/dev/obj/outfile";

    cout << "BUFSIZ = " << (BUFSIZ >> 4) << endl; //BUFSIZ = 8192


    ifstream fin(szFileinPath);
    if(!fin.is_open())
    {
        cout << szFileinPath << " open failed! " << strerror(errno) << endl;
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
    char *pDataBuf = new(std::nothrow) char[size];
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

    do
    {
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
        do
        {
            pos = subStr.find(",", begin);
            if(pos == string::npos)
            {
                cout << "\t@last:[" << subStr.substr(begin).c_str() << "], size:" << subStr.substr(begin).size() << endl;
                break;
            }

            item = subStr.substr(begin, pos - begin);

            if(item.compare("80A") == 0)
            {
                cout << "\t80A --> 80C" << endl;
                item.replace(item.begin(), item.end(), "80C");
                item += " 80A";
                item += " 80B";
                item.append(" ***************");
                cout << "\t@:[" << item << "], size:" << item.size() << endl;
            }
            else
            {
                cout << "\t@:[" << item << "], size:" << item.size() << endl;
            }

            begin = pos + 1;

        } while(1);

        //恢复查找位置
        begin = backup;

    } while(1);

    cout << "string[" << mystr.size() << "]:" << mystr << endl;

    do
    {
        char buffer[] = "1qa|2qs|3qd|4qf|5qg|6qh|7qj|8qk|9ql";
        string line(buffer);

        string::size_type pos = 0;
        string::size_type begin = 0;
        vector<string> vecStringList;
        while(1)
        {
            pos = line.find("|", begin);
            if(string::npos == pos)
            {
                vecStringList.push_back(line.substr(begin)); //!< 最后一个
                break;
            }
            vecStringList.push_back(line.substr(begin, pos - begin));
            begin = pos + 1;
        }

        if(vecStringList.size() == 9)
        {
            cout << "vecStringList.size() = " << vecStringList.size() << endl;
        }

        for(std::size_t index = 0; index < vecStringList.size(); ++index)
        {
            cout << "\t" << vecStringList[index] << endl;
        }

        cout << "##################################################" << endl;

        vecStringList.clear();
        int nCount = Parse2Vector(buffer, vecStringList, "|");
        cout << "nCount = " << nCount << endl;
        for(std::size_t index = 0; index < vecStringList.size(); ++index)
        {
            cout << "\t" << vecStringList[index] << endl;
        }

        cout << "##################################################" << endl;

    } while(0);

    cout << "##################################################" << endl;
    cout << "#########---------------------------------########" << endl;
    cout << "##################################################" << endl;

    vector<string> vecfile;
    vecfile.push_back(string("aax20170802091236"));
    vecfile.push_back(string("aab20170802091235"));
    vecfile.push_back(string("aaa20170802091136"));

    for(vector<string>::size_type i = 0; i < vecfile.size(); ++i)
    {
        cout << vecfile[i] << endl;
    }

    cout << "#########---------------------------------########" << endl;

    std::sort(vecfile.begin(), vecfile.end());

    for(vector<string>::size_type i = 0; i < vecfile.size(); ++i)
    {
        cout << vecfile[i] << endl;
    }

    return kTestPass;
}

eTestRet CTestCase::TestSample()
{
    union _Obj
    {
        union _Obj *_M_free_list_link;
        char _M_client_data[1];
    };

    do
    {
        static const int _NFREELISTS = 16;
        static _Obj *_S_free_list[_NFREELISTS];

        _Obj **p = &_S_free_list[0] + 1;
        (void)p;

        _Obj obj;

        printf("sizeof(obj) = %d, sizeof(obj._M_client_data) = %d, sizeof(obj._M_free_list_link) = %d\n", \
               (int)sizeof(obj), (int)sizeof(obj._M_client_data), (int)sizeof(obj._M_free_list_link));
        printf("%p -> %p -> %p\n", _S_free_list, _S_free_list + 1, _S_free_list + 2);
        printf("@@sizeof(char) = %d, sizeof(int*) = %d\n", (int)sizeof(char), (int)sizeof(int *));

        _Obj temp1;
        temp1._M_client_data[0] = 'A';
        temp1._M_free_list_link = 0;
        _Obj temp2;
        temp2._M_client_data[0] = 'B';
        temp2._M_free_list_link = 0;

        temp1._M_free_list_link = &temp2;

        printf("###(temp1):%p -> (temp2):%p\n", &temp1, &temp2);
        printf("###(temp1):%p -> (temp1._M_free_list_link):%p\n", &temp1, temp1._M_free_list_link);
        printf("###(temp1):%p -> (temp1._M_client_data):%p\n", &temp1, &temp1._M_client_data[0]);

    } while(0);


    do
    {
        int *ptrInt = static_cast<int *>(alloc::allocate(sizeof(int)));
        *ptrInt = 10;
        printf("ptrInt = %p, *ptrInd = %d\n", ptrInt, *ptrInt);

    } while(0);

    return kTestPass;
}

eTestRet CTestCase::TestConfig()
{
    char szHostIP[64] = "";
    CConfigFile::GetInstance()->Load("/app/bjabmdev/lsp/dev/obj/xxx.conf");
    CConfigFile::GetInstance()->GetParamValue("GMDB", "HOST_IP", szHostIP, sizeof(szHostIP));
    printf("### szHostIP = %s\n", szHostIP);
    return kTestPass;
}

int CTestCase::Parse2Vector(const char * pszString, std::vector<std::string> &vecStringList, const char *delim)
{
    if(NULL == pszString || NULL == delim)
    {
        return 0;
    }

    string line(pszString);
    string::size_type pos = 0;
    string::size_type begin = 0;
    
    while(1)
    {
        pos = line.find(delim, begin);
        if(string::npos == pos)
        {
            vecStringList.push_back(line.substr(begin)); //! 最后一个
            break;
        }
        vecStringList.push_back(line.substr(begin, pos - begin));
        begin = pos + 1;
    }

    return(vecStringList.size());
}



int CTestCase::Init()
{
    CCaseElem oElem(TestCTime, "Test func CTime::GetCurrentTime");
    oElem.SetRuntable(false);
    s_vecCase.push_back(oElem);


    oElem.Set(TestAVLTree, "Test AVL Tree");
    oElem.SetRuntable(false);
    s_vecCase.push_back(oElem);


    oElem.Set(TestClient, "Test class TestClient");
    oElem.SetRuntable(false);
    s_vecCase.push_back(oElem);


    oElem.Set(TestSomeCode, "Test Some Code");
    oElem.SetRuntable(false);
    s_vecCase.push_back(oElem);


    oElem.Set(TestStreamCode, "Test file stream Code");
    oElem.SetRuntable(false);
    s_vecCase.push_back(oElem);


    oElem.Set(TestStringCode, "Test string Code");
    oElem.SetRuntable(true);
    s_vecCase.push_back(oElem);


    oElem.Set(TestSample, "Test Sample");
    oElem.SetRuntable(false);
    s_vecCase.push_back(oElem);

    oElem.Set(TestConfig, "Test Config");
    oElem.SetRuntable(false);
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

    } while(ncnt < 10);


    return;
}



