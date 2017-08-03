#include "ConfigFile.h"
#include <stdarg.h> //! va_list  va_start  va_end  vsnprintf
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const char LINE_FEED				= (char)0x0a;
static const char LF = LINE_FEED;
static const char CARRIAGE_RETURN		= (char)0x0d;
static const char CR = CARRIAGE_RETURN;
static const char SPACES				= (char)0x20;
static const char SP = SPACES;
static const char HORIZONTAL_TEXT	    = (char)0x09;
static const char HT = HORIZONTAL_TEXT;


static char *trimLeft(char *str, int length)
{
    if(*str == '\0' || length == 0 || str == NULL)
    {
        return NULL;
    }

    char *pTemp = NULL;
    char *start = NULL;

    pTemp = new char[length + 1];
    memset(pTemp, 0x00, length + 1);
    memcpy(pTemp, str, length);

    start = pTemp;
    while(SP == (*start) || HT == (*start))
    {
        start ++;
    }

    memcpy(str, start, strlen(start));
    str[strlen(start)] = '\0';

    delete [] pTemp;

    return str;
}

static char *trimRight(char *str, int length)
{
    if(*str == '\0' || length == 0 || str == NULL)
    {
        return NULL;
    }

    char *end = NULL;
    end = str + length - 1;

    while(SP == *end || HT == *end)
    {
        *end = '\0';
        if(end == str)
        {
            break;
        }
        end--;
    }

    return str;
}

static char *trim(char *str, int length)
{
    char *s = NULL;
    s = trimLeft(str, length);
    s = trimRight(str, strlen(s));

    return s;
}



CConfigFile CConfigFile::m_Instance;

CConfigFile::CConfigFile(): m_pBuffer(NULL),
    m_nBufferLength(0)
{
    memset(m_szConfigFile, 0x00, sizeof(m_szConfigFile));
    memset(m_szCfgErrMsg, 0x00, sizeof(m_szCfgErrMsg));
    QUEUE_INIT(&m_qSectionQueueHead);
}

CConfigFile::~CConfigFile()
{
}

CConfigFile *CConfigFile::GetInstance()
{
    return(&m_Instance);
}

int CConfigFile::Load(const char *pszFileName)
{
    //char *pTmpFileName = NULL;
    //char *pEnvEncrypt = NULL;
    int nRetCode = 0;
    int nLength = 0;
    FILE *hFile = NULL;
    //char szConfigFileName[256];

    //! 表示已经加载配置文件
    if(m_pBuffer)
    {
        return 0;
    }

    //! 检查输入文件是否存在
    if(pszFileName)
    {
        nRetCode = CheckFileExist(pszFileName);
        if(nRetCode)
        {
            return nRetCode;
        }
        SetConfigFile(pszFileName);
    }
    else
    {
#if 0
        // 如果文件名未设置，则使用环境变量定义的缺省文件名
        //pTmpFileName = getenv(CConfigFileMacroDef::szEnvConfigFile);
        if(!pTmpFileName)
        {
            // 如果环境变量CONFIG_FILE_NAME没有设置，则使用IBAS系统设置的主目录进行组合设置
            // 例如 IBAS_HOME=/app/ibas6，则配置文件目录为/app/ibas6/etc/ibas.conf
            //pTmpFileName = getenv( CConfigFileMacroDef::szEnvHomePath );
            pTmpFileName = getenv("HBCONFIGDIR");
            if(!pTmpFileName)
            {
                SetErrMsg("环境变量%s未设置", "HBCONFIGDIR");
                return -1;
            }
            memset(szConfigFileName, 0x00, sizeof(szConfigFileName));
            (void)snprintf(szConfigFileName, sizeof(szConfigFileName), "%s/abm.conf", pTmpFileName);
            szConfigFileName[sizeof(szConfigFileName) - 1] = 0;
            pTmpFileName = szConfigFileName;
        }
        nRetCode = CheckFileExist(pTmpFileName);
        if(nRetCode)
        {
            return nRetCode;
        }
        SetConfigFile(pTmpFileName);
#endif
    }

#if 0
    //判断环境变量CONFIG_ENCRYPT判断是否要对配置文件进行解密操作
    pEnvEncrypt = getenv(CConfigFileMacroDef::szEnvConfEncrypt);
    if(!pEnvEncrypt)
    {
        // 如果该环境变量未设置，则缺省认为配置文件不加密
        m_bEncryptFlag = false;
    }
    else
    {
        if(atoi(pEnvEncrypt) != 0 && atoi(pEnvEncrypt) != 1)
        {
            SetErrMsg("取环境变量错误");
            return -1;
        }

        m_bEncryptFlag = atoi(pEnvEncrypt) == 0 ? false : true;
    }
#endif

    //! 初始化成员指针m_pBuffer，同时把配置文件内容读入内存
    if(!m_pBuffer)
    {
        hFile = fopen(m_szConfigFile, "rb");
        if(!hFile)
        {
            SetErrMsg("文件%s打开失败", m_szConfigFile);
            return -1;
        }

        nRetCode = fseek(hFile, 0L, SEEK_END);
        if(nRetCode)
        {
            SetErrMsg("定位文件指针%s失败", m_szConfigFile);
            fclose(hFile);
            return -1;
        }

        nLength = ftell(hFile);

        nRetCode = fseek(hFile, 0L, SEEK_SET);
        if(nRetCode)
        {
            SetErrMsg("定位文件指针%s失败", m_szConfigFile);
            return -1;
        }

        m_pBuffer = (char *)malloc(nLength + 1);
        if(!m_pBuffer)
        {
            SetErrMsg("内存空间分配失败");
            return -1;
        }
        m_nBufferLength = nLength + 1;
        memset(m_pBuffer, 0x00, nLength + 1);

        nRetCode = fread(m_pBuffer, sizeof(char), nLength, hFile);
        if(nRetCode < nLength)
        {
            if(!feof(hFile))
            {
                SetErrMsg("读取文件%s失败", m_szConfigFile);
                return -1;
            }
        }
        fclose(hFile);
    }

    // 对配置文件进行分析
    nRetCode = AnalyBuffer();
    if(nRetCode)
    {
        return nRetCode;
    }

    return 0;
}

