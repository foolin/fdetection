// facedetectionView.cpp : CfacedetectionView ���ʵ��
//

#include "stdafx.h"
#include "facedetection.h"

#include "facedetectionDoc.h"
#include "facedetectionView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CfacedetectionView

IMPLEMENT_DYNCREATE(CfacedetectionView, CView)

BEGIN_MESSAGE_MAP(CfacedetectionView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CfacedetectionView ����/����

CfacedetectionView::CfacedetectionView()
{
	// TODO: �ڴ˴���ӹ������

}

CfacedetectionView::~CfacedetectionView()
{
}

BOOL CfacedetectionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CfacedetectionView ����

void CfacedetectionView::OnDraw(CDC* /*pDC*/)
{
	CfacedetectionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CfacedetectionView ��ӡ

BOOL CfacedetectionView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CfacedetectionView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CfacedetectionView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CfacedetectionView ���

#ifdef _DEBUG
void CfacedetectionView::AssertValid() const
{
	CView::AssertValid();
}

void CfacedetectionView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CfacedetectionDoc* CfacedetectionView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CfacedetectionDoc)));
	return (CfacedetectionDoc*)m_pDocument;
}
#endif //_DEBUG


// CfacedetectionView ��Ϣ�������
