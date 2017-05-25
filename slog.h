#ifndef __LOG_INC__
#define __LOG_INC__

#include <cstring> ///< memset
#include <cstdarg> ///< va_lisp


///////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_TIME_LENGTH				25       ///< ʱ���ʽ���� yyyymmdd hh:mm:ss.123456
#define MAX_NAME_LENGTH	            33       ///< �ļ�������
#define MAX_PATH_LENGTH             512      ///< ·������

//����д��־�ļ����
#define LL_DEBUG                    10	     ///< ���Լ�����ͼ����κ���Ϣ��д
#define LL_MSG                      20       ///< �����Ĳ�����ʾ����һ���ǿ��Կ��Ƴ���ִ�й����е���ʾ
#define LL_WARING                   30       ///< �澯����ϵͳ����ֹͣ�ĸ澯��Ϣ
#define LL_ERROR                    40       ///< ���󼶱𣬻�����ϵͳֹͣ�Ĵ�����Ϣ


///////////////////////////////////////////////////////////////////////////////////////////////////
//ǰ������
class CLoggerMgr;
//���嵱ǰ��־����Ϳ���
//extern int g_nLogLevel;
//extern int g_nTraceOn;
//extern CLoggerMgr g_loggerMgr;


///////////////////////////////////////////////////////////////////////////////////////////////////
/// ����ֱ�Ӳ�����־�ĺ꺯���������ĺ������Լӿ�ִ����������µ�ִ���ٶ�
//#define TL_DEBUG	if((g_nLogLevel <= LL_DEBUG  || g_nTraceOn) && g_loggerMgr.MsgPre(LL_DEBUG ,__LINE__,__FILE__)) g_loggerMgr.MsgWrite
//#define TL_MSG		if((g_nLogLevel <= LL_MSG    || g_nTraceOn) && g_loggerMgr.MsgPre(LL_MSG   ,__LINE__,__FILE__)) g_loggerMgr.MsgWrite
//#define TL_WARNING	if((g_nLogLevel <= LL_WARING || g_nTraceOn) && g_loggerMgr.MsgPre(LL_WARING,__LINE__,__FILE__)) g_loggerMgr.MsgWrite
//#define TL_ERROR	if((g_nLogLevel <= LL_ERROR  || g_nTraceOn) && g_loggerMgr.MsgPre(LL_ERROR ,__LINE__,__FILE__)) g_loggerMgr.MsgWrite
#define TL_DEBUG	if(CLoggerMgr::GetInstance()->MsgPre(LL_DEBUG ,__LINE__,__FILE__)) CLoggerMgr::GetInstance()->MsgWrite
#define TL_MSG		if(CLoggerMgr::GetInstance()->MsgPre(LL_MSG   ,__LINE__,__FILE__)) CLoggerMgr::GetInstance()->MsgWrite
#define TL_WARNING	if(CLoggerMgr::GetInstance()->MsgPre(LL_WARING,__LINE__,__FILE__)) CLoggerMgr::GetInstance()->MsgWrite
#define TL_ERROR	if(CLoggerMgr::GetInstance()->MsgPre(LL_ERROR ,__LINE__,__FILE__)) CLoggerMgr::GetInstance()->MsgWrite


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Tlogcontext
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef   struct logContext    Tlogcontext;
struct logContext
{
	char szTime[MAX_TIME_LENGTH];  ///< ��־ʱ��
	char szFile[MAX_NAME_LENGTH];  ///< �ļ�����
	char *pLogData; ///< ��־����
	
	int nLogLevel;           ///< ��־����
	unsigned int nLine;      ///< �ļ�����

	int nPid;      ///< ����ID
	int nThreadId; ///< �߳�ID
	
	logContext()
	{
		memset(szTime, 0x00, sizeof(szTime));
		memset(szFile, 0x00, sizeof(szFile));
		pLogData = 0;
		nLogLevel = LL_DEBUG;
		nLine = 0;
		nPid = 0;
		nThreadId = 0;
	}
	
	~logContext()
	{
		if(pLogData){
			delete [] pLogData;
		}
		pLogData = 0;
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief CLoggerMgr
///////////////////////////////////////////////////////////////////////////////////////////////////
class CLoggerMgr
{
public:
	CLoggerMgr();
	virtual ~CLoggerMgr();
	
	
	int MsgPre(int nLevel, int nLine, const char *szFile);
	void MsgWrite(const char *fmt, ... );
	
	
private:	
	/*
	** @fn WriteLogToFile
	** @brief 
	** 
	** ����־д���ļ�
	** 
	** @param Tlogcontext  ��־�ṹ��
	** @return int 0�����ɹ�������ʧ��
	*/ 
	int WriteLogToFile(Tlogcontext* ptLogcontext); 
	
	/*
	** @fn PrintLogByString
	** @brief  
	** 
	** ���ַ���д�뵽��־�ļ��С�
	** 
	** @param nLevel : 
	** @param nLine  :
	** @param szFile : 
	** @param szMsg  :
	** @param vl     :�����������͵Ĳ�����ַ
	** @return int 0�����ɹ�������ʧ��
	*/ 
	int PrintLogByString(int nLevel, int nLine, const char * szFile, const char *szMsg, va_list vl);
	
	
	
	
public:
	void   SetLogLevel(const int nLevel)   { m_nLogLevel = nLevel; } 
	int    GetLogLevel() const             { return m_nLogLevel;   } 
	static CLoggerMgr* GetInstance()       { return &m_cLoggerMgr; }
	
	unsigned long GetLogSize() const       { return m_lLogSize; }
	
private:
	unsigned long     m_lLogSize;                     ///< ��־��С
	int               m_nLogLevel;                    ///< ��־����
	char              m_szLogPath[MAX_PATH_LENGTH];   ///<��־��ŵ�λ��
	
	void (*xLog)(int, int, const char*, const char *, va_list); ///< Function for logging 
	
	Tlogcontext*      m_pCurLog;    ///<  ��־����
	static CLoggerMgr m_cLoggerMgr; ///<  ��־������ʵ��
};



#endif//__LOG_INC__
