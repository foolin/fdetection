#pragma once


// CDetectDlg �Ի���

class CDetectDlg : public CDialog
{
	DECLARE_DYNAMIC(CDetectDlg)

public:
	CDetectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDetectDlg();

// �Ի�������
	enum { IDD = IDD_DetectDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
