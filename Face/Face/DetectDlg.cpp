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
{
	
	//��ʼ��ShowImage
	CvSize imgSize;
    imgSize.height = SHOWIMAGE_HEIGHT;
    imgSize.width = SHOWIMAGE_WIDTH;
	m_showImage = cvCreateImage( imgSize, IPL_DEPTH_8U, 3 );

	//��ʼ�������ļ�
	m_strCascadeName = m_objConfig.GetConfig(_T("Detect"), _T("CascadeName"));
	if( m_strCascadeName == _T(""))
	{
		m_strCascadeName = _T("xml\\haarcascade_frontalface_alt_tree.xml");
		m_objConfig.SetConfig(_T("Detect"), _T("CascadeName"), m_strCascadeName);
	}

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
END_MESSAGE_MAP()




//������

void CDetectDlg::ShowImage( IplImage* srcImage)	//��ʾͼ�񣬰�����Сͼ��
{
//	IplImage* img = cvCreateImage( cvGetSize( srcImage), srcImage->depth, srcImage->nChannels);
//	cvCopy( srcImage, img, NULL);
//	ResizeImage(img);	//ͼƬ��������
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


	//// ʹ��Ե��ⰴť��Ч
	//GetDlgItem( IDC_DETECT_FACE )->EnableWindow( TRUE );
	//GetDlgItem( IDC_SAVE_IMAGE )->EnableWindow( TRUE );
	////GetDlgItem( IDC_REMOVE_NOISE )->EnableWindow( TRUE );
	////GetDlgItem( IDC_BINARY_IMAGE )->EnableWindow( TRUE );

	SetTips(_T("�Ѵ�ͼƬ��") + strPath);
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
	cvSaveImage(strPath, m_objDetect.m_pReadImage);
	SetTips(_T("�ѱ���ͼƬ����") + strPath);
	//��ʾ�Ƿ���ļ���
	CString strDir = strPath.Left(strPath.ReverseFind(_T('\\')));
	if(MessageBox(_T("�Ѿ����棬�Ƿ�������ļ��У�"), _T("��ܰ��ʾ"), MB_OKCANCEL) == IDOK)
	{
		 ShellExecute(NULL,NULL,strDir,NULL,NULL,SW_SHOW);
	}
}


//�������
void CDetectDlg::OnBnClickedDtbtnDetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_objDetect.FaceDetect(m_strCascadeName))
	{
		//��ʾͼ��
		ShowImage( m_objDetect.m_pReadImage);
		
		//������ʾ
		CString strTips;
		int m_facesCount = m_objDetect.m_nFacesCount;
		if (m_facesCount > 0)
		{
			strTips.Format(_T("�����ϣ�����⵽%d��������"), m_facesCount);
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
