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

//���г�Ա
public:
	IplImage* m_readImage;
	IplImage* m_showImage;
	CvMemStorage* m_storage;
	CvHaarClassifierCascade* m_cascade;
	//char* m_cascadeName;
	CString m_cascadeName;
	int m_facesCount;

//���к���
public:
	void InitConfig();
	void ResizeImage(IplImage* img);
	void ShowImage( IplImage* img, UINT ID );
	void ShowImage( IplImage* srcImage);	//��ʾͼ�񣬰�����С
	void FaceDetect( IplImage* image );
	void SetReadImage( IplImage* image, int width=0, int height=0 );	//����ͼƬ��С�����浽m_readImage

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
};
