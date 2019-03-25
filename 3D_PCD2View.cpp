
// 3D_PCD2View.cpp : CMy3D_PCD2View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "3D_PCD2.h"
#endif

#include "3D_PCD2Doc.h"
#include "3D_PCD2View.h"
#include "ChildFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy3D_PCD2View

IMPLEMENT_DYNCREATE(CMy3D_PCD2View, CView)

BEGIN_MESSAGE_MAP(CMy3D_PCD2View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_COMMAND(ID_FILE_OPEN, &CMy3D_PCD2View::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CMy3D_PCD2View::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CMy3D_PCD2View::OnFileSaveAs)
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CMy3D_PCD2View 构造/析构

CMy3D_PCD2View::CMy3D_PCD2View()
{
	// TODO: 在此处添加构造代码
	((CChildFrame*)AfxGetApp())->m_MyDrawView = this;
	// OpenGL
	m_hGLContext = NULL;
	m_GLPixelIndex = 0;
	// Mouse
	b_LeftButtonDown = FALSE;
	b_RightButtonDown = FALSE;
	//m_CursorRotation = AfxGetApp()->LoadCursor(IDC_CURSOR_ROTATION);
	//初始化各种用于显示的变量
	b_FileInputed = FALSE;
	b_BoundaryChecked = false;
	b_BoundaryCluster = false;
	b_SelectedPoints = false;
	InitGeometry();
	//设为非拾取模式
	b_SelectingMode = false;
	//设为非约束点选择模式
	b_GetingRPMode = false;
	//设置为非点属性选择模式
	b_SelectingPointPropertyMode = false;
	//三视图显示
	b_TriWindowMode = false;
	b_LeftButtonFirstDown = true;
	b_SurfaceGenerated = false;
	b_NormalVectorMode = false;
	b_test = false;

}

void CMy3D_PCD2View::InitGeometry(void)
{
	m_xRotation = 0.0f;
	m_yRotation = 0.0f;
	m_xTranslation = 0.0f;
	m_yTranslation = 0.0f;
	m_zTranslation = 0.0f;
	m_xScaling = 1.0f;
	m_yScaling = 1.0f;
	m_zScaling = 1.0f;
}

CMy3D_PCD2View::~CMy3D_PCD2View()
{
}

BOOL CMy3D_PCD2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMy3D_PCD2View 绘制

void CMy3D_PCD2View::OnDraw(CDC* /*pDC*/)
{
	CMy3D_PCD2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CMy3D_PCD2View 打印

BOOL CMy3D_PCD2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMy3D_PCD2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMy3D_PCD2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CMy3D_PCD2View 诊断

#ifdef _DEBUG
void CMy3D_PCD2View::AssertValid() const
{
	CView::AssertValid();
}

void CMy3D_PCD2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy3D_PCD2Doc* CMy3D_PCD2View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy3D_PCD2Doc)));
	return (CMy3D_PCD2Doc*)m_pDocument;
}
#endif //_DEBUG


// CMy3D_PCD2View 消息处理程序


int CMy3D_PCD2View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);

	if (SetWindowPixelFormat(hDC) == FALSE)
	{
		return 0;
	}

	if (CreateViewGLContext(hDC) == FALSE)
	{
		return 0;
	}
	/*
	glEnable(GL_NORMALIZE);//进行光照模拟前需要对法向量进行单位化才能
	//开始进行光照计算.
	//glClearDepth(1);
	GLfloat light1PosType[] = { 0.0, 1.0, 0.0, 0.0 };//设置一个无限远的光源
	glLightfv(GL_LIGHT1, GL_POSITION, light1PosType);
	// Default : lighting
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	*/
	return 0;
}

bool CMy3D_PCD2View::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;

	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;

	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER | PFD_STEREO_DONTCARE;

	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 8;
	pixelDesc.cRedShift = 16;
	pixelDesc.cGreenBits = 8;
	pixelDesc.cGreenShift = 8;
	pixelDesc.cBlueBits = 8;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 64;
	pixelDesc.cAccumRedBits = 16;
	pixelDesc.cAccumGreenBits = 16;
	pixelDesc.cAccumBlueBits = 16;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 32;
	pixelDesc.cStencilBits = 8;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;

	m_GLPixelIndex = ChoosePixelFormat(hDC, &pixelDesc);
	if (m_GLPixelIndex == 0) // Choose default
	{
		m_GLPixelIndex = 1;
		if (DescribePixelFormat(hDC, m_GLPixelIndex,
			sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc) == 0)
			return FALSE;
	}

	if (!SetPixelFormat(hDC, m_GLPixelIndex, &pixelDesc))
		return FALSE;

	return TRUE;
}

bool CMy3D_PCD2View::CreateViewGLContext(HDC hDC)
{
	m_hGLContext = wglCreateContext(hDC);

	if (m_hGLContext == NULL)
		return FALSE;

	if (wglMakeCurrent(hDC, m_hGLContext) == FALSE)
		return FALSE;

	return TRUE;
}


void CMy3D_PCD2View::OnDestroy()
{
	if (wglGetCurrentContext() != NULL)
		wglMakeCurrent(NULL, NULL);

	if (m_hGLContext != NULL)
	{
		wglDeleteContext(m_hGLContext);
		m_hGLContext = NULL;
	}

	CView::OnDestroy();
}


