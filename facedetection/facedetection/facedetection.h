// facedetection.h : facedetection Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CfacedetectionApp:
// �йش����ʵ�֣������ facedetection.cpp
//

class CfacedetectionApp : public CWinApp
{
public:
	CfacedetectionApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CfacedetectionApp theApp;