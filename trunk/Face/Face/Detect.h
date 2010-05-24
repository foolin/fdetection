#pragma once

//#ifndef MY_OPENCV
//	#include "cv.h"
//	#include "highgui.h"
//	#define MY_OPENCV  1          //标记为已定义
//#endif

#include "stdafx.h"
#include "Utility.h"


//图片的最大规格
#define IMAGE_MAX_WIDTH 800			//图片最大
#define IMAGE_MAX_HEIGHT 800

//人脸图像的规格
#define FACE_WIDTH 128			//人脸大小
#define FACE_HEIGHT 128

//读取图像
#define IMAGE_READ_DEPTH IPL_DEPTH_8U	//读入图像像素的位深度 无符号8bit整型
#define IMAGE_READ_CHANNELS 3	//图像通道，1-灰度图像， 3-彩色图
#define IMAGE_READ_ISCOLOR	1	//读入图像类型，1-原图像， 0 - 强制灰度图像

//灰度图像
#define IMAGE_GRAY_DEPTH IPL_DEPTH_8U	//位深度
#define IMAGE_GRAY_CHANNELS 1	//通道

//二进制图像
#define IMAGE_BINARY_DEPTH IPL_DEPTH_8U
#define IMAGE_BINARY_CHANNELS 1

//人脸检测
#define FACE_DIR _T("Faces")


class CDetect
{
public:
	CDetect(void);
	~CDetect(void);

public:
	IplImage* m_pReadImage;		//读取的图像
	IplImage* m_pGrayImage;		//灰度图像
	IplImage* m_pBinaryImage;	//二进制图像

	//人脸识别相关
	CvMemStorage* m_pStorage;				//人脸识别开辟的内存空间
	CvHaarClassifierCascade* m_pCascade;	//人脸识别分类器
	int m_nFacesCount;						//检测人脸的数目
	bool m_isBinaryMode;					//是否检测二进制模式

	CString m_strErrMessage;				//出错信息

public:
	void Init();							//初始化图像
	bool Load(CString strPath);				//载入图像
	//void ResizeImage(IplImage* image, int maxWidth=0, int maxHeight=0 );			//重置图片大小
	bool SetImage( IplImage* image, int maxWidth=0, int maxHeight=0 );
	bool SetReadImage( IplImage* image, int maxWidth=0, int maxHeight=0 );	//设置图片大小并保存到m_pReadImage/m_pGrayImage/m_pBinaryImage
	bool SetGrayImage( IplImage* image = NULL);		//设置图片大小并保存到m_readImage
	bool SetBinaryImage(IplImage* image = NULL);		//二值化
	bool FaceDetect( CString strCascadeName, CString strEyesCascadeName = _T("")
		);			//人脸检测
	void Normalization( IplImage* image );		//归一化
	void RemoveNoise(bool isGrayImage = false);							//去除噪声
	void EqualizeHist( IplImage *image = NULL);	//灰度图像直方图均衡化
	void Release();						//释放资源

	int HasEyes( IplImage* img, CString strCascadeName);	//是否存在人眼，返回人眼对数
};
