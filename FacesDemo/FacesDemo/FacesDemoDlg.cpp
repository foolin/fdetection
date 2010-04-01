// FacesDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FacesDemo.h"
#include "FacesDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CFacesDemoDlg ��Ϣ�������

BOOL CFacesDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

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

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

    CvSize imgSize;
    imgSize.height = IMAGE_HEIGHT;
    imgSize.width = IMAGE_WIDTH;
	m_readImage = cvCreateImage( imgSize, IPL_DEPTH_8U, IMAGE_CHANNELS );


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
		//MessageBox("���ò���ȷ��");
	}
	else
	{
		m_cascadeName = strCascadeName;
		//MessageBox("������ȷ��");
	}
	*/

	/*
	if(::WritePrivateProfileString("Init","CascadeName", m_cascadeName,".\\student.ini"))
	{
		MessageBox("д��ɹ�");
	}
	*/

	

	//ʹ�������Ȱ�ťʧЧ
	GetDlgItem( IDC_SAVE_IMAGE )->EnableWindow( FALSE );
	GetDlgItem( IDC_DETECT_FACE )->EnableWindow( FALSE );
	GetDlgItem( IDC_REMOVE_NOISE )->EnableWindow( FALSE );
	GetDlgItem( IDC_BINARY_IMAGE )->EnableWindow( FALSE );

	InitConfig();	//��ʼ�������ļ�


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

// �����Ի��������С����ť������Ҫ����Ĵ���
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
        ShowImage( m_readImage, IDC_IMAGE );    // �ػ�ͼƬ����

	}
	else
	{
		CDialog::OnPaint();						// �ػ�Ի���

		CDialog::UpdateWindow();				// ����windows���ڣ�������ⲽ���ã�ͼƬ��ʾ�����������
		ShowImage( m_readImage, IDC_IMAGE );		// �ػ�ͼƬ����

	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFacesDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



/***************************************/
/*          ������                ****/
/***************************************/

void CFacesDemoDlg::InitConfig()
{
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
		//MessageBox("���ò���ȷ��");
	}
	else
	{
		m_cascadeName = strCascadeName;
		//MessageBox("������ȷ��");
	}

	/*
	if(::WritePrivateProfileString("Init","CascadeName", m_cascadeName,".\\student.ini"))
	{
		MessageBox("д��ɹ�");
	}
	*/


}


void CFacesDemoDlg::ShowImage( IplImage* img, UINT ID )	// ID ��Picture Control�ؼ���ID��
{
	CDC* pDC = GetDlgItem( ID ) ->GetDC();		// �����ʾ�ؼ��� DC
	HDC hDC = pDC ->GetSafeHdc();				// ��ȡ HDC(�豸���) �����л�ͼ����

	CRect rect;
	GetDlgItem(ID) ->GetClientRect( &rect );
	int rw = rect.right - rect.left;			// ���ͼƬ�ؼ��Ŀ�͸�
	int rh = rect.bottom - rect.top;
	int iw = img->width;						// ��ȡͼƬ�Ŀ�͸�
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
	// ��ȡͼƬ�Ŀ�͸�
    int w = img->width;
	int h = img->height;

	// �ҳ���͸��еĽϴ�ֵ��
	int max = (w > h)? w: h;

	// ���㽫ͼƬ���ŵ�TheImage��������ı�������
	float scale = (float) ( (float) max / 256.0f );
	
	// ���ź�ͼƬ�Ŀ�͸�
	int nw = (int)( w/scale );
	int nh = (int)( h/scale );

	// Ϊ�˽����ź��ͼƬ���� TheImage �����в�λ�������ͼƬ�� TheImage ���Ͻǵ���������ֵ
	int tlx = (nw > nh)? 0: (int)(256-nw)/2;
	int tly = (nw > nh)? (int)(256-nh)/2: 0;

	// ���� TheImage �� ROI ������������ͼƬ img
	cvSetImageROI( m_readImage, cvRect( tlx, tly, nw, nh) );

	// ��ͼƬ img �������ţ������뵽 TheImage ��
	cvResize( img, m_readImage );

	// ���� TheImage �� ROI ׼��������һ��ͼƬ
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
			///����������������ȡͼƬ����Ϊʵ������Ĵ�С///
			char filename[40] = "face.jpg";
			//IplImage* img2 = cvCreateImage(cvSize(r->width+1,r->height+1), img->depth, img->nChannels ); 
			//CvRect rect2 = cvRect(r->x - r->width, r->y, r->width+1,r->height+1);
			//CvRect rect2 = cvRect(center.x - r->width, center.y - r->height, r->width+1,r->height+1);
			int rect_size = 3;
			CvRect rect2 = cvRect(center.x - radius + rect_size, center.y - radius + rect_size, radius*2 - rect_size*2, radius*2 - rect_size*2);
			IplImage* img2 = cvCreateImage(cvSize(rect2.width , rect2.height), img->depth, img->nChannels ); 
			cvSetImageROI(img, rect2);
			cvCopy(img,img2);//���ƶ������� 
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
	// ��ʾͼ��
	ShowImage(img, IDC_IMAGE);
	//�ͷ�ͼ��
    cvReleaseImage( &gray );
    cvReleaseImage( &small_img );
}

void CFacesDemoDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	cvDestroyAllWindows();
	OnOK();
}


void CFacesDemoDlg::OnBnClickedOpenImage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(
		TRUE, _T("*.bmp"), NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image files (*.bmp; *.jpg) |*.bmp; *.jpg; *.jpeg | All Files (*.*) |*.*||"), NULL
		);										// ѡ��ͼƬ��Լ��
	dlg.m_ofn.lpstrTitle = _T("��ͼƬ");	// ���ļ��Ի���ı�����
	if( dlg.DoModal() != IDOK )					// �ж��Ƿ���ͼƬ
		return;
	
	CString mPath = dlg.GetPathName();			// ��ȡͼƬ·��

	IplImage* ipl = cvLoadImage( mPath, 1 );	// ��ȡͼƬ�����浽һ���ֲ����� ipl ��
	if( !ipl )									// �ж��Ƿ�ɹ���ȡͼƬ
		return;
	if( m_readImage )								// ����һ����ʾ��ͼƬ��������
		cvZero( m_readImage );
	ResizeImage( ipl );	// �Զ����ͼƬ�������ţ�ʹ��������ֵ�߸պõ��� 256���ٸ��Ƶ� TheImage ��
	ShowImage( m_readImage, IDC_IMAGE );			// ������ʾͼƬ����	
	cvReleaseImage( &ipl );						// �ͷ� ipl ռ�õ��ڴ�

	// ʹ��Ե��ⰴť��Ч
	GetDlgItem( IDC_DETECT_FACE )->EnableWindow( TRUE );
	GetDlgItem( IDC_SAVE_IMAGE )->EnableWindow( TRUE );

}


//����ͼƬ
void CFacesDemoDlg::OnBnClickedSaveImage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(
		FALSE, _T("*.bmp"), NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image files (*.jpg; *.bmp) |*.jpg;*.bmp;  *.jpeg | All Files (*.*) |*.*||"), NULL
		);										// ѡ��ͼƬ��Լ��
	dlg.m_ofn.lpstrTitle = _T("����ͼƬ");	// ���ļ��Ի���ı�����
	dlg.m_ofn.lpstrDefExt = "jpg";
	if( dlg.DoModal() != IDOK )				// �ж��Ƿ���ͼƬ
	{
		return;
	}
	CString mPath = dlg.GetPathName();			// ��ȡͼƬ·��
	cvSaveImage(mPath, m_readImage);

}

//���ڶԻ���
void CFacesDemoDlg::OnBnClickedAboutUs()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*
	CString strAbout;
	strAbout = "������� V1.0\n";
	strAbout = strAbout + "Version:  1.0.0\n";
	strAbout = strAbout + "Author :  Foolin\n";
	strAbout = strAbout + "E-mail :  Foolin55@gmail.com\n";
	MessageBox(strAbout);
	*/
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();

	
}


/***************************************/
/*              �������            ****/
/***************************************/
void CFacesDemoDlg::OnBnClickedDetectFace()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//m_cascadeName = _T("E:\\OpenCV2.0\\data\\haarcascades\\haarcascade_frontalface_alt2.xml");
	//MessageBox(m_cascadeName);
	//m_cascadeName = "haarcascade_frontalface_alt2.xml";
	m_cascade = (CvHaarClassifierCascade*)cvLoad( m_cascadeName, 0, 0, 0 );
    if( !m_cascade )
    {
        MessageBox(_T("�Բ����������cascade���ò���ȷ"+m_cascadeName));
        return;
    }
    m_storage = cvCreateMemStorage(0);
	if( !m_readImage )
	{
		MessageBox(_T("���ȴ�ͼ��"));
		return;
	}

    //�������
	FaceDetect(m_readImage);
	
	CString strTips;
	if (m_facesCount > 0)
	{
		strTips.Format("ͼ������ϣ�����⵽%d��������", m_facesCount);
	}
	else
	{
		strTips.Format("ͼ������ϣ�û�м�⵽������", m_facesCount);
	}
	MessageBox(_T(strTips));
}

//��������
void CFacesDemoDlg::OnBnClickedRemoveNoise()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

//��ֵ��
void CFacesDemoDlg::OnBnClickedBinaryImage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
