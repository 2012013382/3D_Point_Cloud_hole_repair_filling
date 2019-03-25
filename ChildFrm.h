
// ChildFrm.h : CChildFrame 类的接口
//
#pragma once
#include"MyRenderToolBoard.h"
#include"3D_PCD2View.h"
#include "PropertyBoard.h"
class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

	// 特性
public:
	CSplitterWnd m_wndSplitter;//用于窗口分割
	CMy3D_PCD2View *m_MyDrawView;//右边的模型显示区域
	MyRenderToolBoard *m_LeftView;//左边的工具栏区域
	PropertyBoard *m_RightView;//右边的属性栏区域
	// 操作
public:

	// 重写
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// 实现
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnMenuBoundaryKeep();
	afx_msg void OnMenuBoundaryCluster();
	afx_msg void OnMenuHoleRepair();
	afx_msg void OnAutomaticHoleRepair();
	afx_msg void OnMenuSurfaceGeneration();
	afx_msg void OnMenuTriShow();
	afx_msg void OnMenuSingleShow();
	afx_msg void OnMenuOneStepRepair();
	afx_msg void OnMenuOneStepRepairAndSurface();
	afx_msg void OnClose();
	afx_msg void OnPaint();
};