void CMy3D_PCD2View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	m_DisplayWidth = cx;
	m_DisplayHeight = cy;
	//glDrawBuffer(GL_BACK);
	//glEnable(GL_DEPTH_TEST);//本程序无需显示深度

}

//按鼠标左键后的消息响应函数
void CMy3D_PCD2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	b_LeftButtonDown = TRUE;
	m_LeftDownPos = point;
	if (b_SelectingMode == true || b_SelectingPointPropertyMode == true)
	{
		SelectObject(point,point);//鼠标拾取
	}
	InvalidateRect(NULL, FALSE);
	CView::OnLButtonDown(nFlags, point);
}

//松鼠标左键后的消息响应函数
void CMy3D_PCD2View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (b_GetingRPMode == true)
	{//此时为约束点选择模式
		SelectObject(point, m_LastPoint);
	}
	b_LeftButtonFirstDown = true;
	b_LeftButtonDown = FALSE;
	InvalidateRect(NULL, FALSE);
	CView::OnLButtonUp(nFlags, point);
}


void CMy3D_PCD2View::OnMouseMove(UINT nFlags, CPoint point)
{
	if (b_LeftButtonDown)//控制物体的转动
	{
		if (b_LeftButtonFirstDown == true)
		{
			m_LastPoint = point;
			b_LeftButtonFirstDown = false;
		}
		if (b_SelectingMode == false && b_GetingRPMode == false && b_SelectingPointPropertyMode == false)
		{
			m_yRotation -= (float)(m_LeftDownPos.x - point.x) / 3.0f;
			m_xRotation -= (float)(m_LeftDownPos.y - point.y) / 3.0f;
			((CChildFrame*)AfxGetApp())->m_RightView->SetAngle();
			m_LeftDownPos = point;
		}
		InvalidateRect(NULL, FALSE);
	}
	else if (b_RightButtonDown)
	{
		m_xScaling -= (float)(m_RightDownPos.x - point.x) / 50;
		if (m_xScaling <= 0)
		{
			m_xScaling += (float)(m_RightDownPos.x - point.x) / 50;
		}
		else
		{
			m_yScaling -= (float)(m_RightDownPos.x - point.x) / 50;
			m_zScaling -= (float)(m_RightDownPos.x - point.x) / 50;
		}
		m_RightDownPos = point;
		InvalidateRect(NULL, FALSE);
	}
	CView::OnMouseMove(nFlags, point);
}

//对窗口内的内容进行绘制


void CMy3D_PCD2View::OnPaint()
{
	// Device context for painting
	CPaintDC dc(this);

	// Useful in multidoc templates
	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);
	wglMakeCurrent(hDC, m_hGLContext);

	// Start rendering...
	//程序在这个地方可以调用一个点云模型的绘制函数
	PointCloudDisplay();
	if (b_SurfaceGenerated)
	{
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		//创建光源
		GLfloat light_position0[] = { 0.0f, 0.0f, -1.0f, 0.0f };
		GLfloat light_position1[] = { 0.0f, 0.0f, 1.0f, 0.0f };
		GLfloat light_position2[] = { 0.0f, 1.0f, 0.0f, 0.0f };
		GLfloat light_position3[] = { 0.0f, -1.0f, 0.0f, 0.0f };
		GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 0.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
		glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
		glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
		glLightfv(GL_LIGHT3, GL_POSITION, light_position3);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse);
		glEnable(GL_LIGHTING);        //启动光照功能，和两个光照
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
		glEnable(GL_LIGHT3);
		glEnable(GL_LIGHT4);
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);//启动双面光照(important!)
		InvalidateRect(NULL, FALSE);
	}
	/*GLfloat light_position[] = { 10.0, 10.0, 10.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	*/
	// Double buffer
	//交换缓冲区，显示缓冲区中的内容，
	//如果不这么做，在双缓存的技术下就看不到图像了。
	SwapBuffers(dc.m_ps.hdc);
}

