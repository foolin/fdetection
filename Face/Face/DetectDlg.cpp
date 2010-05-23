// DetectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Face.h"
#include "DetectDlg.h"


// CDetectDlg 对话框

IMPLEMENT_DYNAMIC(CDetectDlg, CDialog)

CDetectDlg::CDetectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDetectDlg::IDD, pParent)
	//, m_showImage(NULL)
	, m_strAppPath(_T(""))
	, m_blnIsShowGray(false)
{
	

}

CDetectDlg::~CDetectDlg()
{
	//释放ShowImage
	if(m_showImage)
	{
		cvReleaseImage(&m_showImage);
	}

}

void CDetectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDetectDlg, CDialog)
	ON_BN_CLICKED(IDC_DtBtn_OpenDir, &CDetectDlg::OnBnClickedDtbtnOpendir)
	ON_BN_CLICKED(IDC_DtBtn_OpenImage, &CDetectDlg::OnBnClickedDtbtnOpenimage)
	ON_BN_CLICKED(IDC_DtBtn_SaveImage, &CDetectDlg::OnBnClickedDtbtnSaveimage)
	ON_BN_CLICKED(IDC_DtBtn_Detect, &CDetectDlg::OnBnClickedDtbtnDetect)
	ON_BN_CLICKED(IDC_DtBtn_About, &CDetectDlg::OnBnClickedDtbtnAbout)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DtBtn_2Gray, &CDetectDlg::OnBnClickedDtbtn2gray)
	ON_BN_CLICKED(IDC_DtBtn_EqualHist, &CDetectDlg::OnBnClickedDtbtnEqualhist)
	ON_BN_CLICKED(IDC_DtBtn_RemoveNoise, &CDetectDlg::OnBnClickedDtbtnRemovenoise)
END_MESSAGE_MAP()


BOOL CDetectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化


	//初始化ShowImage
	CvSize imgSize;
    imgSize.height = SHOWIMAGE_HEIGHT;
    imgSize.width = SHOWIMAGE_WIDTH;
	m_showImage = cvCreateImage( imgSize, IPL_DEPTH_8U, 3 );

	//初始化配置文件
	m_strAppPath = m_objConfig.GetConfig(_T("AppPath"));
	m_strCascadeName = m_objConfig.GetConfig(_T("Detect"), _T("CascadeName"));
	m_strEyesCascadeName = m_objConfig.GetConfig(_T("Detect"), _T("EyesCascadeName"));
	if( m_strCascadeName == _T(""))
	{
		m_strCascadeName = _T("xml\\haarcascade_frontalface_alt_tree.xml");
		m_objConfig.SetConfig(_T("Detect"), _T("CascadeName"), m_strCascadeName);
	}
	if( m_strEyesCascadeName == _T(""))
	{
		m_strEyesCascadeName = _T("xml\\haarcascade_mcs_eyepair_big.xml");
		m_objConfig.SetConfig(_T("Detect"), _T("EyesCascadeName"), m_strEyesCascadeName);
	}

	//使按钮失效
	GetDlgItem( IDC_DtBtn_SaveImage )->EnableWindow( FALSE );
	GetDlgItem( IDC_DtBtn_2Gray )->EnableWindow( FALSE );
	GetDlgItem( IDC_DtBtn_EqualHist )->EnableWindow( FALSE );
	GetDlgItem( IDC_DtBtn_RemoveNoise )->EnableWindow( FALSE );
	GetDlgItem( IDC_DtBtn_Detect )->EnableWindow( FALSE );
	//GetDlgItem( IDC_DtBtn_OpenDir )->EnableWindow( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



void CDetectDlg::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	//// TODO: 在此处添加消息处理程序代码
	//// 不为绘图消息调用 CDialog::OnPaint()
    CDialog::OnPaint();                    // 重绘对话框
    CDialog::UpdateWindow();                // 更新windows窗口，如果无这步调用，图片显示还会出现问题
    //ShowImage( m_objDetect.m_pReadImage);    // 重绘图片函数
	ShowImage( m_showImage, IDC_DtPc_ShowImage );		// 重绘图片函数

}


//处理函数

void CDetectDlg::ShowImage( IplImage* srcImage)	//显示图像，包括缩小图像
{
//	IplImage* img = cvCreateImage( cvGetSize( srcImage), srcImage->depth, srcImage->nChannels);
//	cvCopy( srcImage, img, NULL);
//	ResizeImage(img);	//图片进行缩放


	if((srcImage->depth != m_showImage->depth) || (srcImage->nChannels != m_showImage->nChannels))
	{
		cvZero(m_showImage);
		m_showImage = cvCreateImage(cvSize(SHOWIMAGE_WIDTH, SHOWIMAGE_HEIGHT), srcImage->depth, srcImage->nChannels);	//测试用
	}


	ResizeImage(srcImage);	//图片进行缩放
	ShowImage( m_showImage, IDC_DtPc_ShowImage );			// 调用显示图片函数	
//	cvReleaseImage( &img);

}

