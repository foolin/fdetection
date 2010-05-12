#include "StdAfx.h"
#include "Check.h"

CCheck::CCheck(void)
{
}

CCheck::~CCheck(void)
{
}


//初始化图像
void CCheck::Init()
{
	m_ReadImage1 = NULL;
	m_ReadImage2 = NULL;
	m_ReadImage2 = NULL;

}


bool CCheck::Load(CString strPath)
{
	if(strPath.IsEmpty())
	{
		return false;
	}
	IplImage *image;
	image = cvLoadImage( strPath, 0);
	if(!image)
		return false;
//	SetImage(image);
	cvReleaseImage(&image);
	return true;
}
/*
bool CCheck::SetImage( IplImage* image, int maxWidth, int maxHeight )
{
	if(!image)
		return false;

	//设置ReadImage
	if(!SetReadImage(image))
		return false;
	//设置GrayImage
	if(!SetGrayImage())
		return false;
	//设置BinaryImage
	if(!SetBinaryImage())
		return false;

	return true;
}
*/