void CMy3D_PCD2View::PointCloudDisplay()
{
	if (b_TriWindowMode)/*侧顶视图*/
	{
		glEnable(GL_SCISSOR_TEST);/*启动剪裁测试*/
		glScissor(0, 0, m_DisplayWidth / 2, m_DisplayHeight);/*第一个显示区域大小为整个客户区的左半*/
		glClearColor(1, 1, 1, 1.0);/*设置第一个客户区的清屏颜色*/
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);/*清除颜色缓存和深度缓存*/
		//glClearDepth(1.0);
		glDisable(GL_SCISSOR_TEST);/*关闭剪裁测试*/
		glViewport(0, 0, m_DisplayWidth / 2, m_DisplayHeight);/*设置视口*/

		glMatrixMode(GL_PROJECTION);//进入投影模式
		glLoadIdentity();/*清空当前矩阵*/
		if ((m_DisplayWidth / 2) <= m_DisplayHeight)/*设置投影方式*/
		{
			glOrtho(-1.5, 1.5, -1.5*(GLdouble)m_DisplayHeight / (GLdouble)(m_DisplayWidth / 2), 1.5*(GLdouble)m_DisplayHeight / (GLdouble)(m_DisplayWidth / 2), -10.0, 10.0);
		}
		else
		{
			glOrtho(-1.5*(GLdouble)(m_DisplayWidth / 2) / (GLdouble)m_DisplayHeight, 1.5*(GLdouble)(m_DisplayWidth / 2) / (GLdouble)m_DisplayHeight, -1.5, 1.5, -10.0, 10.0);
		}
		//设置照相机方向为前方
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 0, 1.5, 0, 0, 0, 0, 1, 0);
		ReadPointCloudData();


		glEnable(GL_SCISSOR_TEST);/*启动剪裁测试*/
		glScissor(m_DisplayWidth / 2, 0, m_DisplayWidth / 2, m_DisplayHeight / 2);/*第二个区域的大小为右半部分的上半部分*/
		glClearColor(1 - 0.05, 1 - 0.05, 1 - 0.05, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClearDepth(1.0);
		glDisable(GL_SCISSOR_TEST);
		glViewport(m_DisplayWidth / 2, 0, m_DisplayWidth / 2, m_DisplayHeight / 2);


		glMatrixMode(GL_PROJECTION);//进入投影模式
		glLoadIdentity();/*清空当前矩阵*/
		if ((m_DisplayWidth) <= m_DisplayHeight)/*设置投影方式*/
		{
			glOrtho(-1.5, 1.5, -1.5*(GLdouble)m_DisplayHeight / (GLdouble)(m_DisplayWidth), 1.5*(GLdouble)m_DisplayHeight / (GLdouble)(m_DisplayWidth), -10.0, 10.0);
		}
		else
		{
			glOrtho(-1.5*(GLdouble)(m_DisplayWidth) / (GLdouble)m_DisplayHeight, 1.5*(GLdouble)(m_DisplayWidth) / (GLdouble)m_DisplayHeight, -1.5, 1.5, -10.0, 10.0);
		}
		//设置照相机方向为右面
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(1.5, 0, 0, 0, 0, 0, 0, 1, 0);
		ReadPointCloudData();

		glEnable(GL_SCISSOR_TEST);
		glScissor(m_DisplayWidth / 2, m_DisplayHeight / 2, m_DisplayWidth / 2, m_DisplayHeight / 2);/*第三个客户区的右半部分的下半部分*/
		glClearColor(1 - 0.1, 1 - 0.1, 1 - 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//	glClearDepth(1.0);
		glDisable(GL_SCISSOR_TEST);
		glViewport(m_DisplayWidth / 2, m_DisplayHeight / 2, m_DisplayWidth / 2, m_DisplayHeight / 2);


		glMatrixMode(GL_PROJECTION);//进入投影模式
		glLoadIdentity();/*清空当前矩阵*/
		if ((m_DisplayWidth) <= m_DisplayHeight)/*设置投影方式*/
		{
			glOrtho(-1.5, 1.5, -1.5*(GLdouble)m_DisplayHeight / (GLdouble)(m_DisplayWidth), 1.5*(GLdouble)m_DisplayHeight / (GLdouble)(m_DisplayWidth), -10.0, 10.0);
		}
		else
		{
			glOrtho(-1.5*(GLdouble)(m_DisplayWidth) / (GLdouble)m_DisplayHeight, 1.5*(GLdouble)(m_DisplayWidth) / (GLdouble)m_DisplayHeight, -1.5, 1.5, -10.0, 10.0);
		}
		//设置照相机方向为上方
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 1.5, 0, 0, 0, 0, 0, 0, -1);
		ReadPointCloudData();
	}
	else/*全景视图下*/
	{
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClearDepth(1.0);
		glViewport(0, 0, m_DisplayWidth, m_DisplayHeight);

		glMatrixMode(GL_PROJECTION);//进入投影模式
		glLoadIdentity();
		if ((m_DisplayWidth) <= m_DisplayHeight)
		{
			glOrtho(-1.5, 1.5, -1.5*(GLdouble)m_DisplayHeight / (GLdouble)(m_DisplayWidth), 1.5*(GLdouble)m_DisplayHeight / (GLdouble)(m_DisplayWidth), -10.0, 10.0);
		}
		else
		{
			glOrtho(-1.5*(GLdouble)(m_DisplayWidth) / (GLdouble)m_DisplayHeight, 1.5*(GLdouble)(m_DisplayWidth) / (GLdouble)m_DisplayHeight, -1.5, 1.5, -10.0, 10.0);
		}
		//glFrustum( -1.0, 1.0, -1.0, 1.0, -1.0, 1.0 );

		//设定摄像机的位置和摄影方向
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 0, 1.5, 0, 0, 0, 0, 1, 0);
		ReadPointCloudData();
	}
	//////////////////////////////////////////////////////////////////
	glFlush();/*强制显示模型*/
}
//读取点云数据
void CMy3D_PCD2View::ReadPointCloudData()
{
	// Position / translation / scale
	glTranslated(m_xTranslation, m_yTranslation, m_zTranslation);
	glRotatef(m_xRotation, 1.0, 0.0, 0.0);
	glRotatef(m_yRotation, 0.0, 1.0, 0.0);
	glScalef(m_xScaling, m_yScaling, m_zScaling);
	//初始化一个临时点云存储器
	if (b_FileInputed && !b_SurfaceGenerated)
	{
		glPointSize(4.0);
		PCPoint* tempPointCloud = m_myPointCloudData.GetOriginPointCloud();//获取点云模型
		for (int num = 0; num < m_myPointCloudData.GetPointSumNumber(); num++)
		{
			glLoadName(num);
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glVertexPointer(3, GL_DOUBLE, 0, tempPointCloud[num].m_Coordinate);
			glColorPointer(3, GL_DOUBLE, 0, tempPointCloud[num].m_Color);
			glDrawArrays(GL_POINTS, 0, 1);
		}
		//显示插入的修补点
		vector<PCPoint>* AddedPoingCLoud = m_myPointCloudData.GetNewAddedPoint();
		for (int num = 0; num < AddedPoingCLoud->size(); num++)
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glVertexPointer(3, GL_DOUBLE, 0, (*AddedPoingCLoud)[num].m_Coordinate);
			glColorPointer(3, GL_DOUBLE, 0, (*AddedPoingCLoud)[num].m_Color);
			glDrawArrays(GL_POINTS, 0, 1);
		}
		//显示边界聚类后的结果
		if (b_test)
		{
			vector<PCPoint>* boundaryPointClusters = m_myPointCloudData.GetBoundaryPointClusters();
			for (int i = 0; i < m_myPointCloudData.GetCloudClusterNum(); i++)
			{
				for (int num = 0; num < boundaryPointClusters[i].size(); num++)
				{
					glEnableClientState(GL_VERTEX_ARRAY);
					glEnableClientState(GL_COLOR_ARRAY);
					glVertexPointer(3, GL_DOUBLE, 0, boundaryPointClusters[i][num].m_Coordinate);
					glColorPointer(3, GL_DOUBLE, 0, boundaryPointClusters[i][num].m_Color);
					glDrawArrays(GL_POINTS, 0, 1);
				}
			}
		}
		if (b_NormalVectorMode)
		{//如果为法矢量显示模式，则进行法矢量的显示
			for (int num = 0; num < m_myPointCloudData.m_SelectedPointsNormalPoints.size(); num++)
			{
				glBegin(GL_LINES);
				glColor3f(1.0, 0.0, 0.0);
				glVertex3f(m_myPointCloudData.m_SelectedPointsNormalPoints[num].m_Coordinate[0]
					, m_myPointCloudData.m_SelectedPointsNormalPoints[num].m_Coordinate[1]
					, m_myPointCloudData.m_SelectedPointsNormalPoints[num].m_Coordinate[2]);
				glVertex3f(m_myPointCloudData.m_SelectedPointsNormalPoints2[num].m_Coordinate[0]
					, m_myPointCloudData.m_SelectedPointsNormalPoints2[num].m_Coordinate[1],
					m_myPointCloudData.m_SelectedPointsNormalPoints2[num].m_Coordinate[2]);
				glEnd();
			}
		}
		///test begin///
	/*	for (int num = 0; num < m_myPointCloudData.m_TestSeletedPointCloud.size() / 2; num++)
		{
		glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex3f(m_myPointCloudData.m_TestSeletedPointCloud[num].m_Coordinate[0], m_myPointCloudData.m_TestSeletedPointCloud[num].m_Coordinate[1], m_myPointCloudData.m_TestSeletedPointCloud[num].m_Coordinate[2]);
		glVertex3f(m_myPointCloudData.m_TestSeletedPointCloud[num + m_myPointCloudData.m_TestSeletedPointCloud.size() / 2].m_Coordinate[0], m_myPointCloudData.m_TestSeletedPointCloud[num + m_myPointCloudData.m_TestSeletedPointCloud.size() / 2].m_Coordinate[1], m_myPointCloudData.m_TestSeletedPointCloud[num + m_myPointCloudData.m_TestSeletedPointCloud.size() / 2].m_Coordinate[2]);
		glEnd();
		}
		*/
		
		/*for (int i = 0; i < m_myPointCloudData.m_TestSeletedPointCloud.size(); i++)
		{
			m_myPointCloudData.m_TestSeletedPointCloud[i].m_Color[1] = 0.4;
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glVertexPointer(3, GL_DOUBLE, 0, m_myPointCloudData.m_TestSeletedPointCloud[i].m_Coordinate);
			glColorPointer(3, GL_DOUBLE, 0, m_myPointCloudData.m_TestSeletedPointCloud[i].m_Color);
			glDrawArrays(GL_POINTS, 0, 1);
		}*/
		///test  end///
		////test begin////*/
		/*PCPoint* ttempPoint = m_myPointCloudData.GetOriginPointCloud();
		m_myPointCloudData.PCLKDtreeNKSearch(ttempPoint[15245], 50);
		for (int i = 0; i < m_myPointCloudData.RkNearestPoints.size(); i++)
		{
			if (i == 0)
				m_myPointCloudData.RkNearestPoints[i].m_Color[1] = 1;
			else
				m_myPointCloudData.RkNearestPoints[i].m_Color[2] = 0.1;
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glVertexPointer(3, GL_DOUBLE, 0, m_myPointCloudData.RkNearestPoints[i].m_Coordinate);
			glColorPointer(3, GL_DOUBLE, 0, m_myPointCloudData.RkNearestPoints[i].m_Color);
			glDrawArrays(GL_POINTS, 0, 1);
		}
		m_myPointCloudData.RkNearestPoints.clear();
		////test end//*/
	}
	else if (b_SurfaceGenerated && b_FileInputed)
	{//如果完成了模型的三角化
		//定义材质
		GLfloat mat_disffuse[] = { 0.8f, 0.8f, 0.5f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_disffuse);
		
		glClearColor(0.0,0.0,0.0,0.0);
		glShadeModel(GL_SMOOTH);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(1.0f,1.0f,0.0f);
		PCPoint* addedRepairingPointCloud = m_myPointCloudData.GetAddedRepairingPointCloud();
		for (size_t i = 0; i < m_myPointCloudData.triangles.polygons.size(); i++)
		{

			PCPoint point1 = addedRepairingPointCloud[m_myPointCloudData.triangles.polygons[i].vertices[0]];
			PCPoint point2 = addedRepairingPointCloud[m_myPointCloudData.triangles.polygons[i].vertices[1]];
			PCPoint point3 = addedRepairingPointCloud[m_myPointCloudData.triangles.polygons[i].vertices[2]];
			
			GLdouble dpoint1[3] = { point1.m_Coordinate[0], point1.m_Coordinate[1], point1.m_Coordinate[2] };
			GLdouble dpoint2[3] = { point2.m_Coordinate[0], point2.m_Coordinate[1], point2.m_Coordinate[2] };
			GLdouble dpoint3[3] = { point3.m_Coordinate[0], point3.m_Coordinate[1], point3.m_Coordinate[2] };
			GLdouble dNormal[3] = {0.0, 0.0, 1.0};
			CaculateNormal(dpoint1, dpoint2, dpoint3, dNormal);
			glBegin(GL_POLYGON);
			glNormal3dv(dNormal);
			glVertex3f(point1.m_Coordinate[0], point1.m_Coordinate[1], point1.m_Coordinate[2]);
			glVertex3f(point2.m_Coordinate[0], point2.m_Coordinate[1], point2.m_Coordinate[2]);
			glVertex3f(point3.m_Coordinate[0], point3.m_Coordinate[1], point3.m_Coordinate[2]);
			glEnd();  //网格绘制完成
		}
	}

