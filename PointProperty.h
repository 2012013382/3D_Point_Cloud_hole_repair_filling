#pragma once


// PointProperty 对话框

class PointProperty : public CDialogEx
{
	DECLARE_DYNAMIC(PointProperty)

public:
	int m_PointNum;
	double m_X;
	double m_Y;
	double m_Z;
	PointProperty(CWnd* pParent = NULL);   // 标准构造函数
	PointProperty(int pointNum, double x, double y, double z, CWnd* pParent = NULL);//重载构造函数
	virtual ~PointProperty();

// 对话框数据
	enum { IDD = IDD_POINTPROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
