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

    //! ��ʾ�Ѿ����������ļ�
    if(m_pBuffer)
    {
        return 0;
    }

    //! ��������ļ��Ƿ����
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
        // ����ļ���δ���ã���ʹ�û������������ȱʡ�ļ���
        //pTmpFileName = getenv(CConfigFileMacroDef::szEnvConfigFile);
        if(!pTmpFileName)
        {
            // �����������CONFIG_FILE_NAMEû�����ã���ʹ��IBASϵͳ���õ���Ŀ¼�����������
            // ���� IBAS_HOME=/app/ibas6���������ļ�Ŀ¼Ϊ/app/ibas6/etc/ibas.conf
            //pTmpFileName = getenv( CConfigFileMacroDef::szEnvHomePath );
            pTmpFileName = getenv("HBCONFIGDIR");
            if(!pTmpFileName)
            {
                SetErrMsg("��������%sδ����", "HBCONFIGDIR");
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
    //�жϻ�������CONFIG_ENCRYPT�ж��Ƿ�Ҫ�������ļ����н��ܲ���
    pEnvEncrypt = getenv(CConfigFileMacroDef::szEnvConfEncrypt);
    if(!pEnvEncrypt)
    {
        // ����û�������δ���ã���ȱʡ��Ϊ�����ļ�������
        m_bEncryptFlag = false;
    }
    else
    {
        if(atoi(pEnvEncrypt) != 0 && atoi(pEnvEncrypt) != 1)
        {
            SetErrMsg("ȡ������������");
            return -1;
        }

        m_bEncryptFlag = atoi(pEnvEncrypt) == 0 ? false : true;
    }
#endif

    //! ��ʼ����Աָ��m_pBuffer��ͬʱ�������ļ����ݶ����ڴ�
    if(!m_pBuffer)
    {
        hFile = fopen(m_szConfigFile, "rb");
        if(!hFile)
        {
            SetErrMsg("�ļ�%s��ʧ��", m_szConfigFile);
            return -1;
        }

        nRetCode = fseek(hFile, 0L, SEEK_END);
        if(nRetCode)
        {
            SetErrMsg("��λ�ļ�ָ��%sʧ��", m_szConfigFile);
            fclose(hFile);
            return -1;
        }

        nLength = ftell(hFile);

        nRetCode = fseek(hFile, 0L, SEEK_SET);
        if(nRetCode)
        {
            SetErrMsg("��λ�ļ�ָ��%sʧ��", m_szConfigFile);
            return -1;
        }

        m_pBuffer = (char *)malloc(nLength + 1);
        if(!m_pBuffer)
        {
            SetErrMsg("�ڴ�ռ����ʧ��");
            return -1;
        }
        m_nBufferLength = nLength + 1;
        memset(m_pBuffer, 0x00, nLength + 1);

        nRetCode = fread(m_pBuffer, sizeof(char), nLength, hFile);
        if(nRetCode < nLength)
        {
            if(!feof(hFile))
            {
                SetErrMsg("��ȡ�ļ�%sʧ��", m_szConfigFile);
                return -1;
            }
        }
        fclose(hFile);
    }

    // �������ļ����з���
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
        if(pSection && strncmp(pSection->m_szSection, pszSection, sizeof(pSection->m_szSection)) == 0)
        {
            QUEUE_FOREACH(qParamters, &pSection->m_qParamQueueHead)
            {
                pParameter = QUEUE_DATA(qParamters, TParameter, m_qParameterQueue);
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
        SetErrMsg("�ڴ����ʧ��");
        return(-1);
    }
    int nRetCode = -1;
    int nOffset = 0;
    TParameter *pParameter = NULL;
    TSection *pCurSection = NULL;
    char szLine[LENGTH_MAX << 4] = "";
    char szSection[LENGTH_MAX] = "";
    TParameter tParameter;

    for(int i = 0; i < m_nBufferLength; ++i)
    {
        //! hex:0A	LF (����)
        if(0x0A == m_pBuffer[i] || i == m_nBufferLength - 1)
        {
            memset(szLine, 0x00, sizeof(szLine));
            memset(szSection, 0x00, sizeof(szSection));
            memcpy(szLine, m_pBuffer + nOffset, i - nOffset);
            if(0x0A == szLine[0])
            {
                szLine[0] = 0x20; //! ���� -> �ո�
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
                        SetErrMsg("�ڴ����ʧ��");
                        return(-1);
                    }
                    memset(pCurSection->m_szSection, 0x00, sizeof(pCurSection->m_szSection));
                    strncpy(pCurSection->m_szSection, szSection, sizeof(pCurSection->m_szSection) - 1);

                    QUEUE_INIT(&pCurSection->m_qParamQueueHead);
                    QUEUE_INSERT_TAIL(&m_qSectionQueueHead, &pCurSection->m_qSectionQueue);
                }
            }
            else
            {
                pCurSection = (TSection *)malloc(sizeof(TSection));
                if(NULL == pCurSection)
                {
                    SetErrMsg("�ڴ����ʧ��");
                    return(-1);
                }
                memset(pCurSection->m_szSection, 0x00, sizeof(pCurSection->m_szSection));
                strncpy(pCurSection->m_szSection, "DEFAULT", sizeof(pCurSection->m_szSection) - 1);

                QUEUE_INIT(&pCurSection->m_qParamQueueHead);
                QUEUE_INSERT_TAIL(&m_qSectionQueueHead, &pCurSection->m_qSectionQueue);
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
                        SetErrMsg("�ڴ����ʧ��");
                        return(-1);
                    }
                    memcpy(temp, &tParameter, sizeof(temp));
                    QUEUE_INSERT_TAIL(&pCurSection->m_qParamQueueHead, &temp->m_qParameterQueue);
                }
            }
            nOffset = i;
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
        SetErrMsg("��������δ����");
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
        SetErrMsg("�ļ�%s��ʧ��", pFileName);
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