/*	vector<PCPoint>* selectedPoint = m_myPointCloudData.GetSelectedPoint();
	for (int num = 0; num < selectedPoint->size() / 2; num++)
	{
		glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex3f((*selectedPoint)[num].m_Coordinate[0], (*selectedPoint)[num].m_Coordinate[1], (*selectedPoint)[num].m_Coordinate[2]);
		glVertex3f((*selectedPoint)[num + selectedPoint->size() / 2].m_Coordinate[0], (*selectedPoint)[num + selectedPoint->size() / 2].m_Coordinate[1], (*selectedPoint)[num + selectedPoint->size() / 2].m_Coordinate[2]);
		glEnd();
	}
	*/
	/*///test begin//
	vector<PCPoint>* selectedPoint2 = m_myPointCloudData.GetSelectedPoint();
		for (int num = 0; num < selectedPoint2->size() / 2; num++)
		{
			(*selectedPoint2)[num].m_Color[0] = 1.0;
			(*selectedPoint2)[num].m_Color[1] = 0.5;
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glVertexPointer(3, GL_DOUBLE, 0, (*selectedPoint2)[num].m_Coordinate);
			glColorPointer(3, GL_DOUBLE, 0, (*selectedPoint2)[num].m_Color);
			glDrawArrays(GL_POINTS, 0, 1);
		}
	//test end///*/

}

