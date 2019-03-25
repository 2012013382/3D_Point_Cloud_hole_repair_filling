#include "stdafx.h"
#include "PointCloudData.h"


PointCloudData::PointCloudData()
{
	m_PointSumNumber = 0;
	b_OneStepMode = false;
	b_AllPoint = false;
}


PointCloudData::~PointCloudData()
{
}

//获取点云数量
int PointCloudData::GetPointSumNumber()
{
	return m_PointSumNumber;
}

//设置点云数量
void PointCloudData::SetPointSumNumber(int number)
{
	m_PointSumNumber = number;
}

//获取点云数据
PCPoint* PointCloudData::GetPointCloud()
{
	return m_PointCloud;
}
//初始化点云数据
void PointCloudData::InitPointCloud(PCPoint* pointCloud)
{
	m_PointCloud = (PCPoint*)new PCPoint[m_PointSumNumber];//点云数据
	m_OriginPointCloud = (PCPoint*)new PCPoint[m_PointSumNumber];//单位化后的点云数据
	for (int num = 0; num < m_PointSumNumber; num++)
	{
		//初始化点云坐标和颜色
		for (int i = 0; i < 3; i++)
		{
			m_PointCloud[num].m_Coordinate[i] = pointCloud[num].m_Coordinate[i];
			m_PointCloud[num].m_Color[i] = pointCloud[num].m_Color[i];
		}
		m_PointCloud[num].m_ID = num;
	}
	PointMoveToOrigin(pointCloud);//将模型置于显示中心
	for (int num = 0; num < m_PointSumNumber; num++)
	{
		for (int j = 0; j < 3; j++)
		{
			m_OriginPointCloud[num].m_Coordinate[j] = pointCloud[num].m_Coordinate[j];
			m_OriginPointCloud[num].m_Color[j] = pointCloud[num].m_Color[j];
		}
		//初始化ID
		m_OriginPointCloud[num].m_ID = num;
	}
	//初始化PCL kd树
	cloud = pcl::PointCloud<pcl::PointXYZ>::Ptr(new pcl::PointCloud<pcl::PointXYZ>());
	cloud->width = m_PointSumNumber;//初始化 PCL 点云
	cloud->height = 1;
	cloud->points.resize(cloud->width * cloud->height);
	int cloudCount = 0;
	for (int num = 0; num < m_PointSumNumber; num++)
	{
			cloud->points[cloudCount].x = m_OriginPointCloud[num].m_Coordinate[0];
			cloud->points[cloudCount].y = m_OriginPointCloud[num].m_Coordinate[1];
			cloud->points[cloudCount].z = m_OriginPointCloud[num].m_Coordinate[2];
			cloudCount++;
	}
	kdtree.setInputCloud(cloud);
}
//获取移动到原点后的点云数据
PCPoint* PointCloudData::GetOriginPointCloud()
{
	return m_OriginPointCloud;
}
//从文件中读取点云数据
bool PointCloudData::ReadPointCloudFile(char* fileName)
{
	m_MaxX = -10000000.0;//给定它一个非常小的初始值
	m_MaxY = -10000000.0;//给定它一个非常小的初始值
	m_MaxZ = -10000000.0;//给定它一个非常小的初始值

	m_MinX = DBL_MAX;//给定它最大值
	m_MinY = DBL_MAX;//给定它最大值
	m_MinZ = DBL_MAX;//给定它最大值

	char str[256];
	int i = 0;
	FILE *fp;
	PCPoint sumPoint;
	PCPoint* tempPointData;//点云数据临时存储器
	//获取点云模型点数量
	if (fopen_s(&fp, fileName, "r") == 0)
	{
		while (fscanf_s(fp, "%s", str, 256) != EOF)
		{
			fscanf_s(fp, "%s", str, 256);
			fscanf_s(fp, "%s", str, 256);
			m_PointSumNumber++;
		}
		fclose(fp);
	}
	//初始化临时点云数据
	tempPointData = (PCPoint*) new PCPoint[m_PointSumNumber];
	sumPoint.m_Coordinate[0] = 0.0;
	sumPoint.m_Coordinate[1] = 0.0;
	sumPoint.m_Coordinate[2] = 0.0;

	if (fopen_s(&fp, fileName, "r") == 0)
	{
		while (fscanf_s(fp, "%s", str, 256) != EOF)
		{
			//将文件中读取数据点的信息存入数组中
			double v[3];
			int c[3];
			int index;

			v[2] = (double)atof(str);
			fscanf_s(fp, "%s", str, 256);
			v[0] = (double)atof(str);
			fscanf_s(fp, "%s", str, 256);
			v[1] = (double)atof(str);
			c[0] = 0.0; c[1] = 0.0; c[2] = 0.0;//设置初始颜色为黑色
			index = i++;
			for (int j = 0; j<3; j++)//把文件中的点存入数组RangeData中
			{
				tempPointData[index].m_Coordinate[j] = v[j];
				tempPointData[index].m_Color[j] = c[j];
				sumPoint.m_Coordinate[j] += v[j];
			}
			if (v[0] > m_MaxX)
			{
				m_MaxX = v[0];//得到最大的x坐标值
			}
			if (v[1] > m_MaxY)
			{
				m_MaxY = v[1];//得到最大的y坐标值
			}
			if (v[2] > m_MaxZ)
			{
				m_MaxZ = v[2];//得到最大的z坐标值
			}
			if (v[0] < m_MinX)
			{
				m_MinX = v[0];//得到最小的x坐标值
			}
			if (v[1] < m_MinY)
			{
				m_MinY = v[1];//得到最小的y坐标值
			}
			if (v[2] < m_MinZ)
			{
				m_MinZ = v[2];//得到最小的z坐标值
			}
		}
		fclose(fp);
		//获取模型的中心点
		m_MidPoint.m_Coordinate[0] = sumPoint.m_Coordinate[0] / m_PointSumNumber;
		m_MidPoint.m_Coordinate[1] = sumPoint.m_Coordinate[1] / m_PointSumNumber;
		m_MidPoint.m_Coordinate[2] = sumPoint.m_Coordinate[2] / m_PointSumNumber;

		InitPointCloud(tempPointData);//初始化点云数据
		delete[] tempPointData;//释放内存
		return 1;//文件打得开则返回1
	}
	else
	{
		return 0;//文件打不开则返回0
	}
}
void PointCloudData::PointMoveToOrigin(PCPoint* & pointCloud)//将模型置于显示中心
{
	m_MaxX -= m_MidPoint.m_Coordinate[0];
	for (int num = 0; num < m_PointSumNumber; num++)
	{
		pointCloud[num].m_Coordinate[0] -= m_MidPoint.m_Coordinate[0];
		pointCloud[num].m_Coordinate[1] -= m_MidPoint.m_Coordinate[1];
		pointCloud[num].m_Coordinate[2] -= m_MidPoint.m_Coordinate[2];
	
		pointCloud[num].m_Coordinate[0] = (pointCloud[num].m_Coordinate[0])
			/ (m_MaxX);
		pointCloud[num].m_Coordinate[1] = (pointCloud[num].m_Coordinate[1])
			/ (m_MaxX);
		pointCloud[num].m_Coordinate[2] = (pointCloud[num].m_Coordinate[2])
			/ (m_MaxX);
	} 
}
//带云参数的PCL kdtree k近邻搜索
void  PointCloudData::PCLCoefficientsKDtreeNKSearch(PCPoint lpoint, int Knum, pcl::PointCloud<pcl::PointXYZ>::Ptr coeCloud
	, pcl::KdTreeFLANN<pcl::PointXYZ> coeKdtree)
{
	pcl::PointXYZ searchPoint;//初始化 搜索点
	searchPoint.x = lpoint.m_Coordinate[0];
	searchPoint.y = lpoint.m_Coordinate[1];
	searchPoint.z = lpoint.m_Coordinate[2];
	std::vector<int> pointIdxNKNSearch(Knum);
	std::vector<float> pointNKNSquaredDistance(Knum);
	PCPoint Ptemp;
	if (coeKdtree.nearestKSearch(searchPoint, Knum, pointIdxNKNSearch, pointNKNSquaredDistance) > 0)
	{
		for (size_t i = 0; i < pointIdxNKNSearch.size(); ++i)
		{
			Ptemp.m_Coordinate[0] = coeCloud->points[pointIdxNKNSearch[i]].x;
			Ptemp.m_Coordinate[1] = coeCloud->points[pointIdxNKNSearch[i]].y;
			Ptemp.m_Coordinate[2] = coeCloud->points[pointIdxNKNSearch[i]].z;
			RkNearestPoints.push_back(Ptemp);
		}
	}
}
//PCL KDtree k近邻搜索
void PointCloudData::PCLKDtreeNKSearch(PCPoint lpoint, int Knum)
{
	pcl::PointXYZ searchPoint;//初始化 搜索点
	searchPoint.x = lpoint.m_Coordinate[0];
	searchPoint.y = lpoint.m_Coordinate[1];
	searchPoint.z = lpoint.m_Coordinate[2];
	std::vector<int> pointIdxNKNSearch(Knum);
	std::vector<float> pointNKNSquaredDistance(Knum);
	PCPoint Ptemp;
	if (kdtree.nearestKSearch(searchPoint, Knum, pointIdxNKNSearch, pointNKNSquaredDistance) > 0)
	{
		for (size_t i = 0; i < pointIdxNKNSearch.size(); ++i)
		{
			Ptemp.m_Coordinate[0] = cloud->points[pointIdxNKNSearch[i]].x;
			Ptemp.m_Coordinate[1] = cloud->points[pointIdxNKNSearch[i]].y;
			Ptemp.m_Coordinate[2] = cloud->points[pointIdxNKNSearch[i]].z;
			RkNearestPoints.push_back(Ptemp);
		}
	}
}
//边界检测
bool PointCloudData::BoundaryCheck(PCPoint lpoint)
{
	double array[BCNum][3], Y[3];
	double array_ML[BCNum][3];
	double A, B, C;
	A = B = C = 0.0;
	ZeroMemory(array, sizeof(array));
	ZeroMemory(Y, sizeof(Y));
	PCLKDtreeNKSearch(lpoint, BCNum);
	for (int i = 0; i < BCNum; i++)
	{
		array_ML[i][0] = array[i][0] = RkNearestPoints[i].m_Coordinate[0];
		array_ML[i][1] = array[i][1] = RkNearestPoints[i].m_Coordinate[1];
		array_ML[i][2] = array[i][2] = RkNearestPoints[i].m_Coordinate[2];
	}
	RkNearestPoints.clear();
	double *Matrix[3], *IMatrix[3];
	for (int i = 0; i < 3; i++)
	{
		Matrix[i] = new double[3];
		IMatrix[i] = new double[3];
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			*(Matrix[i] + j) = 0.0;//全部赋值为0.0
		}
	}
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < BCNum; i++)
		{
			*(Matrix[0] + j) += array[i][0] * array[i][j];
			*(Matrix[1] + j) += array[i][1] * array[i][j];
			*(Matrix[2] + j) += array[i][2] * array[i][j];
			Y[j] -= array[i][j];
		}
	}
	double d = Determinant(Matrix, 3);
	Inverse(Matrix, IMatrix, 3, d);
	for (int i = 0; i < 3; i++)
	{
		A += *(IMatrix[0] + i)*Y[i];
		B += *(IMatrix[1] + i)*Y[i];
		C += *(IMatrix[2] + i)*Y[i];
	}

	for (int i = 0; i < 3; i++)
	{
		delete[] Matrix[i];
		delete[] IMatrix[i];
	}
	for (int i = 0; i<BCNum; i++){
		double t = (A*array[i][0] + B*array[i][1] + C*array[i][2] + 1) / (A*A + B*B + C*C);
		array[i][0] = array[i][0] - A*t;
		array[i][1] = array[i][1] - B*t;
		array[i][2] = array[i][2] - C*t;
	}
	double t = (A*lpoint.m_Coordinate[0] + B*lpoint.m_Coordinate[1] + C*lpoint.m_Coordinate[2] + 1) / (A*A + B*B + C*C);
	lpoint.m_Coordinate[0] = lpoint.m_Coordinate[0] - A*t;
	lpoint.m_Coordinate[1] = lpoint.m_Coordinate[1] - B*t;
	lpoint.m_Coordinate[2] = lpoint.m_Coordinate[2] - C*t;

	PCPoint* pt3d = new PCPoint[BCNum];
	// 	Point* pt2d = new Point[10];
	PCPoint averagePoint; // average point.
	averagePoint.m_Coordinate[0] = 0;
	averagePoint.m_Coordinate[1] = 0;
	averagePoint.m_Coordinate[2] = 0;
	for (int i = 0; i < BCNum; i++)
	{
		pt3d[i].m_Coordinate[0] = array[i][0];
		pt3d[i].m_Coordinate[1] = array[i][1];
		pt3d[i].m_Coordinate[2] = array[i][2];
		averagePoint.m_Coordinate[0] += array[i][0];//get averagePoint
		averagePoint.m_Coordinate[1] += array[i][1];
		averagePoint.m_Coordinate[2] += array[i][2];
	}
	averagePoint.m_Coordinate[0] = averagePoint.m_Coordinate[0] / BCNum;
	averagePoint.m_Coordinate[1] = averagePoint.m_Coordinate[1] / BCNum;
	averagePoint.m_Coordinate[2] = averagePoint.m_Coordinate[2] / BCNum;

	int side1 = 0;
	int side2 = 0;
	PCPoint s;
	s.m_Coordinate[0] = averagePoint.m_Coordinate[0] - lpoint.m_Coordinate[0];
	s.m_Coordinate[1] = averagePoint.m_Coordinate[1] - lpoint.m_Coordinate[1];
	s.m_Coordinate[2] = averagePoint.m_Coordinate[2] - lpoint.m_Coordinate[2];
	for (int i = 0; i < BCNum; i++)
	{
		double t = s.m_Coordinate[0] * (pt3d[i].m_Coordinate[0] - lpoint.m_Coordinate[0])
			+ s.m_Coordinate[1] * (pt3d[i].m_Coordinate[1] - lpoint.m_Coordinate[1])
			+ s.m_Coordinate[2] * (pt3d[i].m_Coordinate[2] - lpoint.m_Coordinate[2]);
		if (t > 0)
		{
			side1++;
		}
		else
		{
			side2++;
		}
	}
	if (abs(((double)side1 - (double)side2) / BCNum) > Threadshold)
	{
		return true;
	}
	return false;
}
//进行孔洞修补
bool PointCloudData::HoleRepair()
{
	//先清空数组
	vector<PCPoint> m_SelectedPoint;//被选中的约束点
	vector<PCPoint> newSelectedPoint;//不含附加约束点
	vector<PCPoint> m_SelectedBoundaryPoint;//被选中的边界点
	
	//新的修补点集合
	vector<PCPoint> repairedPoint;
	//获取选中的边界点集合
	for (int num = 0; num < m_PointSumNumber; num++)
	{
		if (m_OriginPointCloud[num].b_Selected &&
			m_OriginPointCloud[num].b_BoundaryPoint)
		{
			m_SelectedBoundaryPoint.push_back(m_OriginPointCloud[num]);
		}
	}
	//将选中的边界点向周围进行扩充，扩充的点即为约束点，每个点向其周围扩充
	//新的knum个点
	int knum = 15;
	for (int num = 0; num < m_SelectedBoundaryPoint.size(); num++)
	{
		PCLKDtreeNKSearch(m_SelectedBoundaryPoint[num], knum);
		for (int i = 1; i < knum; i++)
		{
			if (NotIn(RkNearestPoints[i], m_SelectedBoundaryPoint,m_SelectedPoint))
			{
				m_SelectedPoint.push_back(RkNearestPoints[i]);
				newSelectedPoint.push_back(RkNearestPoints[i]);
			}
		}
		RkNearestPoints.clear();
	}

	gProgress = 10;//用于显示进度条
	//获取隐式曲面方程的系数
	double* coefficients = GetRBFCoefficients(m_SelectedPoint);
	//test begin
	for (int i = 0; i < m_SelectedPoint.size(); i++)
	{
		m_TestSeletedPointCloud.push_back(m_SelectedPoint[i]);
	}
	//test end
	gProgress = 30;//用于显示进度条
	//插入新的孔洞修补点，病对修补点进行位置的调整
	bool repairingResult = InsertRepairPoint(coefficients, repairedPoint, m_SelectedBoundaryPoint, m_SelectedPoint);
	gProgress = 90;//用于显示进度条

	//将所有的数据重置到初始的未选中状态	
	for (int num = 0; num < m_PointSumNumber; num++)
	{
		m_OriginPointCloud[num].b_Selected = false;
	}
	gProgress = 95;//用于显示进度条

	//进行张量投票特征点检测
	//TensorVoting(repairedPoint);
	//向点云中插入新的修补点
	for (int num = 0; num < repairedPoint.size(); num++)
	{
		/*if (num % 2 == 0 && repairedPoint[num].b_TVPoint == false && num != 0)
		{
			continue;
		}*/
		m_AddedPoingCLoud.push_back(repairedPoint[num]);
	}
	gProgress = -1;//销毁进度条
	if (repairingResult)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//是否在选中的边界集合中
bool PointCloudData::NotIn(PCPoint lpoint, vector<PCPoint> m_SelectedBoundaryPoint, vector<PCPoint> m_SelectedPoint)
{
	for (int num = 0; num < m_SelectedBoundaryPoint.size(); num++)
	{
		if (fabs(lpoint.m_Coordinate[0] - m_SelectedBoundaryPoint[num].m_Coordinate[0]) < 0.0001
			&& fabs(lpoint.m_Coordinate[1] - m_SelectedBoundaryPoint[num].m_Coordinate[1]) < 0.0001
			&& fabs(lpoint.m_Coordinate[2] - m_SelectedBoundaryPoint[num].m_Coordinate[2]) < 0.0001)
		{
			return false;
		}
	}
	for (int num = 0; num < m_SelectedPoint.size(); num++)
	{
		if (fabs(lpoint.m_Coordinate[0] - m_SelectedPoint[num].m_Coordinate[0]) < 0.0001
			&& fabs(lpoint.m_Coordinate[1] - m_SelectedPoint[num].m_Coordinate[1]) < 0.0001
			&& fabs(lpoint.m_Coordinate[2] - m_SelectedPoint[num].m_Coordinate[2]) < 0.0001)
		{
			return false;
		}
	}
	return true;
}
//获取径向基函数的系数
double*  PointCloudData::GetRBFCoefficients(vector<PCPoint>& m_SelectedPoint)
{
	//定义附加约束点
	int RBFNum = m_SelectedPoint.size();
	PCPoint* additionalPoint = new PCPoint[RBFNum];
	//利用选择的约束点获取附加约束点
	for (int num = 0; num < m_SelectedPoint.size(); num++)
	{
		//获取每个数据点的法矢量值及坐标值
		additionalPoint[num].m_Normal = GetNormalVector(m_SelectedPoint[num]);
		additionalPoint[num].m_Coordinate[0] = m_SelectedPoint[num].m_Coordinate[0];
		additionalPoint[num].m_Coordinate[1] = m_SelectedPoint[num].m_Coordinate[1];
		additionalPoint[num].m_Coordinate[2] = m_SelectedPoint[num].m_Coordinate[2];
	}

	//进行法矢量的方向矫正
	RectifyNormals(additionalPoint, RBFNum);

	for (int num = 0; num < RBFNum; num++)
	{
		//获取附加约束点的值
		GetAdditionalPoint(&additionalPoint[num]);
		//为了方便直接将得到的附加约束点加入约束点集合中
		m_SelectedPoint.push_back(additionalPoint[num]);
	}
	//RBF矩阵的大小
	int newRBFNum = 2 * RBFNum;
	//初始化RBF元素值
	double** RBFValue = new double*[newRBFNum];
	for (int num = 0; num < newRBFNum; num++)
	{
		RBFValue[num] = new double[newRBFNum];
	}
	for (int i = 0; i < newRBFNum; i++)
	{
		for (int j = 0; j < newRBFNum; j++)
		{
			RBFValue[i][j] = GetRBFValue(GetTwoPointsDistance(m_SelectedPoint[i],m_SelectedPoint[j]));
		}
	}
	//初始化RBF矩阵
	double** RBFMatrix = new double*[newRBFNum + 4];
	for (int num = 0; num < newRBFNum + 4; num++)
	{
		RBFMatrix[num] = new double[newRBFNum + 5];
	}
	for (int i = 0; i < newRBFNum; i++)
	{
		for (int j = 0; j < newRBFNum; j++)
		{
			RBFMatrix[i][j] = RBFValue[i][j];
		}
	}
	for (int num = 0; num < newRBFNum; num++)
	{
		RBFMatrix[num][newRBFNum] = 1;
		RBFMatrix[num][newRBFNum + 1] = m_SelectedPoint[num].m_Coordinate[0];
		RBFMatrix[num][newRBFNum + 2] = m_SelectedPoint[num].m_Coordinate[1];
		RBFMatrix[num][newRBFNum + 3] = m_SelectedPoint[num].m_Coordinate[2];
	}
	for (int num = 0; num < newRBFNum; num++)
	{
		RBFMatrix[newRBFNum][num] = 1;
		RBFMatrix[newRBFNum + 1][num] = m_SelectedPoint[num].m_Coordinate[0];
		RBFMatrix[newRBFNum + 2][num] = m_SelectedPoint[num].m_Coordinate[1];
		RBFMatrix[newRBFNum + 3][num] = m_SelectedPoint[num].m_Coordinate[2];
	}
	for (int i = newRBFNum; i < newRBFNum + 4; i++)
	{
		for (int j = newRBFNum; j < newRBFNum + 4; j++)
		{
			RBFMatrix[i][j] = 0;
		}
	}
	for (int num = 0; num < RBFNum; num++)
	{
		RBFMatrix[num][newRBFNum + 4] = 0;
	}
	for (int num = RBFNum; num < newRBFNum; num++)
	{
		RBFMatrix[num][newRBFNum + 4] = 1;
	}
	for (int num = newRBFNum; num < newRBFNum + 4; num++)
	{
		RBFMatrix[num][newRBFNum + 4] = 0;
	}
	double* coefficients = new double[newRBFNum + 4];
	//利用列主元高斯消去法对矩阵进行计算，得到隐式曲面方程的系数
	colunmPrincipleGauss(newRBFNum + 4, RBFMatrix);
	//获取隐式曲面方程的系数
	for (int num = 0; num < newRBFNum + 4; num++)
	{
		coefficients[num] = RBFMatrix[num][newRBFNum + 4];
	}
	return coefficients;
}
//法向矫正
void PointCloudData::RectifyNormals(PCPoint* additionalPoint, int RBFNum)
{
	for (int num = 0; num < RBFNum; num++)
	{
		if (additionalPoint[0].m_Normal[0] * additionalPoint[num].m_Normal[0]
			+ additionalPoint[0].m_Normal[1] * additionalPoint[num].m_Normal[1]
			+ additionalPoint[0].m_Normal[2] * additionalPoint[num].m_Normal[2]
			< 0)
		{
			additionalPoint[num].m_Normal[0] = -additionalPoint[num].m_Normal[0];
			additionalPoint[num].m_Normal[1] = -additionalPoint[num].m_Normal[1];
			additionalPoint[num].m_Normal[2] = -additionalPoint[num].m_Normal[2];
		}
	}
	for (int num = 0; num < RBFNum; num++)
	{
		int knum = 10;
		PCLKDtreeNKSearch(additionalPoint[num],knum);
		for (int i = 1; i < knum; i++)
		{
			for (int j = 0; j < RBFNum; j++)
			{
				if (fabs(RkNearestPoints[i].m_Coordinate[0] - additionalPoint[j].m_Coordinate[0]) < 0.0001
					&& fabs(RkNearestPoints[i].m_Coordinate[1] - additionalPoint[j].m_Coordinate[1]) < 0.0001
					&& fabs(RkNearestPoints[i].m_Coordinate[2] - additionalPoint[j].m_Coordinate[2]) < 0.0001)
				{//找到该近邻点
					if (additionalPoint[num].m_Normal[0] * additionalPoint[j].m_Normal[0]
						+ additionalPoint[num].m_Normal[1] * additionalPoint[j].m_Normal[1]
						+ additionalPoint[num].m_Normal[2] * additionalPoint[j].m_Normal[2]
						< 0)
					{//如果两法矢量之间的夹角大于90度，则改变法矢量的方向
						additionalPoint[j].m_Normal[0] = -additionalPoint[j].m_Normal[0];
						additionalPoint[j].m_Normal[1] = -additionalPoint[j].m_Normal[1];
						additionalPoint[j].m_Normal[2] = -additionalPoint[j].m_Normal[2];
					}
					break;
				}
			}
		}
		RkNearestPoints.clear();
	}
}
//获取数据点的法矢量
double* PointCloudData::GetNormalVector(PCPoint lpoint)
{
	/*double* normalVector = new double[3];//定义面法向量
	//以下法向量求法由行列式推理得到
	//lpoint（x1,y1,z1）
	//nearest(x2,y2,z2)
	//nearest2(x3,y3,z3)
	//o(0,0,0)，a(a1,a2,a3)，b(b1,b2,b3)
	//x=a2b3-a3b2,y=a3b1-a1b3,z=a1b2-a2b1
	PCLKDtreeNKSearch(lpoint,10);
	double a1, a2, a3, b1, b2, b3;
	a1 = RkNearestPoints[3].m_Coordinate[0] - lpoint.m_Coordinate[0];
	a2 = RkNearestPoints[3].m_Coordinate[1] - lpoint.m_Coordinate[1];
	a3 = RkNearestPoints[3].m_Coordinate[2] - lpoint.m_Coordinate[2];

	b1 = RkNearestPoints[5].m_Coordinate[0] - lpoint.m_Coordinate[0];
	b2 = RkNearestPoints[5].m_Coordinate[1] - lpoint.m_Coordinate[1];
	b3 = RkNearestPoints[5].m_Coordinate[2] - lpoint.m_Coordinate[2];
	normalVector[0] = a2 * b3 - a3 * b2;
	normalVector[1] = a3 * b1 - a1 * b3;
	normalVector[2] = a1 * b2 - a2 * b1;
	RkNearestPoints.clear();
	return normalVector;*/
	//利用数据点拟合其最小二乘平面，用最小二乘平面的法矢量代替点的法矢量
	double* normalVector = new double[3];
	double array[NORMAL_VECTOR_NUM][3], Y[3];
	double array_ML[NORMAL_VECTOR_NUM][3];
	double A, B, C;
	A = B = C = 0.0;
	ZeroMemory(array, sizeof(array));
	ZeroMemory(Y, sizeof(Y));
	PCLKDtreeNKSearch(lpoint, NORMAL_VECTOR_NUM);
	for (int i = 0; i < NORMAL_VECTOR_NUM; i++)
	{
		array_ML[i][0] = array[i][0] = RkNearestPoints[i].m_Coordinate[0];
		array_ML[i][1] = array[i][1] = RkNearestPoints[i].m_Coordinate[1];
		array_ML[i][2] = array[i][2] = RkNearestPoints[i].m_Coordinate[2];
	}
	RkNearestPoints.clear();
	double *Matrix[3], *IMatrix[3];
	for (int i = 0; i < 3; i++)
	{
		Matrix[i] = new double[3];
		IMatrix[i] = new double[3];
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			*(Matrix[i] + j) = 0.0;//全部赋值为0.0
		}
	}
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < NORMAL_VECTOR_NUM; i++)
		{
			*(Matrix[0] + j) += array[i][0] * array[i][j];
			*(Matrix[1] + j) += array[i][1] * array[i][j];
			*(Matrix[2] + j) += array[i][2] * array[i][j];
			Y[j] -= array[i][j];
		}
	}
	double d = Determinant(Matrix, 3);
	Inverse(Matrix, IMatrix, 3, d);
	for (int i = 0; i < 3; i++)
	{
		A += *(IMatrix[0] + i)*Y[i];
		B += *(IMatrix[1] + i)*Y[i];
		C += *(IMatrix[2] + i)*Y[i];
	}

	normalVector[0] = A;
	normalVector[1] = B;
	normalVector[2] = C;
	return normalVector;
}
//求得张量投票算法的特征点法矢量
double* PointCloudData::GetTVNormalVector(PCPoint lpoint, pcl::PointCloud<pcl::PointXYZ>::Ptr coeCloud
	, pcl::KdTreeFLANN<pcl::PointXYZ> coeKdtree)
{
	double* normalVector = new double[3];
	double array[NORMAL_VECTOR_NUM][3], Y[3];
	double array_ML[NORMAL_VECTOR_NUM][3];
	double A, B, C;
	A = B = C = 0.0;
	ZeroMemory(array, sizeof(array));
	ZeroMemory(Y, sizeof(Y));
	PCLCoefficientsKDtreeNKSearch(lpoint, NORMAL_VECTOR_NUM, coeCloud, coeKdtree);
	for (int i = 0; i < NORMAL_VECTOR_NUM; i++)
	{
		array_ML[i][0] = array[i][0] = RkNearestPoints[i].m_Coordinate[0];
		array_ML[i][1] = array[i][1] = RkNearestPoints[i].m_Coordinate[1];
		array_ML[i][2] = array[i][2] = RkNearestPoints[i].m_Coordinate[2];
	}
	RkNearestPoints.clear();
	double *Matrix[3], *IMatrix[3];
	for (int i = 0; i < 3; i++)
	{
		Matrix[i] = new double[3];
		IMatrix[i] = new double[3];
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			*(Matrix[i] + j) = 0.0;//全部赋值为0.0
		}
	}
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < NORMAL_VECTOR_NUM; i++)
		{
			*(Matrix[0] + j) += array[i][0] * array[i][j];
			*(Matrix[1] + j) += array[i][1] * array[i][j];
			*(Matrix[2] + j) += array[i][2] * array[i][j];
			Y[j] -= array[i][j];
		}
	}
	double d = Determinant(Matrix, 3);
	Inverse(Matrix, IMatrix, 3, d);
	for (int i = 0; i < 3; i++)
	{
		A += *(IMatrix[0] + i)*Y[i];
		B += *(IMatrix[1] + i)*Y[i];
		C += *(IMatrix[2] + i)*Y[i];
	}

	normalVector[0] = A;
	normalVector[1] = B;
	normalVector[2] = C;
	return normalVector;
}
//获取附加约束点的值
void PointCloudData::GetAdditionalPoint(PCPoint* lpoint)
{
	double l = 0.1;
	double pNormal[3];
	double temp[3];
	for (int num = 0; num < 3; num++)
	{
		pNormal[num] = lpoint->m_Normal[num];
	}
	temp[0] = l * pNormal[0] / sqrt(pNormal[0] * pNormal[0]
		+ pNormal[1] * pNormal[1] + pNormal[2] * pNormal[2]) + lpoint->m_Coordinate[0];

	temp[1] = l * pNormal[1] / sqrt(pNormal[0] * pNormal[0]
		+ pNormal[1] * pNormal[1] + pNormal[2] * pNormal[2]) + lpoint->m_Coordinate[1];

	temp[2] = l * pNormal[2] / sqrt(pNormal[0] * pNormal[0]
		+ pNormal[1] * pNormal[1] + pNormal[2] * pNormal[2]) + lpoint->m_Coordinate[2];

	lpoint->m_Coordinate[0] = temp[0];
	lpoint->m_Coordinate[1] = temp[1];
	lpoint->m_Coordinate[2] = temp[2];
}
//获取两个数据点之间的距离
double PointCloudData::GetTwoPointsDistance(PCPoint p, PCPoint q)
{
	double result = sqrt((p.m_Coordinate[0] - q.m_Coordinate[0]) * (p.m_Coordinate[0] - q.m_Coordinate[0])
		+ (p.m_Coordinate[1] - q.m_Coordinate[1]) * (p.m_Coordinate[1] - q.m_Coordinate[1])
		+ (p.m_Coordinate[2] - q.m_Coordinate[2]) * (p.m_Coordinate[2] - q.m_Coordinate[2]));
	return result;
}
//插入新的修补点
bool PointCloudData::InsertRepairPoint(double* coefficients, vector<PCPoint> & repairedPoint, vector<PCPoint> m_SelectedBoundaryPoint, vector<PCPoint> m_SelectedPoint)
{
	PCPoint midPoint;//备选约束点的中心点，不包括附加约束点
	for (int num = 0; num < m_SelectedBoundaryPoint.size(); num++)
	{
		midPoint.m_Coordinate[0] += m_SelectedBoundaryPoint[num].m_Coordinate[0];
		midPoint.m_Coordinate[1] += m_SelectedBoundaryPoint[num].m_Coordinate[1];
		midPoint.m_Coordinate[2] += m_SelectedBoundaryPoint[num].m_Coordinate[2];
	}
	//获得中心点
	midPoint.m_Coordinate[0] /= (m_SelectedBoundaryPoint.size());
	midPoint.m_Coordinate[1] /= (m_SelectedBoundaryPoint.size());
	midPoint.m_Coordinate[2] /= (m_SelectedBoundaryPoint.size());
	
	//获取随机两点之间的距离
	PCLKDtreeNKSearch(m_SelectedBoundaryPoint[0], 2);
	double dp =  GetTwoPointsDistance(m_SelectedBoundaryPoint[0], RkNearestPoints[1]);
	RkNearestPoints.clear();
	//中点
	repairedPoint.push_back(midPoint);
	//依据隐式曲面方程调整孔洞修补点的位置
	RepairedPointDisjust(coefficients, repairedPoint[repairedPoint.size() - 1], m_SelectedPoint);
	//进行插点操作
	for (int num = 0; num < m_SelectedBoundaryPoint.size(); num++)
	{
		if (gProgress > 0)
		{
			gProgress = 30 + 60 * ((double)num / (double)m_SelectedBoundaryPoint.size());//用于显示进度条
		}
		if (true)
		{
			//获取两点之间的距离
			double distance = GetTwoPointsDistance(midPoint, m_SelectedBoundaryPoint[num]);
			//获取插入点的个数
			int pointNum = distance / dp;

			for (int i = 1; i <= pointNum; i++)
			{
				double l = ((double)i / (double)pointNum) * distance;
				PCPoint tempPoint = GetInsertPoint(midPoint, m_SelectedBoundaryPoint[num], l);
				repairedPoint.push_back(tempPoint);
				if (i != 1 && GetTwoPointsDistance(repairedPoint[repairedPoint.size() - 1], repairedPoint[repairedPoint.size() - 2]) > 4.5 * dp)
				{
				    //表示该孔洞不能用径向基函数拟合，因此不能自动修补
					repairedPoint.clear();
				    return false;
				}
				else
				{
					//依据隐式曲面方程调整孔洞修补点的位置
					RepairedPointDisjust(coefficients, repairedPoint[repairedPoint.size() - 1], m_SelectedPoint);
				}
			}
		}
	}
	return true;
}
//获取新插入的修补点
PCPoint PointCloudData::GetInsertPoint(PCPoint p, PCPoint q, double l)
{
	//获取两点的方向向量
	double normal[3];
	normal[0] = q.m_Coordinate[0] - p.m_Coordinate[0];
	normal[1] = q.m_Coordinate[1] - p.m_Coordinate[1];
	normal[2] = q.m_Coordinate[2] - p.m_Coordinate[2];
	//新插入的修补点
	PCPoint repairedPoint;
	repairedPoint.m_Coordinate[0] = l * normal[0] / sqrt(normal[0] * normal[0]
		+ normal[1] * normal[1] + normal[2] * normal[2]) + p.m_Coordinate[0];

	repairedPoint.m_Coordinate[1] = l * normal[1] / sqrt(normal[0] * normal[0]
		+ normal[1] * normal[1] + normal[2] * normal[2]) + p.m_Coordinate[1];

	repairedPoint.m_Coordinate[2] = l * normal[2] / sqrt(normal[0] * normal[0]
		+ normal[1] * normal[1] + normal[2] * normal[2]) + p.m_Coordinate[2];
	//设为
	repairedPoint.m_Color[0] = 1.0;
	repairedPoint.m_Color[1] = 0.0;
	repairedPoint.m_Color[2] = 1.0;
	return repairedPoint;
}

//获取修补点集合
vector<PCPoint>* PointCloudData::GetNewAddedPoint()
{
	return &m_AddedPoingCLoud;
}
/*
//获取约束点集合
vector<PCPoint>* PointCloudData::GetSelectedPoint()
{
	return &m_SelectedPoint;
}*/
//调整孔洞修补点
void PointCloudData::RepairedPointDisjust(double* coefficients, PCPoint& repairedPoint, vector<PCPoint> m_SelectedPoint)
{

		double lastFr = DBL_MAX;//设为最大的double值
		while (true)
		{
			double frPoint[3];
			double frX = 0;
			double frY = 0;
			double frZ = 0;
			for (int i = 0; i < m_SelectedPoint.size(); i++)
			{
				frX += coefficients[i] * (repairedPoint.m_Coordinate[0] - m_SelectedPoint[i].m_Coordinate[0])
					* GetTwoPointsDistance(repairedPoint, m_SelectedPoint[i]);

				frY += coefficients[i] * (repairedPoint.m_Coordinate[1] - m_SelectedPoint[i].m_Coordinate[1])
					* GetTwoPointsDistance(repairedPoint, m_SelectedPoint[i]);

				frZ += coefficients[i] * (repairedPoint.m_Coordinate[2] - m_SelectedPoint[i].m_Coordinate[2])
					* GetTwoPointsDistance(repairedPoint, m_SelectedPoint[i]);
			}
			frX = 3 * frX + coefficients[m_SelectedPoint.size() + 1];
			frY = 3 * frY + coefficients[m_SelectedPoint.size() + 2];
			frZ = 3 * frZ + coefficients[m_SelectedPoint.size() + 3];
			frPoint[0] = frX;
			frPoint[1] = frY;
			frPoint[2] = frZ;

			double Fr = 0;
			for (int i = 0; i < m_SelectedPoint.size(); i++)
			{
				Fr += coefficients[i] * GetRBFValue(GetTwoPointsDistance(repairedPoint, m_SelectedPoint[i]));
			}
			Fr = Fr + coefficients[m_SelectedPoint.size()] + repairedPoint.m_Coordinate[0] * coefficients[m_SelectedPoint.size() + 1]
				+ repairedPoint.m_Coordinate[1] * coefficients[m_SelectedPoint.size() + 2]
				+ repairedPoint.m_Coordinate[2] * coefficients[m_SelectedPoint.size() + 3];
			double tempFr = Fr / (frPoint[0] * frPoint[0]
				+ frPoint[1] * frPoint[1] + frPoint[2] * frPoint[2]);
			if (fabs(Fr) < fabs(lastFr))
			{
				lastFr = Fr;
				repairedPoint.m_Coordinate[0] = repairedPoint.m_Coordinate[0] - tempFr * frPoint[0];
				repairedPoint.m_Coordinate[1] = repairedPoint.m_Coordinate[1] - tempFr * frPoint[1];
				repairedPoint.m_Coordinate[2] = repairedPoint.m_Coordinate[2] - tempFr * frPoint[2];
			}
			else
			{
				break;
			}
		}
}
//进行边界聚类
void PointCloudData::BoundaryClustering()
{
	//获取选中的边界点集合
	vector<PCPoint> m_SelectedBoundaryPoint;
	for (int num = 0; num < m_PointSumNumber; num++)
	{
		if (m_OriginPointCloud[num].b_Selected &&
			m_OriginPointCloud[num].b_BoundaryPoint)
		{
			m_SelectedBoundaryPoint.push_back(m_OriginPointCloud[num]);
		}
	}
	//利用PCL中的聚类函数对得到的边界点进行聚类操作
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr boundarycloud(new pcl::PointCloud<pcl::PointXYZ>);
	for (int num = 0; num < m_PointSumNumber; num++)
	{//获取边界点集合
		if (m_OriginPointCloud[num].b_BoundaryPoint)
		{
			m_SelectedBoundaryPoint.push_back(m_OriginPointCloud[num]);
		}
		m_OriginPointCloud[num].m_Color[0] = 0.0;//全部设为蓝色
		m_OriginPointCloud[num].m_Color[1] = 0.0;//全部设为蓝色
		m_OriginPointCloud[num].m_Color[2] = 1.0;//全部设为蓝色
	}
	boundarycloud->width = m_SelectedBoundaryPoint.size();//初始化 PCL 点云
	boundarycloud->height = 1;
	boundarycloud->points.resize(boundarycloud->width * boundarycloud->height);
	//初始化点云
	int cloudCount = 0;
	for (int num = 0; num < m_SelectedBoundaryPoint.size(); num++)
	{
		boundarycloud->points[cloudCount].x = m_SelectedBoundaryPoint[num].m_Coordinate[0];
		boundarycloud->points[cloudCount].y = m_SelectedBoundaryPoint[num].m_Coordinate[1];
		boundarycloud->points[cloudCount].z = m_SelectedBoundaryPoint[num].m_Coordinate[2];
		cloudCount++;
	}
	tree->setInputCloud(boundarycloud);
	//初始化聚类参数并进行聚类
	vector<pcl::PointIndices> cluster_indices;
	pcl::EuclideanClusterExtraction<pcl::PointXYZ> ec;
	ec.setClusterTolerance(m_clusterDistace);
	ec.setMinClusterSize(m_clusterNum);
	ec.setMaxClusterSize(5000);
	ec.setSearchMethod(tree);
	ec.setInputCloud(boundarycloud);
	ec.extract(cluster_indices);
	//提取聚类完成的各个点云集合
	
	m_CloudClusterNum = cluster_indices.size();
	pcl::PointCloud<pcl::PointXYZ>::Ptr* cloud_cluster; //(new pcl::PointCloud<pcl::PointXYZ>);
	cloud_cluster = new pcl::PointCloud<pcl::PointXYZ>::Ptr[m_CloudClusterNum];
	for (int num = 0; num < m_CloudClusterNum; num++)
	{
		cloud_cluster[num] = pcl::PointCloud<pcl::PointXYZ>::Ptr(new pcl::PointCloud<pcl::PointXYZ>);
	}
	int i = 0;
	for (vector<pcl::PointIndices>::const_iterator it = cluster_indices.begin();
		it != cluster_indices.end(); i++, it++)
	{
		for (vector<int>::const_iterator pit = it->indices.begin();
			pit != it->indices.end(); pit++)
		{
			cloud_cluster[i]->points.push_back(boundarycloud->points[*pit]);
		}
		cloud_cluster[i]->width = cloud_cluster[i]->points.size();
		cloud_cluster[i]->height = 1;
		cloud_cluster[i]->is_dense = true;
	}
	//将得到的聚类划分好的点云集合存储起来
	m_BoundaryPointClusters = new vector<PCPoint>[m_CloudClusterNum];
	for (int i = 0; i < m_CloudClusterNum; i++)
	{
		for (int num = 0; num < cloud_cluster[i]->points.size(); num++)
		{
			PCPoint tempPoint;
			tempPoint.m_Color[0] = ((double)i / double(m_CloudClusterNum)) * 1.0;
			tempPoint.m_Color[1] = 1.0 - ((double)i / double(m_CloudClusterNum)) * 1.0;
			tempPoint.m_Color[2] = 0.5;
			tempPoint.m_Coordinate[0] = cloud_cluster[i]->points[num].x;
			tempPoint.m_Coordinate[1] = cloud_cluster[i]->points[num].y;
			tempPoint.m_Coordinate[2] = cloud_cluster[i]->points[num].z;
			m_BoundaryPointClusters[i].push_back(tempPoint);
		}
	}
}

