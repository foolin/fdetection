#include "StdAfx.h"
#include "Detect.h"

CDetect::CDetect(void)
{
	Init();
}

CDetect::~CDetect(void)
{
	Release();
}


//��ʼ��ͼ��
void CDetect::Init()
{
	m_pReadImage = NULL;
	m_pGrayImage = NULL;
	m_pBinaryImage = NULL;

	m_pStorage = NULL;				//����ʶ�𿪱ٵ��ڴ�ռ�
	m_pCascade = NULL;		//����ʶ�������
	m_nFacesCount = 0;						//�����������Ŀ

	m_strErrMessage = "";

	m_isBinaryMode = false;
}


bool CDetect::Load(CString strPath)
{
	if(strPath.IsEmpty())
	{
		return false;
	}
	IplImage *image;
	image = cvLoadImage( strPath, IMAGE_READ_ISCOLOR);
	if(!image)
		return false;
	SetImage(image);
	cvReleaseImage(&image);
	return true;
}

/*
//����ͼƬ��С��������ʾ��
void CDetect::ResizeImage( IplImage* image, int maxWidth, int maxHeight )
{
	//����Ƿ����ͼƬ
	if(!image)
	{
		return;
	}

	//������ر���
	IplImage *tempImage;		//Ŀ��ͼ��
	double scale = 0.5;		//���ű���Ϊ0.5��

	double scaleW = (double) IMAGE_MAX_WIDTH / image->width;		//Ĭ��
	double scaleH = (double) IMAGE_MAX_HEIGHT / image->height;		//Ĭ��
	CvSize tempSize;			//����ͼ��Ĵ�С
	
	//��������
	if(maxWidth > 0 && maxHeight > 0)
	{
		scaleW = (double) maxWidth / image->width;
		scaleH = (double) maxHeight / image->height;
		scale = scaleW < scaleH ? scaleW : scaleH;
	}
	else if(maxWidth > 0)
	{
		scale = (double) maxWidth / image->width;
	}
	else if(maxHeight > 0)
	{
		scale =  (double) maxWidth / image->width;
	}
	else
	{
		scale = scaleW < scaleH ? scaleW : scaleH;
	}
	
	//���ͼƬ�Ѿ���С��������С
	if ( scale >= 1 )
	{
		return;
	}

	//�����ſ�Ⱥ͸߶�
	tempSize.width = cvRound( image->width * scale );
	tempSize.height = cvRound( image->height * scale);

	//��СͼƬ
	tempImage = cvCreateImage( tempSize, image->depth, image->nChannels);	//����Ŀ��ͼ��
	cvResize(image, tempImage, CV_INTER_LINEAR);	//����Դͼ��Ŀ��ͼ��

	//����ͼ�񲢸�ֵ��image
	cvZero( image );	//���ͼ��
    image = cvCreateImage( tempSize, tempImage->depth, tempImage->nChannels);	//����Ŀ��ͼ��
    cvCopy(tempImage, image, NULL);

	//�ͷ���ʱԭͼ��
	cvReleaseImage( &tempImage);
}
*/

bool CDetect::SetImage( IplImage* image, int maxWidth, int maxHeight )
{
	if(!image)
		return false;

	//����ReadImage
	if(!SetReadImage(image))
		return false;
	//����GrayImage
	if(!SetGrayImage())
		return false;
	//����BinaryImage
	if(!SetBinaryImage())
		return false;

	return true;
}