void CMy3D_PCD2View::OnFileOpen()//打开点云文件
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog fdlg(true, "txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("点云数据(*.txt)||"), NULL);
	fdlg.m_ofn.lpstrTitle = "browse";
	if (fdlg.DoModal() == IDOK) {//生成形式对话框
		CString m_strDirectory = fdlg.GetPathName();
		CString m_filename = fdlg.GetFileName();
		char *filename = (LPSTR)(LPCTSTR)m_filename;
		m_myPointCloudData.ReadPointCloudFile((LPSTR)(LPCTSTR)m_strDirectory);//读取点云数据文件
		//设置模型名称和点云数量，用于显示
		m_ShowFileName = filename;
		m_ShowPointCloudNum = m_myPointCloudData.GetPointSumNumber();
		((CChildFrame*)AfxGetApp())->m_RightView->SetNameAndNum();
		b_FileInputed = TRUE;
		b_FIP = true;
		MessageBox("导入成功", "提示");
	}
	InvalidateRect(NULL, FALSE);
}

void CMy3D_PCD2View::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_RightDownPos = CPoint(0, 0);
	b_RightButtonDown = FALSE;
	CView::OnRButtonUp(nFlags, point);
}


void CMy3D_PCD2View::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	b_RightButtonDown = TRUE;
	m_RightDownPos.x = point.x;
	m_RightDownPos.y = point.y;
	CView::OnRButtonDown(nFlags, point);
}
//进行鼠标拾取
void CMy3D_PCD2View::SelectObject(CPoint point, CPoint lastPoint)
{
	GLuint selectBuf[51200];
	GLint hits;
	GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);
	glSelectBuffer(51200, selectBuf);

	glRenderMode(GL_SELECT);				// Enter the SELECT render mode
	glInitNames();
	glPushName(0);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	if (b_GetingRPMode == true)
	{//当为约束点选择模式时，选择一片方形点云
		gluPickMatrix((GLdouble)((point.x + lastPoint.x) / 2), (GLdouble)(viewport[3] - (point.y + lastPoint.y) / 2), point.x - lastPoint.x, point.y - lastPoint.y, viewport);
	}
	if (b_SelectingMode == true || b_SelectingPointPropertyMode == true)
	{//当为鼠标拾取模式时，每次只拾取一个点
		gluPickMatrix((GLdouble)point.x, (GLdouble)(viewport[3] - point.y), 5.0, 5.0, viewport);
	}
	glViewport(0, 0, m_DisplayWidth, m_DisplayHeight);
	glOrtho(-1.5*(GLdouble)(m_DisplayWidth) / (GLdouble)m_DisplayHeight, 1.5*(GLdouble)(m_DisplayWidth) / (GLdouble)m_DisplayHeight, -1.5, 1.5, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	ReadPointCloudData();
	glPopMatrix();
	glFlush();

	hits = glRenderMode(GL_RENDER);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	PCPoint* tempPointCloud = m_myPointCloudData.GetOriginPointCloud();
	if (hits>0)
	{
		int n = 0; double minz = selectBuf[1];
		for (int i = 1; i<hits; i++)
		{
			if (b_SelectingMode == true)
			{
				if (selectBuf[1 + i * 4] < minz)
				{
					n = i;
					minz = selectBuf[1 + i * 4];
				}
			}
			if (b_GetingRPMode == true)
			{
				tempPointCloud[selectBuf[3 + i * 4]].b_Selected = true;//设为被选择
				tempPointCloud[selectBuf[3 + i * 4]].m_Color[1] = 1.0;//设为绿色
			}
		}
		if (b_SelectingMode == true)
		{
			tempPointCloud[selectBuf[3 + n * 4]].b_Selected = true;//设为被选择
			tempPointCloud[selectBuf[3 + n * 4]].m_Color[1] = 1.0;//设为绿色
		}
		if (b_SelectingPointPropertyMode == true)
		{
			tempPointCloud[selectBuf[3 + n * 4]].b_Selected = true;//设为被选择
			tempPointCloud[selectBuf[3 + n * 4]].m_Color[1] = 1.0;//设为绿色
			//显示点属性对话框
			PointProperty *pdlg = new PointProperty(tempPointCloud[selectBuf[3 + n * 4]].m_ID
				, tempPointCloud[selectBuf[3 + n * 4]].m_Coordinate[0]
				, tempPointCloud[selectBuf[3 + n * 4]].m_Coordinate[1]
				, tempPointCloud[selectBuf[3 + n * 4]].m_Coordinate[2]);
			pdlg->Create(IDD_POINTPROPERTY, this);
			pdlg->ShowWindow(SW_SHOW);
		}
	}
}
//设为鼠标拾取模式
void  CMy3D_PCD2View::SetSelectingMode()
{
	b_SelectingMode = true;
	b_GetingRPMode = false;
	b_NormalVectorMode = false;
}
//设为普通显示模式
void CMy3D_PCD2View::SetNormalMode()
{
	b_SelectingMode = false;
	b_GetingRPMode = false;
	b_SelectingPointPropertyMode = false;
	//将鼠标设置为普通模式
	SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW));
}
//设置为约束点选择模式
void CMy3D_PCD2View::SetGetRPMode()
{
	b_GetingRPMode = true;
	b_SelectingMode = false;
	//将鼠标设置为十字
	SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_CROSS));
}
//设置为鼠标属性选择模式
void  CMy3D_PCD2View::SetGetPointPropertyMode()
{
	b_SelectingPointPropertyMode = true;
	b_GetingRPMode = false;
}

