#pragma once


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
};