//获取边界的聚类数量
int PointCloudData::GetCloudClusterNum()
{
	return m_CloudClusterNum;
}
//获取边界聚类集合
vector<PCPoint>* PointCloudData::GetBoundaryPointClusters()
{
	return m_BoundaryPointClusters;
}
//自动进行孔洞修补
void  PointCloudData::AutomaticHoleRepair()
{
	m_AddedPoingCLoud.clear();
    //对聚类出的孔洞边界集合分别进行孔洞修补的工作
	//先判断出不进行修补的孔洞
	//存储判断距离
	double distance = m_MaxX - m_MinX;
	if (distance < m_MaxY - m_MinY)
	{
		distance = m_MaxY - m_MinY;
	}
	if (distance < m_MaxZ - m_MinZ)
	{
		distance = m_MaxZ - m_MinZ;
	}
	//存储不进行修补的孔洞序号
	vector<int> NotRepair;
	for (int num = 0; num < m_CloudClusterNum; num++)
	{
		//先将其设为0
		double maxDistance = 0.0;
		for (int i = 0; i < m_BoundaryPointClusters[num].size(); i++)
		{//获取最大距离
			double tempDistance = GetTwoPointsDistance(m_BoundaryPointClusters[num][0], m_BoundaryPointClusters[num][i]);
			if (tempDistance > maxDistance)
			{
				maxDistance = tempDistance;
			}
		}
		if (maxDistance > distance / 2)
		{//如果大于该距离则不进行修补
			NotRepair.push_back(num);
		}
	}
	if (b_OneStepMode)
	{
		gProgressAu = 10;//用于显示进度条
	}
	else
	{
		gProgressAu = 5;//用于显示进度条
	}

	//用于存储每个孔洞的约束点集合
	vector<PCPoint> selectedPoint;
	//新的修补点集合
	vector<PCPoint> repairedPoint;
	//用于判断是否修补
	bool flag = false;
	for (int num = 0; num < m_CloudClusterNum; num++)
	{
		if (b_OneStepMode)
		{
			gProgressAu = 10 + 20 * ((double)num / (double)m_CloudClusterNum);//用于显示进度条
		}
		else
		{
			gProgressAu = 5 + 45 * ((double)num / (double)m_CloudClusterNum);//用于显示进度条
		}
		//先判断是否进行修补
		for (int i = 0; i < NotRepair.size(); i++)
		{
			if (NotRepair[i] == num)
			{
				flag = true;
				break;
			}
		}
		if (flag)
		{//该孔洞不被修补，进入下一个孔洞集合
			flag = false;
			continue;
		}
		//清空selected,repair
		selectedPoint.clear();
		repairedPoint.clear();
		//将选中的边界点向周围进行扩充，扩充的点即为约束点，每个点向其周围扩充
		//新的knum个点
		int knum = 15;
		for (int i = 0; i < m_BoundaryPointClusters[num].size(); i++)
		{
			PCLKDtreeNKSearch(m_BoundaryPointClusters[num][i], knum);
			for (int j = 1; j < knum; j++)
			{
				if (NotIn(RkNearestPoints[j], m_BoundaryPointClusters[num], selectedPoint))
				{
					selectedPoint.push_back(RkNearestPoints[j]);
				}
			}
			RkNearestPoints.clear();
		}
		//获取隐式曲面方程的系数
		double* coefficients = GetRBFCoefficients(selectedPoint);
		//插入新的孔洞修补点，病对修补点进行位置的调整
		bool repairingResult = InsertRepairPoint(coefficients, repairedPoint, m_BoundaryPointClusters[num], selectedPoint);
		//将所有的数据重置到初始的未选中状态	
		for (int num = 0; num < m_PointSumNumber; num++)
		{
			m_OriginPointCloud[num].b_Selected = false;
		}
		//向点云中插入新的修补点
		for (int num = 0; num < repairedPoint.size(); num++)
		{
			if (b_AllPoint)
			{
				if (num % 2 == 0 && repairedPoint[num].b_TVPoint == false && num != 0)
				{
					continue;
				}
			}
			m_AddedPoingCLoud.push_back(repairedPoint[num]);
		}
	}
}