void CMy3D_PCD2View::OnFileSave()
{
	// TODO: 在此添加命令处理程序代码
	b_Saved = true;
	CFileDialog hFileDlg(false, "txt", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY,
		_T("点云数据(*.txt)||"), NULL);
	hFileDlg.m_ofn.nFilterIndex = 1;
	hFileDlg.m_ofn.hwndOwner = m_hWnd;
	hFileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME);
	hFileDlg.m_ofn.lpstrTitle = TEXT("保存图像\0");
	hFileDlg.m_ofn.nMaxFile = MAX_PATH;
	if (hFileDlg.DoModal() == IDOK)
	{
		CString m_path = hFileDlg.GetPathName();
		//其实接下来就是c++文件操作的东西啦 ，这个简单呀！！！
		CStdioFile file;
		file.Open(_T(m_path), CFile::modeCreate | CFile::modeWrite | CFile::typeText);
		file.Close();
		FILE* fp;
		fopen_s(&fp, m_path, "w");
		PCPoint* tempPointCloud = m_myPointCloudData.GetOriginPointCloud();
		for (int i = 0; i<m_myPointCloudData.GetPointSumNumber(); i++)
		{
			if (tempPointCloud[i].b_Selected == false)//保存精简后的数据点
			{
				fprintf(fp, "%f %f %f\n", tempPointCloud[i].m_Coordinate[2], tempPointCloud[i].m_Coordinate[0], tempPointCloud[i].m_Coordinate[1]);
			}

		}
		fclose(fp);
		MessageBox("保存成功！", "提示");
		UpdateData(FALSE);//这句能够关闭对话框
	}

}


