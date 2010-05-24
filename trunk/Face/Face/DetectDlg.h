#pragma once
#include "Detect.h"
#include "Config.h"


#define SHOWIMAGE_WIDTH 350		//显示图片控件区域大小
#define SHOWIMAGE_HEIGHT 330	

// CDetectDlg 对话框

class CDetectDlg : public CDialog
{
	DECLARE_DYNAMIC(CDetectDlg)

public:
	CDetectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDetectDlg();

// 对话框数据
	enum { IDD = IDD_DetectDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDtbtnOpendir();		//打开目录
	afx_msg void OnBnClickedDtbtnOpenimage();	//打开图像
	afx_msg void OnBnClickedDtbtnSaveimage();	//保存图像
	afx_msg void OnBnClickedDtbtnDetect();		//人脸检测
	afx_msg void OnBnClickedDtbtnAbout();		//关于


public:
	void ResizeImage(IplImage* img);	//显示图像
	void ShowImage( IplImage* img, UINT ID );	// ID 是Picture Control控件的ID号
	void ShowImage( IplImage* srcImage);	//重载，显示图像，包括缩小图像
	void SetTips( CString strTips);			//提示信息

private:
	CString m_strAppPath;	//程序路径
	IplImage* m_showImage;	//显示的图像
	CDetect m_objDetect;	//人脸检测对象
	CConfig m_objConfig;	//配置文件对象，读取ini文件
	CString m_strCascadeName;	//人脸级联分类器xml路径
	CString m_strEyesCascadeName;	//人眼级联分类器xml路径
	bool m_blnIsShowGray;		//标识位，是否已经显示灰度图像


public:
	virtual BOOL OnInitDialog();	//对话框初始化
	afx_msg void OnPaint();			//窗口函数
	afx_msg void OnBnClickedDtbtn2gray();	//灰度图像
	afx_msg void OnBnClickedDtbtnEqualhist();	//直方图均衡化
	afx_msg void OnBnClickedDtbtnRemovenoise();	//去除噪声
};
