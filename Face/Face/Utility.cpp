#include "StdAfx.h"
#include "Utility.h"

CUtility::CUtility(void)
{
}

CUtility::~CUtility(void)
{
}



//�ж��ļ��Ƿ����
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


//����Ŀ¼
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
