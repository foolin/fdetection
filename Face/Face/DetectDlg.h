#pragma once
#include "Detect.h"
#include "Config.h"


#define SHOWIMAGE_WIDTH 350		//��ʾͼƬ�ؼ������С
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
	afx_msg void OnBnClickedDtbtnOpendir();		//��Ŀ¼
	afx_msg void OnBnClickedDtbtnOpenimage();	//��ͼ��
	afx_msg void OnBnClickedDtbtnSaveimage();	//����ͼ��
	afx_msg void OnBnClickedDtbtnDetect();		//�������
	afx_msg void OnBnClickedDtbtnAbout();		//����


public:
	void ResizeImage(IplImage* img);	//��ʾͼ��
	void ShowImage( IplImage* img, UINT ID );	// ID ��Picture Control�ؼ���ID��
	void ShowImage( IplImage* srcImage);	//���أ���ʾͼ�񣬰�����Сͼ��
	void SetTips( CString strTips);			//��ʾ��Ϣ

private:
	CString m_strAppPath;	//����·��
	IplImage* m_showImage;	//��ʾ��ͼ��
	CDetect m_objDetect;	//����������
	CConfig m_objConfig;	//�����ļ����󣬶�ȡini�ļ�
	CString m_strCascadeName;	//��������������xml·��
	CString m_strEyesCascadeName;	//���ۼ���������xml·��
	bool m_blnIsShowGray;		//��ʶλ���Ƿ��Ѿ���ʾ�Ҷ�ͼ��


public:
	virtual BOOL OnInitDialog();	//�Ի����ʼ��
	afx_msg void OnPaint();			//���ں���
	afx_msg void OnBnClickedDtbtn2gray();	//�Ҷ�ͼ��
	afx_msg void OnBnClickedDtbtnEqualhist();	//ֱ��ͼ���⻯
	afx_msg void OnBnClickedDtbtnRemovenoise();	//ȥ������
};
