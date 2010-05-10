#pragma once
#include "afxwin.h"

class CConfig :
	public CDialog
{
public:
	CConfig(void);
	~CConfig(void);

public:
	CString m_strConfigPath;

public:
	//���ñ���
	void SetConfig(CString keyName, CString keyValue);
	void SetConfig(CString appName, CString keyName, CString keyValue);

	//��ȡ����
	CString GetConfig(CString keyName);
	CString GetConfig(CString appName, CString keyName);

};
