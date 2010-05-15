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
	ON_BN_CLICKED(IDC_CheckDlg_Shell, &CCheckDlg::OnBnClickedCheckdlgShell)
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
	
	m_objCheck.m_ReadImage1=cvLoadImage(strPath,0);
	if(!m_objCheck.m_ReadImage1)
	{
		MessageBox(_T("�޷���ͼƬ"));
		return;
	}
	ShowImage( m_objCheck.m_ReadImage1,IDC_CheckDlg_Picture1);
	cvReleaseImage( &m_objCheck.m_ReadImage1);
	
//	SetTips(_T("�Ѵ�ͼƬ��") + strPath);
	MessageBox(_T("�Ѵ�ͼƬ"));
	UpdateData(TRUE);
}

void CCheckDlg::OnBnClickedCheckdlgShell()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
  //  ShellExecute(NULL,"open","D:\\Backup\\�ҵ��ĵ�\\Visual Studio 2008\\Projects\\test1\\Debug\\test1.exe",NULL,NULL,SW_SHOWNORMAL); 
	
	/*for(int i=0;i<8;i++)
	{
	//	if(unsort[i].number==8&&i==7)
			MessageBox(_T("����İ����!"));
	}*/
	bool checkflag1,checkflag2,checkflag3;
	DataField tempcheck;
	tempcheck=m_objCheck.Data("Faces/������/01/3.jpg",8);
	checkflag1=m_objCheck.SecondData(m_objCheck.temp1,tempcheck);
	checkflag2=m_objCheck.SecondData(m_objCheck.temp2,tempcheck);
	checkflag3=m_objCheck.SecondData(m_objCheck.temp3,tempcheck);

	if(checkflag1&&checkflag2&&checkflag3)
		MessageBox(_T("��İ����!"));
	else
		MessageBox(_T("����İ����!"));
//	getchar();

}

BOOL CCheckDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

for(int i=0;i<8;i++)
	{
		m_objCheck.temp1[i].number=0;
		m_objCheck.temp1[i].value=0;
		m_objCheck.temp1[i].x=0;
		m_objCheck.temp1[i].y=0;
	}
	//��ʼ��������������е�һ���˵����ݳ��������Ӧ�ľֲ���ֵ������������ں�
	m_objCheck.temp1[0]=m_objCheck.Data("Faces/������/01/1.jpg",1);
	m_objCheck.temp1[1]=m_objCheck.Data("Faces/������/01/2.jpg",2);
	m_objCheck.temp1[2]=m_objCheck.Data("Faces/������/01/3.jpg",3);
	m_objCheck.temp1[3]=m_objCheck.Data("Faces/������/01/4.jpg",4);
	m_objCheck.temp1[4]=m_objCheck.Data("Faces/������/01/5.jpg",5);
	m_objCheck.temp1[5]=m_objCheck.Data("Faces/������/01/6.jpg",6);
	m_objCheck.temp1[6]=m_objCheck.Data("Faces/������/01/7.jpg",7);
	//��ʼ��������������еڶ����˵����ݳ��������Ӧ�ľֲ���ֵ������������ں�
	m_objCheck.temp2[0]=m_objCheck.Data("Faces/������/02/1.jpg",1);
	m_objCheck.temp2[1]=m_objCheck.Data("Faces/������/02/2.jpg",2);
	m_objCheck.temp2[2]=m_objCheck.Data("Faces/������/02/3.jpg",3);
	m_objCheck.temp2[3]=m_objCheck.Data("Faces/������/02/4.jpg",4);
	m_objCheck.temp2[4]=m_objCheck.Data("Faces/������/02/5.jpg",5);
	m_objCheck.temp2[5]=m_objCheck.Data("Faces/������/02/6.jpg",6);
	m_objCheck.temp2[6]=m_objCheck.Data("Faces/������/02/7.jpg",7);
	//��ʼ��������������е������˵����ݳ��������Ӧ�ľֲ���ֵ������������ں�
	m_objCheck.temp3[0]=m_objCheck.Data("Faces/������/04/1.jpg",1);
	m_objCheck.temp3[1]=m_objCheck.Data("Faces/������/04/2.jpg",2);
	m_objCheck.temp3[2]=m_objCheck.Data("Faces/������/04/3.jpg",3);
	m_objCheck.temp3[3]=m_objCheck.Data("Faces/������/04/4.jpg",4);
	m_objCheck.temp3[4]=m_objCheck.Data("Faces/������/04/5.jpg",5);
	m_objCheck.temp3[5]=m_objCheck.Data("Faces/������/04/6.jpg",6);
	m_objCheck.temp3[6]=m_objCheck.Data("Faces/������/04/7.jpg",7);


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