//����ͼƬ��С�����浽m_pReadImage
bool CDetect::SetReadImage( IplImage* image, int maxWidth, int maxHeight )
{
	//����Ƿ����ͼƬ
	if(!image)
	{
		m_strErrMessage = _T("����ReadImageͼƬ���������ڴ������image");
		return false;
	}

	//������ر���
	IplImage *tempImage;		//Ŀ��ͼ��
	double scale = 0.5;		//���ű���Ϊ0.5��

	double scaleW = (double) IMAGE_MAX_WIDTH / image->width;		//Ĭ��
	double scaleH = (double) IMAGE_MAX_HEIGHT / image->height;		//Ĭ��
	CvSize tempSize;			//����ͼ��Ĵ�С
	
	//��������
	if(maxWidth > 0 && maxHeight > 0)
	{
		scaleW = (double) maxWidth / image->width;
		scaleH = (double) maxHeight / image->height;
		scale = scaleW < scaleH ? scaleW : scaleH;
	}
	else if(maxWidth > 0)
	{
		scale = (double) maxWidth / image->width;
	}
	else if(maxHeight > 0)
	{
		scale =  (double) maxWidth / image->width;
	}
	else
	{
		scale = scaleW < scaleH ? scaleW : scaleH;
	}
	
	//���ReadImageͼ��
	if(m_pReadImage)
	{
		cvZero( m_pReadImage );
	}


	//���ͼƬ�Ѿ���С��������С
	if ( scale >= 1 )
	{
		//����ԭͼ��
		m_pReadImage = cvCreateImage( cvGetSize(image), image->depth, image->nChannels);	//����Ŀ��ͼ��
		cvCopy( image, m_pReadImage, NULL);
		return true;
	}

	//�����ſ�Ⱥ͸߶�
	tempSize.width = cvRound( image->width * scale );
	tempSize.height = cvRound( image->height * scale);

	//��СͼƬ
	tempImage = cvCreateImage( tempSize, image->depth, image->nChannels);	//����Ŀ��ͼ��
	cvResize(image, tempImage, CV_INTER_LINEAR);	//����Դͼ��Ŀ��ͼ��

	//����ͼ�񲢸�ֵ��image
	cvZero( image );	//���ͼ��
    m_pReadImage = cvCreateImage( tempSize, tempImage->depth, tempImage->nChannels);	//����Ŀ��ͼ��
    cvCopy(tempImage, m_pReadImage, NULL);

	//�ͷ���ʱԭͼ��
	cvReleaseImage( &tempImage);
	return true;

}


//���ûҶ�ͼ��
bool CDetect::SetGrayImage(IplImage *image)
{
	if(m_pGrayImage)
	{
		cvZero(m_pGrayImage);
	}
	if(image != NULL)
	{
		if((image->depth != m_pGrayImage->depth) || (image->nChannels != m_pGrayImage->nChannels))
		{
			m_pGrayImage = cvCreateImage(cvSize(image->width,image->height), IMAGE_GRAY_DEPTH, IMAGE_GRAY_CHANNELS);
			 //�Ѳ�ɫͼ��ת��Ϊ�Ҷ�ͼ��
			cvCvtColor(image, m_pGrayImage, CV_RGB2GRAY);
		}
		else
		{
			cvCopy(image, m_pGrayImage);
		}
	}
	else
	{
		if(!m_pReadImage)
		{
			m_strErrMessage = _T("����GrayImageͼ����������ڴ���image����ReadImage");
			return false;
		}
		m_pGrayImage = cvCreateImage(cvSize(m_pReadImage->width,m_pReadImage->height), IMAGE_GRAY_DEPTH, IMAGE_GRAY_CHANNELS);
		//�Ѳ�ɫͼ��ת��Ϊ�Ҷ�ͼ��
		cvCvtColor(m_pReadImage, m_pGrayImage, CV_RGB2GRAY);
	}
	
	return true;
}
void CDetect::EqualizeHist( IplImage* image)	//�Ҷ�ͼ��ֱ��ͼ���⻯
{
	if(!image)
	{
		cvEqualizeHist(m_pGrayImage, m_pGrayImage);
	}
	else
	{
		cvEqualizeHist(image, m_pGrayImage);
	}
}

//���ö�ֵ��ͼ��
bool CDetect::SetBinaryImage(IplImage* image)
{


	if(m_pBinaryImage)
	{
		cvZero(m_pBinaryImage);
	}
	//�������image
	if(image != NULL)
	{
		m_pBinaryImage = cvCreateImage( cvGetSize( image), image->depth, image->nChannels);	//����Ŀ��ͼ��
		cvThreshold( image, m_pBinaryImage , 10, 255, CV_THRESH_BINARY ); //ȡ��ֵΪ50��ͼ��תΪ��ֵͼ��
		return true;
	}
	//�������m_pGrayImage
	if(m_pReadImage && !m_pGrayImage)
	{
		SetGrayImage();
	}
	if(m_pGrayImage)
	{
		m_pBinaryImage = cvCreateImage( cvGetSize( m_pGrayImage), m_pGrayImage->depth, m_pGrayImage->nChannels);	//����Ŀ��ͼ��
		cvThreshold( m_pGrayImage, m_pBinaryImage , 80, 255, CV_THRESH_BINARY ); //ȡ��ֵΪ50��ͼ��תΪ��ֵͼ��
		return true;
	}
	
	m_strErrMessage = _T("���ö�ֵ��ͼ�����������GrayImageͼ����ߴ������imageͼ��");
	return false;
}


