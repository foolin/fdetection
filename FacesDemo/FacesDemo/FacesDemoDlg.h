// FacesDemoDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CFacesDemoDlg �Ի���
class CFacesDemoDlg : public CDialog
{
// ����
public:
	CFacesDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FACESDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

//���г�Ա
public:
	IplImage* m_readImage;					//�����ͼƬ
	IplImage* m_showImage;					//��ʾ��ͼƬ
	CvMemStorage* m_storage;				//����ʶ�𿪱ٵ��ڴ�ռ�
	CvHaarClassifierCascade* m_cascade;		//����ʶ�������
	CString m_cascadeName;					//�������ļ���
	int m_facesCount;						//�����������Ŀ
	CString m_programPath;					//����·��

//���к���
public:
	void InitConfig();							//��ʼ�������ļ�
	void ResizeImage(IplImage* img);			//����ͼƬ��С��������ʾ��
	void ShowImage( IplImage* img, UINT ID );	//��ʾͼƬ
	void ShowImage( IplImage* srcImage);		//��ʾͼ�񣬰�����С�����أ�
	void FaceDetect( IplImage* image );			//�������
	void SetReadImage( IplImage* image, int width=0, int height=0 );	//����ͼƬ��С�����浽m_readImage
	void SetTips( CString strTips);				//������Ϣ��ʾ
///��Ϣӳ��
public:
	afx_msg void OnBnClickedOpenImage();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSaveImage();
	afx_msg void OnBnClickedAboutUs();
	afx_msg void OnBnClickedDetectFace();
	afx_msg void OnBnClickedRemoveNoise();
	afx_msg void OnBnClickedBinaryImage();
	afx_msg void OnBnClickedMinimize();
	afx_msg LRESULT OnWindMinimize(WPARAM wParam,LPARAM lParam);	//��С����Ϣ��Ӧ
	bool TrayMessage(DWORD dwFlag, UINT uIconId=IDR_MAINFRAME);		//��С��������Ӧ����
	afx_msg void OnTrayOpen();
	afx_msg void OnTrayQuit();
	afx_msg void OnTrayAbout();
};
