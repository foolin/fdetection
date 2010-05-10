#include "StdAfx.h"
#include "Utility.h"

CUtility::CUtility(void)
{
}

CUtility::~CUtility(void)
{
}



//判断文件是否存在
BOOL CUtility::FileExist(CString FileName)
{
    CFileFind fFind;
    return fFind.FindFile(FileName);
}


BOOL CUtility::DirExist(CString Path)
{
	BOOL ret = FALSE;
	CFileFind   fFind;   
	if(fFind.FindFile(Path))
	{
		ret = TRUE;
	}
	else
	{
		ret = FALSE;
	}
 return ret;
}


//创建目录
BOOL CUtility::CreateDir(CString path)
{
    SECURITY_ATTRIBUTES attrib;
    attrib.bInheritHandle = FALSE;
    attrib.lpSecurityDescriptor = NULL;
    attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
     return ::CreateDirectory( path, &attrib);
}

CString CUtility::RandDate()
{
	CString str = _T("");	//返回字符串
	CTime t = CTime::GetCurrentTime();	//当前时间
	//产生随机数
	int ra = 0;		//随机数
	srand( (unsigned) time( NULL ) ); 
	ra = rand() % 100 + 1;
	//格式化
	str.Format(_T("%d%d%d%d%d%d%d"), t.GetYear(), t.GetMonth(), t.GetDay(),
				t.GetHour(), t.GetMinute(), t.GetSecond(), ra);
	return str;
}
