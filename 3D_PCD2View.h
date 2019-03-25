
// 3D_PCD2View.h : CMy3D_PCD2View 类的接口
//

#pragma once
#include"3D_PCD2Doc.h"
#include "PointCloudData.h"
#include "PointProperty.h"
class CMy3D_PCD2View : public CView
{
protected: // 仅从序列化创建
	CMy3D_PCD2View();
	DECLARE_DYNCREATE(CMy3D_PCD2View)

	// 特性
public:
	CMy3D_PCD2Doc* GetDocument() const;

	// OpenGL环境配置变量
	HGLRC m_hGLContext;
	int m_GLPixelIndex;
	//OpenGL图形变量
	float m_xRotation;
	float m_yRotation;
	float m_xTranslation;
	float m_yTranslation;
	float m_zTranslation;
	float m_xScaling;
	float m_yScaling;
	float m_zScaling;

	// Mouse 
	BOOL b_LeftButtonDown;
	BOOL b_RightButtonDown;
	CPoint m_LeftDownPos;
	CPoint m_RightDownPos;
	HCURSOR m_CursorRotation;
	//工作显示区的宽度
	int m_DisplayWidth;

	//工作显示区的长度
	int m_DisplayHeight;

	//已导入文件
	BOOL b_FileInputed;
	//已经进行了边界保留
	bool b_BoundaryChecked;
	//已经进行了边界聚类
	bool b_BoundaryCluster;
	//已经进行了约束点的选取
	bool b_SelectedPoints;
	//用于在属性栏上显示模型名称
	char* m_ShowFileName;
	//用于在属性栏上显示模型点云数量
	int m_ShowPointCloudNum;

	PointCloudData m_myPointCloudData;//点云数据
private:
	//拾取模式
	bool b_SelectingMode;
	//约束点选择的模式
	bool b_GetingRPMode;
	//点属性模式
	bool b_SelectingPointPropertyMode;
	//三视图显示
	bool b_TriWindowMode;
	//某次按住鼠标左键移动时
	bool b_LeftButtonFirstDown;
	//存储上一个点
	CPoint m_LastPoint;
	//test
	bool b_test;
	//完成模型三角化
	bool b_SurfaceGenerated;
	//法矢量显示模式
	bool b_NormalVectorMode;

	// 操作
public:
	//OpenGL环境配置函数
	bool SetWindowPixelFormat(HDC hDC);
	bool CreateViewGLContext(HDC hDC);
	//初始化图形变量
	void InitGeometry(void);
	//显示点云模型
	void PointCloudDisplay();
	//读取点云模型
	void ReadPointCloudData();
	//鼠标拾取对象
	void SelectObject(CPoint point, CPoint lastPoint);
	//设置为鼠标拾取状态
	void SetSelectingMode();
	//设置为普通显示模式
	void SetNormalMode();
	//设置为约束点选择模式
	void SetGetRPMode();
	//保存
	void OnFileSaveAs();
	//保存
	void OnFileSave();
	//边界保留
	void PointCloudBoundaryCheck();
	//孔洞修补
	void PointCloudHoleRepair();
	//三视图显示
	void SetTriWindowsMode();
	//普通视图显示
	void SetSingleWindowMode();
	//边界聚类
	void PointCloudBoundaryClustering();
	//自动孔洞修补
	void PointCloudAutomaticHoleRepair();
	//点云模型三角化
	void PointCLoudSurfaceGenerating();
	//计算法矢量
	void CaculateNormal(GLdouble* dVertex1, GLdouble* dVertex2,
		GLdouble* dVertex3, GLdouble* dNormal);
	//进行重绘，主要给其他两个面板调用
	void ViewInvalidateRect();
	//设置为鼠标属性选择模式
	void SetGetPointPropertyMode();
	//获取所有数据点的法矢量
	void PointCloudGetSelectedPointsNormalVectors();
	//一键修补模型孔洞
	void PointCloudOneStepRepair();
	//一键修补模型孔洞并且进行表面重建
	void PointCloudOneStepRepairAndSurface();
	// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// 实现
public:
	virtual ~CMy3D_PCD2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnFileOpen();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // 3D_PCD2View.cpp 中的调试版本
inline CMy3D_PCD2Doc* CMy3D_PCD2View::GetDocument() const
{
	return reinterpret_cast<CMy3D_PCD2Doc*>(m_pDocument);
}
#endif

