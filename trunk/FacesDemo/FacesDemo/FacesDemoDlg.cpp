// FacesDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FacesDemo.h"
#include "FacesDemoDlg.h"
#include "Helper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_MY_WIND_MINIMIZE (WM_USER + 1)	//�Զ�����Ϣ����С������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CFacesDemoDlg �Ի���




CFacesDemoDlg::CFacesDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFacesDemoDlg::IDD, pParent)
	, m_readImage(NULL)
	, m_showImage(NULL)
	, m_storage(0)
	, m_cascade(0)
	, m_cascadeName(_T(""))
	, m_facesCount(0)
	, m_programPath(_T(""))
	, m_runCamera(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFacesDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_CamList, m_camList);
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
	//ON_BN_CLICKED(IDC_REMOVE_NOISE, &CFacesDemoDlg::OnBnClickedRemoveNoise)
	//ON_BN_CLICKED(IDC_BINARY_IMAGE, &CFacesDemoDlg::OnBnClickedBinaryImage)
	ON_BN_CLICKED(IDC_MINIMIZE, &CFacesDemoDlg::OnBnClickedMinimize)
	ON_MESSAGE(WM_MY_WIND_MINIMIZE, OnWindMinimize)
	ON_COMMAND(ID_Min2Tray, &CFacesDemoDlg::OnBnClickedMinimize)
	ON_COMMAND(ID_Open, &CFacesDemoDlg::OnOpen)
	ON_COMMAND(ID_Quit, &CFacesDemoDlg::OnQuit)
	ON_COMMAND(ID_About, &CFacesDemoDlg::OnAbout)
	ON_COMMAND(ID_OpenImage, &CFacesDemoDlg::OnBnClickedOpenImage)
	ON_COMMAND(ID_SaveImage, &CFacesDemoDlg::OnBnClickedSaveImage)
	ON_COMMAND(ID_FaceDect, &CFacesDemoDlg::OnBnClickedDetectFace)
	ON_COMMAND(ID_OpenFacesDir, &CFacesDemoDlg::OnBnClickedOpenFacesDir)
	ON_BN_CLICKED(IDC_RunCam, &CFacesDemoDlg::OnBnClickedRuncam)
	ON_BN_CLICKED(IDC_StopCam, &CFacesDemoDlg::OnBnClickedStopcam)
	ON_BN_CLICKED(IDC_OPEN_FACES_DIR, &CFacesDemoDlg::OnBnClickedOpenFacesDir)
	ON_COMMAND(ID_HomePage, &CFacesDemoDlg::OnHomepage)
	ON_COMMAND(ID_PostMsg, &CFacesDemoDlg::OnPostmsg)
END_MESSAGE_MAP()


// CFacesDemoDlg ��Ϣ��������

