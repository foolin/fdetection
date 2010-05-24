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
			cvCopy(image, m_pGrayImage);	//��ֵͼ��
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
	//�������m_pReadImage�Ҳ�����m_pGrayImage
	if(m_pReadImage && !m_pGrayImage)
	{
		SetGrayImage();
	}
	//������ڻҶ�ͼ��
	if(m_pGrayImage)
	{
		m_pBinaryImage = cvCreateImage( cvGetSize( m_pGrayImage), m_pGrayImage->depth, m_pGrayImage->nChannels);	//����Ŀ��ͼ��
		cvThreshold( m_pGrayImage, m_pBinaryImage , 50, 255, CV_THRESH_BINARY ); //ȡ��ֵΪ50��ͼ��תΪ��ֵͼ��
		return true;
	}
	
	m_strErrMessage = _T("���ö�ֵ��ͼ�����������GrayImageͼ����ߴ������imageͼ��");
	return false;
}


//�������
bool CDetect::FaceDetect( CString strCascadeName, CString strEyesCascadeName )
{

	//ȥ����
	RemoveNoise(true);	//��ֵ�˲�ȥ����


	//��ȡĿ¼������
	CString strFaceName = FACE_DIR;
	CUtility util;
	//�ж��Ƿ����Ŀ¼����������ڣ��򴴽�
	if(!util.DirExist(strFaceName))
	{
		util.CreateDir(strFaceName);
	}

	//���ؼ���������
	m_pCascade = (CvHaarClassifierCascade*)cvLoad( strCascadeName, 0, 0, 0 );
    if( !m_pCascade )
    {
		m_strErrMessage = _T("�޷����ļ�");
		m_strErrMessage.Append(strCascadeName);
        return false;
    }

	//�����ڴ�飬�����С�� 0 byte, �򽫸ÿ����ó�Ĭ��ֵ -- ��ǰĬ�ϴ�СΪ64k. 
    m_pStorage = cvCreateMemStorage(0);

	//Ԥ������ɫ����������
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

	//ͼ�����ű���
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

	//����һ���Ҷ�ͼ��gray
    IplImage* gray = cvCreateImage( cvSize(image->width, image->height), 8, 1 );

	//����һ��Сͼ��
    IplImage* small_img = cvCreateImage( cvSize( cvRound (image->width/scale),
                         cvRound (image->height/scale)),
                     8, 1 );

	//�ж�ͼ���Ƿ��Ѿ�Ϊ�Ҷ�ͼ�����Ϊ�Ҷ�ͼ����ֱ�Ӹ��ƣ�������תΪ�Ҷ�ͼ��
	if( (image->depth == 8) && (image->nChannels == 1))
	{
		cvCopy(image, gray);
	}
	else
	{
		//ͼ��ҶȻ�����RGBת��Ϊ�Ҷ�ͼ��
		cvCvtColor( image, gray, CV_BGR2GRAY );
	}

	//����Ҷ�ͼ����С��small_img
    cvResize( gray, small_img, CV_INTER_LINEAR );
	
	//�Ҷ�ͼ��ֱ��ͼ���⻯
    cvEqualizeHist( small_img, small_img );

	//����ڴ�洢�� 
    cvClearMemStorage( m_pStorage );
 
    if( m_pCascade )
    {
		//���ش�������ƽ̨���¼�(��������ʼ CPU ��ticks ��Ŀ, ��1970�꿪ʼ��΢����Ŀ�ȵ�)��ʼ�� tics ����Ŀ,
		//�ú������ھ�ȷ��������/�û������ִ��ʱ���Ǻ����õġ�
		//Ҫת�� tics ����ĿΪʱ�䵥λ��ʹ�ú��� cvGetTickFrequency 
        double t = (double)cvGetTickCount();

/*		
cvHaarDetectObjects�����ͼ���е�Ŀ�꺯��
	image 
	  ����ͼ�� 
	cascade 
	  harr �������������ڲ���ʶ��ʽ 
	storage 
	  �����洢��⵽��һ���к�ѡĿ����ο���ڴ����� 
	scale_factor 
	  ��ǰ��������̵�ɨ���У��������ڵı���ϵ��������1.1ָ������������������10%�� 
	min_neighbors 
	  ���ɼ��Ŀ������ھ��ε���С����(ȱʡ��1)��
	  �����ɼ��Ŀ���С���εĸ�����С��min_neighbors-1 ���ᱻ�ų���
	  ���min_neighbors Ϊ 0, ���������κβ����ͷ������еı����ѡ���ο������趨ֵһ�������û��Զ���Լ��������ϳ����ϡ� 
	flags 
	  ������ʽ����ǰΨһ���Զ���Ĳ�����ʽ�� CV_HAAR_DO_CANNY_PRUNING��
	  ������趨����������Canny��Ե��������ų�һЩ��Ե���ٻ��ߺܶ��ͼ��������Ϊ����������һ�㲻������Ŀ�ꡣ
	  ���������ͨ���趨��ֵʹ�������ַ��������������˼���ٶȡ� 
	min_size 
	  ��ⴰ�ڵ���С�ߴ硣ȱʡ������±���Ϊ������ѵ��ʱ���õ������ߴ�(���������ȱʡ��С��~20��20)�� 
*/
        CvSeq* faces = cvHaarDetectObjects( small_img, m_pCascade, m_pStorage,
                                            1.1, 2, CV_HAAR_DO_CANNY_PRUNING,
                                            cvSize(40, 40) );
		//��������Ҫ��ʱ��
        t = (double)cvGetTickCount() - t;
        //printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );

		//�����������
		if(faces)
		{
			m_nFacesCount = faces->total;
		}

		//ѭ������������λ�ͷָ�����������ͼ��ʱ��
        for(int i = 0; i < (faces ? faces->total : 0); i++ )
        {
			//cvGetSeqElem���� cvGetSeqElem ����������������ָ����Ԫ�أ�������ָ���Ԫ�ص�ָ�롣���Ԫ�ز����ڣ��򷵻� 0��
			// ��ȡ��ǰ�����ľ���
            CvRect* r = (CvRect*)cvGetSeqElem( faces, i );

			/*
			//��Բ��
            CvPoint center;
            int radius;
            center.x = cvRound((r->x + r->width*0.5)*scale);
            center.y = cvRound((r->y + r->height*0.5)*scale);
            radius = cvRound((r->width + r->height)*0.25*scale);
			int rect_size = 3;
			CvRect rect2 = cvRect(center.x - radius + rect_size, center.y - radius + rect_size, radius*2 - rect_size*2, radius*2 - rect_size*2);
			*/

			//���ݱ����Ŵ�ԭ���ο�
			int iRectScale = 0;	//΢�����β�����0��ʾ����
			CvRect rect2 = cvRect( cvRound((r->x + iRectScale) * scale), cvRound( (r->y + iRectScale)  * scale), cvRound( (r->width - iRectScale*2) * scale), cvRound( (r->height - iRectScale*2) * scale));
			//��������ͼ��
			IplImage* img2 = cvCreateImage(cvSize(rect2.width , rect2.height), m_pGrayImage->depth, m_pGrayImage->nChannels );
			//���ø���Ȥ���򣨼�����������
			cvSetImageROI(m_pGrayImage, rect2);
			//���ƶ������� 
			cvCopy(m_pGrayImage,img2);
			//
			cvResetImageROI(m_pGrayImage);

			int isHasEyes = -1;	//Ĭ��Ϊ-1
			//���������������Ϊ�գ���������ۼ��
			if( strEyesCascadeName != _T(""))
			{
				//�������ۼ��
				isHasEyes = HasEyes(img2, strEyesCascadeName);
			}
			if(isHasEyes == 0)	//����0����ʾ��ⲻ��˫�ۣ�����Ϊ���
			{
				m_nFacesCount--;	//��������1
				cvReleaseImage(&img2);	//�ͷ�ͼ��img2
				continue;
			}
			else
			{

				//��ȡ������·��
				strFaceName.Format(_T("%s/%s%d.jpg"), FACE_DIR, util.RandDate(), i+1);
				//����ͳһ��С������ͼ��
				IplImage* resultImage = cvCreateImage( cvSize(FACE_WIDTH, FACE_HEIGHT) , img2->depth, img2->nChannels);
				//������ͼ���Թ��
				cvResize(img2, resultImage, CV_INTER_LINEAR);	//����Դͼ��Ŀ��ͼ��,CV_INTER_NN - ����ڲ�ֵ,CV_INTER_LINEAR - ˫���Բ�ֵ.
				//����ͼ��
				cvSaveImage(strFaceName, resultImage);

				//���Զ���ͼ�񻭾��ο�
				cvRectangle( m_pReadImage, cvPoint(rect2.x, rect2.y), cvPoint(rect2.x + rect2.width, rect2.y + rect2.height), colors[i%8], 3, 8, 0);
				//�ԻҶ�ͼ�񻭾��ο�
				cvRectangle( m_pGrayImage, cvPoint(rect2.x, rect2.y), cvPoint(rect2.x + rect2.width, rect2.y + rect2.height), colors[i%8], 3, 8, 0);
				//��Բ
				//cvCircle( img, center, radius, colors[i%8], 3, 8, 0 );

				//�ͷ�ͼ��resultImage
				cvReleaseImage(&resultImage);
			}


			//�ͷ�ͼ��img2
			cvReleaseImage(&img2);

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
	//�ж��Ƿ�ֻ�ԻҶ�ͼ����
	if(isGrayImage)
	{
		//ֻ�ԻҶ�ͼ����
		IplImage* dst = cvCreateImage( cvGetSize( m_pGrayImage),
							m_pGrayImage->depth, m_pGrayImage->nChannels);
		cvSmooth( m_pGrayImage,dst,CV_MEDIAN,3,0);	//��ֵ�˲�
		SetGrayImage( dst );	//���ûҶ�ͼ��
		cvReleaseImage( &dst);
	}
	else
	{
		//�Զ���ͼ����д���
		IplImage* dst = cvCreateImage( cvGetSize( m_pReadImage),
							m_pReadImage->depth, m_pReadImage->nChannels);
		cvSmooth( m_pReadImage,dst,CV_MEDIAN,3,0);	//��ֵ�˲�
		SetImage( dst );	//��ͼ��ֵ��m_pReadImage,m_pGrayImage, m_pBinaryImage
		cvReleaseImage( &dst);
	}
}

//��һ������
void CDetect::Normalization( IplImage* image )
{

	//��¡ͼ��
	IplImage* imgCopy = cvCloneImage(image);
	//������ͼ������
	cvZero( image );
	//���ø���Ȥ����
	cvSetImageROI( image, cvRect( 0, 0, FACE_WIDTH, FACE_HEIGHT) );
	//�����۽������ţ�����ͼ���С���
	cvResize( imgCopy, image , CV_INTER_LINEAR);
	//�ͷ�ͼ��
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


//�ж��Ƿ��������
int CDetect::HasEyes( IplImage* img, CString strCascadeName)
{
	int iEyesCount = 0;	//������
    double scale = 1.3;	//������
	CvMemStorage* storage = NULL;		//�ڴ�ռ�
	CvHaarClassifierCascade* cascade;	//����������
    IplImage* gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );	//�Ҷ�ͼ��
    IplImage* small_img = cvCreateImage( cvSize( cvRound (img->width/scale),	//��С�Ĵ����ͼ��
                         cvRound (img->height/scale)),
                     8, 1 );
	//�ж�ͼ���Ƿ��Ѿ�Ϊ�Ҷ�ͼ�����Ϊ�Ҷ�ͼ����ֱ�Ӹ��ƣ�������תΪ�Ҷ�ͼ��
	if( (img->depth == 8) && (img->nChannels == 1))
	{
		cvCopy(img, gray);	//ֱ�Ӹ���
	}
	else
	{
		cvCvtColor( img, gray, CV_BGR2GRAY );	//��ͼ��ҶȻ�����ֵ��gray
	}

	//�Ҷ���С��small_img
    cvResize( gray, small_img, CV_INTER_LINEAR );

	//ֱ��ͼ���⻯�������Աȶ�
    cvEqualizeHist( small_img, small_img );

	//�����ڴ�飬�����С�� 0 byte, �򽫸ÿ����ó�Ĭ��ֵ -- ��ǰĬ�ϴ�СΪ64k. 
    storage = cvCreateMemStorage(0);
    cvClearMemStorage( storage );
 
	//���ؼ���������
	cascade = (CvHaarClassifierCascade*)cvLoad( strCascadeName, 0, 0, 0 );
    if( !cascade )
    {
        return -1;
    }

	//�ж��Ƿ���ؼ����������ɹ�
    if( cascade )
    {
		//�������ۼ��
        CvSeq* eyes = cvHaarDetectObjects( small_img, cascade, storage,
                                            1.1, 2, CV_HAAR_DO_CANNY_PRUNING,
                                            cvSize(5, 5) );
		//�����������
		if(eyes)
		{
			iEyesCount = eyes->total;	//���˫�۵Ķ�����ֵ��iEyesCount
		}

    }
	//�ͷ�ͼ��
    cvReleaseImage( &gray );
    cvReleaseImage( &small_img );
	//������������-1Ϊ�޷����ؼ�����������0��ʾû�м�⵽���ۣ�������ʾ���۵���Ŀ
	return iEyesCount;
}