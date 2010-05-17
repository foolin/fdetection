// CheckDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Face.h"
#include "CheckDlg.h"
#include "string.h"
#include "Config.h"


// CCheckDlg �Ի���

IMPLEMENT_DYNAMIC(CCheckDlg, CDialog)

CCheckDlg::CCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckDlg::IDD, pParent)
{
	IsAddFace=false;
	IsOpen=false;
	
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
	ON_BN_CLICKED(IDC_CheckDlg_AddFace, &CCheckDlg::OnBnClickedCheckdlgAddface)
END_MESSAGE_MAP()


//������

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

// CCheckDlg ��Ϣ�������


//���������⺯��
void CCheckDlg::OnBnClickedCheckdlgAddface()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!IsAddFace)
	{
		//���ó���Ĭ��·��
		if(m_strAppPath != _T(""))
		{
			SetCurrentDirectory(m_strAppPath);
		}

		for(int i=0;i<8;i++)
		{
			m_objCheck.temp1[i].number=0;
			m_objCheck.temp1[i].value=0;
			m_objCheck.temp1[i].x=0;
			m_objCheck.temp1[i].y=0;
		}
		//��ʼ��������������е�һ���˵����ݳ��������Ӧ�ľֲ���ֵ������������ں�
		m_objCheck.temp1[0]=m_objCheck.Data("Faces/lib/01/1.jpg",1);
		m_objCheck.temp1[1]=m_objCheck.Data("Faces/lib/01/2.jpg",2);
		m_objCheck.temp1[2]=m_objCheck.Data("Faces/lib/01/3.jpg",3);
		m_objCheck.temp1[3]=m_objCheck.Data("Faces/lib/01/4.jpg",4);
		m_objCheck.temp1[4]=m_objCheck.Data("Faces/lib/01/5.jpg",5);
		m_objCheck.temp1[5]=m_objCheck.Data("Faces/lib/01/6.jpg",6);
		m_objCheck.temp1[6]=m_objCheck.Data("Faces/lib/01/7.jpg",7);
		//��ʼ��������������еڶ����˵����ݳ��������Ӧ�ľֲ���ֵ������������ں�
		m_objCheck.temp2[0]=m_objCheck.Data("Faces/lib/02/1.jpg",1);
		m_objCheck.temp2[1]=m_objCheck.Data("Faces/lib/02/2.jpg",2);
		m_objCheck.temp2[2]=m_objCheck.Data("Faces/lib/02/3.jpg",3);
		m_objCheck.temp2[3]=m_objCheck.Data("Faces/lib/02/4.jpg",4);
		m_objCheck.temp2[4]=m_objCheck.Data("Faces/lib/02/5.jpg",5);
		m_objCheck.temp2[5]=m_objCheck.Data("Faces/lib/02/6.jpg",6);
		m_objCheck.temp2[6]=m_objCheck.Data("Faces/lib/02/7.jpg",7);
		//��ʼ��������������е������˵����ݳ��������Ӧ�ľֲ���ֵ������������ں�
		m_objCheck.temp3[0]=m_objCheck.Data("Faces/lib/04/1.jpg",1);
		m_objCheck.temp3[1]=m_objCheck.Data("Faces/lib/04/2.jpg",2);
		m_objCheck.temp3[2]=m_objCheck.Data("Faces/lib/04/3.jpg",3);
		m_objCheck.temp3[3]=m_objCheck.Data("Faces/lib/04/4.jpg",4);
		m_objCheck.temp3[4]=m_objCheck.Data("Faces/lib/04/5.jpg",5);
		m_objCheck.temp3[5]=m_objCheck.Data("Faces/lib/04/6.jpg",6);
		m_objCheck.temp3[6]=m_objCheck.Data("Faces/lib/04/7.jpg",7);
		
		IsAddFace=true;
		MessageBox(_T("������ɣ�"));
	}
	else
		MessageBox(_T("�������Ѿ����أ�"));
}


void CCheckDlg::OnBnClickedCheckdlgOpenpicture()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(IsAddFace)
	{
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
		m_objCheck.tempcheck=m_objCheck.Data(strPath,8);
	
		MessageBox(_T("�Ѵ�ͼƬ"));
		UpdateData(TRUE);

		//���ó���Ĭ��·��
		if(m_strAppPath != _T(""))
		{
			SetCurrentDirectory(m_strAppPath);
		}
		IsOpen=true;
	}
	else
		MessageBox(_T("���ȼ��������⣡"));

}

void CCheckDlg::OnBnClickedCheckdlgShell()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(IsOpen)
	{
		bool checkflag1,checkflag2,checkflag3;
		//DataField tempcheck;
		//tempcheck=m_objCheck.Data("Faces/lib/01/2.jpg",8);
		checkflag1=m_objCheck.SecondData(m_objCheck.temp1,m_objCheck.tempcheck);
		checkflag2=m_objCheck.SecondData(m_objCheck.temp2,m_objCheck.tempcheck);
		checkflag3=m_objCheck.SecondData(m_objCheck.temp3,m_objCheck.tempcheck);
   
		if(checkflag1&&checkflag2&&checkflag3)
			MessageBox(_T("��İ����!"));
		else
			MessageBox(_T("����İ����!"));
		IsOpen=false;
	}
	else
		MessageBox(_T("���������������ͼƬ��"));
}

BOOL CCheckDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��	return TRUE;  

	//��ȡ��ǰ����Ŀ¼
	m_strAppPath = m_objConfig.GetConfig("AppPath");
	
	// �쳣: OCX ����ҳӦ���� FALSE
	return true;
}