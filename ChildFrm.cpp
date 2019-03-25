
// ChildFrm.cpp : CChildFrame 类的实现
//

#include "stdafx.h"
#include "3D_PCD2.h"

#include "ChildFrm.h"
#include "Guass.h"
#include"ProgressDialog.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_COMMAND(ID_32773, &CChildFrame::OnMenuBoundaryKeep)
	ON_COMMAND(ID_32774, &CChildFrame::OnMenuBoundaryCluster)
	ON_COMMAND(ID_32775, &CChildFrame::OnMenuHoleRepair)
	ON_COMMAND(ID_32776, &CChildFrame::OnAutomaticHoleRepair)
	ON_COMMAND(ID_32777, &CChildFrame::OnMenuSurfaceGeneration)
	ON_COMMAND(ID_32778, &CChildFrame::OnMenuTriShow)
	ON_COMMAND(ID_32779, &CChildFrame::OnMenuSingleShow)
	ON_COMMAND(ID_32771, &CChildFrame::OnMenuOneStepRepair)
	ON_COMMAND(ID_32772, &CChildFrame::OnMenuOneStepRepairAndSurface)
	ON_WM_CLOSE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CChildFrame 构造/析构

CChildFrame::CChildFrame()
{
	// TODO: 在此添加成员初始化代码

}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式
	cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | FWS_ADDTOTITLE;
	cs.style = cs.style&~FWS_ADDTOTITLE;//去掉标题条中的文档名
	cs.lpszName = "点云模型";
	if (!CMDIChildWnd::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

// CChildFrame 诊断

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}
#endif //_DEBUG

// CChildFrame 消息处理程序


BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{

	// TODO: 在此添加专用代码和/或调用基类
	if (!m_wndSplitter.CreateStatic(this, 1, 3, WS_CHILD | WS_VISIBLE))
	{
		TRACE("Failed to CreateStaticSplitter\n");
		return FALSE;
	}

	// 左边的工具栏区域
	if (!m_wndSplitter.CreateView(0, 0,
		RUNTIME_CLASS(MyRenderToolBoard), CSize(45, 200), pContext))
	{
		TRACE("Failed to create command view pane\n");
		return FALSE;
	}

	// 中间的模型显示区域
	if (!m_wndSplitter.CreateView(0, 1,
		RUNTIME_CLASS(CMy3D_PCD2View), CSize(1155, 200), pContext))
	{
		TRACE("Failed to create preview pane\n");
		return FALSE;
	}

	// 右边的属性显示栏区域
	if (!m_wndSplitter.CreateView(0, 2,
		RUNTIME_CLASS(PropertyBoard), CSize(100, 200), pContext))
	{
		TRACE("Failed to create preview pane\n");
		return FALSE;
	}
	m_MyDrawView = (CMy3D_PCD2View*) m_wndSplitter.GetPane(0, 1);
	SetActiveView(m_MyDrawView, true);
	m_LeftView = (MyRenderToolBoard*) m_wndSplitter.GetPane(0, 0);
	m_RightView = (PropertyBoard*) m_wndSplitter.GetPane(0, 2);
	return TRUE;
}

//菜单栏的边界保留
void CChildFrame::OnMenuBoundaryKeep()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_MyDrawView->b_FileInputed)
	{//若已经导入模型文件则
		m_MyDrawView->b_BoundaryChecked = true;
		m_RightView->GetProperties();//从属性板获取算法参数
		AfxBeginThread(BCfunction, this);//申请一个进程来执行function1，进行边界保留
		ProgressDialog dlg;
		dlg.DoModal();
	}
	else
	{
		MessageBox("未导入任何模型!", "提示");
	}
}

//菜单栏的边界聚类
void CChildFrame::OnMenuBoundaryCluster()
{
	// TODO:  在此添加控件通知处理程序代码
	if (((CChildFrame*)AfxGetApp())->m_MyDrawView->b_FileInputed)
	{//若已经导入模型文件则
		if (((CChildFrame*)AfxGetApp())->m_MyDrawView->b_BoundaryChecked)
		{
			((CChildFrame*)AfxGetApp())->m_MyDrawView->b_BoundaryCluster = true;
			((CChildFrame*)AfxGetApp())->m_RightView->GetProperties();//从属性板获取算法参数
			((CChildFrame*)AfxGetApp())->m_MyDrawView->PointCloudBoundaryClustering();
		}
		else
		{
			MessageBox("请先进行模型的边界保留!", "提示");
		}
	}
	else
	{
		MessageBox("未导入任何模型!", "提示");
	}
}