BOOL CFacesDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵������ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��


	//ShowWindow(SW_MINIMIZE);
	//ShowWindow(SW_SHOWNORMAL); 

	// TODO: �ڴ����Ӷ���ĳ�ʼ������

    CvSize imgSize;
    imgSize.height = IMAGE_HEIGHT;
    imgSize.width = IMAGE_WIDTH;
	m_showImage = cvCreateImage( imgSize, IPL_DEPTH_8U, IMAGE_CHANNELS );


	CMenu menu;
	menu.LoadMenu(IDR_MENU_TOP);
	SetMenu(&menu);
	menu.Detach();


	/*
	//��ȡ�����ļ�
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
		//SetTips("���ò���ȷ��");
	}
	else
	{
		m_cascadeName = strCascadeName;
		//SetTips("������ȷ��");
	}
	*/

	/*
	if(::WritePrivateProfileString("Init","CascadeName", m_cascadeName,".\\student.ini"))
	{
		SetTips("д��ɹ�");
	}
	*/

	

	//ʹ�������Ȱ�ťʧЧ
	GetDlgItem( IDC_SAVE_IMAGE )->EnableWindow( FALSE );
	GetDlgItem( IDC_DETECT_FACE )->EnableWindow( FALSE );
	GetDlgItem( IDC_StopCam )->EnableWindow( FALSE );
	//GetDlgItem( IDC_REMOVE_NOISE )->EnableWindow( FALSE );
	//GetDlgItem( IDC_BINARY_IMAGE )->EnableWindow( FALSE );

	//��ȡ����·��
	CString strPath;
	GetCurrentDirectory(MAX_PATH,strPath.GetBuffer(MAX_PATH));
	strPath.ReleaseBuffer();
	m_programPath = strPath;

	//��ʼ�������ļ�
	InitConfig();	


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի���������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFacesDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);

        CDialog::OnPaint();                    // �ػ�Ի���
        CDialog::UpdateWindow();                // ����windows���ڣ�������ⲽ���ã�ͼƬ��ʾ�����������
        ShowImage( m_showImage, IDC_IMAGE );    // �ػ�ͼƬ����

	}
	else
	{
		CDialog::OnPaint();						// �ػ�Ի���

		CDialog::UpdateWindow();				// ����windows���ڣ�������ⲽ���ã�ͼƬ��ʾ�����������
		ShowImage( m_showImage, IDC_IMAGE );		// �ػ�ͼƬ����

	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFacesDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



/***************************************/
/*          ��������                ****/
/***************************************/

void CFacesDemoDlg::InitConfig()
{
	//��ȡ�����ļ�
	CString strCascadeName;
	::GetPrivateProfileString(_T("Init"),_T("CascadeName"),_T("-1"), strCascadeName.GetBuffer(MAX_PATH),
							MAX_PATH , _T(".\\config.ini")); 
	if(strCascadeName == _T("-1"))
	{
		/*
		CString strPath;
		GetCurrentDirectory(MAX_PATH,strPath.GetBuffer(MAX_PATH));
		strPath.ReleaseBuffer();
		m_cascadeName = strPath + "\\haarcascade_frontalface_alt2.xml";
		*/
		m_cascadeName = m_programPath + _T("\\haarcascade_frontalface_alt_tree.xml");
		::WritePrivateProfileString(_T("Init"),_T("CascadeName"), m_cascadeName,_T(".\\config.ini"));
		//SetTips("���ò���ȷ��");
	}
	else
	{
		m_cascadeName = strCascadeName;
		//SetTips("������ȷ��");
	}

	/*
	if(::WritePrivateProfileString("Init","CascadeName", m_cascadeName,".\\student.ini"))
	{
		SetTips("д��ɹ�");
	}
	*/
	
	//�����������Ŀ¼���򴴽�������Ŀ¼
	if( !MyDirExist(_T("Faces")) )
	{
		MyCreateDir(_T("Faces"));
	}


}

void CFacesDemoDlg::ShowImage( IplImage* srcImage)	//��ʾͼ�񣬰�����Сͼ��
{
//	IplImage* img = cvCreateImage( cvGetSize( srcImage), srcImage->depth, srcImage->nChannels);
//	cvCopy( srcImage, img, NULL);
//	ResizeImage(img);	//ͼƬ��������
	ResizeImage(srcImage);	//ͼƬ��������
	ShowImage( m_showImage, IDC_IMAGE );			// ������ʾͼƬ����	
//	cvReleaseImage( &img);

}


void CFacesDemoDlg::ShowImage( IplImage* img, UINT ID )	// ID ��Picture Control�ؼ���ID��
{
	CDC* pDC = GetDlgItem( ID ) ->GetDC();		// �����ʾ�ؼ��� DC
	HDC hDC = pDC ->GetSafeHdc();				// ��ȡ HDC(�豸���) �����л�ͼ����

	CRect rect;
	GetDlgItem(ID) ->GetClientRect( &rect );
	int rw = rect.right - rect.left;			// ���ͼƬ�ؼ��Ŀ��͸�
	int rh = rect.bottom - rect.top;
	int iw = img->width;						// ��ȡͼƬ�Ŀ��͸�
	int ih = img->height;
	int tx = (int)(rw - iw)/2;					// ʹͼƬ����ʾλ�������ڿؼ�������
	int ty = (int)(rh - ih)/2;
	SetRect( rect, tx, ty, tx+iw, ty+ih );

	CvvImage cimg;
	cimg.CopyOf( img );							// ����ͼƬ
	cimg.DrawToHDC( hDC, &rect );				// ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������

	ReleaseDC( pDC );
}


void CFacesDemoDlg::ResizeImage(IplImage* img)
{
	// ��ȡͼƬ�Ŀ��͸�
    int w = img->width;
	int h = img->height;

	// �ҳ����͸��еĽϴ�ֵ��
	int max = (w > h)? w: h;
	int maxDef = (IMAGE_WIDTH > IMAGE_HEIGHT)? IMAGE_WIDTH : IMAGE_HEIGHT;

	// ���㽫ͼƬ���ŵ�m_showImage��������ı�������
	float scale = (float) ( (float) max / maxDef);
	
	// ���ź�ͼƬ�Ŀ��͸�
	int nw = (int)( w/scale );
	int nh = (int)( h/scale );

	// Ϊ�˽����ź��ͼƬ���� m_showImage �����в�λ�������ͼƬ�� m_showImage ���Ͻǵ���������ֵ
	int tlx = (nw > nh)? 0: (int)(IMAGE_WIDTH - nw) / 2;
	int tly = (nw > nh)? (int)(IMAGE_HEIGHT - nh) / 2 : 0;

	// ����һ����ʾ��ͼƬ��������
	if( m_showImage )
	{
		cvZero( m_showImage );
	}

	// ���� TheImage �� ROI ������������ͼƬ img
	cvSetImageROI( m_showImage, cvRect( tlx, tly, nw, nh) );
	// ��ͼƬ img �������ţ������뵽 m_showImage ��
	cvResize( img, m_showImage );
	// ���� TheImage �� ROI ׼��������һ��ͼƬ
	cvResetImageROI( m_showImage );
}


/*** ����ͼƬ��С ***/
void CFacesDemoDlg::SetReadImage( IplImage* image, int maxWidth, int maxHeight)	//����ͼƬ��С
{
	//����Ƿ����ͼƬ
	if(!image)
	{
		return;
	}

	//������ر���
	IplImage *tempImage;		//Ŀ��ͼ��
	double scale = 0.5;		//���ű���Ϊ0.5��
	//double scaleW = (double) IMAGE_MAX_WIDTH / srcImage->width;		//Ĭ��
	//double scaleH = (double) IMAGE_MAX_HEIGHT / srcImage->height;		//Ĭ��
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
		if(m_readImage)
		{
			cvZero( m_readImage );	//���ͼ��
		}
		//����ԭͼ��
		m_readImage = cvCreateImage( cvGetSize(image), image->depth, image->nChannels);	//����Ŀ��ͼ��
		cvCopy( image, m_readImage, NULL);
		return;
	}

	//�����ſ��Ⱥ͸߶�
	tempSize.width = cvRound( image->width * scale );
	tempSize.height = cvRound( image->height * scale);

	//��СͼƬ
	tempImage = cvCreateImage( tempSize, image->depth, image->nChannels);	//����Ŀ��ͼ��
	cvResize(image, tempImage, CV_INTER_LINEAR);	//����Դͼ��Ŀ��ͼ��

	//����ͼ�񲢸�ֵ��image
	if(m_readImage)
	{
		cvZero( m_readImage );	//���ͼ��
	}
    m_readImage = cvCreateImage( tempSize, tempImage->depth, tempImage->nChannels);	//����Ŀ��ͼ��
    cvCopy(tempImage, m_readImage, NULL);

	//�ͷ���ʱԭͼ��
	cvReleaseImage( &tempImage);

}

