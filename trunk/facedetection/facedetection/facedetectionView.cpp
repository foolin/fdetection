// facedetectionView.cpp : CfacedetectionView 类的实现
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CfacedetectionView 构造/析构

CfacedetectionView::CfacedetectionView()
{
	// TODO: 在此处添加构造代码

}

CfacedetectionView::~CfacedetectionView()
{
}

BOOL CfacedetectionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CfacedetectionView 绘制

void CfacedetectionView::OnDraw(CDC* /*pDC*/)
{
	CfacedetectionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CfacedetectionView 打印

BOOL CfacedetectionView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CfacedetectionView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CfacedetectionView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CfacedetectionView 诊断

#ifdef _DEBUG
void CfacedetectionView::AssertValid() const
{
	CView::AssertValid();
}

void CfacedetectionView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CfacedetectionDoc* CfacedetectionView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CfacedetectionDoc)));
	return (CfacedetectionDoc*)m_pDocument;
}
#endif //_DEBUG


// CfacedetectionView 消息处理程序
