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
	//设置变量
	void SetConfig(CString keyName, CString keyValue);
	void SetConfig(CString appName, CString keyName, CString keyValue);

	//读取变量
	CString GetConfig(CString keyName);
	CString GetConfig(CString appName, CString keyName);

};