void CFacesDemoDlg::Normalization(IplImage* image)
{

	IplImage* imgCopy = cvCloneImage(image);
	cvZero( image );
	cvSetImageROI( image, cvRect( 0, 0, FACE_WIDTH, FACE_HEIGHT) );
	cvResize( imgCopy, image , CV_INTER_LINEAR);
	cvReleaseImage( &imgCopy);

	// ���� TheImage �� ROI ������������ͼƬ img
	//cvSetImageROI( m_showImage, cvRect( tlx, tly, nw, nh) );
	// ��ͼƬ img �������ţ������뵽 m_showImage ��
	//cvResize( img, m_showImage );
	// ���� TheImage �� ROI ׼��������һ��ͼƬ
	//cvResetImageROI( m_showImage );
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

        //CvSeq* faces = cvHaarDetectObjects( small_img, m_cascade, m_storage,
                                           // 1.1, 2, 0/*CV_HAAR_DO_CANNY_PRUNING*/,
                                            //cvSize(30, 30) );
        CvSeq* faces = cvHaarDetectObjects( small_img, m_cascade, m_storage,
                                            1.1, 2, CV_HAAR_DO_CANNY_PRUNING,
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
			
			//������Ƭ
			CString faceName = _T("Faces/");
			//IplImage* img2 = cvCreateImage(cvSize(r->width+1,r->height+1), img->depth, img->nChannels ); 
			//CvRect rect2 = cvRect(r->x - r->width, r->y, r->width+1,r->height+1);
			//CvRect rect2 = cvRect(center.x - r->width, center.y - r->height, r->width+1,r->height+1);
			int rect_size = 3;
			CvRect rect2 = cvRect(center.x - radius + rect_size, center.y - radius + rect_size, radius*2 - rect_size*2, radius*2 - rect_size*2);
			IplImage* img2 = cvCreateImage(cvSize(rect2.width , rect2.height), img->depth, img->nChannels ); 
			cvSetImageROI(img, rect2);
			cvCopy(img,img2);//���ƶ������� 
			//cvShowImage( "result", img2 );
			cvResetImageROI(img);
			faceName.Format(_T("Faces/%s%d.jpg"), MyGetRand(), i+1);
			//sprintf(filename,"images/face%d.jpg",i+1);
			Normalization(img2);	//��һ������ͷ��
			cvSaveImage(faceName, img2);
			//printf( "Save img:%s\n", filename );
			//������ʾ
			SetTips(faceName);
			
			//������
			cvRectangle( img, cvPoint(rect2.x, rect2.y), cvPoint(rect2.x + rect2.width, rect2.y + rect2.height), colors[i%8], 3, 8, 0);
			//��Բ
            //cvCircle( img, center, radius, colors[i%8], 3, 8, 0 );
        }
    }
 

	// ��ʾͼ��
	//ShowImage(img, IDC_IMAGE);
	//ShowImage(img);

	//�ͷ�ͼ��
    cvReleaseImage( &gray );
    cvReleaseImage( &small_img );
}

