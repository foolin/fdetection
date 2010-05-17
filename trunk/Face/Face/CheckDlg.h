#pragma once
#include "Config.h"
#include "Check.h"
#include "Detect.h"
#include "afxcmn.h"
#include "afxwin.h"

#define SHOWIMAGE_WIDTH 128		//��ʾͼƬ��С
#define SHOWIMAGE_HEIGHT 128	

// CCheckDlg �Ի���

class CCheckDlg : public CDialog
{
	DECLARE_DYNAMIC(CCheckDlg)

public:
	CCheckDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCheckDlg();

// �Ի�������
	enum { IDD = IDD_CheckDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    int picture_num;  //Ϊ��ʵ�����δ�3��ͼƬ����picture_numͳ�Ƶ�ǰ�򿪵ڼ���ͼ
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
	
	bool IsAddFace;	//�ж��Ƿ��Ѽ���������
	bool IsOpen;	//�жϿ�ʼʶ��֮ǰ���Ƿ��ѽ�ͼƬ����
public:
	afx_msg void OnBnClickedCheckdlgShell();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckdlgAddface();
	CProgressCtrl m_AddFaceProgress;
	CStatic m_AddFacesTips;
};