int CConfigFile::GetParamValue(const char *pszSection, const char *pszParamName, char *pszParamValue, int nLength)
{
    if(NULL == pszParamName || NULL == pszParamValue || 0 == nLength)
    {
        return(-1);
    }

    if(NULL == pszSection)
    {
        //TODO
        return(-1);
    }

    QUEUE *qSection = NULL;
    QUEUE *qParamters = NULL;
    TSection *pSection = NULL;
    TParameter *pParameter = NULL;

    QUEUE_FOREACH(qSection, &m_qSectionQueueHead)
    {
        pSection = QUEUE_DATA(qSection, TSection, m_qSectionQueue);
        printf("m_qParamQueueHead[0] = %p, m_qParamQueueHead[1] = %p\n", pSection->m_qParamQueueHead[0], pSection->m_qParamQueueHead[1]);
        if(pSection && strncmp(pSection->m_szSection, pszSection, sizeof(pSection->m_szSection)) == 0)
        {
            QUEUE_FOREACH(qParamters, &pSection->m_qParamQueueHead)
            {
                pParameter = QUEUE_DATA(qParamters, TParameter, m_qParameterQueue);
                printf("m_qParameterQueue[0] = %p, m_qParameterQueue[1] = %p\n", pParameter->m_qParameterQueue[0], pParameter->m_qParameterQueue[1]);
                if(pParameter && strncmp(pParameter->m_szName, pszParamName, sizeof(pParameter->m_szName)) == 0)
                {
                    strncpy(pszParamValue, pParameter->m_szValue, nLength);
                    pszParamValue[nLength - 1] = 0;
                    break;
                }
            }
        }
    }

    return(1);
}

int CConfigFile::AnalyBuffer()
{
    if(NULL == m_pBuffer)
    {
        SetErrMsg("内存分配失败");
        return(-1);
    }

    int nRetCode = -1;
    int nOffset = 0;
    TSection *pCurSection = NULL;
    char szLine[LENGTH_MAX << 4] = "";
    char szSection[LENGTH_MAX] = "";
    TParameter tParameter;

    for(int i = 0; i < m_nBufferLength; ++i)
    {
        //! hex:0A	LF (换行)
        if(0x0A == m_pBuffer[i] || i == m_nBufferLength - 1)
        {
            memset(szLine, 0x00, sizeof(szLine));
            memset(szSection, 0x00, sizeof(szSection));
            memcpy(szLine, m_pBuffer + nOffset, i - nOffset);
            if(0x0A == szLine[0])
            {
                szLine[0] = 0x20; //! 换行 -> 空格
            }

            nRetCode = GetSection(szLine, szSection, sizeof(szSection));
            if(!nRetCode)
            {
                bool bFind = false;
                QUEUE *q;
                TSection *temp = NULL;

                QUEUE_FOREACH(q, &m_qSectionQueueHead)
                {
                    temp = QUEUE_DATA(q, TSection, m_qSectionQueue);
                    if(temp && strncmp(temp->m_szSection, szSection, sizeof(temp->m_szSection)) == 0)
                    {
                        bFind = true;
                        pCurSection = temp;
                        break;
                    }
                }

                if(!bFind)
                {
                    pCurSection = (TSection *)malloc(sizeof(TSection));
                    if(NULL == pCurSection)
                    {
                        SetErrMsg("内存分配失败");
                        return(-1);
                    }
                    memset(pCurSection->m_szSection, 0x00, sizeof(pCurSection->m_szSection));
                    strncpy(pCurSection->m_szSection, szSection, sizeof(pCurSection->m_szSection) - 1);

                    QUEUE_INIT(&pCurSection->m_qSectionQueue);
                    QUEUE_INIT(&pCurSection->m_qParamQueueHead);
                    QUEUE_INSERT_TAIL(&m_qSectionQueueHead, &pCurSection->m_qSectionQueue);
                }
            }


            nRetCode = GetParam(szLine, tParameter);
            if(!nRetCode)
            {
                bool bFind = false;
                QUEUE *q;
                TParameter *temp = NULL;

                QUEUE_FOREACH(q, &pCurSection->m_qParamQueueHead)
                {
                    temp = QUEUE_DATA(q, TParameter, m_qParameterQueue);
                    if(temp && strncmp(temp->m_szName, tParameter.m_szName, sizeof(temp->m_szName)) == 0)
                    {
                        bFind = true;
                        strncpy(temp->m_szValue, tParameter.m_szValue, sizeof(temp->m_szValue) - 1);
                        break;
                    }
                }

                if(!bFind)
                {
                    temp = (TParameter *)malloc(sizeof(TParameter));
                    if(NULL == pCurSection)
                    {
                        SetErrMsg("内存分配失败");
                        return(-1);
                    }

                    memset(temp->m_szName, 0x00, sizeof(temp->m_szName));
                    memset(temp->m_szValue, 0x00, sizeof(temp->m_szValue));
                    QUEUE_INIT(&temp->m_qParameterQueue);

                    //memcpy(temp, &tParameter, sizeof(temp));
                    strcpy(temp->m_szName, tParameter.m_szName);
                    strcpy(temp->m_szValue, tParameter.m_szValue);
                    QUEUE_INSERT_TAIL(&pCurSection->m_qParamQueueHead, &temp->m_qParameterQueue);
                }
            }
            nOffset = i;
        }
    }

    return(0);
}

