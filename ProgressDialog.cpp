// ProgressDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "3D_PCD2.h"
#include "ProgressDialog.h"
#include "afxdialogex.h"
#include "Guass.h"

// ProgressDialog 对话框

IMPLEMENT_DYNAMIC(ProgressDialog, CDialogEx)

ProgressDialog::ProgressDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(ProgressDialog::IDD, pParent)
{
	
}

ProgressDialog::~ProgressDialog()
{
}

void ProgressDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ProgressDialog, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &ProgressDialog::OnNMCustomdrawProgress1)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// ProgressDialog 消息处理程序


void ProgressDialog::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}


void ProgressDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnTimer(nIDEvent);
	CProgressCtrl* m_progress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	if (gProgress > 0)
	{
		m_progress->SetPos(gProgress);
	}
	if (gProgressAu > 0)
	{
		m_progress->SetPos(gProgressAu);
	}
	if (gProgress == -1)
	{
		this->EndDialog(true);
		gProgress = 1;
	}
	if (gProgressAu == -1)
	{
		this->EndDialog(true);
		gProgressAu = 1;
	}
}


void ProgressDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO:  在此处添加消息处理程序代码
	CProgressCtrl* m_progress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	this->KillTimer(1);
	this->SetTimer(1, 20, NULL);
	m_progress->SetRange(1, 50);
}