void CDetectDlg::ShowImage( IplImage* img, UINT ID )	// ID 是Picture Control控件的ID号
{
	CDC* pDC = GetDlgItem( ID ) ->GetDC();		// 获得显示控件的 DC
	HDC hDC = pDC ->GetSafeHdc();				// 获取 HDC(设备句柄) 来进行绘图操作

	CRect rect;
	GetDlgItem(ID) ->GetClientRect( &rect );
	int rw = rect.right - rect.left;			// 求出图片控件的宽和高
	int rh = rect.bottom - rect.top;
	int iw = img->width;						// 读取图片的宽和高
	int ih = img->height;
	int tx = (int)(rw - iw)/2;					// 使图片的显示位置正好在控件的正中
	int ty = (int)(rh - ih)/2;
	SetRect( rect, tx, ty, tx+iw, ty+ih );

	CvvImage cimg;
	cimg.CopyOf( img );							// 复制图片
	cimg.DrawToHDC( hDC, &rect );				// 将图片绘制到显示控件的指定区域内

	ReleaseDC( pDC );
}


void CDetectDlg::ResizeImage(IplImage* img)
{
	// 读取图片的宽和高
    int w = img->width;
	int h = img->height;

	// 找出宽和高中的较大值者
	int max = (w > h)? w: h;
	int maxDef = (SHOWIMAGE_WIDTH > SHOWIMAGE_HEIGHT)? SHOWIMAGE_WIDTH : SHOWIMAGE_HEIGHT;

	// 计算将图片缩放到m_showImage区域所需的比例因子
	float scale = (float) ( (float) max / maxDef);
	
	// 缩放后图片的宽和高
	int nw = (int)( w/scale );
	int nh = (int)( h/scale );

	// 为了将缩放后的图片存入 m_showImage 的正中部位，需计算图片在 m_showImage 左上角的期望坐标值
	int tlx = (nw > nh)? 0: (int)(SHOWIMAGE_WIDTH - nw) / 2;
	int tly = (nw > nh)? (int)(SHOWIMAGE_HEIGHT - nh) / 2 : 0;

	// 对上一幅显示的图片数据清零
	if( m_showImage )
	{
		cvZero( m_showImage );
	}

	// 设置 TheImage 的 ROI 区域，用来存入图片 img
	cvSetImageROI( m_showImage, cvRect( tlx, tly, nw, nh) );
	// 对图片 img 进行缩放，并存入到 m_showImage 中
	cvResize( img, m_showImage );
	// 重置 TheImage 的 ROI 准备读入下一幅图片
	cvResetImageROI( m_showImage );
}


//设置消息提示
void CDetectDlg::SetTips( CString strTips)
{
	GetDlgItem( IDC_DtTxt_Tips )->SetWindowText( strTips );
}




// CDetectDlg 消息处理程序

//打开图像
void CDetectDlg::OnBnClickedDtbtnOpenimage()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg( TRUE, _T("*.bmp"), NULL,
					OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
					_T("image files (*.bmp; *.jpg; *.tiff) |*.bmp; *.jpg; *.tiff| All Files (*.*) |*.*||"),
					NULL );										// 选项图片的约定
	dlg.m_ofn.lpstrTitle = _T("打开图片");	// 打开文件对话框的标题名
	if( dlg.DoModal() != IDOK )				// 判断是否获得图片
	{
		return;
	}
	
	CString strPath = dlg.GetPathName();			// 获取图片路径

	if(!m_objDetect.Load(strPath))
	{
		MessageBox(_T("无法打开图片"));
		return;
	}
	ShowImage( m_objDetect.m_pReadImage);
	//m_objDetect.SetGrayImage();
	//ShowImage( m_objDetect.m_pGrayImage);


	//// 使按钮生效
	GetDlgItem( IDC_DtBtn_SaveImage )->EnableWindow( TRUE );
	GetDlgItem( IDC_DtBtn_2Gray )->EnableWindow( TRUE );
	GetDlgItem( IDC_DtBtn_EqualHist )->EnableWindow( TRUE );
	GetDlgItem( IDC_DtBtn_RemoveNoise )->EnableWindow( TRUE );
	GetDlgItem( IDC_DtBtn_Detect )->EnableWindow( TRUE );
	//GetDlgItem( IDC_DtBtn_OpenDir )->EnableWindow( TRUE );

	SetTips(_T("已打开图片：") + strPath);

	//重置默认目录，解决xml路径不正确问题
	if(m_strAppPath != _T(""))
	{
		SetCurrentDirectory(m_strAppPath);
	}

	m_blnIsShowGray = false;	//重置显示灰度图标志位
}


