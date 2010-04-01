// FacesDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FacesDemo.h"
#include "FacesDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CFacesDemoDlg 对话框




CFacesDemoDlg::CFacesDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFacesDemoDlg::IDD, pParent)
	, m_readImage(NULL)
	, m_storage(0)
	, m_cascade(0)
	, m_cascadeName(_T(""))
	, m_facesCount(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFacesDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFacesDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OPEN_IMAGE, &CFacesDemoDlg::OnBnClickedOpenImage)
	ON_BN_CLICKED(IDOK, &CFacesDemoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_SAVE_IMAGE, &CFacesDemoDlg::OnBnClickedSaveImage)
	ON_BN_CLICKED(IDC_ABOUT_US, &CFacesDemoDlg::OnBnClickedAboutUs)
	ON_BN_CLICKED(IDC_DETECT_FACE, &CFacesDemoDlg::OnBnClickedDetectFace)
	ON_BN_CLICKED(IDC_REMOVE_NOISE, &CFacesDemoDlg::OnBnClickedRemoveNoise)
	ON_BN_CLICKED(IDC_BINARY_IMAGE, &CFacesDemoDlg::OnBnClickedBinaryImage)
END_MESSAGE_MAP()


// CFacesDemoDlg 消息处理程序

BOOL CFacesDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标


	//ShowWindow(SW_MINIMIZE);
	//ShowWindow(SW_SHOWNORMAL); 

	// TODO: 在此添加额外的初始化代码

    CvSize imgSize;
    imgSize.height = IMAGE_HEIGHT;
    imgSize.width = IMAGE_WIDTH;
	m_readImage = cvCreateImage( imgSize, IPL_DEPTH_8U, IMAGE_CHANNELS );


	/*
	//读取配置文件
	CString strCascadeName;
	::GetPrivateProfileStringA("Init","CascadeName","-1", 
							strCascadeName.GetBuffer(MAX_PATH),
							MAX_PATH ,
							".\\config.ini"); 
	if(strCascadeName == "-1")
	{
		CString strPath;
		GetCurrentDirectory(MAX_PATH,strPath.GetBuffer(MAX_PATH));
		strPath.ReleaseBuffer();
		m_cascadeName = strPath + "\\haarcascade_frontalface_alt2.xml";
		::WritePrivateProfileString("Init","CascadeName", m_cascadeName,".\\config.ini");
		//MessageBox("配置不正确！");
	}
	else
	{
		m_cascadeName = strCascadeName;
		//MessageBox("配置正确！");
	}
	*/

	/*
	if(::WritePrivateProfileString("Init","CascadeName", m_cascadeName,".\\student.ini"))
	{
		MessageBox("写入成功");
	}
	*/

	

	//使人脸检测等按钮失效
	GetDlgItem( IDC_SAVE_IMAGE )->EnableWindow( FALSE );
	GetDlgItem( IDC_DETECT_FACE )->EnableWindow( FALSE );
	GetDlgItem( IDC_REMOVE_NOISE )->EnableWindow( FALSE );
	GetDlgItem( IDC_BINARY_IMAGE )->EnableWindow( FALSE );

	InitConfig();	//初始化配置文件


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFacesDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFacesDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);

        CDialog::OnPaint();                    // 重绘对话框
        CDialog::UpdateWindow();                // 更新windows窗口，如果无这步调用，图片显示还会出现问题
        ShowImage( m_readImage, IDC_IMAGE );    // 重绘图片函数

	}
	else
	{
		CDialog::OnPaint();						// 重绘对话框

		CDialog::UpdateWindow();				// 更新windows窗口，如果无这步调用，图片显示还会出现问题
		ShowImage( m_readImage, IDC_IMAGE );		// 重绘图片函数

	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFacesDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



/***************************************/
/*          处理函数                ****/
/***************************************/

void CFacesDemoDlg::InitConfig()
{
	//读取配置文件
	CString strCascadeName;
	::GetPrivateProfileStringA("Init","CascadeName","-1", 
							strCascadeName.GetBuffer(MAX_PATH),
							MAX_PATH ,
							".\\config.ini"); 
	if(strCascadeName == "-1")
	{
		CString strPath;
		GetCurrentDirectory(MAX_PATH,strPath.GetBuffer(MAX_PATH));
		strPath.ReleaseBuffer();
		m_cascadeName = strPath + "\\haarcascade_frontalface_alt2.xml";
		::WritePrivateProfileString("Init","CascadeName", m_cascadeName,".\\config.ini");
		//MessageBox("配置不正确！");
	}
	else
	{
		m_cascadeName = strCascadeName;
		//MessageBox("配置正确！");
	}

	/*
	if(::WritePrivateProfileString("Init","CascadeName", m_cascadeName,".\\student.ini"))
	{
		MessageBox("写入成功");
	}
	*/


}


void CFacesDemoDlg::ShowImage( IplImage* img, UINT ID )	// ID 是Picture Control控件的ID号
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


void CFacesDemoDlg::ResizeImage(IplImage* img)
{
	// 读取图片的宽和高
    int w = img->width;
	int h = img->height;

	// 找出宽和高中的较大值者
	int max = (w > h)? w: h;

	// 计算将图片缩放到TheImage区域所需的比例因子
	float scale = (float) ( (float) max / 256.0f );
	
	// 缩放后图片的宽和高
	int nw = (int)( w/scale );
	int nh = (int)( h/scale );

	// 为了将缩放后的图片存入 TheImage 的正中部位，需计算图片在 TheImage 左上角的期望坐标值
	int tlx = (nw > nh)? 0: (int)(256-nw)/2;
	int tly = (nw > nh)? (int)(256-nh)/2: 0;

	// 设置 TheImage 的 ROI 区域，用来存入图片 img
	cvSetImageROI( m_readImage, cvRect( tlx, tly, nw, nh) );

	// 对图片 img 进行缩放，并存入到 TheImage 中
	cvResize( img, m_readImage );

	// 重置 TheImage 的 ROI 准备读入下一幅图片
	cvResetImageROI( m_readImage );
}


void CFacesDemoDlg::FaceDetect( IplImage* img )
{
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
    IplImage* gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );
    IplImage* small_img = cvCreateImage( cvSize( cvRound (img->width/scale),
                         cvRound (img->height/scale)),
                     8, 1 );
    int i;
 
    cvCvtColor( img, gray, CV_BGR2GRAY );
    cvResize( gray, small_img, CV_INTER_LINEAR );
    cvEqualizeHist( small_img, small_img );
    cvClearMemStorage( m_storage );
 
    if( m_cascade )
    {
        double t = (double)cvGetTickCount();
        CvSeq* faces = cvHaarDetectObjects( small_img, m_cascade, m_storage,
                                            1.1, 2, 0/*CV_HAAR_DO_CANNY_PRUNING*/,
                                            cvSize(30, 30) );
        t = (double)cvGetTickCount() - t;
        //printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );
		if(faces)
		{
			m_facesCount = faces->total;
		}
        for( i = 0; i < (faces ? faces->total : 0); i++ )
        {
            CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
            CvPoint center;
            int radius;
            center.x = cvRound((r->x + r->width*0.5)*scale);
            center.y = cvRound((r->y + r->height*0.5)*scale);
            radius = cvRound((r->width + r->height)*0.25*scale);

			/*
			///方法二：仅仅将获取图片保存为实际区域的大小///
			char filename[40] = "face.jpg";
			//IplImage* img2 = cvCreateImage(cvSize(r->width+1,r->height+1), img->depth, img->nChannels ); 
			//CvRect rect2 = cvRect(r->x - r->width, r->y, r->width+1,r->height+1);
			//CvRect rect2 = cvRect(center.x - r->width, center.y - r->height, r->width+1,r->height+1);
			int rect_size = 3;
			CvRect rect2 = cvRect(center.x - radius + rect_size, center.y - radius + rect_size, radius*2 - rect_size*2, radius*2 - rect_size*2);
			IplImage* img2 = cvCreateImage(cvSize(rect2.width , rect2.height), img->depth, img->nChannels ); 
			cvSetImageROI(img, rect2);
			cvCopy(img,img2);//复制对象区域 
			cvShowImage( "result", img2 );
			cvResetImageROI(img);
			sprintf(filename,"images/face%d.jpg",i+1);
			cvSaveImage(filename, img2);
			printf( "Save img:%s\n", filename );
			*/


            cvCircle( img, center, radius, colors[i%8], 3, 8, 0 );
        }
    }
 
    //cvShowImage( "result", img );
	// 显示图像
	ShowImage(img, IDC_IMAGE);
	//释放图像
    cvReleaseImage( &gray );
    cvReleaseImage( &small_img );
}

void CFacesDemoDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	cvDestroyAllWindows();
	OnOK();
}


void CFacesDemoDlg::OnBnClickedOpenImage()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(
		TRUE, _T("*.bmp"), NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image files (*.bmp; *.jpg) |*.bmp; *.jpg; *.jpeg | All Files (*.*) |*.*||"), NULL
		);										// 选项图片的约定
	dlg.m_ofn.lpstrTitle = _T("打开图片");	// 打开文件对话框的标题名
	if( dlg.DoModal() != IDOK )					// 判断是否获得图片
		return;
	
	CString mPath = dlg.GetPathName();			// 获取图片路径

	IplImage* ipl = cvLoadImage( mPath, 1 );	// 读取图片、缓存到一个局部变量 ipl 中
	if( !ipl )									// 判断是否成功读取图片
		return;
	if( m_readImage )								// 对上一幅显示的图片数据清零
		cvZero( m_readImage );
	ResizeImage( ipl );	// 对读入的图片进行缩放，使其宽或高最大值者刚好等于 256，再复制到 TheImage 中
	ShowImage( m_readImage, IDC_IMAGE );			// 调用显示图片函数	
	cvReleaseImage( &ipl );						// 释放 ipl 占用的内存

	// 使边缘检测按钮生效
	GetDlgItem( IDC_DETECT_FACE )->EnableWindow( TRUE );
	GetDlgItem( IDC_SAVE_IMAGE )->EnableWindow( TRUE );

}


