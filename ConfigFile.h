/*!
 * @file      ConfigFile.h
 * @brief     配置文件读取类
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
    ** @brief     获取类实例
    ** @return    CConfigFile*
    */
    static CConfigFile *GetInstance();

    /*!
    ** @name        Load
    ** @brief       加载配置文件
    ** @param[in]   pszFileName 配置文件名
    ** @return      int
    ** @retval      0  装载成功
    ** @retval      -1 装载失败
    */
    int Load(const char *pszFileName = NULL);

    /*!
    ** @name      GetParamValue
    ** @brief     根据section和parameter name，获取parameter value
    ** @param[in]    pszSection
    ** @param[in]    pszParamName
    ** @param[out]   pszParamValue
    ** @param[in]    nLength
    ** @return
    */
    int GetParamValue(const char *pszSection, const char *pszParamName, char *pszParamValue, int nLength);


    /*!
    ** @name        GetLastErrMsg
    ** @brief       获取最后错误信息
    ** @param[in]   无
    ** @return      错误信息指针
    */
    const char *GetLastErrMsg() const;

private:
    enum {LENGTH_MAX = 256};

    //! parameters define
    struct tagParameter
    {
        char m_szName[LENGTH_MAX];   //!< name
        char m_szValue[LENGTH_MAX]; //!< value
        QUEUE m_qParameterQueue; //!< parameter队列
    };
    typedef struct tagParameter TParameter;

    //! sections define
    struct tagSection
    {
        char m_szSection[LENGTH_MAX]; //!< section
        QUEUE m_qSectionQueue;   //!< section队列
        QUEUE m_qParamQueueHead;   //!< parameter队列头
    };
    typedef struct tagSection TSection;

private:
    /*!
    ** @name      AnalyBuffer
    ** @brief     对读入的配置文件进行分析，按配置项和对应参数进行分解
    ** @return    int
    ** @retval    0   分析成功
    ** @retval    -1  分析不成功
    */
    int AnalyBuffer();

    /*!
    ** @name         GetSection
    ** @brief        对读入的配置文件进行分析，根据[]进行分解，存放到pKey变量中
    ** @param[in]    pszLine  行数据
    ** @param[out]   pszSection  section缓存区
    ** @param[out]   nLength     缓存区大小
    ** @return       0 成功; 否则, 失败
    */
    int GetSection(const char *pszLine, char *pszSection, int nLength);

    /*!
    ** @name         GetSection
    ** @brief        对读入的配置文件进行分析，根据=进行分解，存放到tParameter变量中
    ** @param[in]    pszLine  行数据
    ** @param[out]   tParameter  name-value存放区
    ** @return       0 成功; 否则, 失败
    */
    int GetParam(const char *pszLine, TParameter &tParameter);

    /*!
    ** @name        SetConfigFile
    ** @brief       设置文件名
    ** @param[in]   pFileName 文件名
    ** @return      void
    */
    void SetConfigFile(const char *pFileName);

    /*!
    ** @name        CheckFileExist
    ** @brief       检查输入的文件名是否存在
    ** @param[in]   pFileName 文件名
    ** @return      int
    ** @retval      0   检查文件存在
    ** @retval      -1	检查文件不存在
    */
    int CheckFileExist(const char *pFileName);

    /*!
    ** @name        SetErrMsg
    ** @brief       设置错误信息
    ** @param[in]   fmt 格式
    ** @return      void
    */
    void SetErrMsg(const char *fmt, ...);

private:
    static CConfigFile m_Instance; //!类实例

    char m_szConfigFile[LENGTH_MAX]; //!< 配置文件名，包括绝对路径和文件名
    char *m_pBuffer; //!< 用来存放配置文件内容
    int m_nBufferLength; //!< 缓存区大小
    char m_szCfgErrMsg[LENGTH_MAX << 2]; //!< 错误信息
    QUEUE m_qSectionQueueHead; //!< section队列头
};

#endif//!_CONFIG_FILE_H_