void CFacesDemoDlg::SetTips( CString strTips)
{
	GetDlgItem( IDC_STATIC_TIPS )->SetWindowText( strTips );
}

void CFacesDemoDlg::OnBnClickedOk()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	cvDestroyAllWindows();
	OnOK();
}


void CFacesDemoDlg::OnBnClickedOpenImage()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	CFileDialog dlg( TRUE, _T("*.bmp"), NULL,
					OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
					_T("image files (*.bmp; *.jpg; *.tiff) |*.bmp; *.jpg; *.tiff| All Files (*.*) |*.*||"),
					NULL );										// ѡ��ͼƬ��Լ��
	dlg.m_ofn.lpstrTitle = _T("��ͼƬ");	// ���ļ��Ի���ı�����
	if( dlg.DoModal() != IDOK )				// �ж��Ƿ���ͼƬ
	{
		return;
	}
	
	CString mPath = dlg.GetPathName();			// ��ȡͼƬ·��
	IplImage* srcImage = cvLoadImage( mPath, 1 );	// ��ȡͼƬ�����浽һ���ֲ����� srcImage ��

	if( !srcImage )									// �ж��Ƿ�ɹ���ȡͼƬ
	{
		return;
	}

	SetReadImage( srcImage, 800, 600 );	//�������


	//if( m_showImage )								// ����һ����ʾ��ͼƬ��������
		//cvZero( m_showImage );
	/*
	ResizeImage( srcImage );	// �Զ����ͼƬ�������ţ�ʹ���������ֵ�߸պõ��� 256���ٸ��Ƶ� TheImage ��
	ShowImage( m_showImage, IDC_IMAGE );			// ������ʾͼƬ����	
	*/
	ShowImage( srcImage );			//��ʾͼ��
	cvReleaseImage( &srcImage );	// �ͷ�ռ�õ��ڴ�

	// ʹ��Ե��ⰴť��Ч
	GetDlgItem( IDC_DETECT_FACE )->EnableWindow( TRUE );
	GetDlgItem( IDC_SAVE_IMAGE )->EnableWindow( TRUE );
	//GetDlgItem( IDC_REMOVE_NOISE )->EnableWindow( TRUE );
	//GetDlgItem( IDC_BINARY_IMAGE )->EnableWindow( TRUE );

	SetTips(_T("�Ѵ�ͼƬ��") + mPath);

}