//�������
bool CDetect::FaceDetect( CString strCascadeName )
{

	//ȥ����
	RemoveNoise();	//��ֵ�˲�ȥ����


	//��ȡĿ¼������
	CString strFaceName = FACE_DIR;
	CUtility util;
	//�ж��Ƿ����Ŀ¼����������ڣ��򴴽�
	if(!util.DirExist(strFaceName))
	{
		util.CreateDir(strFaceName);
	}


	m_pCascade = (CvHaarClassifierCascade*)cvLoad( strCascadeName, 0, 0, 0 );
    if( !m_pCascade )
    {
		m_strErrMessage = _T("�޷����ļ�");
		m_strErrMessage.Append(strCascadeName);
        return false;
    }
    m_pStorage = cvCreateMemStorage(0);


    static CvScalar colors[] = 
    {
        {{0,0,255}},
        {{0,128,255}},
        {{0,255,255}},
        {{0,255,0}},
        {{255,128,0}},
        {{255,255,0}},
        {{255,0,0}},
        {{255,0,255}}
    };
    double scale = 1.3;

	//�жϼ��ģʽ
	IplImage* image;
	if(m_isBinaryMode)
	{
		//��ֵ��ͼƬ���ģʽ
		if( !m_pBinaryImage )
		{
			m_strErrMessage = _T("�����ڶ�ֵ��ͼƬ");
			return false;
		}
		image = cvCreateImage( cvSize(m_pBinaryImage->width, m_pBinaryImage->height), m_pBinaryImage->depth, m_pBinaryImage->nChannels);
		cvCopy( m_pBinaryImage, image);
	}
	else
	{
		//��ֵͼ����ģʽ
		if( !m_pGrayImage )
		{
			m_strErrMessage = _T("�����ڻҶ�ͼƬ");
			return false;
		}
		image = cvCreateImage( cvSize(m_pGrayImage->width, m_pGrayImage->height), m_pGrayImage->depth, m_pGrayImage->nChannels);
		cvCopy( m_pGrayImage, image);
	}

    IplImage* gray = cvCreateImage( cvSize(image->width, image->height), 8, 1 );
    IplImage* small_img = cvCreateImage( cvSize( cvRound (image->width/scale),
                         cvRound (image->height/scale)),
                     8, 1 );
    int i;
 
	//�ж�ͼ���Ƿ��Ѿ�Ϊ�Ҷ�ͼ�����Ϊ�Ҷ�ͼ����ֱ�Ӹ��ƣ�������תΪ�Ҷ�ͼ��
	if( (image->depth == 8) && (image->nChannels == 1))
	{
		cvCopy(image, gray);
	}
	else
	{
		cvCvtColor( image, gray, CV_BGR2GRAY );
	}
    cvResize( gray, small_img, CV_INTER_LINEAR );
    cvEqualizeHist( small_img, small_img );	//�Ҷ�ͼ��ֱ��ͼ���⻯
    cvClearMemStorage( m_pStorage );
 
    if( m_pCascade )
    {
        double t = (double)cvGetTickCount();

        //CvSeq* faces = cvHaarDetectObjects( small_img, m_cascade, m_storage,
                                           // 1.1, 2, 0/*CV_HAAR_DO_CANNY_PRUNING*/,
                                            //cvSize(30, 30) );
        CvSeq* faces = cvHaarDetectObjects( small_img, m_pCascade, m_pStorage,
                                            1.1, 2, CV_HAAR_DO_CANNY_PRUNING,
                                            cvSize(40, 40) );
        t = (double)cvGetTickCount() - t;
        //printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );
		if(faces)
		{
			m_nFacesCount = faces->total;
		}
        for( i = 0; i < (faces ? faces->total : 0); i++ )
        {
            CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
            CvPoint center;
            int radius;
            center.x = cvRound((r->x + r->width*0.5)*scale);
            center.y = cvRound((r->y + r->height*0.5)*scale);
            radius = cvRound((r->width + r->height)*0.25*scale);
			
			//������Ƭ

			//IplImage* img2 = cvCreateImage(cvSize(r->width+1,r->height+1), img->depth, img->nChannels ); 
			//CvRect rect2 = cvRect(r->x - r->width, r->y, r->width+1,r->height+1);
			//CvRect rect2 = cvRect(center.x - r->width, center.y - r->height, r->width+1,r->height+1);
			int rect_size = 3;
			CvRect rect2 = cvRect(center.x - radius + rect_size, center.y - radius + rect_size, radius*2 - rect_size*2, radius*2 - rect_size*2);
			IplImage* img2 = cvCreateImage(cvSize(rect2.width , rect2.height), m_pGrayImage->depth, m_pGrayImage->nChannels ); 
			cvSetImageROI(m_pGrayImage, rect2);
			cvCopy(m_pGrayImage,img2);//���ƶ������� 
			//cvShowImage( "result", img2 );
			cvResetImageROI(m_pGrayImage);

			//��ȡ������·��
			strFaceName.Format(_T("%s/%s%d.jpg"), FACE_DIR, util.RandDate(), i+1);


			IplImage* resultImage = cvCreateImage( cvSize(FACE_WIDTH, FACE_HEIGHT) , img2->depth, img2->nChannels);
			cvResize(img2, resultImage, CV_INTER_LINEAR);	//����Դͼ��Ŀ��ͼ��
			cvSaveImage(strFaceName, resultImage);
			cvReleaseImage(&resultImage);

			//sprintf(filename,"images/face%d.jpg",i+1);
			//Normalization(img2);	//��һ������ͷ��
			//cvSaveImage(strFaceName, img2);
			cvReleaseImage(&img2);
			//printf( "Save img:%s\n", filename );
			//������ʾ
			//SetTips(faceName);

			//������
			cvRectangle( m_pReadImage, cvPoint(rect2.x, rect2.y), cvPoint(rect2.x + rect2.width, rect2.y + rect2.height), colors[i%8], 3, 8, 0);
			cvRectangle( m_pGrayImage, cvPoint(rect2.x, rect2.y), cvPoint(rect2.x + rect2.width, rect2.y + rect2.height), colors[i%8], 3, 8, 0);
			//��Բ
            //cvCircle( img, center, radius, colors[i%8], 3, 8, 0 );
        }
    }
 

	// ��ʾͼ��
	//ShowImage(img, IDC_IMAGE);
	//ShowImage(img);

	//�ͷ�ͼ��
	if(image)
	{
		cvReleaseImage( &image);
	}
	if(gray)
	{
		cvReleaseImage( &gray );
	}
	if(small_img)
	{
		cvReleaseImage( &small_img );
	}
	return true;
}

