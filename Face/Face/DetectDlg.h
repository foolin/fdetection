#pragma once
#include "Detect.h"
#include "Config.h"


#define SHOWIMAGE_WIDTH 350		//��ʾͼƬ��С
#define SHOWIMAGE_HEIGHT 330	

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
public:
	afx_msg void OnBnClickedDtbtnOpendir();
	afx_msg void OnBnClickedDtbtnOpenimage();
	afx_msg void OnBnClickedDtbtnSaveimage();
	afx_msg void OnBnClickedDtbtnDetect();
	afx_msg void OnBnClickedDtbtnAbout();


public:
	void ResizeImage(IplImage* img);
	void ShowImage( IplImage* img, UINT ID );	// ID ��Picture Control�ؼ���ID��
	void ShowImage( IplImage* srcImage);	//��ʾͼ�񣬰�����Сͼ��
	void SetTips( CString strTips);

private:
	IplImage* m_showImage;
	CDetect m_objDetect;
	CConfig m_objConfig;
	CString m_strCascadeName;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	// ����·��
	CString m_strAppPath;
};
