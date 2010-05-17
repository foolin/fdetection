#pragma once
#include "Config.h"
#include "Check.h"
#include "Detect.h"
#include "afxcmn.h"
#include "afxwin.h"

#define SHOWIMAGE_WIDTH 128		//显示图片大小
#define SHOWIMAGE_HEIGHT 128	

// CCheckDlg 对话框

class CCheckDlg : public CDialog
{
	DECLARE_DYNAMIC(CCheckDlg)

public:
	CCheckDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCheckDlg();

// 对话框数据
	enum { IDD = IDD_CheckDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    int picture_num;  //为了实现依次打开3张图片，让picture_num统计当前打开第几张图
public:
	
	afx_msg void OnBnClickedCheckdlgOpenpicture();
public:
	void ShowImage( IplImage* img, UINT ID );

private:
	IplImage* m_showImage;
	CCheck  m_objCheck; 
	CDetect m_objDetect;
	CConfig m_objConfig;
	CString m_strAppPath;
	
	bool IsAddFace;	//判断是否已加载人脸库
	bool IsOpen;	//判断开始识别之前，是否已将图片载入
public:
	afx_msg void OnBnClickedCheckdlgShell();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckdlgAddface();
	CProgressCtrl m_AddFaceProgress;
	CStatic m_AddFacesTips;
};
