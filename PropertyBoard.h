#pragma once


// PropertyBoard 窗体视图

class PropertyBoard : public CFormView
{
	DECLARE_DYNCREATE(PropertyBoard)

protected:
	PropertyBoard();           // 动态创建所使用的受保护的构造函数
	virtual ~PropertyBoard();

public:
	enum { IDD = IDD_PROPERTYBOARD };
	//显示名称和点云数
	void SetNameAndNum();
	//获取属性板中的参数
	void GetAndSetAngleProperties();
	//显示观察角度参数
	void SetAngle();
	//用于获取属性板的参数
	void GetProperties();
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	//设置一些默认参数
	double m_RotationX;
	double m_RotationY;
	double m_textBoundary;
	double m_clusterDistace;
	double m_clusterNum;
	double m_trianMaxDistace;
	double m_trianMaxAngle;
	double m_trianMinAngle;
	double m_trianMaxSAngle;
	//第一次
	bool b_FirstTime;
public:
	afx_msg void OnBnClickedSetangle();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedMyhelp();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


