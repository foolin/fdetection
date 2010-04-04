#include "StdAfx.h"
#include "Helper.h"

/*
CHelper::CHelper(void)
{
}

CHelper::~CHelper(void)
{
}

//�ж��ļ��Ƿ����
BOOL CHelper::FileExist(CString FileName){
    CFileFind fFind;
    return fFind.FindFile(FileName);
}

//�ж�Ŀ¼�Ƿ����
BOOL CHelper::DirExist(CString Path){
    WIN32_FIND_DATA fd;
    BOOL ret = FALSE;
    HANDLE hFind = FindFirstFile(Path, &fd);
    if ((hFind != INVALID_HANDLE_VALUE) &&
		(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))   
	{
      	//Ŀ¼����
		ret = TRUE;
    }
    FindClose(hFind);
    return ret;
}

//����Ŀ¼
BOOL CHelper::CreateDir(CString path){
    SECURITY_ATTRIBUTES attrib;
    attrib.bInheritHandle = FALSE;
    attrib.lpSecurityDescriptor = NULL;
    attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
     return ::CreateDirectory( path, &attrib);
} 
*/


//�ж��ļ��Ƿ����
BOOL MyFileExist(CString FileName)
{
    CFileFind fFind;
    return fFind.FindFile(FileName);
}

/*
//�ж�Ŀ¼�Ƿ����(��������Ч)
BOOL MyDirExist(CString Path){
    WIN32_FIND_DATA fd;
    BOOL ret = FALSE;
    HANDLE hFind = FindFirstFile(Path, &fd);
    if ((hFind != INVALID_HANDLE_VALUE) &&
		(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))   
	{
      	//Ŀ¼����
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


//����Ŀ¼
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
	CString str = _T("");	//�����ַ���
	CTime t = CTime::GetCurrentTime();	//��ǰʱ��
	//���������
	int ra = 0;		//�����
	srand( (unsigned) time( NULL ) ); 
	ra = rand() % 100 + 1;
	//��ʽ��
	str.Format(_T("%d%d%d%d%d%d%d"), t.GetYear(), t.GetMonth(), t.GetDay(),
				t.GetHour(), t.GetMinute(), t.GetSecond(), ra);
	return str;
}

