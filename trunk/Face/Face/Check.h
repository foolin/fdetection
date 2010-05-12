#pragma once

#include "stdafx.h"
#include "Utility.h"

//ͼƬ�������
#define IMAGE_MAX_WIDTH 800			//ͼƬ���
#define IMAGE_MAX_HEIGHT 800

//����ͼ��Ĺ��
#define FACE_WIDTH 256			//������С
#define FACE_HEIGHT 256

//��ȡͼ��
#define IMAGE_READ_DEPTH IPL_DEPTH_8U
#define IMAGE_READ_CHANNELS 3	//1-�Ҷ�ͼ�� 3-��ɫͼ��

#define IMAGE_READ_ISCOLOR	1		//����ͼ�����ͣ�1-ԭͼ�� 0 - ǿ�ƻҶ�ͼ��

//�Ҷ�ͼ��
#define IMAGE_GRAY_DEPTH IPL_DEPTH_8U
#define IMAGE_GRAY_CHANNELS 1

class CCheck
{
public:
	CCheck(void);
	~CCheck(void);
//Attributes:
public:
	IplImage* m_ReadImage1;		//���Զ�ȡ��������
	IplImage* m_ReadImage2;	
	IplImage* m_ReadImage3;	
	CString m_strErrMessage;					//������Ϣ
//Operations��
public:
	//����:
	void Init();							//��ʼ��ͼ��
	bool Load(CString strPath);				//����ͼ��
	bool SetImage( IplImage* image, int maxWidth=0, int maxHeight=0);
	bool SetReadImage( IplImage* image, int maxWidth=0, int maxHeight=0 );	//����ͼƬ��С�����浽m_pReadImage/m_pGrayImage/m_pBinaryImage
	bool SetGrayImage( IplImage* image = NULL);		//����ͼƬ��С�����浽m_readImage
	void Release();						//�ͷ���Դ

	//�Լ���

};
