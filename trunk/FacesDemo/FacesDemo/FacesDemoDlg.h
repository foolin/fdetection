// FacesDemoDlg.h : 头文件
//

#pragma once


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
public:
	afx_msg void OnBnClickedOpenImage();
	IplImage* m_readImage;

public:
	void ShowImage( IplImage* img, UINT ID );
	void ResizeImage(IplImage* img);
	void FaceDetect( IplImage* image );
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSaveImage();
	afx_msg void OnBnClickedAboutUs();
	afx_msg void OnBnClickedDetectFace();
	CvMemStorage* m_storage;
	CvHaarClassifierCascade* m_cascade;
	char* m_cascadeName;

};
