#pragma once


// CComposeDlg �Ի���

class CComposeDlg : public CDialog
{
	DECLARE_DYNAMIC(CComposeDlg)

public:
	CComposeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CComposeDlg();

// �Ի�������
	enum { IDD = IDD_ComposeDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
