#ifndef __CTIME_INC__
#define __CTIME_INC__


class CTime
{
public:	
	/*
	** @brief GetCurrentTime  ȡ��ǰʱ��, 14λ���� YYYYMMDDHH24MISS
	**        
	** @param fmt  eg: %Y%m%d%H%M%S
	** @param time 
	** @param length
	**  
	** @return ret code
	**
	** @see strftime <ctime>
	** http://www.cplusplus.com/reference/ctime/strftime/
	**
	*/
	static int format(const char* fmt, char* time, int length);
	
	/*
	** @brief GetCurrentTime  ȡ��ǰʱ��, 14λ���� YYYYMMDDHH24MISS
	**        
	** @param szCurDate
	** @param length
	**  
	** @return ��ǰʱ��
	**
	*/
	static char* GetCurrentTime(char *szCurDate, int length);
	
	/*
	** @brief GetCurrentTime  ȡ��ǰʱ��, YYYYMMDD HH24:MI:SS:123456��΢�뼶
	**        
	** @param szCurDate
	** @param length
	**  
	** @return ��ǰʱ��
	**
	*/
	static char* GetCurTimeMicSecond(char *szCurDate, int length);
	
	/*
	** @brief GetMonth  ȡ��ǰ�·�
	**         
	** @return ��ǰ�·�
	**
	*/
	static int GetMonth();
	
	/*
	** @brief GetMonth  ȡ�����ַ������·�
	** 
	** @param szCurDate
	**        
	** @return ��ǰ�·�
	**
	*/
	static int GetMonth(char *szDate);
	
	/*
	** @brief GetMonth  ȡ��ǰ����
	**         
	** @return ��ǰday
	**
	*/
	static int GetDay();
	
	/*
	** @brief GetMonth  ȡ�����ַ�������
	** 
	** @param szDate
	**        
	** @return ��ǰ����
	**
	*/
	static int GetDay(char *szDate);
	
	/*
	** @brief GetWeekDay  ȡ��ǰ���ڵ�����(1-7)
	**         
	** @return ��ǰ���ڵ�����
	**
	*/
	static int GetWeekDay();
	
	
	/*
	** @brief BeginTime  ��ʼ��ʱ    
	**
	*/
	static void BeginTime();
	
	/*
	** @brief EndTime  ��ʱ����    
	**
	** @return ���غ���
	*/
	static long EndTime();
};

#endif//__CTIME_INC__
