#pragma once

/*
class CHelper
{
public:
	CHelper(void);
	~CHelper(void);
public:
	BOOL FileExist(CString FileName);	//判断文件是否存在
	BOOL DirExist(CString Path);		//判断目录是否存在
	BOOL CreateDir(CString path);		//创建目录
};
*/

BOOL MyFileExist(CString FileName);	//判断文件是否存在
BOOL MyDirExist(CString Path);		//判断目录是否存在
BOOL MyCreateDir(CString path);		//创建目录
CString MyGetRand();				//把当前时间变成字符串（文件命名）

