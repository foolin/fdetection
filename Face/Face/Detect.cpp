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


//初始化图像
void CDetect::Init()
{
	m_pReadImage = NULL;
	m_pGrayImage = NULL;
	m_pBinaryImage = NULL;

	m_pStorage = NULL;				//人脸识别开辟的内存空间
	m_pCascade = NULL;		//人脸识别分类器
	m_nFacesCount = 0;						//检测人脸的数目

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
//重置图片大小（辅助显示）
void CDetect::ResizeImage( IplImage* image, int maxWidth, int maxHeight )
{
	//检测是否存在图片
	if(!image)
	{
		return;
	}

	//声明相关变量
	IplImage *tempImage;		//目标图像
	double scale = 0.5;		//缩放倍数为0.5倍

	double scaleW = (double) IMAGE_MAX_WIDTH / image->width;		//默认
	double scaleH = (double) IMAGE_MAX_HEIGHT / image->height;		//默认
	CvSize tempSize;			//缩放图像的大小
	
	//求缩放率
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
	
	//如果图片已经够小，则不用缩小
	if ( scale >= 1 )
	{
		return;
	}

	//求缩放宽度和高度
	tempSize.width = cvRound( image->width * scale );
	tempSize.height = cvRound( image->height * scale);

	//缩小图片
	tempImage = cvCreateImage( tempSize, image->depth, image->nChannels);	//构造目标图象
	cvResize(image, tempImage, CV_INTER_LINEAR);	//缩放源图像到目标图像

	//缩放图像并赋值给image
	cvZero( image );	//清空图像
    image = cvCreateImage( tempSize, tempImage->depth, tempImage->nChannels);	//构造目标图象
    cvCopy(tempImage, image, NULL);

	//释放临时原图像
	cvReleaseImage( &tempImage);
}
*/

bool CDetect::SetImage( IplImage* image, int maxWidth, int maxHeight )
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


//设置图片大小并保存到m_pReadImage
bool CDetect::SetReadImage( IplImage* image, int maxWidth, int maxHeight )
{
	//检测是否存在图片
	if(!image)
	{
		m_strErrMessage = _T("设置ReadImage图片出错，不存在传入参数image");
		return false;
	}

	//声明相关变量
	IplImage *tempImage;		//目标图像
	double scale = 0.5;		//缩放倍数为0.5倍

	double scaleW = (double) IMAGE_MAX_WIDTH / image->width;		//默认
	double scaleH = (double) IMAGE_MAX_HEIGHT / image->height;		//默认
	CvSize tempSize;			//缩放图像的大小
	
	//求缩放率
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
	
	//清空ReadImage图像
	if(m_pReadImage)
	{
		cvZero( m_pReadImage );
	}


	//如果图片已经够小，则不用缩小
	if ( scale >= 1 )
	{
		//保留原图像
		m_pReadImage = cvCreateImage( cvGetSize(image), image->depth, image->nChannels);	//构造目标图象
		cvCopy( image, m_pReadImage, NULL);
		return true;
	}

	//求缩放宽度和高度
	tempSize.width = cvRound( image->width * scale );
	tempSize.height = cvRound( image->height * scale);

	//缩小图片
	tempImage = cvCreateImage( tempSize, image->depth, image->nChannels);	//构造目标图象
	cvResize(image, tempImage, CV_INTER_LINEAR);	//缩放源图像到目标图像

	//缩放图像并赋值给image
	cvZero( image );	//清空图像
    m_pReadImage = cvCreateImage( tempSize, tempImage->depth, tempImage->nChannels);	//构造目标图象
    cvCopy(tempImage, m_pReadImage, NULL);

	//释放临时原图像
	cvReleaseImage( &tempImage);
	return true;

}


//设置灰度图像
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
			 //把彩色图像转换为灰度图像
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
			m_strErrMessage = _T("设置GrayImage图像出错，不存在传入image或者ReadImage");
			return false;
		}
		m_pGrayImage = cvCreateImage(cvSize(m_pReadImage->width,m_pReadImage->height), IMAGE_GRAY_DEPTH, IMAGE_GRAY_CHANNELS);
		//把彩色图像转换为灰度图像
		cvCvtColor(m_pReadImage, m_pGrayImage, CV_RGB2GRAY);
	}
	
	return true;
}
void CDetect::EqualizeHist( IplImage* image)	//灰度图像直方图均衡化
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

//设置二值化图像
bool CDetect::SetBinaryImage(IplImage* image)
{


	if(m_pBinaryImage)
	{
		cvZero(m_pBinaryImage);
	}
	//如果存在image
	if(image != NULL)
	{
		m_pBinaryImage = cvCreateImage( cvGetSize( image), image->depth, image->nChannels);	//构造目标图象
		cvThreshold( image, m_pBinaryImage , 10, 255, CV_THRESH_BINARY ); //取阈值为50把图像转为二值图像
		return true;
	}
	//如果存在m_pGrayImage
	if(m_pReadImage && !m_pGrayImage)
	{
		SetGrayImage();
	}
	if(m_pGrayImage)
	{
		m_pBinaryImage = cvCreateImage( cvGetSize( m_pGrayImage), m_pGrayImage->depth, m_pGrayImage->nChannels);	//构造目标图象
		cvThreshold( m_pGrayImage, m_pBinaryImage , 80, 255, CV_THRESH_BINARY ); //取阈值为50把图像转为二值图像
		return true;
	}
	
	m_strErrMessage = _T("设置二值化图像出错，不存在GrayImage图像或者传入参数image图像");
	return false;
}


