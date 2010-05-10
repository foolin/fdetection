// DetectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Face.h"
#include "DetectDlg.h"


// CDetectDlg 对话框

IMPLEMENT_DYNAMIC(CDetectDlg, CDialog)

CDetectDlg::CDetectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDetectDlg::IDD, pParent)
{

}

CDetectDlg::~CDetectDlg()
{
}

void CDetectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDetectDlg, CDialog)
END_MESSAGE_MAP()


// CDetectDlg 消息处理程序
