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
			cvCopy(image, m_pGrayImage);	//赋值图像
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
	//如果存在m_pReadImage且不存在m_pGrayImage
	if(m_pReadImage && !m_pGrayImage)
	{
		SetGrayImage();
	}
	//如果存在灰度图像
	if(m_pGrayImage)
	{
		m_pBinaryImage = cvCreateImage( cvGetSize( m_pGrayImage), m_pGrayImage->depth, m_pGrayImage->nChannels);	//构造目标图象
		cvThreshold( m_pGrayImage, m_pBinaryImage , 50, 255, CV_THRESH_BINARY ); //取阈值为50把图像转为二值图像
		return true;
	}
	
	m_strErrMessage = _T("设置二值化图像出错，不存在GrayImage图像或者传入参数image图像");
	return false;
}


//人脸检测
bool CDetect::FaceDetect( CString strCascadeName, CString strEyesCascadeName )
{

	//去噪声
	RemoveNoise(true);	//中值滤波去噪声


	//获取目录和名字
	CString strFaceName = FACE_DIR;
	CUtility util;
	//判断是否存在目录，如果不存在，则创建
	if(!util.DirExist(strFaceName))
	{
		util.CreateDir(strFaceName);
	}

	//加载级联分类器
	m_pCascade = (CvHaarClassifierCascade*)cvLoad( strCascadeName, 0, 0, 0 );
    if( !m_pCascade )
    {
		m_strErrMessage = _T("无法打开文件");
		m_strErrMessage.Append(strCascadeName);
        return false;
    }

	//创建内存块，如果大小是 0 byte, 则将该块设置成默认值 -- 当前默认大小为64k. 
    m_pStorage = cvCreateMemStorage(0);

	//预定义颜色表，画矩形用
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

	//图像缩放比率
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

	//创建一个灰度图像gray
    IplImage* gray = cvCreateImage( cvSize(image->width, image->height), 8, 1 );

	//创建一幅小图像
    IplImage* small_img = cvCreateImage( cvSize( cvRound (image->width/scale),
                         cvRound (image->height/scale)),
                     8, 1 );

	//判断图像是否已经为灰度图像，如果为灰度图像则直接复制，否则则转为灰度图像
	if( (image->depth == 8) && (image->nChannels == 1))
	{
		cvCopy(image, gray);
	}
	else
	{
		//图像灰度化：从RGB转换为灰度图像
		cvCvtColor( image, gray, CV_BGR2GRAY );
	}

	//将大灰度图像缩小到small_img
    cvResize( gray, small_img, CV_INTER_LINEAR );
	
	//灰度图像直方图均衡化
    cvEqualizeHist( small_img, small_img );

	//清空内存存储块 
    cvClearMemStorage( m_pStorage );
 
    if( m_pCascade )
    {
		//返回从依赖于平台的事件(从启动开始 CPU 的ticks 数目, 从1970年开始的微秒数目等等)开始的 tics 的数目,
		//该函数对于精确测量函数/用户代码的执行时间是很有用的。
		//要转化 tics 的数目为时间单位，使用函数 cvGetTickFrequency 
        double t = (double)cvGetTickCount();

/*		
cvHaarDetectObjects：检测图像中的目标函数
	image 
	  被检图像 
	cascade 
	  harr 分类器级联的内部标识形式 
	storage 
	  用来存储检测到的一序列候选目标矩形框的内存区域。 
	scale_factor 
	  在前后两次相继的扫描中，搜索窗口的比例系数。例如1.1指将搜索窗口依次扩大10%。 
	min_neighbors 
	  构成检测目标的相邻矩形的最小个数(缺省－1)。
	  如果组成检测目标的小矩形的个数和小于min_neighbors-1 都会被排除。
	  如果min_neighbors 为 0, 则函数不做任何操作就返回所有的被检候选矩形框，这种设定值一般用在用户自定义对检测结果的组合程序上。 
	flags 
	  操作方式。当前唯一可以定义的操作方式是 CV_HAAR_DO_CANNY_PRUNING。
	  如果被设定，函数利用Canny边缘检测器来排除一些边缘很少或者很多的图像区域，因为这样的区域一般不含被检目标。
	  人脸检测中通过设定阈值使用了这种方法，并因此提高了检测速度。 
	min_size 
	  检测窗口的最小尺寸。缺省的情况下被设为分类器训练时采用的样本尺寸(人脸检测中缺省大小是~20×20)。 
*/
        CvSeq* faces = cvHaarDetectObjects( small_img, m_pCascade, m_pStorage,
                                            1.1, 2, CV_HAAR_DO_CANNY_PRUNING,
                                            cvSize(40, 40) );
		//求出检测需要的时间
        t = (double)cvGetTickCount() - t;
        //printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );

		//如果存在人脸
		if(faces)
		{
			m_nFacesCount = faces->total;
		}

		//循环进行人脸定位和分割（当含多个人脸图像时）
        for(int i = 0; i < (faces ? faces->total : 0); i++ )
        {
			//cvGetSeqElem函数 cvGetSeqElem 查找序列中索引所指定的元素，并返回指向该元素的指针。如果元素不存在，则返回 0。
			// 获取当前人脸的矩形
            CvRect* r = (CvRect*)cvGetSeqElem( faces, i );

			/*
			//求圆心
            CvPoint center;
            int radius;
            center.x = cvRound((r->x + r->width*0.5)*scale);
            center.y = cvRound((r->y + r->height*0.5)*scale);
            radius = cvRound((r->width + r->height)*0.25*scale);
			int rect_size = 3;
			CvRect rect2 = cvRect(center.x - radius + rect_size, center.y - radius + rect_size, radius*2 - rect_size*2, radius*2 - rect_size*2);
			*/

			//根据比例放大原矩形框
			int iRectScale = 0;	//微调矩形参数，0表示不调
			CvRect rect2 = cvRect( cvRound((r->x + iRectScale) * scale), cvRound( (r->y + iRectScale)  * scale), cvRound( (r->width - iRectScale*2) * scale), cvRound( (r->height - iRectScale*2) * scale));
			//创建人脸图像
			IplImage* img2 = cvCreateImage(cvSize(rect2.width , rect2.height), m_pGrayImage->depth, m_pGrayImage->nChannels );
			//设置感兴趣区域（即是人脸区域）
			cvSetImageROI(m_pGrayImage, rect2);
			//复制对象区域 
			cvCopy(m_pGrayImage,img2);
			//
			cvResetImageROI(m_pGrayImage);

			int isHasEyes = -1;	//默认为-1
			//如果级联分类器不为空，则进行人眼检测
			if( strEyesCascadeName != _T(""))
			{
				//进行人眼检测
				isHasEyes = HasEyes(img2, strEyesCascadeName);
			}
			if(isHasEyes == 0)	//等于0，表示检测不到双眼，即定为误检
			{
				m_nFacesCount--;	//人脸数减1
				cvReleaseImage(&img2);	//释放图像img2
				continue;
			}
			else
			{

				//获取人脸的路径
				strFaceName.Format(_T("%s/%s%d.jpg"), FACE_DIR, util.RandDate(), i+1);
				//创建统一大小的人脸图像
				IplImage* resultImage = cvCreateImage( cvSize(FACE_WIDTH, FACE_HEIGHT) , img2->depth, img2->nChannels);
				//将人脸图像显规格化
				cvResize(img2, resultImage, CV_INTER_LINEAR);	//缩放源图像到目标图像,CV_INTER_NN - 最近邻插值,CV_INTER_LINEAR - 双线性插值.
				//保存图像
				cvSaveImage(strFaceName, resultImage);

				//画对读入图像画矩形框
				cvRectangle( m_pReadImage, cvPoint(rect2.x, rect2.y), cvPoint(rect2.x + rect2.width, rect2.y + rect2.height), colors[i%8], 3, 8, 0);
				//对灰度图像画矩形框
				cvRectangle( m_pGrayImage, cvPoint(rect2.x, rect2.y), cvPoint(rect2.x + rect2.width, rect2.y + rect2.height), colors[i%8], 3, 8, 0);
				//画圆
				//cvCircle( img, center, radius, colors[i%8], 3, 8, 0 );

				//释放图像resultImage
				cvReleaseImage(&resultImage);
			}


			//释放图像img2
			cvReleaseImage(&img2);

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
	//判断是否只对灰度图像处理
	if(isGrayImage)
	{
		//只对灰度图像处理
		IplImage* dst = cvCreateImage( cvGetSize( m_pGrayImage),
							m_pGrayImage->depth, m_pGrayImage->nChannels);
		cvSmooth( m_pGrayImage,dst,CV_MEDIAN,3,0);	//中值滤波
		SetGrayImage( dst );	//设置灰度图像
		cvReleaseImage( &dst);
	}
	else
	{
		//对读入图像进行处理
		IplImage* dst = cvCreateImage( cvGetSize( m_pReadImage),
							m_pReadImage->depth, m_pReadImage->nChannels);
		cvSmooth( m_pReadImage,dst,CV_MEDIAN,3,0);	//中值滤波
		SetImage( dst );	//将图像赋值给m_pReadImage,m_pGrayImage, m_pBinaryImage
		cvReleaseImage( &dst);
	}
}

//归一化人脸
void CDetect::Normalization( IplImage* image )
{

	//克隆图像
	IplImage* imgCopy = cvCloneImage(image);
	//将传入图像清零
	cvZero( image );
	//设置感兴趣区域
	cvSetImageROI( image, cvRect( 0, 0, FACE_WIDTH, FACE_HEIGHT) );
	//对人眼进行缩放，即是图像大小规格化
	cvResize( imgCopy, image , CV_INTER_LINEAR);
	//释放图像
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


//判断是否存在人眼
int CDetect::HasEyes( IplImage* img, CString strCascadeName)
{
	int iEyesCount = 0;	//人眼数
    double scale = 1.3;	//缩放率
	CvMemStorage* storage = NULL;		//内存空间
	CvHaarClassifierCascade* cascade;	//级联分类器
    IplImage* gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );	//灰度图像
    IplImage* small_img = cvCreateImage( cvSize( cvRound (img->width/scale),	//缩小的待检测图像
                         cvRound (img->height/scale)),
                     8, 1 );
	//判断图像是否已经为灰度图像，如果为灰度图像则直接复制，否则则转为灰度图像
	if( (img->depth == 8) && (img->nChannels == 1))
	{
		cvCopy(img, gray);	//直接复制
	}
	else
	{
		cvCvtColor( img, gray, CV_BGR2GRAY );	//将图像灰度化并赋值到gray
	}

	//灰度缩小到small_img
    cvResize( gray, small_img, CV_INTER_LINEAR );

	//直方图均衡化，调整对比度
    cvEqualizeHist( small_img, small_img );

	//创建内存块，如果大小是 0 byte, 则将该块设置成默认值 -- 当前默认大小为64k. 
    storage = cvCreateMemStorage(0);
    cvClearMemStorage( storage );
 
	//加载级联分类器
	cascade = (CvHaarClassifierCascade*)cvLoad( strCascadeName, 0, 0, 0 );
    if( !cascade )
    {
        return -1;
    }

	//判断是否加载级联分类器成功
    if( cascade )
    {
		//进行人眼检测
        CvSeq* eyes = cvHaarDetectObjects( small_img, cascade, storage,
                                            1.1, 2, CV_HAAR_DO_CANNY_PRUNING,
                                            cvSize(5, 5) );
		//如果存在人眼
		if(eyes)
		{
			iEyesCount = eyes->total;	//检测双眼的对数赋值给iEyesCount
		}

    }
	//释放图像
    cvReleaseImage( &gray );
    cvReleaseImage( &small_img );
	//返回人眼数：-1为无法加载级联分类器，0表示没有检测到人眼，其它表示人眼的数目
	return iEyesCount;
}