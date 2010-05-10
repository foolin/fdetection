#include "StdAfx.h"
#include "Config.h"

CConfig::CConfig(void)
{
	m_strConfigPath = _T(".\\config.ini");
}

CConfig::~CConfig(void)
{
}

//设置变量
void CConfig::SetConfig(CString keyName, CString keyValue)
{
	::WritePrivateProfileString(_T("Init"), keyName, keyValue, m_strConfigPath);
}

void CConfig::SetConfig(CString appName, CString keyName, CString keyValue)
{
	::WritePrivateProfileString( appName, keyName, keyValue, m_strConfigPath);
}

//读取变量
CString CConfig::GetConfig(CString keyName)
{
	//读取配置文件
	CString strValue;
	::GetPrivateProfileString(_T("Init"), keyName, _T(""), strValue.GetBuffer(MAX_PATH),
							MAX_PATH , m_strConfigPath);
	return strValue;
}

CString CConfig::GetConfig(CString appName, CString keyName)
{
	//读取配置文件
	CString strValue;
	::GetPrivateProfileString( appName, keyName, _T(""), strValue.GetBuffer(MAX_PATH),
							MAX_PATH , m_strConfigPath);
	return strValue;
}
