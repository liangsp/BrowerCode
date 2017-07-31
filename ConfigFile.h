/*!
 * @file      ConfigFile.h
 * @brief     �����ļ���ȡ��
 *
 * Copyright (c) 2015 chinasoft Tech.Co.,Ltd
 *
 * @author    lsp74391
 * @date      2017/7/25
 * @version   1.0.0
 *
*******************************************************************************
 */
#ifndef _CONFIG_FILE_H_
#define _CONFIG_FILE_H_

#include "queue.h"

class CConfigFile
{
public:
    CConfigFile();
    virtual ~CConfigFile();

    /*!
    ** @name      GetInstance
    ** @brief     ��ȡ��ʵ��
    ** @return    CConfigFile*
    */
    static CConfigFile* GetInstance();

    /*!
    ** @name        Load
    ** @brief       ���������ļ�
    ** @param[in]   pszFileName �����ļ���
    ** @return      int
    ** @retval      0  װ�سɹ�
    ** @retval      -1 װ��ʧ��
    */
    int Load(const char *pszFileName = NULL);

    /*!
    ** @name      GetParamValue
    ** @brief     ����section��parameter name����ȡparameter value
    ** @param[in]    pszSection
    ** @param[in]    pszParamName
    ** @param[out]   pszParamValue
    ** @param[in]    nLength
    ** @return    
    */
    int GetParamValue(const char *pszSection, const char *pszParamName, char *pszParamValue, int nLength);

    /*!
    ** @name        GetLastErrMsg
    ** @brief       ��ȡ��������Ϣ
    ** @param[in]   ��
    ** @return      ������Ϣָ��
    */
    const char *GetLastErrMsg() const;

private:
    enum {LENGTH_MAX = 256};

    //! parameters define
    struct tagParameter
    {
        char m_szName[LENGTH_MAX];   //!< name
        char m_szValue[LENGTH_MAX]; //!< value
        QUEUE m_qParameterQueue; //!< parameter����
    };
    typedef struct tagParameter TParameter;

    //! sections define
    struct tagSection
    {
        char m_szSection[LENGTH_MAX]; //!< section
        QUEUE m_qSectionQueue;   //!< section����
        QUEUE m_qParamQueueHead;   //!< parameter����ͷ
    };
    typedef struct tagSection TSection;

private:
    /*!
    ** @name      AnalyBuffer
    ** @brief     �Զ���������ļ����з�������������Ͷ�Ӧ�������зֽ�
    ** @return    int
    ** @retval    0   �����ɹ�
    ** @retval    -1  �������ɹ�
    */
    int AnalyBuffer();

    /*!
    ** @name         GetSection
    ** @brief        �Զ���������ļ����з���������[]���зֽ⣬��ŵ�pKey������
    ** @param[in]    pszLine  ������
    ** @param[out]   pszSection  section������
    ** @param[out]   nLength     ��������С
    ** @return       0 �ɹ�; ����, ʧ��
    */
    int GetSection(const char *pszLine, char *pszSection, int nLength);

    /*!
    ** @name         GetSection
    ** @brief        �Զ���������ļ����з���������=���зֽ⣬��ŵ�tParameter������
    ** @param[in]    pszLine  ������
    ** @param[out]   tParameter  name-value�����
    ** @return       0 �ɹ�; ����, ʧ��
    */
    int GetParam(const char *pszLine, TParameter &tParameter);

    /*!
    ** @name        SetConfigFile
    ** @brief       �����ļ���
    ** @param[in]   pFileName �ļ���
    ** @return      void
    */
    void SetConfigFile(const char *pFileName);

    /*!
    ** @name        CheckFileExist
    ** @brief       ���������ļ����Ƿ����
    ** @param[in]   pFileName �ļ���
    ** @return      int
    ** @retval      0   ����ļ�����
    ** @retval      -1	����ļ�������
    */
    int CheckFileExist(const char *pFileName);

    /*!
    ** @name        SetErrMsg
    ** @brief       ���ô�����Ϣ
    ** @param[in]   fmt ��ʽ
    ** @return      void
    */
    void SetErrMsg(const char *fmt, ...);

private:
    static CConfigFile m_Instance; //!��ʵ��
    char m_szConfigFile[LENGTH_MAX]; //!< �����ļ�������������·�����ļ���
    char *m_pBuffer; //!< ������������ļ�����
    int m_nBufferLength; //!< ��������С
    char m_szCfgErrMsg[LENGTH_MAX << 2]; //!< ������Ϣ
    QUEUE m_qSectionQueueHead; //!< section����ͷ
};

#endif//!_CONFIG_FILE_H_