//ȥ������
void CDetect::RemoveNoise(bool isGrayImage)
{
	if(isGrayImage)
	{
		IplImage* dst = cvCreateImage( cvGetSize( m_pGrayImage),
							m_pGrayImage->depth, m_pGrayImage->nChannels);
		cvSmooth( m_pGrayImage,dst,CV_MEDIAN,3,0);	//��ֵ�˲�
		SetGrayImage( dst );
		cvReleaseImage( &dst);
	}
	else
	{
		IplImage* dst = cvCreateImage( cvGetSize( m_pReadImage),
							m_pReadImage->depth, m_pReadImage->nChannels);
		cvSmooth( m_pReadImage,dst,CV_MEDIAN,3,0);	//��ֵ�˲�
		SetImage( dst );
		cvReleaseImage( &dst);
	}
}

//��һ������
void CDetect::Normalization( IplImage* image )
{

	IplImage* imgCopy = cvCloneImage(image);
	cvZero( image );
	cvSetImageROI( image, cvRect( 0, 0, FACE_WIDTH, FACE_HEIGHT) );
	cvResize( imgCopy, image , CV_INTER_LINEAR);
	cvReleaseImage( &imgCopy);
}

//�ͷ���Դ
void CDetect::Release()
{
	//�ͷŶ���ͼ��
	if ( m_pReadImage != NULL)
	{
		cvReleaseImage( &m_pReadImage);
	}
	//�ͷŻҶ�ͼ��
	if ( m_pGrayImage != NULL)
	{
		cvReleaseImage(& m_pGrayImage);
	}

	//�ͷŶ�����ͼ��
	if ( m_pBinaryImage != NULL)
	{
		cvReleaseImage( &m_pBinaryImage);
	}
}