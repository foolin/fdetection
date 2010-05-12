// CheckDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Face.h"
#include "CheckDlg.h"


// CCheckDlg �Ի���

IMPLEMENT_DYNAMIC(CCheckDlg, CDialog)

CCheckDlg::CCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckDlg::IDD, pParent)
{
	picture_num=0;
}

CCheckDlg::~CCheckDlg()
{
}

void CCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCheckDlg, CDialog)
	ON_BN_CLICKED(IDC_CheckDlg_OpenPicture, &CCheckDlg::OnBnClickedCheckdlgOpenpicture)
END_MESSAGE_MAP()


//������

void CCheckDlg::ShowImage( IplImage* srcImage)	//��ʾͼ�񣬰�����Сͼ��
{
//	IplImage* img = cvCreateImage( cvGetSize( srcImage), srcImage->depth, srcImage->nChannels);
//	cvCopy( srcImage, img, NULL);
//	ResizeImage(img);	//ͼƬ��������
	ResizeImage(srcImage);	//ͼƬ��������
	ShowImage( m_showImage, IDC_DtPc_ShowImage );			// ������ʾͼƬ����	
//	cvReleaseImage( &img);

}

void CCheckDlg::ShowImage( IplImage* img, UINT ID )	// ID ��Picture Control�ؼ���ID��
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


void CCheckDlg::ResizeImage(IplImage* img)
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
void CCheckDlg::SetTips( CString strTips)
{
	GetDlgItem( IDC_CheckDlg_Tips )->SetWindowText( strTips );
}


// CCheckDlg ��Ϣ�������


void CCheckDlg::OnBnClickedCheckdlgOpenpicture()
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

	if(picture_num%3==0)
	{
		m_objCheck.m_ReadImage1=cvLoadImage(strPath,0);
		if(!m_objCheck.m_ReadImage1)
		{
			MessageBox(_T("�޷���ͼƬ"));
			return;
		}
		ShowImage( m_objCheck.m_ReadImage1,IDC_CheckDlg_Picture1);
		cvReleaseImage( &m_objCheck.m_ReadImage1);
	}

	if(picture_num%3==1)
	{
		m_objCheck.m_ReadImage2=cvLoadImage(strPath,0);
		if(!m_objCheck.m_ReadImage2)
		{
			MessageBox(_T("�޷���ͼƬ"));
			return;
		}
		ShowImage( m_objCheck.m_ReadImage2,IDC_CheckDlg_Picture2);
		cvReleaseImage( &m_objCheck.m_ReadImage2);
	}

	if(picture_num%3==2)
	{
		m_objCheck.m_ReadImage3=cvLoadImage(strPath,0);
		if(!m_objCheck.m_ReadImage3)
		{
			MessageBox(_T("�޷���ͼƬ"));
			return;
		}
		ShowImage( m_objCheck.m_ReadImage3,IDC_CheckDlg_Picture3);
		cvReleaseImage( &m_objCheck.m_ReadImage3);
	//	picture_num=0;
	}
    picture_num++;

//	SetTips(_T("�Ѵ�ͼƬ��") + strPath);
	MessageBox(_T("�Ѵ�ͼƬ"));
}