void CMy3D_PCD2View::OnFileSaveAs()
{
	// TODO: 在此添加命令处理程序代码
	b_Saved = true;
	CFileDialog hFileDlg(false, "txt", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY,
		_T("点云数据(*.txt)||"), NULL);
	hFileDlg.m_ofn.nFilterIndex = 1;
	hFileDlg.m_ofn.hwndOwner = m_hWnd;
	hFileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME);
	hFileDlg.m_ofn.lpstrTitle = TEXT("保存图像\0");
	hFileDlg.m_ofn.nMaxFile = MAX_PATH;
	if (hFileDlg.DoModal() == IDOK)
	{
		CString m_path = hFileDlg.GetPathName();
		//其实接下来就是c++文件操作的东西啦 ，这个简单呀！！！
		CStdioFile file;
		file.Open(_T(m_path), CFile::modeCreate | CFile::modeWrite | CFile::typeText);
		file.Close();
		FILE* fp;
		fopen_s(&fp, m_path, "w");
		PCPoint* tempPointCloud = m_myPointCloudData.GetOriginPointCloud();
		for (int i = 0; i<m_myPointCloudData.GetPointSumNumber(); i++)
		{

			if (tempPointCloud[i].b_Selected == false)//保存精简后的数据点
			{

				fprintf(fp, "%f %f %f\n", tempPointCloud[i].m_Coordinate[2], tempPointCloud[i].m_Coordinate[0], tempPointCloud[i].m_Coordinate[1]);
			}

		}
		fclose(fp);
		MessageBox("保存成功！", "提示");
		UpdateData(FALSE);//这句能够关闭对话框，但是并没有触发保存操作，只需要进行相应的文件操作就好了。。。

	}
}
//点云模型的边界检测
void CMy3D_PCD2View::PointCloudBoundaryCheck()
{
	PCPoint* tempPointCloud = m_myPointCloudData.GetOriginPointCloud();
	for (int num = 0; num < m_myPointCloudData.GetPointSumNumber(); num++)
	{//设置为初始状态
		tempPointCloud[num].b_BoundaryPoint = false;
		tempPointCloud[num].m_Color[0] = 0.0;
		tempPointCloud[num].m_Color[1] = 0.0;
		tempPointCloud[num].m_Color[2] = 0.0;
	}
	/*PCPoint* tempPointCloud = m_myPointCloudData.GetOriginPointCloud();
	for (int num = 0; num < m_myPointCloudData.GetPointSumNumber(); num++)
	{//对每个数据点进行边界检测
		gProgress = 50 * ((double)num / (double)m_myPointCloudData.GetPointSumNumber());//用于显示进度条
		if (m_myPointCloudData.BoundaryCheck(tempPointCloud[num]) == true)
		{
			tempPointCloud[num].b_BoundaryPoint = true;//设为边界点
			tempPointCloud[num].m_Color[0] = 1.0;//设为红色
		}
		else
		{
			tempPointCloud[num].m_Color[2] = 1.0;//设为蓝色
		}
	}*/
	//PCL中的模型边界保留算法
	m_myPointCloudData.PCLBoundaryKeep();
	gProgressAu = -1;//销毁进度条
	MessageBox("边界保留完成！","提示");
	InvalidateRect(NULL, FALSE);
}
//点云模型孔洞修补
void CMy3D_PCD2View::PointCloudHoleRepair()
{
	b_Saved = false;//文件已修改
	//进行孔洞修补，并返回修补结果
	if (m_myPointCloudData.HoleRepair())
	{
		MessageBox("空洞修补完成！", "提示");
	}
	else
	{
		MessageBox("该孔洞无法实现修补", "提示");
	}
	InvalidateRect(NULL, FALSE);
}
//三视图显示
void CMy3D_PCD2View::SetTriWindowsMode()
{
	b_TriWindowMode = true;
	InvalidateRect(NULL, FALSE);
}
//普通视图显示
void CMy3D_PCD2View::SetSingleWindowMode()
{
	b_TriWindowMode = false;
	InvalidateRect(NULL, FALSE);
}
//边界聚类
void CMy3D_PCD2View::PointCloudBoundaryClustering()
{
	//进行边界聚类
	m_myPointCloudData.BoundaryClustering();
	MessageBox("边界聚类完成","提示");
	InvalidateRect(NULL, FALSE);
	b_test = true;
}
//自动修补
void CMy3D_PCD2View::PointCloudAutomaticHoleRepair()
{
	b_Saved = false;//文件已修改
	gProgressAu = 0;
	m_myPointCloudData.AutomaticHoleRepair();
	gProgressAu = -1;
	MessageBox("自动修补完成", "提示");
	InvalidateRect(NULL, FALSE);
}
//点云模型三角化
void CMy3D_PCD2View::PointCLoudSurfaceGenerating()
{
	//进行模型三角化
	gProgressAu = 0;
	m_myPointCloudData.SurfaceGenerating();
	MessageBox("模型表面重建完成", "提示");
	//三角化完成
	b_SurfaceGenerated = true;
	InvalidateRect(NULL, FALSE);
}
//进行重绘，主要给其他两个面板调用
void  CMy3D_PCD2View::ViewInvalidateRect()
{
	InvalidateRect(NULL, FALSE);
}
//获取选取数据点的法矢量
void  CMy3D_PCD2View::PointCloudGetSelectedPointsNormalVectors()
{
	if (b_NormalVectorMode == false)
	{
		bool flag = false;
		PCPoint* tempPointCloud = m_myPointCloudData.GetOriginPointCloud();
		for (int num = 0; num < m_myPointCloudData.GetPointSumNumber(); num++)
		{
			if (tempPointCloud[num].b_Selected == true)
			{
				flag = true;
			}
		}
		if (flag)
		{
			m_myPointCloudData.GetSelectedPointsNormalVectors();
			InvalidateRect(NULL, FALSE);
			b_NormalVectorMode = true;
		}
		else
		{
			MessageBox("请先进入约束点选择模式，选择需要的数据点！", "提示");
		}
	}
	else
	{
		b_NormalVectorMode = false;
		InvalidateRect(NULL, FALSE);
	}
}
//一键修补模型
void CMy3D_PCD2View::PointCloudOneStepRepair()
{
	m_myPointCloudData.b_OneStepMode = true;
	//模型边界保留
	/*PCPoint* tempPointCloud = m_myPointCloudData.GetOriginPointCloud();
	for (int num = 0; num < m_myPointCloudData.GetPointSumNumber(); num++)
	{//对每个数据点进行边界检测
		gProgressAu = 10 * ((double)num / (double)m_myPointCloudData.GetPointSumNumber());//用于显示进度条
		if (m_myPointCloudData.BoundaryCheck(tempPointCloud[num]) == true)
		{
			tempPointCloud[num].b_BoundaryPoint = true;//设为边界点
		}
	}*/
	//PCL中的模型边界保留算法
	m_myPointCloudData.PCLBoundaryKeep();
	//边界聚类
	m_myPointCloudData.BoundaryClustering();
	//自动修补
	m_myPointCloudData.AutomaticHoleRepair();
	gProgressAu = -1;
	m_myPointCloudData.b_OneStepMode = false;
	MessageBox("一键修补完成！","提示");
	InvalidateRect(NULL, FALSE);
}
//一键修补模型并且进行表面重建
void CMy3D_PCD2View::PointCloudOneStepRepairAndSurface()
{
	
	m_myPointCloudData.b_OneStepMode = true; 
	m_myPointCloudData.b_AllPoint = true;
	//模型边界保留
	/*PCPoint* tempPointCloud = m_myPointCloudData.GetOriginPointCloud();
	for (int num = 0; num < m_myPointCloudData.GetPointSumNumber(); num++)
	{//对每个数据点进行边界检测
		gProgressAu = 5 * ((double)num / (double)m_myPointCloudData.GetPointSumNumber());//用于显示进度条
		if (m_myPointCloudData.BoundaryCheck(tempPointCloud[num]) == true)
		{
			tempPointCloud[num].b_BoundaryPoint = true;//设为边界点
		}
	}*/
	//PCL中的模型边界保留算法
	m_myPointCloudData.PCLBoundaryKeep();
	//边界聚类
	m_myPointCloudData.BoundaryClustering();
	//自动修补
	m_myPointCloudData.AutomaticHoleRepair();
	//模型表面重建
	m_myPointCloudData.SurfaceGenerating();
	b_SurfaceGenerated = true;
	gProgressAu = -1;
	m_myPointCloudData.b_OneStepMode = false;
	m_myPointCloudData.b_AllPoint = false;
	MessageBox("一键修补并重建表面完成！", "提示");
	InvalidateRect(NULL, FALSE);
}
//计算法矢量
void CMy3D_PCD2View::CaculateNormal(GLdouble* dVertex1, GLdouble* dVertex2,
	GLdouble* dVertex3, GLdouble* dNormal)               //计算每个三角的法向量，用于光照的显示
{
	GLdouble dVector1[3], dVector2[3];     //定义了两个向量
	dVector1[0] = dVertex2[0] - dVertex1[0];
	dVector1[1] = dVertex2[1] - dVertex1[1];
	dVector1[2] = dVertex2[2] - dVertex1[2];   //dVector1保存的是1,2两点之间的向量
	dVector2[0] = dVertex3[0] - dVertex1[0];
	dVector2[1] = dVertex3[1] - dVertex1[1];
	dVector2[2] = dVertex3[2] - dVertex1[2];   //dVector2保存的是1,3两点之间的向量
	dNormal[0] = dVector1[1] * dVector2[2] - dVector1[2] * dVector2[1];//这个法线是垂直于这两条向量的
	dNormal[1] = dVector1[2] * dVector2[0] - dVector1[0] * dVector2[2];//算x时就不含有x，算y时就不含有y
	dNormal[2] = dVector1[0] * dVector2[1] - dVector1[1] * dVector2[0];

	double dNormalLength = sqrt(dNormal[0] * dNormal[0] +
		dNormal[1] * dNormal[1] + dNormal[2] * dNormal[2]);
	//将法向量单位化
	if (dNormalLength != 0.0)
	{
		dNormal[0] = dNormal[0] / dNormalLength;
		dNormal[1] = dNormal[1] / dNormalLength;
		dNormal[2] = dNormal[2] / dNormalLength;
	}
	//当计算的法向量模为零时，就取z正向的一个单位向量
	else
	{
		dNormal[0] = 0.0;
		dNormal[1] = 0.0;
		dNormal[2] = 1.0;
	}
}