// DetectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Face.h"
#include "DetectDlg.h"


// CDetectDlg �Ի���

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
	//�ͷ�ShowImage
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��


	//��ʼ��ShowImage
	CvSize imgSize;
    imgSize.height = SHOWIMAGE_HEIGHT;
    imgSize.width = SHOWIMAGE_WIDTH;
	m_showImage = cvCreateImage( imgSize, IPL_DEPTH_8U, 3 );

	//��ʼ�������ļ�
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

	//ʹ��ťʧЧ
	GetDlgItem( IDC_DtBtn_SaveImage )->EnableWindow( FALSE );
	GetDlgItem( IDC_DtBtn_2Gray )->EnableWindow( FALSE );
	GetDlgItem( IDC_DtBtn_EqualHist )->EnableWindow( FALSE );
	GetDlgItem( IDC_DtBtn_RemoveNoise )->EnableWindow( FALSE );
	GetDlgItem( IDC_DtBtn_Detect )->EnableWindow( FALSE );
	//GetDlgItem( IDC_DtBtn_OpenDir )->EnableWindow( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}



void CDetectDlg::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	//// TODO: �ڴ˴������Ϣ����������
	//// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
    CDialog::OnPaint();                    // �ػ�Ի���
    CDialog::UpdateWindow();                // ����windows���ڣ�������ⲽ���ã�ͼƬ��ʾ�����������
    //ShowImage( m_objDetect.m_pReadImage);    // �ػ�ͼƬ����
	ShowImage( m_showImage, IDC_DtPc_ShowImage );		// �ػ�ͼƬ����

}


//������

void CDetectDlg::ShowImage( IplImage* srcImage)	//��ʾͼ�񣬰�����Сͼ��
{
//	IplImage* img = cvCreateImage( cvGetSize( srcImage), srcImage->depth, srcImage->nChannels);
//	cvCopy( srcImage, img, NULL);
//	ResizeImage(img);	//ͼƬ��������


	if((srcImage->depth != m_showImage->depth) || (srcImage->nChannels != m_showImage->nChannels))
	{
		cvZero(m_showImage);
		m_showImage = cvCreateImage(cvSize(SHOWIMAGE_WIDTH, SHOWIMAGE_HEIGHT), srcImage->depth, srcImage->nChannels);	//������
	}


	ResizeImage(srcImage);	//ͼƬ��������
	ShowImage( m_showImage, IDC_DtPc_ShowImage );			// ������ʾͼƬ����	
//	cvReleaseImage( &img);

}

void CDetectDlg::ShowImage( IplImage* img, UINT ID )	// ID ��Picture Control�ؼ���ID��
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


void CDetectDlg::ResizeImage(IplImage* img)
{
	// ��ȡͼƬ�Ŀ�͸�
    int w = img->width;
	int h = img->height;

	// �ҳ���͸��еĽϴ�ֵ��
	int max = (w > h)? w: h;
	int maxDef = (SHOWIMAGE_WIDTH > SHOWIMAGE_HEIGHT)? SHOWIMAGE_WIDTH : SHOWIMAGE_HEIGHT;

	// ���㽫ͼƬ���ŵ�m_showImage��������ı�������
	float scale = (float) ( (float) max / maxDef);
	
	// ���ź�ͼƬ�Ŀ�͸�
	int nw = (int)( w/scale );
	int nh = (int)( h/scale );

	// Ϊ�˽����ź��ͼƬ���� m_showImage �����в�λ�������ͼƬ�� m_showImage ���Ͻǵ���������ֵ
	int tlx = (nw > nh)? 0: (int)(SHOWIMAGE_WIDTH - nw) / 2;
	int tly = (nw > nh)? (int)(SHOWIMAGE_HEIGHT - nh) / 2 : 0;

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


//������Ϣ��ʾ
void CDetectDlg::SetTips( CString strTips)
{
	GetDlgItem( IDC_DtTxt_Tips )->SetWindowText( strTips );
}




// CDetectDlg ��Ϣ�������

//��ͼ��
void CDetectDlg::OnBnClickedDtbtnOpenimage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg( TRUE, _T("*.bmp"), NULL,
					OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
					_T("image files (*.bmp; *.jpg; *.tiff) |*.bmp; *.jpg; *.tiff| All Files (*.*) |*.*||"),
					NULL );										// ѡ��ͼƬ��Լ��
	dlg.m_ofn.lpstrTitle = _T("��ͼƬ");	// ���ļ��Ի���ı�����
	if( dlg.DoModal() != IDOK )				// �ж��Ƿ���ͼƬ
	{
		return;
	}
	
	CString strPath = dlg.GetPathName();			// ��ȡͼƬ·��

	if(!m_objDetect.Load(strPath))
	{
		MessageBox(_T("�޷���ͼƬ"));
		return;
	}
	ShowImage( m_objDetect.m_pReadImage);
	//m_objDetect.SetGrayImage();
	//ShowImage( m_objDetect.m_pGrayImage);


	//// ʹ��ť��Ч
	GetDlgItem( IDC_DtBtn_SaveImage )->EnableWindow( TRUE );
	GetDlgItem( IDC_DtBtn_2Gray )->EnableWindow( TRUE );
	GetDlgItem( IDC_DtBtn_EqualHist )->EnableWindow( TRUE );
	GetDlgItem( IDC_DtBtn_RemoveNoise )->EnableWindow( TRUE );
	GetDlgItem( IDC_DtBtn_Detect )->EnableWindow( TRUE );
	//GetDlgItem( IDC_DtBtn_OpenDir )->EnableWindow( TRUE );

	SetTips(_T("�Ѵ�ͼƬ��") + strPath);

	//����Ĭ��Ŀ¼�����xml·������ȷ����
	if(m_strAppPath != _T(""))
	{
		SetCurrentDirectory(m_strAppPath);
	}

	m_blnIsShowGray = false;	//������ʾ�Ҷ�ͼ��־λ
}