//保存图像
void CDetectDlg::OnBnClickedDtbtnSaveimage()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(
		FALSE, _T("*.bmp"), NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image files (*.jpg; *.bmp) |*.jpg;*.bmp;  *.jpeg | All Files (*.*) |*.*||"), NULL
		);										// 选项图片的约定
	dlg.m_ofn.lpstrTitle = _T("保存图片");	// 打开文件对话框的标题名
	dlg.m_ofn.lpstrDefExt = _T("jpg");
	if( dlg.DoModal() != IDOK )				// 判断是否获得图片
	{
		return;
	}
	CString strPath = dlg.GetPathName();			// 获取图片路径
	if(m_blnIsShowGray)
	{
		cvSaveImage(strPath, m_objDetect.m_pGrayImage);
	}
	else
	{
		cvSaveImage(strPath, m_objDetect.m_pReadImage);
	}
	SetTips(_T("已保存图片到：") + strPath);
	//提示是否打开文件夹
	CString strDir = strPath.Left(strPath.ReverseFind(_T('\\')));
	if(MessageBox(_T("已经保存，是否打开所在文件夹？"), _T("温馨提示"), MB_OKCANCEL) == IDOK)
	{
		 ShellExecute(NULL,NULL,strDir,NULL,NULL,SW_SHOW);
	}

	//重置默认目录，解决xml路径不正确问题
	if(m_strAppPath != _T(""))
	{
		SetCurrentDirectory(m_strAppPath);
	}
}


//人脸检测
void CDetectDlg::OnBnClickedDtbtnDetect()
{
	// TODO: 在此添加控件通知处理程序代码
		//重置默认目录，解决xml路径不正确问题
	if(m_strAppPath != _T(""))
	{
		SetCurrentDirectory(m_strAppPath);
	}
	//如果直接检测，则自动进行去噪处理
	if(!m_blnIsShowGray)
	{
		m_objDetect.RemoveNoise();
	}
	if(m_objDetect.FaceDetect(m_strCascadeName, m_strEyesCascadeName))
	{
		//显示图像
		if(m_blnIsShowGray)
		{
			ShowImage( m_objDetect.m_pGrayImage);
		}
		else
		{
			ShowImage( m_objDetect.m_pReadImage);
		}
		
		//增加提示
		CString strTips;
		int m_facesCount = m_objDetect.m_nFacesCount;
		if (m_facesCount > 0)
		{
			strTips.Format(_T("检测完毕！共检测到%d张人脸！\n人脸图像已保存!"), m_facesCount, m_strAppPath);
		}
		else
		{
			strTips.Format(_T("检测完毕！没有检测到人脸！"), m_facesCount);
		}
		SetTips(strTips);
	}
	else
	{
		SetTips(_T("检测失败：") + m_objDetect.m_strErrMessage);	//检测失败
	}
	//MessageBox("人脸处理完毕");
	GetDlgItem( IDC_DtBtn_Detect )->EnableWindow( FALSE );

}


//打开目录
void CDetectDlg::OnBnClickedDtbtnOpendir()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL,NULL,_T("Faces"),NULL,NULL,SW_SHOW);
}


//关于我们
void CDetectDlg::OnBnClickedDtbtnAbout()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL,"open","http://www.liufu.org/ling",NULL,NULL,SW_SHOW); 
}




void CDetectDlg::OnBnClickedDtbtn2gray()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!m_blnIsShowGray)	//判断是否已经显示灰度图像
	{
		m_objDetect.SetGrayImage();
	}
	ShowImage( m_objDetect.m_pGrayImage);
	m_blnIsShowGray = true;
}

void CDetectDlg::OnBnClickedDtbtnEqualhist()
{
	// TODO: 在此添加控件通知处理程序代码
	m_objDetect.EqualizeHist();
	ShowImage( m_objDetect.m_pGrayImage);
	m_blnIsShowGray = true;
}

void CDetectDlg::OnBnClickedDtbtnRemovenoise()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_objDetect.RemoveNoise(true);
	m_objDetect.RemoveNoise(m_blnIsShowGray);
	ShowImage( m_objDetect.m_pGrayImage);
}
