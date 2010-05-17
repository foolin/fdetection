#pragma once
#include "Config.h"
#include "Check.h"
#include "Detect.h"

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
	void ResizeImage(IplImage* img);
	void ShowImage( IplImage* img, UINT ID );	// ID ��Picture Control�ؼ���ID��
	void ShowImage( IplImage* srcImage);	//��ʾͼ�񣬰�����Сͼ��
	void SetTips( CString strTips);

private:
	IplImage* m_showImage;
	CCheck  m_objCheck; 
	CDetect m_objDetect;
	CConfig m_objConfig;
	CString m_strAppPath;
public:
	afx_msg void OnBnClickedCheckdlgShell();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckdlgAddface();
};
