#pragma once

class CUtility
{
public:
	CUtility(void);
	~CUtility(void);

public:
	BOOL FileExist(CString FileName);	//�ж��ļ��Ƿ����
	BOOL DirExist(CString Path);		//�ж�Ŀ¼�Ƿ����
	BOOL CreateDir(CString path);		//����Ŀ¼
	CString RandDate();				//�ѵ�ǰʱ�����ַ������ļ�������
};


