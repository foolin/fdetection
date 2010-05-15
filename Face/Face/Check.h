#pragma once

#include "stdafx.h"
//#include "Utility.h"
#include "highgui.h"
#include "cv.h"
#include "math.h"

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

struct DataField
{
	int number;	//ͼ��ı��
	int x;		//��¼ͼ���ĳ�����صĺ�����
	int y;		//��¼ͼ���ĳ�����ص�������
	double value;	//��¼ͼ��ĳ������ת��Ϊ���ݳ������ֵ
};




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
public:
	
	DataField Data(char* name,int num);	//�������ݳ��������ͼ��ľֲ���ֵ������������ں�
	void BubbleSort(DataField *a,int n);	//ð�����򣬶�DataField���͵�������н�������
	bool SecondData(DataField *a,DataField tempcheck);	//�����������ݳ����ҳ���Ⱥ��
	DataField temp1[8];				//����������е�һ���������󼰴������
	DataField temp2[8];				//����������еڶ����������󼰴������
	DataField temp3[8];				//����������е������������󼰴������


};
