// CheckDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Face.h"
#include "CheckDlg.h"


// CCheckDlg 对话框

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


//处理函数

void CCheckDlg::ShowImage( IplImage* srcImage)	//显示图像，包括缩小图像
{
//	IplImage* img = cvCreateImage( cvGetSize( srcImage), srcImage->depth, srcImage->nChannels);
//	cvCopy( srcImage, img, NULL);
//	ResizeImage(img);	//图片进行缩放
	ResizeImage(srcImage);	//图片进行缩放
	ShowImage( m_showImage, IDC_DtPc_ShowImage );			// 调用显示图片函数	
//	cvReleaseImage( &img);

}

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


void CCheckDlg::ResizeImage(IplImage* img)
{
	// 读取图片的宽和高
    int w = img->width;
	int h = img->height;

	// 找出宽和高中的较大值者
	int max = (w > h)? w: h;
	int maxDef = (SHOWIMAGE_WIDTH > SHOWIMAGE_HEIGHT)? SHOWIMAGE_WIDTH : SHOWIMAGE_HEIGHT;

	// 计算将图片缩放到m_showImage区域所需的比例因子
	float scale = (float) ( (float) max / maxDef);
	
	// 缩放后图片的宽和高
	int nw = (int)( w/scale );
	int nh = (int)( h/scale );

	// 为了将缩放后的图片存入 m_showImage 的正中部位，需计算图片在 m_showImage 左上角的期望坐标值
	int tlx = (nw > nh)? 0: (int)(SHOWIMAGE_WIDTH - nw) / 2;
	int tly = (nw > nh)? (int)(SHOWIMAGE_HEIGHT - nh) / 2 : 0;

	// 对上一幅显示的图片数据清零
	if( m_showImage )
	{
		cvZero( m_showImage );
	}

	// 设置 TheImage 的 ROI 区域，用来存入图片 img
	cvSetImageROI( m_showImage, cvRect( tlx, tly, nw, nh) );
	// 对图片 img 进行缩放，并存入到 m_showImage 中
	cvResize( img, m_showImage );
	// 重置 TheImage 的 ROI 准备读入下一幅图片
	cvResetImageROI( m_showImage );
}


//设置消息提示
void CCheckDlg::SetTips( CString strTips)
{
	GetDlgItem( IDC_CheckDlg_Tips )->SetWindowText( strTips );
}


// CCheckDlg 消息处理程序


void CCheckDlg::OnBnClickedCheckdlgOpenpicture()
{
	// TODO: 在此添加控件通知处理程序代码
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

	if(picture_num%3==0)
	{
		m_objCheck.m_ReadImage1=cvLoadImage(strPath,0);
		if(!m_objCheck.m_ReadImage1)
		{
			MessageBox(_T("无法打开图片"));
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
			MessageBox(_T("无法打开图片"));
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
			MessageBox(_T("无法打开图片"));
			return;
		}
		ShowImage( m_objCheck.m_ReadImage3,IDC_CheckDlg_Picture3);
		cvReleaseImage( &m_objCheck.m_ReadImage3);
	//	picture_num=0;
	}
    picture_num++;

//	SetTips(_T("已打开图片：") + strPath);
	MessageBox(_T("已打开图片"));
}
