// FacesDemoDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CFacesDemoDlg 对话框
class CFacesDemoDlg : public CDialog
{
// 构造
public:
	CFacesDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FACESDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

//公有成员
public:
	IplImage* m_readImage;					//读入的图片
	IplImage* m_showImage;					//显示的图片
	CvMemStorage* m_storage;				//人脸识别开辟的内存空间
	CvHaarClassifierCascade* m_cascade;		//人脸识别分类器
	CString m_cascadeName;					//分类器文件名
	int m_facesCount;						//检测人脸的数目
	CString m_programPath;					//程序路径

//共有函数
public:
	void InitConfig();							//初始化配置文件
	void ResizeImage(IplImage* img);			//重置图片大小（辅助显示）
	void ShowImage( IplImage* img, UINT ID );	//显示图片
	void ShowImage( IplImage* srcImage);		//显示图像，包括缩小（重载）
	void FaceDetect( IplImage* image );			//人脸检测
	void SetReadImage( IplImage* image, int width=0, int height=0 );	//设置图片大小并保存到m_readImage
	void SetTips( CString strTips);				//设置消息提示
///消息映射
public:
	afx_msg void OnBnClickedOpenImage();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSaveImage();
	afx_msg void OnBnClickedAboutUs();
	afx_msg void OnBnClickedDetectFace();
	afx_msg void OnBnClickedRemoveNoise();
	afx_msg void OnBnClickedBinaryImage();
	afx_msg void OnBnClickedMinimize();
	afx_msg LRESULT OnWindMinimize(WPARAM wParam,LPARAM lParam);	//最小化消息响应
	bool TrayMessage(DWORD dwFlag, UINT uIconId=IDR_MAINFRAME);		//最小化托盘响应函数
	afx_msg void OnTrayOpen();
	afx_msg void OnTrayQuit();
	afx_msg void OnTrayAbout();
};
