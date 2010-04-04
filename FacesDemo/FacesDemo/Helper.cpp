#include "StdAfx.h"
#include "Helper.h"

/*
CHelper::CHelper(void)
{
}

CHelper::~CHelper(void)
{
}

//判断文件是否存在
BOOL CHelper::FileExist(CString FileName){
    CFileFind fFind;
    return fFind.FindFile(FileName);
}

//判断目录是否存在
BOOL CHelper::DirExist(CString Path){
    WIN32_FIND_DATA fd;
    BOOL ret = FALSE;
    HANDLE hFind = FindFirstFile(Path, &fd);
    if ((hFind != INVALID_HANDLE_VALUE) &&
		(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))   
	{
      	//目录存在
		ret = TRUE;
    }
    FindClose(hFind);
    return ret;
}

//创建目录
BOOL CHelper::CreateDir(CString path){
    SECURITY_ATTRIBUTES attrib;
    attrib.bInheritHandle = FALSE;
    attrib.lpSecurityDescriptor = NULL;
    attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
     return ::CreateDirectory( path, &attrib);
} 
*/


//判断文件是否存在
BOOL MyFileExist(CString FileName)
{
    CFileFind fFind;
    return fFind.FindFile(FileName);
}

/*
//判断目录是否存在(经测试无效)
BOOL MyDirExist(CString Path){
    WIN32_FIND_DATA fd;
    BOOL ret = FALSE;
    HANDLE hFind = FindFirstFile(Path, &fd);
    if ((hFind != INVALID_HANDLE_VALUE) &&
		(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))   
	{
      	//目录存在
		ret = TRUE;
    }
    FindClose(hFind);
    return ret;
}
*/

BOOL MyDirExist(CString Path)
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
BOOL MyCreateDir(CString path)
{
    SECURITY_ATTRIBUTES attrib;
    attrib.bInheritHandle = FALSE;
    attrib.lpSecurityDescriptor = NULL;
    attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
     return ::CreateDirectory( path, &attrib);
}

CString MyGetRand()
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