//����ͼ��
void CDetectDlg::OnBnClickedDtbtnSaveimage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	if(m_blnIsShowGray)
	{
		cvSaveImage(strPath, m_objDetect.m_pGrayImage);
	}
	else
	{
		cvSaveImage(strPath, m_objDetect.m_pReadImage);
	}
	SetTips(_T("�ѱ���ͼƬ����") + strPath);
	//��ʾ�Ƿ���ļ���
	CString strDir = strPath.Left(strPath.ReverseFind(_T('\\')));
	if(MessageBox(_T("�Ѿ����棬�Ƿ�������ļ��У�"), _T("��ܰ��ʾ"), MB_OKCANCEL) == IDOK)
	{
		 ShellExecute(NULL,NULL,strDir,NULL,NULL,SW_SHOW);
	}

	//����Ĭ��Ŀ¼�����xml·������ȷ����
	if(m_strAppPath != _T(""))
	{
		SetCurrentDirectory(m_strAppPath);
	}
}


//�������
void CDetectDlg::OnBnClickedDtbtnDetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		//����Ĭ��Ŀ¼�����xml·������ȷ����
	if(m_strAppPath != _T(""))
	{
		SetCurrentDirectory(m_strAppPath);
	}
	//���ֱ�Ӽ�⣬���Զ�����ȥ�봦��
	if(!m_blnIsShowGray)
	{
		m_objDetect.RemoveNoise();
	}
	if(m_objDetect.FaceDetect(m_strCascadeName, m_strEyesCascadeName))
	{
		//��ʾͼ��
		if(m_blnIsShowGray)
		{
			ShowImage( m_objDetect.m_pGrayImage);
		}
		else
		{
			ShowImage( m_objDetect.m_pReadImage);
		}
		
		//������ʾ
		CString strTips;
		int m_facesCount = m_objDetect.m_nFacesCount;
		if (m_facesCount > 0)
		{
			strTips.Format(_T("�����ϣ�����⵽%d��������\n����ͼ���ѱ���!"), m_facesCount, m_strAppPath);
		}
		else
		{
			strTips.Format(_T("�����ϣ�û�м�⵽������"), m_facesCount);
		}
		SetTips(strTips);
	}
	else
	{
		SetTips(_T("���ʧ�ܣ�") + m_objDetect.m_strErrMessage);	//���ʧ��
	}
	//MessageBox("�����������");
	GetDlgItem( IDC_DtBtn_Detect )->EnableWindow( FALSE );

}


//��Ŀ¼
void CDetectDlg::OnBnClickedDtbtnOpendir()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShellExecute(NULL,NULL,_T("Faces"),NULL,NULL,SW_SHOW);
}


//��������
void CDetectDlg::OnBnClickedDtbtnAbout()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShellExecute(NULL,"open","http://www.liufu.org/ling",NULL,NULL,SW_SHOW); 
}




void CDetectDlg::OnBnClickedDtbtn2gray()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!m_blnIsShowGray)	//�ж��Ƿ��Ѿ���ʾ�Ҷ�ͼ��
	{
		m_objDetect.SetGrayImage();
	}
	ShowImage( m_objDetect.m_pGrayImage);
	m_blnIsShowGray = true;
}

void CDetectDlg::OnBnClickedDtbtnEqualhist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_objDetect.EqualizeHist();
	ShowImage( m_objDetect.m_pGrayImage);
	m_blnIsShowGray = true;
}

void CDetectDlg::OnBnClickedDtbtnRemovenoise()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//m_objDetect.RemoveNoise(true);
	m_objDetect.RemoveNoise(m_blnIsShowGray);
	ShowImage( m_objDetect.m_pGrayImage);
}