//����ͼƬ
void CFacesDemoDlg::OnBnClickedSaveImage()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	CFileDialog dlg(
		FALSE, _T("*.bmp"), NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image files (*.jpg; *.bmp) |*.jpg;*.bmp;  *.jpeg | All Files (*.*) |*.*||"), NULL
		);										// ѡ��ͼƬ��Լ��
	dlg.m_ofn.lpstrTitle = _T("����ͼƬ");	// ���ļ��Ի���ı�����
	dlg.m_ofn.lpstrDefExt = _T("jpg");
	if( dlg.DoModal() != IDOK )				// �ж��Ƿ���ͼƬ
	{
		return;
	}
	CString strPath = dlg.GetPathName();			// ��ȡͼƬ·��
	cvSaveImage(strPath, m_readImage);
	SetTips(_T("�ѱ���ͼƬ����") + strPath);
	//��ʾ�Ƿ���ļ���
	CString strDir = strPath.Left(strPath.ReverseFind(_T('/')));
	if(MessageBox(_T("�Ѿ����棬�Ƿ�������ļ��У�"), _T("��ܰ��ʾ"), MB_OKCANCEL) == IDOK)
	{
		 ShellExecute(NULL,NULL,strDir,NULL,NULL,SW_SHOW);
	}

}

//���ڶԻ���
void CFacesDemoDlg::OnBnClickedAboutUs()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();

	
}


/***************************************/
/*              �������            ****/
/***************************************/
void CFacesDemoDlg::OnBnClickedDetectFace()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	//m_cascadeName = _T("E:\\OpenCV2.0\\data\\haarcascades\\haarcascade_frontalface_alt2.xml");
	//SetTips(m_cascadeName);
	//m_cascadeName = "haarcascade_frontalface_alt2.xml";

	//����Ĭ��Ŀ¼�����xml·������ȷ����
	SetCurrentDirectory(m_programPath);
	m_cascade = (CvHaarClassifierCascade*)cvLoad( m_cascadeName, 0, 0, 0 );
    if( !m_cascade )
    {
        MessageBox(_T("�Բ����������cascade���ò���ȷ"));
        return;
    }
    m_storage = cvCreateMemStorage(0);
	if( !m_readImage )
	{
		 MessageBox(_T("���ȴ�ͼ��"));
		return;
	}

	/*
	//ȥ������
	SetTips(_T("����ȥ������..."));
	IplImage* dst = cvCreateImage( cvGetSize( m_readImage),
							m_readImage->depth, m_readImage->nChannels);
	cvSmooth( m_readImage,dst,CV_MEDIAN,3,0);
	SetReadImage( dst );
	ShowImage( dst );


    //�������
	SetTips(_T("�����������..."));
	FaceDetect( dst );
	SetReadImage( dst );
	ShowImage(dst);
	cvReleaseImage( &dst);
	*/

    //�������
	SetTips(_T("�����������..."));
	FaceDetect( m_readImage );
	ShowImage(m_readImage);
	
	CString strTips;
	if (m_facesCount > 0)
	{
		strTips.Format(_T("�����ϣ�����⵽%d��������"), m_facesCount);
	}
	else
	{
		strTips.Format(_T("�����ϣ�û�м�⵽������"), m_facesCount);
	}
	SetTips(strTips);
	/*
	if(m_facesCount > 0)
	{
		if(MessageBox(strTips + _T("\n\n�����������ļ��У�"), _T("��ܰ��ʾ"), MB_OKCANCEL) == IDOK)
		{
			ShellExecute(NULL,NULL,_T("Faces"),NULL,NULL,SW_SHOW);
		}
	}
	*/

}

