// facedetectionDoc.cpp : CfacedetectionDoc ���ʵ��
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


// CfacedetectionDoc ����/����

CfacedetectionDoc::CfacedetectionDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CfacedetectionDoc::~CfacedetectionDoc()
{
}

BOOL CfacedetectionDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CfacedetectionDoc ���л�

void CfacedetectionDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CfacedetectionDoc ���

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


// CfacedetectionDoc ����
