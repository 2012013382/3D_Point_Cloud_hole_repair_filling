// PropertyBoard.cpp : 实现文件
//

#include "stdafx.h"
#include "3D_PCD2.h"
#include "PropertyBoard.h"
#include "ChildFrm.h"
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
// PropertyBoard

IMPLEMENT_DYNCREATE(PropertyBoard, CFormView)

PropertyBoard::PropertyBoard()
	: CFormView(PropertyBoard::IDD)
	, m_RotationX(0.0)
	, m_RotationY(0.0)
	, m_textBoundary(0.05)
	, m_clusterDistace(0.05)
	, m_clusterNum(20)
	, m_trianMaxDistace(0.05)
	, m_trianMaxAngle(4.5)
	, m_trianMinAngle(0.1)
	, m_trianMaxSAngle(0.9)
{
	((CChildFrame*)AfxGetApp())->m_RightView = this;
	b_FirstTime = true;
}

PropertyBoard::~PropertyBoard()
{
}

void PropertyBoard::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	if (b_FirstTime)
	{
		SetDlgItemText(IDC_EDITX, "0.0");
		SetDlgItemText(IDC_EDITY, "0.0");
		SetDlgItemText(IDC_EDITZ, "0.0");
		SetDlgItemText(IDC_EDITBCOE, "0.05");
		SetDlgItemText(IDC_EDITMINDISTANCE, "0.05");
		SetDlgItemText(IDC_EDITMINNUM, "20");
		SetDlgItemText(IDC_EDITMAXDISTANCE, "0.05");
		SetDlgItemText(IDC_EDITMAXANGLE, "4.5");
		SetDlgItemText(IDC_EDITMINANGLE, "0.1");
		SetDlgItemText(IDC_EDITMAXSEARCHD, "0.9");
		b_FirstTime = false;
	}
	HBITMAP   hBitmap1;
	hBitmap1 = (HBITMAP)::LoadImage(
		NULL,
		"D://help.bmp",                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE  
	((CButton *)GetDlgItem(IDC_MYHELP))->SetBitmap(hBitmap1);
	
	HBITMAP   hBitmap2;
	hBitmap2 = (HBITMAP)::LoadImage(
		NULL,
		"D://sure.bmp",                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE  
	((CButton *)GetDlgItem(IDC_SETANGLE))->SetBitmap(hBitmap2);
}

BEGIN_MESSAGE_MAP(PropertyBoard, CFormView)
	ON_BN_CLICKED(IDC_SETANGLE, &PropertyBoard::OnBnClickedSetangle)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_MYHELP, &PropertyBoard::OnBnClickedMyhelp)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// PropertyBoard 诊断

#ifdef _DEBUG
void PropertyBoard::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void PropertyBoard::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// PropertyBoard 消息处理程序
void PropertyBoard::SetNameAndNum()
{
	char* fileName = ((CChildFrame*)AfxGetApp())->m_MyDrawView->m_ShowFileName;
	SetDlgItemText(IDC_EDITNAME, fileName);
	int num = ((CChildFrame*)AfxGetApp())->m_MyDrawView->m_ShowPointCloudNum;
	char cNum[10];
	wsprintf(cNum, "%d", num);
	SetDlgItemText(IDC_EDITPOINTNUM, cNum);
}
//获取并设置观察角度参数
void PropertyBoard::GetAndSetAngleProperties()
{
	char textX[10];
	char textY[10];
	//获取用户输入值
	GetDlgItemText(IDC_EDITX, textX, 10);
	GetDlgItemText(IDC_EDITY, textY, 10);
	double dX = atof(textX);
	double dY = atof(textY);
	((CChildFrame*)AfxGetApp())->m_MyDrawView->m_xRotation = dX;
	((CChildFrame*)AfxGetApp())->m_MyDrawView->m_yRotation = dY;
}
//设置观察角度
void PropertyBoard::OnBnClickedSetangle()
{
	// TODO:  在此添加控件通知处理程序代码
	GetAndSetAngleProperties();
	//进行重绘
	((CChildFrame*)AfxGetApp())->m_MyDrawView->ViewInvalidateRect();
}
//显示观察角度参数
void PropertyBoard::SetAngle()
{
	double x, y;
	x = ((CChildFrame*)AfxGetApp())->m_MyDrawView->m_xRotation;
	y = ((CChildFrame*)AfxGetApp())->m_MyDrawView->m_yRotation;
	char cX[20];
	char cY[20];
	sprintf(cX, "%f", x);
	sprintf(cY, "%f", y);
	SetDlgItemText(IDC_EDITX, cX);
	SetDlgItemText(IDC_EDITY, cY);
}
//获取属性板参数
void PropertyBoard::GetProperties()
{
	char textBoundary[10];
	char clusterDistace[10];
	char clusterNum[10];
	char trianMaxDistace[10];
	char trianMaxAngle[10];
	char trianMinAngle[10];
	char trianMaxSAngle[10];
	GetDlgItemText(IDC_EDITBCOE, textBoundary, 10);
	GetDlgItemText(IDC_EDITMINDISTANCE, clusterDistace, 10);
	GetDlgItemText(IDC_EDITMINNUM, clusterNum, 10);
	GetDlgItemText(IDC_EDITMAXDISTANCE, trianMaxDistace, 10);
	GetDlgItemText(IDC_EDITMAXANGLE, trianMaxAngle, 10);
	GetDlgItemText(IDC_EDITMINANGLE, trianMinAngle, 10);
	GetDlgItemText(IDC_EDITMAXSEARCHD, trianMaxSAngle, 10);
	
	double d_textBoundary = atof(textBoundary);
	double d_clusterDistace = atof(clusterDistace);
	double d_clusterNum = atof(clusterNum);
	double d_trianMaxDistace = atof(trianMaxDistace);
	double d_trianMaxAngle = atof(trianMaxAngle);
	double d_trianMinAngle = atof(trianMinAngle);
	double d_trianMaxSAngle = atof(trianMaxSAngle);
	((CChildFrame*)AfxGetApp())->m_MyDrawView->m_myPointCloudData.Threadshold = d_textBoundary;
	((CChildFrame*)AfxGetApp())->m_MyDrawView->m_myPointCloudData.m_clusterDistace = d_clusterDistace;
	((CChildFrame*)AfxGetApp())->m_MyDrawView->m_myPointCloudData.m_clusterNum = d_clusterNum;
	((CChildFrame*)AfxGetApp())->m_MyDrawView->m_myPointCloudData.m_trianMaxDistace = d_trianMaxDistace;
	((CChildFrame*)AfxGetApp())->m_MyDrawView->m_myPointCloudData.m_trianMaxAngle = d_trianMaxAngle;
	((CChildFrame*)AfxGetApp())->m_MyDrawView->m_myPointCloudData.m_trianMinAngle = d_trianMinAngle;
	((CChildFrame*)AfxGetApp())->m_MyDrawView->m_myPointCloudData.m_trianMaxSAngle = d_trianMaxSAngle;

}

void PropertyBoard::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CFormView::OnPaint()
//	CRect   rect;
//	GetClientRect(rect);
//	dc.FillSolidRect(rect, RGB(100,100,100));   //设置为黑色背景
}


void PropertyBoard::OnBnClickedMyhelp()
{
	// TODO:  在此添加控件通知处理程序代码
	MessageBox("请查看说明文档！","提示");
}


HBRUSH PropertyBoard::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	/*// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_STATIC)
	{
		pDC->SetBkColor(RGB(100, 100, 100));//背景色为黑色
		//pDC->SetBkMode(TRANSPARENT);//设背景为透明
		pDC->SetTextColor(RGB(255, 255, 255));//文字为白色
	}
	if (pWnd->GetDlgCtrlID() == IDC_SETANGLE)
	{
		pDC->SetBkColor(RGB(100, 100, 100));//背景色为黑色
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔*/
	return hbr;
}
