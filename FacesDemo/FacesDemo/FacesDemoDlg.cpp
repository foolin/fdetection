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

	ShowWindow(SW_MINIMIZE);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
    CvSize imgSize;
    imgSize.height = IMAGE_HEIGHT;
    imgSize.width = IMAGE_WIDTH;
	m_readImage = cvCreateImage( imgSize, IPL_DEPTH_8U, IMAGE_CHANNELS );


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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFacesDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFacesDemoDlg::OnBnClickedOpenImage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*
	CFileDialog fileDlg(TRUE,NULL,NULL,OFN_EXPLORER|OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
		(LPCTSTR)_T("Image Files (*.jpg;*.bmp)|*.jpg;*.bmp|All Files (*.*)|*.*||"),NULL);
	//fileDlg.m_ofn.lpstrFilter = "Image File(*.jpg;*bmp)\0*jpg;*bmp\0All Files(*.*)\0*.*\0\0";
	if(fileDlg.DoModal() != IDOK)
	{
		return;
	}
	{
		CString pathName = fileDlg.GetPathName();
		*/
		/*
		m_loadImage = cvLoadImage(pathName, 1);
		if(!m_loadImage)
		{
			MessageBox("�޷���ͼ��");
		}
		ShowImage(m_loadImage, IDC_IMAGE);
		*/
	/*
		IplImage* img = cvvLoadImage(pathName);
		ShowImage(img, IDC_IMAGE);
		cvReleaseImage( &img ); //�ͷ�ͼ��image 
		MessageBox("Test");
	}
		*/

	CFileDialog dlg(
		TRUE, _T("*.bmp"), NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image files (*.bmp; *.jpg) |*.bmp; *.jpg; *.jpeg | All Files (*.*) |*.*||"), NULL
		);										// ѡ��ͼƬ��Լ��
	dlg.m_ofn.lpstrTitle = _T("Open Image");	// ���ļ��Ի���ı�����
	if( dlg.DoModal() != IDOK )					// �ж��Ƿ���ͼƬ
		return;
	
	CString mPath = dlg.GetPathName();			// ��ȡͼƬ·��

	IplImage* ipl = cvLoadImage( mPath, 1 );	// ��ȡͼƬ�����浽һ���ֲ����� ipl ��
	if( !ipl )									// �ж��Ƿ�ɹ���ȡͼƬ
		return;
	if( m_readImage )								// ����һ����ʾ��ͼƬ��������
		cvZero( m_readImage );
	// ʹ��Ե��ⰴť��Ч
	//GetDlgItem( IDC_EdgeDetect )->EnableWindow( TRUE );

	ResizeImage( ipl );	// �Զ����ͼƬ�������ţ�ʹ��������ֵ�߸պõ��� 256���ٸ��Ƶ� TheImage ��
	ShowImage( m_readImage, IDC_IMAGE );			// ������ʾͼƬ����	
	cvReleaseImage( &ipl );						// �ͷ� ipl ռ�õ��ڴ�

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

void CFacesDemoDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	cvDestroyAllWindows();
	OnOK();
}

void CFacesDemoDlg::OnBnClickedSaveImage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CFacesDemoDlg::OnBnClickedAboutUs()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CFacesDemoDlg::OnBnClickedDetectFace()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
