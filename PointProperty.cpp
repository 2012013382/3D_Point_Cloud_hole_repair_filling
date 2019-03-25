// PointProperty.cpp : 实现文件
//

#include "stdafx.h"
#include "3D_PCD2.h"
#include "PointProperty.h"
#include "afxdialogex.h"


// PointProperty 对话框

IMPLEMENT_DYNAMIC(PointProperty, CDialogEx)

PointProperty::PointProperty(CWnd* pParent /*=NULL*/)
	: CDialogEx(PointProperty::IDD, pParent)
{

}
PointProperty::PointProperty(int pointNum, double x, double y, double z, CWnd* pParent /*=NULL*/)
	: CDialogEx(PointProperty::IDD, pParent)
{
	m_PointNum = pointNum;
	m_X = x;
	m_Y = y;
	m_Z = z;
}
PointProperty::~PointProperty()
{
}

void PointProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	char cPointNum[10];
	char cX[20];
	char cY[20];
	char cZ[20];
	wsprintf(cPointNum, "%d", m_PointNum);
	sprintf(cX, "%f", m_X);
	sprintf(cY, "%f", m_Y);
	sprintf(cZ, "%f", m_Z);
	SetDlgItemText(IDC_EDITPOINTNUM, cPointNum);
	SetDlgItemText(IDC_EDITPOINTX, cX);
	SetDlgItemText(IDC_EDITPOINTY, cY);
	SetDlgItemText(IDC_EDITPOINTZ, cZ);
}


BEGIN_MESSAGE_MAP(PointProperty, CDialogEx)
END_MESSAGE_MAP()


// PointProperty 消息处理程序
