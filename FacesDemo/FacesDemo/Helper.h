#pragma once

/*
class CHelper
{
public:
	CHelper(void);
	~CHelper(void);
public:
	BOOL FileExist(CString FileName);	//�ж��ļ��Ƿ����
	BOOL DirExist(CString Path);		//�ж�Ŀ¼�Ƿ����
	BOOL CreateDir(CString path);		//����Ŀ¼
};
*/

BOOL MyFileExist(CString FileName);	//�ж��ļ��Ƿ����
BOOL MyDirExist(CString Path);		//�ж�Ŀ¼�Ƿ����
BOOL MyCreateDir(CString path);		//����Ŀ¼
CString MyGetRand();				//�ѵ�ǰʱ�����ַ������ļ�������

