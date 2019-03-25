#pragma once


// ProgressDialog 对话框

class ProgressDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ProgressDialog)

public:
	ProgressDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ProgressDialog();

// 对话框数据
	enum { IDD = IDD_PROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
