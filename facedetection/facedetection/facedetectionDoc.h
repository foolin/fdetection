// facedetectionDoc.h : CfacedetectionDoc ��Ľӿ�
//


#pragma once


class CfacedetectionDoc : public CDocument
{
protected: // �������л�����
	CfacedetectionDoc();
	DECLARE_DYNCREATE(CfacedetectionDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CfacedetectionDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


