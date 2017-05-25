#ifndef __LOG_INC__
#define __LOG_INC__

#include <cstring> ///< memset
#include <cstdarg> ///< va_lisp


///////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_TIME_LENGTH				25       ///< 时间格式长度 yyyymmdd hh:mm:ss.123456
#define MAX_NAME_LENGTH	            33       ///< 文件名长度
#define MAX_PATH_LENGTH             512      ///< 路径长度

//定义写日志的级别宏
#define LL_DEBUG                    10	     ///< 调试级别，最低级，任何消息都写
#define LL_MSG                      20       ///< 正常的操作提示级别，一般是可以控制程序执行过程中的提示
#define LL_WARING                   30       ///< 告警级别，系统不会停止的告警消息
#define LL_ERROR                    40       ///< 错误级别，会引起系统停止的错误消息


///////////////////////////////////////////////////////////////////////////////////////////////////
//前置声明
class CLoggerMgr;
//定义当前日志级别和开关
//extern int g_nLogLevel;
//extern int g_nTraceOn;
//extern CLoggerMgr g_loggerMgr;


///////////////////////////////////////////////////////////////////////////////////////////////////
/// 定义直接操作日志的宏函数，这样的函数可以加快执行正常情况下的执行速度
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
	char szTime[MAX_TIME_LENGTH];  ///< 日志时间
	char szFile[MAX_NAME_LENGTH];  ///< 文件名称
	char *pLogData; ///< 日志内容
	
	int nLogLevel;           ///< 日志级别
	unsigned int nLine;      ///< 文件行数

	int nPid;      ///< 进程ID
	int nThreadId; ///< 线程ID
	
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
	** 把日志写入文件
	** 
	** @param Tlogcontext  日志结构体
	** @return int 0——成功，其他失败
	*/ 
	int WriteLogToFile(Tlogcontext* ptLogcontext); 
	
	/*
	** @fn PrintLogByString
	** @brief  
	** 
	** 把字符串写入到日志文件中。
	** 
	** @param nLevel : 
	** @param nLine  :
	** @param szFile : 
	** @param szMsg  :
	** @param vl     :不定参数类型的参数地址
	** @return int 0——成功，其他失败
	*/ 
	int PrintLogByString(int nLevel, int nLine, const char * szFile, const char *szMsg, va_list vl);
	
	
	
	
public:
	void   SetLogLevel(const int nLevel)   { m_nLogLevel = nLevel; } 
	int    GetLogLevel() const             { return m_nLogLevel;   } 
	static CLoggerMgr* GetInstance()       { return &m_cLoggerMgr; }
	
	unsigned long GetLogSize() const       { return m_lLogSize; }
	
private:
	unsigned long     m_lLogSize;                     ///< 日志大小
	int               m_nLogLevel;                    ///< 日志级别
	char              m_szLogPath[MAX_PATH_LENGTH];   ///<日志存放的位置
	
	void (*xLog)(int, int, const char*, const char *, va_list); ///< Function for logging 
	
	Tlogcontext*      m_pCurLog;    ///<  日志内容
	static CLoggerMgr m_cLoggerMgr; ///<  日志管理器实例
};



#endif//__LOG_INC__
