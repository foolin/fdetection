// FacesDemoDlg.h : ͷ�ļ�
//

#pragma once


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
public:
	afx_msg void OnBnClickedOpenImage();
	IplImage* m_readImage;

public:
	void ShowImage( IplImage* img, UINT ID );
	void ResizeImage(IplImage* img);
	void FaceDetect( IplImage* image );
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSaveImage();
	afx_msg void OnBnClickedAboutUs();
	afx_msg void OnBnClickedDetectFace();
	CvMemStorage* m_storage;
	CvHaarClassifierCascade* m_cascade;
	char* m_cascadeName;

};
