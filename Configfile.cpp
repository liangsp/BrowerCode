#include "ConfigFile.h"
#include <stdarg.h> //! va_list  va_start  va_end  vsnprintf
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


CConfigFile CConfigFile::m_Instance;

CConfigFile::CConfigFile(): m_pBuffer(NULL),
    m_nBufferLength(0)
{
    memset(m_szConfigFile, 0x00, sizeof(m_szConfigFile));
    memset(m_szCfgErrMsg, 0x00, sizeof(m_szCfgErrMsg));
    QUEUE_INIT(&m_qSectionQueue);
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

int CConfigFile::AnalyBuffer()
{
    if(NULL == m_pBuffer)
    {
        SetErrMsg("内存分配失败");
        return(-1);
    }
    int nRetCode = -1;
    int nOffset = 0;
    TParameter *ptParameter = NULL;
    char szLine[LENGTH_MAX << 4] = "";
    char szSection[LENGTH_MAX] = "";

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
        }
    }

    return(0);
}

int CConfigFile::GetSection(const char *pszLine, char *pszSection, int32_t nLength)
{
    if(NULL == pszLine || NULL == pszSection || 0 == nLength)
    {
        return(-1);
    }

    int size = 0;
    char *pSectionBegin = NULL;
    char *pSectionEnd = NULL;
    char *pToken = NULL;

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

    char *pParamBegin = NULL;
    char *pParamEnd = NULL;

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

    if(pParamEnd)
    {
        size = pParamEnd - pParamBegin - 1;
    }
    else
    {
        size = strlen(pszLine) - (pParamBegin - pszLine);
    }

    strncpy(tParameter.m_szName, pParamBegin  + 1, size);

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



