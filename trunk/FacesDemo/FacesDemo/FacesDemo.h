// FacesDemo.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#include "cv.h"
#include "highgui.h"

#include <stdarg.h>
#include <string.h>
#include <time.h>

#define IMAGE_WIDTH 266		//显示图片大小
#define IMAGE_HEIGHT 266	
#define IMAGE_CHANNELS 3
#define IMAGE_MAX_WIDTH 1000			//图片最大
#define IMAGE_MAX_HEIGHT 1000
#define FACE_WIDTH 128			//人脸大小
#define FACE_HEIGHT 128


// CFacesDemoApp:
// 有关此类的实现，请参阅 FacesDemo.cpp
//

class CFacesDemoApp : public CWinApp
{
public:
	CFacesDemoApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CFacesDemoApp theApp;