//进行点云三角化
void PointCloudData::SurfaceGenerating()
{
	//先获取所有的需要三角化的点云
	vector<PCPoint> pointSurfaceCloud;
	//初始化显示用的点云
	m_AddedRepairingPointCloud = new PCPoint[m_PointSumNumber + m_AddedPoingCLoud.size()];

	for (int num = 0; num < m_PointSumNumber; num++)
	{
		if (b_OneStepMode)
		{
			gProgressAu = 30 + 5 * ((double)num / (double)m_PointSumNumber);//用于显示进度条
		}
		else
		{
			gProgressAu = 10 * (double)num / (double)m_PointSumNumber;//显示进度条
		}
		pointSurfaceCloud.push_back(m_OriginPointCloud[num]);
		m_AddedRepairingPointCloud[num].m_Coordinate[0] = m_OriginPointCloud[num].m_Coordinate[0];
		m_AddedRepairingPointCloud[num].m_Coordinate[1] = m_OriginPointCloud[num].m_Coordinate[1];
		m_AddedRepairingPointCloud[num].m_Coordinate[2] = m_OriginPointCloud[num].m_Coordinate[2];
	}

	for (int num = 0; num < m_AddedPoingCLoud.size(); num++)
	{
		pointSurfaceCloud.push_back(m_AddedPoingCLoud[num]);
		m_AddedRepairingPointCloud[num + m_PointSumNumber].m_Coordinate[0] = m_AddedPoingCLoud[num].m_Coordinate[0];
		m_AddedRepairingPointCloud[num + m_PointSumNumber].m_Coordinate[1] = m_AddedPoingCLoud[num].m_Coordinate[1];
		m_AddedRepairingPointCloud[num + m_PointSumNumber].m_Coordinate[2] = m_AddedPoingCLoud[num].m_Coordinate[2];
	}
	//初始化点云
	pcl::PointCloud<pcl::PointXYZ>::Ptr surfaceCloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> n;//计算法矢量
	//存储估计的法矢量
	pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);
	//定义kdtree
	pcl::search::KdTree<pcl::PointXYZ>::Ptr surfaceTree(new pcl::search::KdTree<pcl::PointXYZ>);
	//初始化 PCL 点云
	surfaceCloud->width = m_PointSumNumber + m_AddedPoingCLoud.size();
	surfaceCloud->height = 1;
	surfaceCloud->points.resize(surfaceCloud->width * surfaceCloud->height);
	int cloudCount = 0;
	for (int num = 0; num <m_PointSumNumber + m_AddedPoingCLoud.size(); num++)
	{
		if (b_OneStepMode)
		{
			gProgressAu = 35 + 5 * ((double)num / (double)(m_PointSumNumber + m_AddedPoingCLoud.size()));//用于显示进度条
		}
		else
		{
			gProgressAu = 10 + 10 * (double)num / (double)(m_PointSumNumber + m_AddedPoingCLoud.size());
		}
		surfaceCloud->points[cloudCount].x = pointSurfaceCloud[num].m_Coordinate[0];
		surfaceCloud->points[cloudCount].y = pointSurfaceCloud[num].m_Coordinate[1];
		surfaceCloud->points[cloudCount].z = pointSurfaceCloud[num].m_Coordinate[2];
		cloudCount++;
	}
	
	//用cloud构建tree对象
	surfaceTree->setInputCloud(surfaceCloud);
    //为法线估计对象设置输入点云
	n.setInputCloud(surfaceCloud);
	n.setSearchMethod(surfaceTree);
	n.setKSearch(20);
	n.compute(*normals);
	//进行点云的三角化
	pcl::PointCloud<pcl::PointNormal>::Ptr surfaceCloudWithNormals(new pcl::PointCloud<pcl::PointNormal>);
	pcl::concatenateFields(*surfaceCloud, *normals, *surfaceCloudWithNormals);
	//对三角化相关的变量进行定义
	pcl::search::KdTree<pcl::PointNormal>::Ptr surfaceTreeWithNormals(new pcl::search::KdTree<pcl::PointNormal>);
	surfaceTreeWithNormals->setInputCloud(surfaceCloudWithNormals);
	pcl::GreedyProjectionTriangulation<pcl::PointNormal> gp3;
	
	gp3.setSearchRadius(m_trianMaxDistace);//三角化后得到的每个三角形的最大可能边长
	gp3.setMu(m_trianMaxAngle);//设置近邻搜索的最远距离
	gp3.setMaximumNearestNeighbors(100);//设置样本可搜索的领域个数
	gp3.setMaximumSurfaceAngle(M_PI / 4);//设角度为45度
	gp3.setMinimumAngle(M_PI * m_trianMinAngle);//设置三角化后得到三角形内角最小角度
	gp3.setMaximumAngle(M_PI * m_trianMaxSAngle);//设置三角化后得到三角形内角最大角度
	gp3.setNormalConsistency(false);//使得法矢量朝向一致

	gp3.setInputCloud(surfaceCloudWithNormals);
	gp3.setSearchMethod(surfaceTreeWithNormals);
	if (b_OneStepMode)
	{
		gProgressAu = 40;
	}
	else
	{
		gProgressAu = 30;
	}
	gp3.reconstruct(triangles);

	/*pcl::Poisson<pcl::PointNormal> pn;
	pn.setConfidence(false);
	pn.setIsoDivide(2);
	pn.setInputCloud(surfaceCloudWithNormals);
	pn.setSearchMethod(surfaceTreeWithNormals);
	pn.reconstruct(triangles);
	*/
	gProgressAu = 50;
	gProgressAu = -1;
	
	/*
	// Additional vertex information
	std::vector<int> parts = gp3.getPartIDs();
	std::vector<int> states = gp3.getPointStates();
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));
	viewer->setBackgroundColor(0, 0, 0);
	viewer->addPolygonMesh(triangles, "my");

	viewer->addCoordinateSystem(1.0);
	viewer->initCameraParameters();
	// 主循环
	while (!viewer->wasStopped())
	{
		viewer->spinOnce(100);
		boost::this_thread::sleep(boost::posix_time::microseconds(100000));
	}
	*/
}

