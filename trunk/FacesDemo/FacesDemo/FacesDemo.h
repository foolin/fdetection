// FacesDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#include "cv.h"
#include "highgui.h"

#include <stdarg.h>
#include <string.h>
#include <time.h>

#define IMAGE_WIDTH 264		//��ʾͼƬ��С
#define IMAGE_HEIGHT 264	
#define IMAGE_CHANNELS 3
#define IMAGE_MAX_WIDTH 800			//ͼƬ���
#define IMAGE_MAX_HEIGHT 600


// CFacesDemoApp:
// �йش����ʵ�֣������ FacesDemo.cpp
//

class CFacesDemoApp : public CWinApp
{
public:
	CFacesDemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFacesDemoApp theApp;