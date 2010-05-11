#pragma once

class CUtility
{
public:
	CUtility(void);
	~CUtility(void);

public:
	BOOL FileExist(CString FileName);	//判断文件是否存在
	BOOL DirExist(CString Path);		//判断目录是否存在
	BOOL CreateDir(CString path);		//创建目录
	CString RandDate();				//把当前时间变成字符串（文件命名）
};


