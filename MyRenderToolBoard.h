#pragma once
UINT BCfunction(LPVOID pParam);
UINT HRfunction(LPVOID pParam);
UINT AHRfunction(LPVOID pParam);
UINT Sufunction(LPVOID pParam);
UINT OneStepfunction(LPVOID pParam);
UINT OneStepAndSurfacefunction(LPVOID pParam);
// MyRenderToolBoard 窗体视图

class MyRenderToolBoard : public CFormView
{
	DECLARE_DYNCREATE(MyRenderToolBoard)

protected:
	MyRenderToolBoard();           // 动态创建所使用的受保护的构造函数
	virtual ~MyRenderToolBoard();

public:
	CToolTipCtrl    m_Mytip;
	BOOL PreTranslateMessage(MSG* pMsg);
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY_RENDER_TOOL };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGetpoint();
	afx_msg void OnBnClickedNormalmode();
	afx_msg void OnBnClickedGetrepoint();
	afx_msg void OnBnClickedBsave();
	afx_msg void OnBnClickedHolerepair();
	afx_msg void OnBnClickedProgress();
	afx_msg void OnBnClickedSinglewindow();
	afx_msg void OnBnClickedTriwindow();
	afx_msg void OnBnClickedCluster();
	afx_msg void OnBnClickedLimitself();
	afx_msg void OnBnClickedSurface();
	afx_msg void OnBnClickedPointproperty();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedMaxersize();
	afx_msg void OnBnClickedMinersize();
	afx_msg void OnBnClickedNormalvector();
	afx_msg void OnBnClickedAhrepair();
	afx_msg void OnBnClickedHrasur();
};


