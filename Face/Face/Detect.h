#pragma once

//#ifndef MY_OPENCV
//	#include "cv.h"
//	#include "highgui.h"
//	#define MY_OPENCV  1          //���Ϊ�Ѷ���
//#endif

#include "stdafx.h"
#include "Utility.h"


//ͼƬ�������
#define IMAGE_MAX_WIDTH 800			//ͼƬ���
#define IMAGE_MAX_HEIGHT 800

//����ͼ��Ĺ��
#define FACE_WIDTH 128			//������С
#define FACE_HEIGHT 128

//��ȡͼ��
#define IMAGE_READ_DEPTH IPL_DEPTH_8U	//����ͼ�����ص�λ��� �޷���8bit����
#define IMAGE_READ_CHANNELS 3	//ͼ��ͨ����1-�Ҷ�ͼ�� 3-��ɫͼ
#define IMAGE_READ_ISCOLOR	1	//����ͼ�����ͣ�1-ԭͼ�� 0 - ǿ�ƻҶ�ͼ��

//�Ҷ�ͼ��
#define IMAGE_GRAY_DEPTH IPL_DEPTH_8U	//λ���
#define IMAGE_GRAY_CHANNELS 1	//ͨ��

//������ͼ��
#define IMAGE_BINARY_DEPTH IPL_DEPTH_8U
#define IMAGE_BINARY_CHANNELS 1

//�������
#define FACE_DIR _T("Faces")


class CDetect
{
public:
	CDetect(void);
	~CDetect(void);

public:
	IplImage* m_pReadImage;		//��ȡ��ͼ��
	IplImage* m_pGrayImage;		//�Ҷ�ͼ��
	IplImage* m_pBinaryImage;	//������ͼ��

	//����ʶ�����
	CvMemStorage* m_pStorage;				//����ʶ�𿪱ٵ��ڴ�ռ�
	CvHaarClassifierCascade* m_pCascade;	//����ʶ�������
	int m_nFacesCount;						//�����������Ŀ
	bool m_isBinaryMode;					//�Ƿ��������ģʽ

	CString m_strErrMessage;				//������Ϣ

public:
	void Init();							//��ʼ��ͼ��
	bool Load(CString strPath);				//����ͼ��
	//void ResizeImage(IplImage* image, int maxWidth=0, int maxHeight=0 );			//����ͼƬ��С
	bool SetImage( IplImage* image, int maxWidth=0, int maxHeight=0 );
	bool SetReadImage( IplImage* image, int maxWidth=0, int maxHeight=0 );	//����ͼƬ��С�����浽m_pReadImage/m_pGrayImage/m_pBinaryImage
	bool SetGrayImage( IplImage* image = NULL);		//����ͼƬ��С�����浽m_readImage
	bool SetBinaryImage(IplImage* image = NULL);		//��ֵ��
	bool FaceDetect( CString strCascadeName, CString strEyesCascadeName = _T("")
		);			//�������
	void Normalization( IplImage* image );		//��һ��
	void RemoveNoise(bool isGrayImage = false);							//ȥ������
	void EqualizeHist( IplImage *image = NULL);	//�Ҷ�ͼ��ֱ��ͼ���⻯
	void Release();						//�ͷ���Դ

	int HasEyes( IplImage* img, CString strCascadeName);	//�Ƿ�������ۣ��������۶���
};
