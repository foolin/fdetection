#pragma once
#include "Detect.h"
#include "Config.h"


#define SHOWIMAGE_WIDTH 350		//显示图片大小
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
	afx_msg void OnBnClickedDtbtnOpendir();
	afx_msg void OnBnClickedDtbtnOpenimage();
	afx_msg void OnBnClickedDtbtnSaveimage();
	afx_msg void OnBnClickedDtbtnDetect();
	afx_msg void OnBnClickedDtbtnAbout();


public:
	void ResizeImage(IplImage* img);
	void ShowImage( IplImage* img, UINT ID );	// ID 是Picture Control控件的ID号
	void ShowImage( IplImage* srcImage);	//显示图像，包括缩小图像
	void SetTips( CString strTips);

private:
	IplImage* m_showImage;
	CDetect m_objDetect;
	CConfig m_objConfig;
	CString m_strCascadeName;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	// 程序路径
	CString m_strAppPath;
};
