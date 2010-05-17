// CheckDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Face.h"
#include "CheckDlg.h"
#include "string.h"
#include "Config.h"


// CCheckDlg 对话框

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


//处理函数

void CCheckDlg::ShowImage( IplImage* img, UINT ID )	// ID 是Picture Control控件的ID号
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

// CCheckDlg 消息处理程序


//加载人脸库函数
void CCheckDlg::OnBnClickedCheckdlgAddface()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!IsAddFace)
	{
		//设置程序默认路径
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
		//初始化，算出人脸库中第一个人的数据场，求出相应的局部极值，并完成坐标融合
		m_objCheck.temp1[0]=m_objCheck.Data("Faces/lib/01/1.jpg",1);
		m_objCheck.temp1[1]=m_objCheck.Data("Faces/lib/01/2.jpg",2);
		m_objCheck.temp1[2]=m_objCheck.Data("Faces/lib/01/3.jpg",3);
		m_objCheck.temp1[3]=m_objCheck.Data("Faces/lib/01/4.jpg",4);
		m_objCheck.temp1[4]=m_objCheck.Data("Faces/lib/01/5.jpg",5);
		m_objCheck.temp1[5]=m_objCheck.Data("Faces/lib/01/6.jpg",6);
		m_objCheck.temp1[6]=m_objCheck.Data("Faces/lib/01/7.jpg",7);
		//初始化，算出人脸库中第二个人的数据场，求出相应的局部极值，并完成坐标融合
		m_objCheck.temp2[0]=m_objCheck.Data("Faces/lib/02/1.jpg",1);
		m_objCheck.temp2[1]=m_objCheck.Data("Faces/lib/02/2.jpg",2);
		m_objCheck.temp2[2]=m_objCheck.Data("Faces/lib/02/3.jpg",3);
		m_objCheck.temp2[3]=m_objCheck.Data("Faces/lib/02/4.jpg",4);
		m_objCheck.temp2[4]=m_objCheck.Data("Faces/lib/02/5.jpg",5);
		m_objCheck.temp2[5]=m_objCheck.Data("Faces/lib/02/6.jpg",6);
		m_objCheck.temp2[6]=m_objCheck.Data("Faces/lib/02/7.jpg",7);
		//初始化，算出人脸库中第三个人的数据场，求出相应的局部极值，并完成坐标融合
		m_objCheck.temp3[0]=m_objCheck.Data("Faces/lib/04/1.jpg",1);
		m_objCheck.temp3[1]=m_objCheck.Data("Faces/lib/04/2.jpg",2);
		m_objCheck.temp3[2]=m_objCheck.Data("Faces/lib/04/3.jpg",3);
		m_objCheck.temp3[3]=m_objCheck.Data("Faces/lib/04/4.jpg",4);
		m_objCheck.temp3[4]=m_objCheck.Data("Faces/lib/04/5.jpg",5);
		m_objCheck.temp3[5]=m_objCheck.Data("Faces/lib/04/6.jpg",6);
		m_objCheck.temp3[6]=m_objCheck.Data("Faces/lib/04/7.jpg",7);
		
		IsAddFace=true;
		MessageBox(_T("加载完成！"));
	}
	else
		MessageBox(_T("人脸库已经加载！"));
}


void CCheckDlg::OnBnClickedCheckdlgOpenpicture()
{
	// TODO: 在此添加控件通知处理程序代码
	if(IsAddFace)
	{
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
		m_objCheck.m_ReadImage1=cvLoadImage(strPath,0);
		
		if(!m_objCheck.m_ReadImage1)
		{
			MessageBox(_T("无法打开图片"));
			return;
		}
		ShowImage( m_objCheck.m_ReadImage1,IDC_CheckDlg_Picture1);
		cvReleaseImage( &m_objCheck.m_ReadImage1);
		m_objCheck.tempcheck=m_objCheck.Data(strPath,8);
	
		MessageBox(_T("已打开图片"));
		UpdateData(TRUE);

		//设置程序默认路径
		if(m_strAppPath != _T(""))
		{
			SetCurrentDirectory(m_strAppPath);
		}
		IsOpen=true;
	}
	else
		MessageBox(_T("请先加载人脸库！"));

}

void CCheckDlg::OnBnClickedCheckdlgShell()
{
	// TODO: 在此添加控件通知处理程序代码
	if(IsOpen)
	{
		bool checkflag1,checkflag2,checkflag3;
		//DataField tempcheck;
		//tempcheck=m_objCheck.Data("Faces/lib/01/2.jpg",8);
		checkflag1=m_objCheck.SecondData(m_objCheck.temp1,m_objCheck.tempcheck);
		checkflag2=m_objCheck.SecondData(m_objCheck.temp2,m_objCheck.tempcheck);
		checkflag3=m_objCheck.SecondData(m_objCheck.temp3,m_objCheck.tempcheck);
   
		if(checkflag1&&checkflag2&&checkflag3)
			MessageBox(_T("是陌生人!"));
		else
			MessageBox(_T("不是陌生人!"));
		IsOpen=false;
	}
	else
		MessageBox(_T("请先载入待测人脸图片！"));
}

BOOL CCheckDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化	return TRUE;  

	//获取当前程序目录
	m_strAppPath = m_objConfig.GetConfig("AppPath");
	
	// 异常: OCX 属性页应返回 FALSE
	return true;
}