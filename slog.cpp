#include "slog.h"
#include "CTime.h"
#include <pthread.h> ///< pthread_t pthread_self(void);
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <new>    ///< std::nothrow

//int g_nLogLevel = 20;	             ///< 缺省的系统日志记录级别
//int g_nTraceOn  = 0;	             ///< 是否打开Trace
//CLoggerMgr g_loggerMgr;              ///< 日志管理器


typedef struct __inner_log_level_string   __identifier;
struct __inner_log_level_string{
	int   nlevel;
	char  szMsg[16];
};


static const __identifier g_logLevel[] = {
	{LL_DEBUG,    "DEBUG"},
	{LL_MSG,      "MSG"},
	{LL_WARING,   "WARNING"},
	{LL_ERROR,    "ERROR"}
};


//按指定格式转换成字符串,不会溢出
char * g_vsprintf(const char *fmt, va_list ap)
{
    int iLength = 4096;
    char *pBuffer = new char[iLength];

    //如果申请的缓冲区不够大,则重新申请
    ///< TODO 不同系统vsnprintf返回值可能不同
    while(vsnprintf(pBuffer, iLength, fmt, ap) <= 0)
    {
        delete pBuffer;
        iLength = (int)(iLength * 1.25);
        pBuffer = new char[iLength];
    }

    return pBuffer;
};

unsigned long GetFileSize(const char * path)
{
	unsigned long lFileSize = -1;
	struct stat statBuffer;
	
	if(stat(path, &statBuffer) == -1)
		return lFileSize;
		
	lFileSize = statBuffer.st_size;
	
	return lFileSize;
}


///////////////////////////////////////////////////////////////////////////////////
///< CLoggrMgr静态成员函数初始化
CLoggerMgr CLoggerMgr::m_cLoggerMgr;

///////////////////////////////////////////////////////////////////////////////////
/// @brief  CLoggerMgr的实现
///////////////////////////////////////////////////////////////////////////////////	
CLoggerMgr::CLoggerMgr(): m_lLogSize(0L), \
	  m_nLogLevel(LL_ERROR), \
	  xLog(0), \
	  m_pCurLog(0)
{
	memset(m_szLogPath, 0x00, sizeof(m_szLogPath));
	strncpy(m_szLogPath, "/app/bjabmdev/lsp/dev/unit_test/log", sizeof(m_szLogPath));
	m_lLogSize = GetFileSize("/app/bjabmdev/nohup.out");
}

CLoggerMgr::~CLoggerMgr()
{
	
}

int CLoggerMgr::MsgPre(int nLevel, int nLine, const char *szFile)
{
	const char *pLastName = 0;
	m_pCurLog = new(std::nothrow) Tlogcontext();
    m_pCurLog->nLogLevel = nLevel;
    m_pCurLog->nLine = nLine;
    m_pCurLog->nPid = (int)getpid();;
    pLastName = strrchr(szFile, '/');
    if(pLastName)
        strncpy(m_pCurLog->szFile, pLastName+1, sizeof(m_pCurLog->szFile));
    else
    	strncpy(m_pCurLog->szFile, szFile, sizeof(m_pCurLog->szFile));
    
    return 1;
}

void CLoggerMgr::MsgWrite(const char *fmt, ... )
{
	if(GetLogLevel() <= m_pCurLog->nLogLevel)
	{
		va_list vl;
		va_start(vl, fmt);
		m_pCurLog->pLogData = g_vsprintf(fmt, vl);
		WriteLogToFile(m_pCurLog);
		va_end(vl);
	}
	
	if(m_pCurLog) delete m_pCurLog;
	m_pCurLog = 0;
}

int CLoggerMgr::WriteLogToFile(Tlogcontext* ptLogcontext)
{
	FILE *fp;
	
	char szFileName[MAX_NAME_LENGTH] = "";
	char szFile[MAX_PATH_LENGTH+MAX_NAME_LENGTH] = "";
	strncpy(szFileName,"test.log", sizeof(szFileName));
	snprintf(szFile, sizeof(szFile), "%s/%s", m_szLogPath, szFileName);
	
	fp = fopen(szFile, "at");
	if(NULL == fp) {return -1;}
	
	
	CTime::GetCurTimeMicSecond(ptLogcontext->szTime, sizeof(ptLogcontext->szTime));
	
	fprintf(fp, "%-15s|%9d|%8s[%2d]|%20s|%5d| %s\n", \
				ptLogcontext->szTime+9, \
				ptLogcontext->nPid, \
				g_logLevel[ptLogcontext->nLogLevel/10-1].szMsg, \
				ptLogcontext->nLogLevel, \
				ptLogcontext->szFile, \
				ptLogcontext->nLine, \
				ptLogcontext->pLogData);
	       
    fflush(fp);
    fclose(fp);	       
	
	return 1;
}

int PrintLogByString(int nLevel, int nLine, const char * szFile, const char *szMsg, va_list vl)
{
	return 1;
}




