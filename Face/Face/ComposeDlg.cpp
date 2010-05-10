// ComposeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Face.h"
#include "ComposeDlg.h"


// CComposeDlg 对话框

IMPLEMENT_DYNAMIC(CComposeDlg, CDialog)

CComposeDlg::CComposeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CComposeDlg::IDD, pParent)
{

}

CComposeDlg::~CComposeDlg()
{
}

void CComposeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CComposeDlg, CDialog)
END_MESSAGE_MAP()


// CComposeDlg 消息处理程序