/*
//��������
void CFacesDemoDlg::OnBnClickedRemoveNoise()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	IplImage* dst = cvCreateImage( cvGetSize( m_readImage),
						m_readImage->depth, m_readImage->nChannels);
	cvSmooth( m_readImage,dst,CV_MEDIAN,3,0);
	SetReadImage( dst );
	ShowImage( dst );
	cvReleaseImage( &dst);
	SetTips(_T("ȥ������������ϣ�"));
	
}

//��ֵ��
void CFacesDemoDlg::OnBnClickedBinaryImage()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	IplImage* dst = cvCreateImage( cvGetSize( m_readImage), m_readImage->depth, m_readImage->nChannels);
	//IplImage* dst = cvCreateImage( cvGetSize( m_readImage), IPL_DEPTH_8U, 1);
	//cvCvtColor(m_readImage, dst, CV_BGR2GRAY);	//��ɫ��Ҷ�
	cvThreshold( m_readImage, dst , 80, 255, CV_THRESH_BINARY ); //ȡ��ֵΪ50��ͼ��תΪ��ֵͼ��
	SetReadImage( dst );
	ShowImage( dst );
	cvReleaseImage( &dst);
	SetTips(_T("��ֵ��������ϣ�"));
}
*/

void CFacesDemoDlg::OnBnClickedMinimize()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	TrayMessage(NIM_ADD);	//��С��
	ShowWindow(SW_HIDE);	//���������� 
}

LRESULT CFacesDemoDlg::OnWindMinimize(WPARAM wParam,LPARAM lParam) 
//wParam���յ���ͼ���ID����lParam���յ���������Ϊ 
{ 
	if(wParam!=IDR_MAINFRAME) 
		return 1; 
	switch(lParam) 
	{ 
		case WM_RBUTTONUP://�Ҽ�����ʱ������ݲ˵�������ֻ��һ�����رա� 
		{
			LPPOINT lpoint = new tagPOINT; 
			::GetCursorPos(lpoint);//�õ����λ�� 
			
			/*
			CMenu menu; 
			menu.CreatePopupMenu();//����һ������ʽ�˵� 
			//���Ӳ˵���رա������������ϢWM_DESTROY�������ڣ��� 
			//���أ�������������� 
			menu.AppendMenu(MF_STRING, WM_DESTROY, "�ر�"); 
			//ȷ������ʽ�˵���λ�� 
			menu.TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this); 
			//��Դ���� 
			HMENU hmenu = menu.Detach(); 
			menu.DestroyMenu(); 
			*/

			CMenu menu;
			menu.LoadMenu(IDR_MENU_TRAY);
			CMenu* pPopup = menu.GetSubMenu(0);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this);
			//��Դ���� 
			HMENU hmenu = pPopup->Detach(); 
			pPopup->DestroyMenu();
			menu.DestroyMenu();

			delete lpoint; 
		} 
		break; 


		case WM_LBUTTONDBLCLK://˫������Ĵ��� 
		{ 
			TrayMessage(NIM_DELETE);	//ɾ������ͼ��
			this->ShowWindow(SW_SHOW);	//��ʾ������
		} 
		break; 
	} 

	return 0; 
}

//��С��������Ӧ����
bool CFacesDemoDlg::TrayMessage( DWORD dwFlag, UINT uIconId)
{
	CString strTips;
	//strTips = _T("Test");
	NOTIFYICONDATA notify;

	notify.cbSize=sizeof(NOTIFYICONDATA);
	notify.hWnd= m_hWnd;
	notify.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP;
	notify.uCallbackMessage = WM_MY_WIND_MINIMIZE;   //�û�����Ļص���Ϣ
	//wcscpy_s(notify.szTip, 128, strTips);
	strcpy(notify.szTip,_T("���������ָ����")); //��Ϣ��ʾ
	notify.uID=uIconId;
	HICON  hIcon= AfxGetApp()->LoadIcon(uIconId);
	notify.hIcon=hIcon;

	return ::Shell_NotifyIcon(dwFlag, &notify)?true:false;
}

void CFacesDemoDlg::OnOpen()
{
	// TODO: �ڴ�����������������
	TrayMessage(NIM_DELETE);	//ɾ������ͼ��
	this->ShowWindow(SW_SHOW);	//��ʾ������
}

void CFacesDemoDlg::OnQuit()
{
	// TODO: �ڴ�����������������
	PostQuitMessage(0);	//�����˳���Ϣ
}