int CConfigFile::GetSection(const char *pszLine, char *pszSection, int nLength)
{
    if(NULL == pszLine || NULL == pszSection || 0 == nLength)
    {
        return(-1);
    }

    int size = 0;
    const char *pSectionBegin = NULL;
    const char *pSectionEnd = NULL;
    const char *pToken = NULL;

    pSectionBegin = strchr(pszLine, '[');
    pSectionEnd = strchr(pszLine, ']');
    pToken = strchr(pszLine, '#');

    if((!pSectionBegin) || (!pSectionEnd) || (pToken && (pSectionEnd > pToken)))
    {
        SetErrMsg("环境变量未设置");
        return(-1);
    }

    if(pSectionEnd - pSectionBegin >= nLength)
    {
        size = nLength;
    }
    else
    {
        size = pSectionEnd - pSectionBegin - 1;
    }

    memset(pszSection, 0x00, nLength);
    strncpy(pszSection, pSectionBegin + 1, size);

    return(0);
}

int CConfigFile::GetParam(const char *pszLine, TParameter &tParameter)
{
    if(NULL == pszLine)
    {
        return(-1);
    }

    const char *pParamBegin = NULL;
    const char *pParamEnd = NULL;

    pParamBegin = strchr(pszLine, '=');
    pParamEnd = strchr(pszLine, '#');
    if((!pParamBegin) || (pParamEnd && (pParamBegin > pParamEnd)))
    {
        return(-1);
    }

    int size = 0;
    memset(&tParameter, 0x00, sizeof(tParameter));

    size = pParamBegin - pszLine;
    strncpy(tParameter.m_szName, pszLine, size);
    trim(tParameter.m_szName, sizeof(tParameter.m_szName));

    if(pParamEnd)
    {
        size = pParamEnd - pParamBegin - 1;
    }
    else
    {
        size = strlen(pszLine) - (pParamBegin - pszLine);
    }

    strncpy(tParameter.m_szValue, pParamBegin  + 1, size);
    trim(tParameter.m_szValue, sizeof(tParameter.m_szValue));
    printf("tParameter.m_szName = #%s#, tParameter.m_szValue = #%s#\n", tParameter.m_szName, tParameter.m_szValue);

    return(0);
}

void CConfigFile::SetConfigFile(const char *pFileName)
{
    if(NULL == pFileName)
    {
        return;
    }

    memset(m_szConfigFile, 0x00, sizeof(m_szConfigFile));
    strncpy(m_szConfigFile, pFileName, sizeof(m_szConfigFile) - 1);
}

int CConfigFile::CheckFileExist(const char *pFileName)
{
    if(NULL == pFileName)
    {
        return -1;
    }

    FILE *hFile = NULL;
    hFile = fopen(pFileName, "rb");
    if(!hFile)
    {
        SetErrMsg("文件%s打开失败", pFileName);
        return -1;
    }
    fclose(hFile);

    return 0;

}

const char *CConfigFile::GetLastErrMsg() const
{
    return(&m_szCfgErrMsg[0]);
}

void CConfigFile::SetErrMsg(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    (void)vsnprintf(m_szCfgErrMsg, sizeof(m_szCfgErrMsg), fmt, ap);
    va_end(ap);
}



