#pragma once


// CComposeDlg 对话框

class CComposeDlg : public CDialog
{
	DECLARE_DYNAMIC(CComposeDlg)

public:
	CComposeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CComposeDlg();

// 对话框数据
	enum { IDD = IDD_ComposeDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