//人脸检测
bool CDetect::FaceDetect( CString strCascadeName )
{

	//去噪声
	RemoveNoise();	//中值滤波去噪声


	//获取目录和名字
	CString strFaceName = FACE_DIR;
	CUtility util;
	//判断是否存在目录，如果不存在，则创建
	if(!util.DirExist(strFaceName))
	{
		util.CreateDir(strFaceName);
	}


	m_pCascade = (CvHaarClassifierCascade*)cvLoad( strCascadeName, 0, 0, 0 );
    if( !m_pCascade )
    {
		m_strErrMessage = _T("无法打开文件");
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

	//判断检测模式
	IplImage* image;
	if(m_isBinaryMode)
	{
		//二值化图片检测模式
		if( !m_pBinaryImage )
		{
			m_strErrMessage = _T("不存在二值化图片");
			return false;
		}
		image = cvCreateImage( cvSize(m_pBinaryImage->width, m_pBinaryImage->height), m_pBinaryImage->depth, m_pBinaryImage->nChannels);
		cvCopy( m_pBinaryImage, image);
	}
	else
	{
		//灰值图像检测模式
		if( !m_pGrayImage )
		{
			m_strErrMessage = _T("不存在灰度图片");
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
 
	//判断图像是否已经为灰度图像，如果为灰度图像则直接复制，否则则转为灰度图像
	if( (image->depth == 8) && (image->nChannels == 1))
	{
		cvCopy(image, gray);
	}
	else
	{
		cvCvtColor( image, gray, CV_BGR2GRAY );
	}
    cvResize( gray, small_img, CV_INTER_LINEAR );
    cvEqualizeHist( small_img, small_img );	//灰度图像直方图均衡化
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
			
			//保存相片

			//IplImage* img2 = cvCreateImage(cvSize(r->width+1,r->height+1), img->depth, img->nChannels ); 
			//CvRect rect2 = cvRect(r->x - r->width, r->y, r->width+1,r->height+1);
			//CvRect rect2 = cvRect(center.x - r->width, center.y - r->height, r->width+1,r->height+1);
			int rect_size = 3;
			CvRect rect2 = cvRect(center.x - radius + rect_size, center.y - radius + rect_size, radius*2 - rect_size*2, radius*2 - rect_size*2);
			IplImage* img2 = cvCreateImage(cvSize(rect2.width , rect2.height), m_pGrayImage->depth, m_pGrayImage->nChannels ); 
			cvSetImageROI(m_pGrayImage, rect2);
			cvCopy(m_pGrayImage,img2);//复制对象区域 
			//cvShowImage( "result", img2 );
			cvResetImageROI(m_pGrayImage);

			//获取人脸的路径
			strFaceName.Format(_T("%s/%s%d.jpg"), FACE_DIR, util.RandDate(), i+1);


			IplImage* resultImage = cvCreateImage( cvSize(FACE_WIDTH, FACE_HEIGHT) , img2->depth, img2->nChannels);
			cvResize(img2, resultImage, CV_INTER_LINEAR);	//缩放源图像到目标图像
			cvSaveImage(strFaceName, resultImage);
			cvReleaseImage(&resultImage);

			//sprintf(filename,"images/face%d.jpg",i+1);
			//Normalization(img2);	//归一化脸部头像
			//cvSaveImage(strFaceName, img2);
			cvReleaseImage(&img2);
			//printf( "Save img:%s\n", filename );
			//保存提示
			//SetTips(faceName);

			//画矩形
			cvRectangle( m_pReadImage, cvPoint(rect2.x, rect2.y), cvPoint(rect2.x + rect2.width, rect2.y + rect2.height), colors[i%8], 3, 8, 0);
			cvRectangle( m_pGrayImage, cvPoint(rect2.x, rect2.y), cvPoint(rect2.x + rect2.width, rect2.y + rect2.height), colors[i%8], 3, 8, 0);
			//画圆
            //cvCircle( img, center, radius, colors[i%8], 3, 8, 0 );
        }
    }
 

	// 显示图像
	//ShowImage(img, IDC_IMAGE);
	//ShowImage(img);

	//释放图像
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

//去除噪声
void CDetect::RemoveNoise(bool isGrayImage)
{
	if(isGrayImage)
	{
		IplImage* dst = cvCreateImage( cvGetSize( m_pGrayImage),
							m_pGrayImage->depth, m_pGrayImage->nChannels);
		cvSmooth( m_pGrayImage,dst,CV_MEDIAN,3,0);	//中值滤波
		SetGrayImage( dst );
		cvReleaseImage( &dst);
	}
	else
	{
		IplImage* dst = cvCreateImage( cvGetSize( m_pReadImage),
							m_pReadImage->depth, m_pReadImage->nChannels);
		cvSmooth( m_pReadImage,dst,CV_MEDIAN,3,0);	//中值滤波
		SetImage( dst );
		cvReleaseImage( &dst);
	}
}

//归一化人脸
void CDetect::Normalization( IplImage* image )
{

	IplImage* imgCopy = cvCloneImage(image);
	cvZero( image );
	cvSetImageROI( image, cvRect( 0, 0, FACE_WIDTH, FACE_HEIGHT) );
	cvResize( imgCopy, image , CV_INTER_LINEAR);
	cvReleaseImage( &imgCopy);
}

//释放资源
void CDetect::Release()
{
	//释放读入图像
	if ( m_pReadImage != NULL)
	{
		cvReleaseImage( &m_pReadImage);
	}
	//释放灰度图像
	if ( m_pGrayImage != NULL)
	{
		cvReleaseImage(& m_pGrayImage);
	}

	//释放二进制图像
	if ( m_pBinaryImage != NULL)
	{
		cvReleaseImage( &m_pBinaryImage);
	}
}