//获取修补之后的点云数据
PCPoint* PointCloudData::GetAddedRepairingPointCloud()
{
	return m_AddedRepairingPointCloud;
}
//获取选取数据点的法矢量
void PointCloudData::GetSelectedPointsNormalVectors()
{
	m_SelectedPointsNormalPoints.clear();
	m_SelectedPointsNormalPoints2.clear();
	for (int num = 0; num < m_PointSumNumber; num++)
	{//得到选择的数据点集
		if (m_OriginPointCloud[num].b_Selected == true)
		{
			m_SelectedPointsNormalPoints.push_back(m_OriginPointCloud[num]);
		}
	}
	//获取法矢量
	PCPoint* tempPointCloud = new PCPoint[m_SelectedPointsNormalPoints.size()];
	for (int num = 0; num < m_SelectedPointsNormalPoints.size(); num++)
	{
		m_SelectedPointsNormalPoints[num].m_Normal
			= GetNormalVector(m_SelectedPointsNormalPoints[num]);
		tempPointCloud[num].m_Coordinate[0] = m_SelectedPointsNormalPoints[num].m_Coordinate[0];
		tempPointCloud[num].m_Coordinate[1] = m_SelectedPointsNormalPoints[num].m_Coordinate[1];
		tempPointCloud[num].m_Coordinate[2] = m_SelectedPointsNormalPoints[num].m_Coordinate[2];
		tempPointCloud[num].m_Normal = m_SelectedPointsNormalPoints[num].m_Normal;
	}
	//进行法矢量方向矫正
	RectifyNormals(tempPointCloud, m_SelectedPointsNormalPoints.size());
	int selectedSize = m_SelectedPointsNormalPoints.size();
	m_SelectedPointsNormalPoints.clear();
	for (int num = 0; num < selectedSize; num++)
	{
		m_SelectedPointsNormalPoints.push_back(tempPointCloud[num]);
	}
	for (int num = 0; num < m_SelectedPointsNormalPoints.size(); num++)
	{
		PCPoint* tempPoint = new PCPoint;
		tempPoint->m_Coordinate[0] = m_SelectedPointsNormalPoints[num].m_Coordinate[0];
		tempPoint->m_Coordinate[1] = m_SelectedPointsNormalPoints[num].m_Coordinate[1];
		tempPoint->m_Coordinate[2] = m_SelectedPointsNormalPoints[num].m_Coordinate[2];
		tempPoint->m_Normal = m_SelectedPointsNormalPoints[num].m_Normal;
		GetAdditionalPoint(tempPoint);
		m_SelectedPointsNormalPoints2.push_back(*tempPoint);
	}
}
//PCL中的模型 边界保留
void PointCloudData::PCLBoundaryKeep()
{
	//PCL 中的方法 begin//
	pcl::PointCloud<pcl::Boundary> boundaries;
	pcl::BoundaryEstimation<pcl::PointXYZ, pcl::Normal, pcl::Boundary> boundEst;
	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> normEst;
	pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);
	pcl::search::KdTree<pcl::PointXYZ>::Ptr searchTree(new pcl::search::KdTree<pcl::PointXYZ>);
	
	searchTree->setInputCloud(cloud);
	normEst.setInputCloud(cloud);
	//normEst.setRadiusSearch(0.1);
	if (b_OneStepMode)
	{
		gProgressAu = 5 * (10.0 / 50.0);
	}
	else
	{
		gProgressAu = 10;
	}
	normEst.setSearchMethod(searchTree);
	normEst.setKSearch(10);
	normEst.compute(*normals);
	if (b_OneStepMode)
	{
		gProgressAu = 5 * (30.0 / 50.0);
	}
	else
	{
		gProgressAu = 30;
	}

	boundEst.setInputCloud(cloud);
	boundEst.setInputNormals(normals);
	boundEst.setRadiusSearch(Threadshold);
	boundEst.setAngleThreshold(M_PI / 4);
	boundEst.setSearchMethod(pcl::search::KdTree<pcl::PointXYZ>::Ptr(new pcl::search::KdTree<pcl::PointXYZ>));
	boundEst.compute(boundaries);
	if (b_OneStepMode)
	{
		gProgressAu = 5 * (45.0 / 50.0);
	}
	else
	{
		gProgressAu = 45;
	}

	for (int num = 0; num < m_PointSumNumber; num++)
	{
		if (boundaries.points[num].boundary_point == 1)
		{
			m_OriginPointCloud[num].b_BoundaryPoint = true;
			m_OriginPointCloud[num].m_Color[0] = 1.0;
		}
		else
		{
			m_OriginPointCloud[num].m_Color[2] = 1.0;
		}
	}
	if (b_OneStepMode)
	{
		gProgressAu = 5 * (50.0 / 50.0);
	}
	else
	{
		gProgressAu = 50;
	}

	//end//
}
//张量投票算法保留特征点
void PointCloudData::TensorVoting(vector<PCPoint>& repairedPoint)
{
	int k = 0;
	double gussValue = 0.5;//the parameter of guss function.
	double eigenvalue1;//define three eigenvalues.
	double eigenvalue2;
	double eigenvalue3;
	double dm;
	double wi;
	double wMax = 1.45;
	double wMin = 1.0;
	//用于存储修补点的点云数据结构
	pcl::PointCloud<pcl::PointXYZ>::Ptr coeCloud; // PCL 点云存储空间
	pcl::KdTreeFLANN<pcl::PointXYZ> coeKdtree;
	//初始化kd树和点云数据
	coeCloud = pcl::PointCloud<pcl::PointXYZ>::Ptr(new pcl::PointCloud<pcl::PointXYZ>());
	coeCloud->width = repairedPoint.size();//初始化 PCL 点云
	coeCloud->height = 1;
	coeCloud->points.resize(coeCloud->width * coeCloud->height);
	int cloudCount = 0;
	for (int num = 0; num < repairedPoint.size(); num++)
	{
		coeCloud->points[cloudCount].x = repairedPoint[num].m_Coordinate[0];
		coeCloud->points[cloudCount].y = repairedPoint[num].m_Coordinate[1];
		coeCloud->points[cloudCount].z = repairedPoint[num].m_Coordinate[2];
		cloudCount++;
	}
	coeKdtree.setInputCloud(coeCloud);

	for (int num = 0; num < repairedPoint.size(); num++)
	{

		eigenvalue1 = 0;
		eigenvalue2 = 0;
		eigenvalue3 = 0;
		dm = 0;
	   
		//进行k近邻搜索
		PCLCoefficientsKDtreeNKSearch(repairedPoint[num], 10,coeCloud, coeKdtree);
		vector<PCPoint>::iterator it = RkNearestPoints.begin() + 1;
		for (int i = 0; i < 9; it++, i++)//use the function to get tensor T.
		{//count the T tensor of RangeData[num].
			dm = (it->m_Coordinate[0] - repairedPoint[num].m_Coordinate[0])*(it->m_Coordinate[0] - repairedPoint[num].m_Coordinate[0])
				+ (it->m_Coordinate[1] - repairedPoint[num].m_Coordinate[1])*(it->m_Coordinate[1] - repairedPoint[num].m_Coordinate[1])
				+ (it->m_Coordinate[2] - repairedPoint[num].m_Coordinate[2])*(it->m_Coordinate[2] - repairedPoint[num].m_Coordinate[2]);
			PCPoint p;
			p.m_Coordinate[0] = it->m_Coordinate[0];
			p.m_Coordinate[1] = it->m_Coordinate[1];
			p.m_Coordinate[2] = it->m_Coordinate[2];
			eigenvalue1 += Guss(GetTwoPointsDistance(p, repairedPoint[num]), gussValue)*(1 - (it->m_Coordinate[0] - repairedPoint[num].m_Coordinate[0])*(it->m_Coordinate[0] - repairedPoint[num].m_Coordinate[0]) / dm);
			eigenvalue2 += Guss(GetTwoPointsDistance(p, repairedPoint[num]), gussValue)*(1 - (it->m_Coordinate[1] - repairedPoint[num].m_Coordinate[1])*(it->m_Coordinate[1] - repairedPoint[num].m_Coordinate[1]) / dm);
			eigenvalue3 += Guss(GetTwoPointsDistance(p, repairedPoint[num]), gussValue)*(1 - (it->m_Coordinate[2] - repairedPoint[num].m_Coordinate[2])*(it->m_Coordinate[2] - repairedPoint[num].m_Coordinate[2]) / dm);
		}
		RkNearestPoints.clear();//clear 
		sortValue(eigenvalue1, eigenvalue2, eigenvalue3);//sort the three values from max to mini.
		wi = (eigenvalue2 + eigenvalue3) / eigenvalue1;// get w.

		if (wi > wMax)
		{
			repairedPoint[num].b_TVPoint = true;
			repairedPoint[num].m_Color[0] = 0.0;
			repairedPoint[num].m_Color[1] = 0.0;
			repairedPoint[num].m_Color[2] = 0.0;
		}
	}
	//对特征点进行位置的调整
	int rpCount = 0;
	for (int i = 0; i < repairedPoint.size(); i++)
	{
		if (repairedPoint[i].b_TVPoint)
		{
			rpCount++;
		}
	}
	//只有超过10个点的时候才进行特征强化
	if (rpCount > 10)
	{
			for (int num = 0; num < repairedPoint.size(); num++)
		{
		if (repairedPoint[num].b_TVPoint)
		{//如果是特征点
		//获取数据点法矢量
		repairedPoint[num].m_Normal = GetTVNormalVector(repairedPoint[num], coeCloud, coeKdtree);
		for (int j = 0; j < 3; j++)
		{
		repairedPoint[num].m_Normal[j] = -repairedPoint[num].m_Normal[j];
		}
		PCLKDtreeNKSearch(m_OriginPointCloud[0],2);
		double l = GetTwoPointsDistance(RkNearestPoints[0], RkNearestPoints[1]) / 2;
		RkNearestPoints.clear();
		double pNormal[3];
		double temp[3];
		for (int i = 0; i < 3; i++)
		{
		pNormal[i] = repairedPoint[num].m_Normal[i];
		}
		temp[0] = l * pNormal[0] / sqrt(pNormal[0] * pNormal[0]
		+ pNormal[1] * pNormal[1] + pNormal[2] * pNormal[2]) + repairedPoint[num].m_Coordinate[0];

		temp[1] = l * pNormal[1] / sqrt(pNormal[0] * pNormal[0]
		+ pNormal[1] * pNormal[1] + pNormal[2] * pNormal[2]) + repairedPoint[num].m_Coordinate[1];

		temp[2] = l * pNormal[2] / sqrt(pNormal[0] * pNormal[0]
		+ pNormal[1] * pNormal[1] + pNormal[2] * pNormal[2]) + repairedPoint[num].m_Coordinate[2];

		repairedPoint[num].m_Coordinate[0] = temp[0];
		repairedPoint[num].m_Coordinate[1] = temp[1];
		repairedPoint[num].m_Coordinate[2] = temp[2];
		}
		}
	}
}