#pragma once

#include "stdafx.h"
#include "Utility.h"

//图片的最大规格
#define IMAGE_MAX_WIDTH 800			//图片最大
#define IMAGE_MAX_HEIGHT 800

//人脸图像的规格
#define FACE_WIDTH 256			//人脸大小
#define FACE_HEIGHT 256

//读取图像
#define IMAGE_READ_DEPTH IPL_DEPTH_8U
#define IMAGE_READ_CHANNELS 3	//1-灰度图像， 3-彩色图像

#define IMAGE_READ_ISCOLOR	1		//读入图像类型，1-原图像， 0 - 强制灰度图像

//灰度图像
#define IMAGE_GRAY_DEPTH IPL_DEPTH_8U
#define IMAGE_GRAY_CHANNELS 1

class CCheck
{
public:
	CCheck(void);
	~CCheck(void);
//Attributes:
public:
	IplImage* m_ReadImage1;		//用以读取待测人脸
	IplImage* m_ReadImage2;	
	IplImage* m_ReadImage3;	
	CString m_strErrMessage;					//出错信息
//Operations：
public:
	//引用:
	void Init();							//初始化图像
	bool Load(CString strPath);				//载入图像
	bool SetImage( IplImage* image, int maxWidth=0, int maxHeight=0);
	bool SetReadImage( IplImage* image, int maxWidth=0, int maxHeight=0 );	//设置图片大小并保存到m_pReadImage/m_pGrayImage/m_pBinaryImage
	bool SetGrayImage( IplImage* image = NULL);		//设置图片大小并保存到m_readImage
	void Release();						//释放资源

	//自己：

};
