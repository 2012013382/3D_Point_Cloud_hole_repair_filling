#include "stdafx.h"
#include "PCPoint.h"


PCPoint::PCPoint()
{
	//初始化成员变量
	for (int i = 0; i < 3; i++)
	{
		m_Coordinate[i] = 0.0;
		m_Color[i] = 0.0;
	}
	m_Normal = NULL;
	m_ID = 0;
	b_BoundaryPoint = false;
	b_Selected = false;
	b_TVPoint = false;
}


PCPoint::~PCPoint()
{
}