//保存图片
void CFacesDemoDlg::OnBnClickedSaveImage()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(
		FALSE, _T("*.bmp"), NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image files (*.jpg; *.bmp) |*.jpg;*.bmp;  *.jpeg | All Files (*.*) |*.*||"), NULL
		);										// 选项图片的约定
	dlg.m_ofn.lpstrTitle = _T("保存图片");	// 打开文件对话框的标题名
	dlg.m_ofn.lpstrDefExt = "jpg";
	if( dlg.DoModal() != IDOK )				// 判断是否获得图片
	{
		return;
	}
	CString mPath = dlg.GetPathName();			// 获取图片路径
	cvSaveImage(mPath, m_readImage);

}

//关于对话框
void CFacesDemoDlg::OnBnClickedAboutUs()
{
	// TODO: 在此添加控件通知处理程序代码
	/*
	CString strAbout;
	strAbout = "人脸检测 V1.0\n";
	strAbout = strAbout + "Version:  1.0.0\n";
	strAbout = strAbout + "Author :  Foolin\n";
	strAbout = strAbout + "E-mail :  Foolin55@gmail.com\n";
	MessageBox(strAbout);
	*/
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();

	
}


/***************************************/
/*              人脸检测            ****/
/***************************************/
void CFacesDemoDlg::OnBnClickedDetectFace()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_cascadeName = _T("E:\\OpenCV2.0\\data\\haarcascades\\haarcascade_frontalface_alt2.xml");
	//MessageBox(m_cascadeName);
	//m_cascadeName = "haarcascade_frontalface_alt2.xml";
	m_cascade = (CvHaarClassifierCascade*)cvLoad( m_cascadeName, 0, 0, 0 );
    if( !m_cascade )
    {
        MessageBox(_T("对不起，人脸检测cascade配置不正确"+m_cascadeName));
        return;
    }
    m_storage = cvCreateMemStorage(0);
	if( !m_readImage )
	{
		MessageBox(_T("请先打开图像"));
		return;
	}

    //人脸检测
	FaceDetect(m_readImage);
	
	CString strTips;
	if (m_facesCount > 0)
	{
		strTips.Format("图像处理完毕！共检测到%d张人脸！", m_facesCount);
	}
	else
	{
		strTips.Format("图像处理完毕！没有检测到人脸！", m_facesCount);
	}
	MessageBox(_T(strTips));
}

//消除噪声
void CFacesDemoDlg::OnBnClickedRemoveNoise()
{
	// TODO: 在此添加控件通知处理程序代码
}

//二值化
void CFacesDemoDlg::OnBnClickedBinaryImage()
{
	// TODO: 在此添加控件通知处理程序代码
}