//菜单栏的孔洞修补
void CChildFrame::OnMenuHoleRepair()
{
	// TODO:  在此添加控件通知处理程序代码
	if (((CChildFrame*)AfxGetApp())->m_MyDrawView->b_FileInputed)
	{//若已经导入模型文件则
		if (((CChildFrame*)AfxGetApp())->m_MyDrawView->b_SelectedPoints)
		{
			AfxBeginThread(HRfunction, this);//在申请一个进程来执行function2,进行模型的孔洞修补
			ProgressDialog dlg;
			dlg.DoModal();
			((CChildFrame*)AfxGetApp())->m_MyDrawView->b_SelectedPoints = false;
		}
		else
		{
			MessageBox("请先进行约束点的选择！", "提示");
		}
	}
	else
	{
		MessageBox("未导入任何模型!", "提示");
	}
}

//菜单栏的自动孔洞修补
void CChildFrame::OnAutomaticHoleRepair()
{
	// TODO:  在此添加控件通知处理程序代码
	if (((CChildFrame*)AfxGetApp())->m_MyDrawView->b_FileInputed)
	{//若已经导入模型文件则
		if (((CChildFrame*)AfxGetApp())->m_MyDrawView->b_BoundaryCluster)
		{
			AfxBeginThread(AHRfunction, this);//在申请一个进程来执行function2,进行模型的孔洞修补
			ProgressDialog dlg;
			dlg.DoModal();
		}
		else
		{
			MessageBox("请先进行边界聚类!", "提示");
		}
	}
	else
	{
		MessageBox("未导入任何模型!", "提示");
	}
}

//菜单栏模型的表面生成
void CChildFrame::OnMenuSurfaceGeneration()
{
	// TODO:  在此添加控件通知处理程序代码
	if (((CChildFrame*)AfxGetApp())->m_MyDrawView->b_FileInputed)
	{//若已经导入模型文件则
		((CChildFrame*)AfxGetApp())->m_RightView->GetProperties();//从属性板获取算法参数
		AfxBeginThread(Sufunction, this);//在申请一个进程来执行function2,进行模型的孔洞修补
		ProgressDialog dlg;
		dlg.DoModal();
	}
	else
	{
		MessageBox("未导入任何模型!", "提示");
	}
}

//菜单栏的三屏显示模式
void CChildFrame::OnMenuTriShow()
{
	((CChildFrame*)AfxGetApp())->m_MyDrawView->SetTriWindowsMode();
}

//菜单栏单屏显示
void CChildFrame::OnMenuSingleShow()
{
	((CChildFrame*)AfxGetApp())->m_MyDrawView->SetSingleWindowMode();
}

//菜单栏一键修补
void CChildFrame::OnMenuOneStepRepair()
{
	// TODO:  在此添加控件通知处理程序代码
	if (((CChildFrame*)AfxGetApp())->m_MyDrawView->b_FileInputed)
	{//若已经导入模型文件则
		((CChildFrame*)AfxGetApp())->m_RightView->GetProperties();//从属性板获取算法参数
		AfxBeginThread(OneStepfunction, this);//在申请一个进程来执行function2,进行模型的孔洞修补
		ProgressDialog dlg;
		dlg.DoModal();
	}
	else
	{
		MessageBox("未导入任何模型!", "提示");
	}
}

//菜单栏一键修补并进行表面生成
void CChildFrame::OnMenuOneStepRepairAndSurface()
{
	// TODO:  在此添加控件通知处理程序代码
	if (((CChildFrame*)AfxGetApp())->m_MyDrawView->b_FileInputed)
	{//若已经导入模型文件则
		((CChildFrame*)AfxGetApp())->m_RightView->GetProperties();//从属性板获取算法参数
		AfxBeginThread(OneStepAndSurfacefunction, this);//在申请一个进程来执行function2,进行模型的孔洞修补
		ProgressDialog dlg;
		dlg.DoModal();
	}
	else
	{
		MessageBox("未导入任何模型!", "提示");
	}
}


void CChildFrame::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (b_Saved == false && b_FIP == true)
	{
		if (IDNO == ::MessageBox(this->m_hWnd, _T("还未进行保存,是否直接关闭?"), _T("提示"), MB_ICONQUESTION | MB_YESNO))
		{
			return;
		}
	}
	CMDIChildWnd::OnClose();
}


void CChildFrame::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CMDIChildWnd::OnPaint()
	/*CRect   rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(100, 100, 100));   //设置为黑色背景
	*/
}
