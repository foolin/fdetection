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

#define IMAGE_WIDTH 266		//��ʾͼƬ��С
#define IMAGE_HEIGHT 266	
#define IMAGE_CHANNELS 3
#define IMAGE_MAX_WIDTH 1000			//ͼƬ���
#define IMAGE_MAX_HEIGHT 1000
#define FACE_WIDTH 128			//������С
#define FACE_HEIGHT 128


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