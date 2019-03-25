// MyRenderToolBoard.cpp : 实现文件
//渲染板类，实现工作区内左面渲染工具的选择。

#include "stdafx.h"
#include "3D_PCD2.h"
#include "MyRenderToolBoard.h"
#include "ProgressDialog.h"
#include "3D_PCD2View.h"
#include"ChildFrm.h"
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
// MyRenderToolBoard



MyRenderToolBoard::MyRenderToolBoard()
: CFormView(IDD_MY_RENDER_TOOL)
{
	((CChildFrame*)AfxGetApp())->m_LeftView = this;
}

MyRenderToolBoard::~MyRenderToolBoard()
{
}

void MyRenderToolBoard::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//为按钮插入图片
	HBITMAP   hBitmap1;
	hBitmap1 = (HBITMAP)::LoadImage(
		NULL,
		"res\1.bmp",                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE  
	((CButton *)GetDlgItem(IDC_NORMALMODE))->SetBitmap(hBitmap1);

	HBITMAP   hBitmap2;
	hBitmap2 = (HBITMAP)::LoadImage(
		NULL,
		"res\2.bmp",                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE  
	((CButton *)GetDlgItem(IDC_GETPOINT))->SetBitmap(hBitmap2);

	HBITMAP   hBitmap3;
	hBitmap3 = (HBITMAP)::LoadImage(
		NULL,
		"res\3.bmp",                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE  
	((CButton *)GetDlgItem(IDC_GETREPOINT))->SetBitmap(hBitmap3);
	
	HBITMAP   hBitmap4;
	hBitmap4 = (HBITMAP)::LoadImage(
		NULL,
		"res\4.bmp",                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE  
	((CButton *)GetDlgItem(IDC_BSAVE))->SetBitmap(hBitmap4);

	HBITMAP   hBitmap5;
	hBitmap5 = (HBITMAP)::LoadImage(
		NULL,
		"res\5.bmp",                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE  
	((CButton *)GetDlgItem(IDC_HOLEREPAIR))->SetBitmap(hBitmap5);

	HBITMAP   hBitmap6;
	hBitmap6 = (HBITMAP)::LoadImage(
		NULL,
		"res\6.bmp",                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE  
	((CButton *)GetDlgItem(IDC_POINTPROPERTY))->SetBitmap(hBitmap6);
	
	HBITMAP   hBitmap7;
	hBitmap7 = (HBITMAP)::LoadImage(
		NULL,
		"res\7.bmp",                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE  
	((CButton *)GetDlgItem(IDC_SINGLEWINDOW))->SetBitmap(hBitmap7);
	
	HBITMAP   hBitmap8;
	hBitmap8 = (HBITMAP)::LoadImage(
		NULL,
		"res\8.bmp",                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE  
	((CButton *)GetDlgItem(IDC_TRIWINDOW))->SetBitmap(hBitmap8);
	
	HBITMAP   hBitmap9;
	hBitmap9 = (HBITMAP)::LoadImage(
		NULL,
		"res\9.bmp",                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE  
	((CButton *)GetDlgItem(IDC_SURFACE))->SetBitmap(hBitmap9);

	HBITMAP   hBitmap10;
	hBitmap10 = (HBITMAP)::LoadImage(
		NULL,
		"res\10.bmp",                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE  
	((CButton *)GetDlgItem(IDC_LIMITSELF))->SetBitmap(hBitmap10);

	HBITMAP   hBitmap11;
	hBitmap11 = (HBITMAP)::LoadImage(
		NULL,
		"res\11.bmp",                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE  
	((CButton *)GetDlgItem(IDC_CLUSTER))->SetBitmap(hBitmap11);

	HBITMAP   hBitmap12;
	hBitmap12 = (HBITMAP)::LoadImage(
		NULL,
		"res\12.bmp",                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE  
	((CButton *)GetDlgItem(IDC_MAXERSIZE))->SetBitmap(hBitmap12);
	
	HBITMAP   hBitmap13;
	hBitmap13 = (HBITMAP)::LoadImage(
		NULL,
		"res\13.bmp",                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE  
	((CButton *)GetDlgItem(IDC_MINERSIZE))->SetBitmap(hBitmap13);

	HBITMAP   hBitmap14;
	hBitmap14 = (HBITMAP)::LoadImage(
		NULL,
		"res\14.bmp",                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE  
	((CButton *)GetDlgItem(IDC_NORMALVECTOR))->SetBitmap(hBitmap14);

	HBITMAP   hBitmap15;
	hBitmap15 = (HBITMAP)::LoadImage(
		NULL,
		"res\15.bmp",                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE  
	((CButton *)GetDlgItem(IDC_AHREPAIR))->SetBitmap(hBitmap15);

	HBITMAP   hBitmap16;
	hBitmap16 = (HBITMAP)::LoadImage(
		NULL,
		"res\16.bmp",                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE  
	((CButton *)GetDlgItem(IDC_HRASUR))->SetBitmap(hBitmap16);

	//添加按钮的提示信息
	m_Mytip.Create(this);
	m_Mytip.AddTool(GetDlgItem(IDC_NORMALMODE), "普通模式");
	m_Mytip.AddTool(GetDlgItem(IDC_GETPOINT), "鼠标拾取模式");
	m_Mytip.AddTool(GetDlgItem(IDC_POINTPROPERTY), "点属性拾取模式");
	m_Mytip.AddTool(GetDlgItem(IDC_GETREPOINT), "约束点选择模式");
	m_Mytip.AddTool(GetDlgItem(IDC_BSAVE), "边界保留");
	m_Mytip.AddTool(GetDlgItem(IDC_HOLEREPAIR), "孔洞修补");
	m_Mytip.AddTool(GetDlgItem(IDC_LIMITSELF), "自动孔洞修补");
	m_Mytip.AddTool(GetDlgItem(IDC_CLUSTER), "边界聚类");
	m_Mytip.AddTool(GetDlgItem(IDC_SURFACE), "表面生成");
	m_Mytip.AddTool(GetDlgItem(IDC_MAXERSIZE), "放大");
	m_Mytip.AddTool(GetDlgItem(IDC_MINERSIZE), "缩小");
	m_Mytip.AddTool(GetDlgItem(IDC_SINGLEWINDOW), "单屏显示");
	m_Mytip.AddTool(GetDlgItem(IDC_TRIWINDOW), "多屏显示");
	m_Mytip.AddTool(GetDlgItem(IDC_NORMALVECTOR), "显示/不显示法矢量");
	m_Mytip.AddTool(GetDlgItem(IDC_AHREPAIR), "一键修补");
	m_Mytip.AddTool(GetDlgItem(IDC_HRASUR), "一键修补并表面重建");
	m_Mytip.SetDelayTime(200); //设置延迟
	//m_Mytip.SetTipTextColor(RGB(0, 0, 255)); //设置提示文本的颜色
	//m_Mytip.SetTipBkColor(RGB(255, 255, 255)); //设置提示框的背景颜色
	m_Mytip.Activate(TRUE); //设置是否启用提示

}
IMPLEMENT_DYNCREATE(MyRenderToolBoard, CFormView)

BEGIN_MESSAGE_MAP(MyRenderToolBoard, CFormView)
	ON_BN_CLICKED(IDC_GETPOINT, &MyRenderToolBoard::OnBnClickedGetpoint)
	ON_BN_CLICKED(IDC_NORMALMODE, &MyRenderToolBoard::OnBnClickedNormalmode)
	ON_BN_CLICKED(IDC_GETREPOINT, &MyRenderToolBoard::OnBnClickedGetrepoint)
	ON_BN_CLICKED(IDC_BSAVE, &MyRenderToolBoard::OnBnClickedBsave)
	ON_BN_CLICKED(IDC_HOLEREPAIR, &MyRenderToolBoard::OnBnClickedHolerepair)
	ON_BN_CLICKED(IDC_SINGLEWINDOW, &MyRenderToolBoard::OnBnClickedSinglewindow)
	ON_BN_CLICKED(IDC_TRIWINDOW, &MyRenderToolBoard::OnBnClickedTriwindow)
	ON_BN_CLICKED(IDC_CLUSTER, &MyRenderToolBoard::OnBnClickedCluster)
	ON_BN_CLICKED(IDC_LIMITSELF, &MyRenderToolBoard::OnBnClickedLimitself)
	ON_BN_CLICKED(IDC_SURFACE, &MyRenderToolBoard::OnBnClickedSurface)
	ON_BN_CLICKED(IDC_POINTPROPERTY, &MyRenderToolBoard::OnBnClickedPointproperty)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_MAXERSIZE, &MyRenderToolBoard::OnBnClickedMaxersize)
	ON_BN_CLICKED(IDC_MINERSIZE, &MyRenderToolBoard::OnBnClickedMinersize)
	ON_BN_CLICKED(IDC_NORMALVECTOR, &MyRenderToolBoard::OnBnClickedNormalvector)
	ON_BN_CLICKED(IDC_AHREPAIR, &MyRenderToolBoard::OnBnClickedAhrepair)
	ON_BN_CLICKED(IDC_HRASUR, &MyRenderToolBoard::OnBnClickedHrasur)
END_MESSAGE_MAP()


// MyRenderToolBoard 诊断

#ifdef _DEBUG
void MyRenderToolBoard::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void MyRenderToolBoard::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// MyRenderToolBoard 消息处理程序

//设置为鼠标拾取模式
void MyRenderToolBoard::OnBnClickedGetpoint()
{
	// TODO:  在此添加控件通知处理程序代码
	if (((CChildFrame*)AfxGetApp())->m_MyDrawView->b_FileInputed)
	{//若已经导入模型文件则
		((CChildFrame*)AfxGetApp())->m_MyDrawView->SetSelectingMode();
	}
	else
	{
		MessageBox("未导入任何模型!", "提示");
	}
}

//设置为普通显示模式
void MyRenderToolBoard::OnBnClickedNormalmode()
{
	// TODO:  在此添加控件通知处理程序代码
	((CChildFrame*)AfxGetApp())->m_MyDrawView->SetNormalMode();
}

//设为约束点选择模式
void MyRenderToolBoard::OnBnClickedGetrepoint()
{
	// TODO:  在此添加控件通知处理程序代码
	if (((CChildFrame*)AfxGetApp())->m_MyDrawView->b_FileInputed)
	{//若已经导入模型文件则
		((CChildFrame*)AfxGetApp())->m_MyDrawView->b_SelectedPoints = true;
		((CChildFrame*)AfxGetApp())->m_MyDrawView->SetGetRPMode();
	}
	else
	{
		MessageBox("未导入任何模型!","提示");
	}
}

//对模型进行边界保留
void MyRenderToolBoard::OnBnClickedBsave()
{
	// TODO:  在此添加控件通知处理程序代码
	if (((CChildFrame*)AfxGetApp())->m_MyDrawView->b_FileInputed)
	{//若已经导入模型文件则
		((CChildFrame*)AfxGetApp())->m_MyDrawView->b_BoundaryChecked = true;
		((CChildFrame*)AfxGetApp())->m_RightView->GetProperties();//从属性板获取算法参数
		AfxBeginThread(BCfunction, this);//申请一个进程来执行function1，进行边界保留
		ProgressDialog dlg;
		dlg.DoModal();
	}
	else
	{
		MessageBox("未导入任何模型!", "提示");
	}
}

//进行孔洞修补
void MyRenderToolBoard::OnBnClickedHolerepair()
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


void MyRenderToolBoard::OnBnClickedProgress()
{
	// TODO:  在此添加控件通知处理程序代码
}

UINT BCfunction(LPVOID pParam)
{
	((CChildFrame*)AfxGetApp())->m_MyDrawView->PointCloudBoundaryCheck();
	gProgress = 0;
	return 0;
}

UINT HRfunction(LPVOID pParam)
{
	((CChildFrame*)AfxGetApp())->m_MyDrawView->PointCloudHoleRepair();
	gProgress = 0;
	return 0;
}
UINT AHRfunction(LPVOID pParam)
{
	((CChildFrame*)AfxGetApp())->m_MyDrawView->PointCloudAutomaticHoleRepair();
	gProgressAu = 0;
	return 0;
}
UINT Sufunction(LPVOID pParam)
{
	((CChildFrame*)AfxGetApp())->m_MyDrawView->PointCLoudSurfaceGenerating();
	gProgress = 0;
	return 0;
}
UINT OneStepfunction(LPVOID pParam)
{
	((CChildFrame*)AfxGetApp())->m_MyDrawView->PointCloudOneStepRepair();
	gProgressAu = 0;
	return 0;
}
UINT OneStepAndSurfacefunction(LPVOID pParam)
{
	((CChildFrame*)AfxGetApp())->m_MyDrawView->PointCloudOneStepRepairAndSurface();
	gProgressAu = 0;
	return 0;
}
//设置为单一窗口
void MyRenderToolBoard::OnBnClickedSinglewindow()
{
	// TODO:  在此添加控件通知处理程序代码
	((CChildFrame*)AfxGetApp())->m_MyDrawView->SetSingleWindowMode();
}

//设置为三视图
void MyRenderToolBoard::OnBnClickedTriwindow()
{
	// TODO:  在此添加控件通知处理程序代码
	((CChildFrame*)AfxGetApp())->m_MyDrawView->SetTriWindowsMode();
}

//进行边界聚类
void MyRenderToolBoard::OnBnClickedCluster()
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

//自动修补
void MyRenderToolBoard::OnBnClickedLimitself()
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

//进行点云的三角化
void MyRenderToolBoard::OnBnClickedSurface()
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


void MyRenderToolBoard::OnBnClickedPointproperty()
{
	// TODO:  在此添加控件通知处理程序代码
	if (((CChildFrame*)AfxGetApp())->m_MyDrawView->b_FileInputed)
	{//若已经导入模型文件则
		((CChildFrame*)AfxGetApp())->m_MyDrawView->SetGetPointPropertyMode();
	}
	else
	{
		MessageBox("未导入任何模型!", "提示");
	}
	
}


void MyRenderToolBoard::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CFormView::OnPaint()
	CRect   rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(100,100,100));   //设置为黑色背景
	
}

BOOL MyRenderToolBoard::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_MOUSEMOVE)
	{
		m_Mytip.RelayEvent(pMsg);
	}
   return CFormView::PreTranslateMessage(pMsg);
}

//放大按钮
void MyRenderToolBoard::OnBnClickedMaxersize()
{
	// TODO:  在此添加控件通知处理程序代码
	if (((CChildFrame*)AfxGetApp())->m_MyDrawView->b_FileInputed)
	{//若已经导入模型文件则
		((CChildFrame*)AfxGetApp())->m_MyDrawView->m_xScaling = 1.2
			* ((CChildFrame*)AfxGetApp())->m_MyDrawView->m_xScaling;

		((CChildFrame*)AfxGetApp())->m_MyDrawView->m_yScaling = 1.2
			* ((CChildFrame*)AfxGetApp())->m_MyDrawView->m_yScaling;

		((CChildFrame*)AfxGetApp())->m_MyDrawView->m_zScaling = 1.2
			* ((CChildFrame*)AfxGetApp())->m_MyDrawView->m_zScaling;
		((CChildFrame*)AfxGetApp())->m_MyDrawView->ViewInvalidateRect();
	}
	else
	{
		MessageBox("未导入任何模型!", "提示");
	}

}
//缩小按钮
void MyRenderToolBoard::OnBnClickedMinersize()
{
	// TODO:  在此添加控件通知处理程序代码
	if (((CChildFrame*)AfxGetApp())->m_MyDrawView->b_FileInputed)
	{//若已经导入模型文件则
		((CChildFrame*)AfxGetApp())->m_MyDrawView->m_xScaling = 0.8
			* ((CChildFrame*)AfxGetApp())->m_MyDrawView->m_xScaling;

		((CChildFrame*)AfxGetApp())->m_MyDrawView->m_yScaling = 0.8
			* ((CChildFrame*)AfxGetApp())->m_MyDrawView->m_yScaling;

		((CChildFrame*)AfxGetApp())->m_MyDrawView->m_zScaling = 0.8
			* ((CChildFrame*)AfxGetApp())->m_MyDrawView->m_zScaling;
		((CChildFrame*)AfxGetApp())->m_MyDrawView->ViewInvalidateRect();
	}
	else
	{
		MessageBox("未导入任何模型!", "提示");
	}

}

//显示所有数据点的法矢量
void MyRenderToolBoard::OnBnClickedNormalvector()
{
	// TODO:  在此添加控件通知处理程序代码
	if (((CChildFrame*)AfxGetApp())->m_MyDrawView->b_FileInputed)
	{//若已经导入模型文件则
		((CChildFrame*)AfxGetApp())->m_MyDrawView->PointCloudGetSelectedPointsNormalVectors();
	}
	else
	{
		MessageBox("未导入任何模型!", "提示");
	}

}

//模型一键修补
void MyRenderToolBoard::OnBnClickedAhrepair()
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

//模型一键修补并进行表面修补
void MyRenderToolBoard::OnBnClickedHrasur()
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


