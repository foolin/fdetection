// facedetectionDoc.cpp : CfacedetectionDoc 类的实现
//

#include "stdafx.h"
#include "facedetection.h"

#include "facedetectionDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CfacedetectionDoc

IMPLEMENT_DYNCREATE(CfacedetectionDoc, CDocument)

BEGIN_MESSAGE_MAP(CfacedetectionDoc, CDocument)
END_MESSAGE_MAP()


// CfacedetectionDoc 构造/析构

CfacedetectionDoc::CfacedetectionDoc()
{
	// TODO: 在此添加一次性构造代码

}

CfacedetectionDoc::~CfacedetectionDoc()
{
}

BOOL CfacedetectionDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CfacedetectionDoc 序列化

void CfacedetectionDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CfacedetectionDoc 诊断

#ifdef _DEBUG
void CfacedetectionDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CfacedetectionDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CfacedetectionDoc 命令