void CFacesDemoDlg::OnAbout()
{
	// TODO: �ڴ�����������������
	CAboutDlg dlg;
	dlg.DoModal();
}
void CFacesDemoDlg::OnBnClickedRuncam()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	m_runCamera = 1;

	CvCapture* capture = 0;
	IplImage *frame;
	capture = cvCaptureFromCAM(0);
	if(!capture)
	{
		MessageBox("δ����������ͷ����������ͷ�豸��");
		return;
	}

	// ʹ��Ե��ⰴť��Ч
	GetDlgItem( IDC_DETECT_FACE )->EnableWindow( TRUE );
	GetDlgItem( IDC_SAVE_IMAGE )->EnableWindow( TRUE );
	GetDlgItem( IDC_RunCam )->EnableWindow( FALSE );
	GetDlgItem( IDC_StopCam )->EnableWindow( TRUE );

	// ����һ����Ƶ���Ŵ���
	cvNamedWindow("camera");
	// ���������������С
	cvResizeWindow("camera",0,0);
	// ��ȡ���ھ��
	HWND hWnd = (HWND) cvGetWindowHandle("camera");
	// ��ȡ�ô��ڵĸ����ھ��
	HWND hParent = ::GetParent(hWnd);
	// ��ȡmymfc��GUI���ھ��
	/*
	HWND hwnd1=::FindWindow("CFacesDemoDlg","FacesDemo");
	// ��GUI��������Ϊ��Ƶ���Ŵ��ڵĸ����ڣ�ʹ��Ƶ�� GUI ��ָ�����򲥷�
	::SetParent(hWnd, hwnd1);
	*/
	::SetParent(hWnd, this->m_hWnd);
	// ��������������Ƶ���Ŵ���
	::ShowWindow(hWnd, SW_HIDE);
	::ShowWindow(hParent, SW_HIDE);



	//����Ĭ��Ŀ¼�����xml·������ȷ����
	SetCurrentDirectory(m_programPath);
	m_cascade = (CvHaarClassifierCascade*)cvLoad( m_cascadeName, 0, 0, 0 );
    if( !m_cascade )
    {
        MessageBox(_T("�Բ����������cascade���ò���ȷ"));
        return;
    }
    m_storage = cvCreateMemStorage(0);


	int i = 0;
	while(m_runCamera)
	{

		/*
		IplImage *frame_copy = 0;

        if( !cvGrabFrame( capture ))
            break;
		frame = cvRetrieveFrame( capture );
        if( !frame )
            break;
        if( !frame_copy )
            frame_copy = cvCreateImage( cvSize(frame->width,frame->height),
                                        IPL_DEPTH_8U, frame->nChannels );
        if( frame->origin == IPL_ORIGIN_TL )
            cvCopy( frame, frame_copy, 0 );
        else
            cvFlip( frame, frame_copy, 0 );
		*/

		//��ȡһ֡
		frame = cvQueryFrame( capture );
		IplImage* frameCopy = cvCloneImage(frame);

		//�������
		FaceDetect( frameCopy );

		SetReadImage(frameCopy);
		ShowImage(frameCopy);
		cvWaitKey(500);
		//Sleep(2000);
		CString strTips;
		strTips.Format(_T("�Ѿ���������ͷ:%ds"), ++i);
		SetTips(strTips);

		cvReleaseImage( &frameCopy );

	}

    cvReleaseCapture( &capture );


}

void CFacesDemoDlg::OnBnClickedStopcam()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	m_runCamera = 0;
	GetDlgItem( IDC_RunCam )->EnableWindow( TRUE );
	GetDlgItem( IDC_StopCam )->EnableWindow( FALSE );
	SetTips(_T("�Ѿ�ֹͣ����ͷ��"));
}

void CFacesDemoDlg::OnBnClickedOpenFacesDir()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	ShellExecute(NULL,NULL,_T("Faces"),NULL,NULL,SW_SHOW);
}

void CFacesDemoDlg::OnHomepage()
{
	// TODO: �ڴ�����������������
	ShellExecute(NULL,"open","http://www.liufu.org/ling",NULL,NULL,SW_SHOW); 
}

void CFacesDemoDlg::OnPostmsg()
{
	// TODO: �ڴ�����������������
	ShellExecute(NULL,"open","http://www.liufu.org/Ling/guestbook.asp",NULL,NULL,SW_SHOW); 
}