// facedetectionView.h : CfacedetectionView ��Ľӿ�
//


#pragma once


class CfacedetectionView : public CView
{
protected: // �������л�����
	CfacedetectionView();
	DECLARE_DYNCREATE(CfacedetectionView)

// ����
public:
	CfacedetectionDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CfacedetectionView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // facedetectionView.cpp �еĵ��԰汾
inline CfacedetectionDoc* CfacedetectionView::GetDocument() const
   { return reinterpret_cast<CfacedetectionDoc*>(m_pDocument); }
#endif

