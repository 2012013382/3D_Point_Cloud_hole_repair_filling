#pragma once
#include"Guass.h"
#include <pcl/point_cloud.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>
#include <pcl/search/kdtree.h>
#include <pcl/point_types.h>
#include <pcl/features/normal_3d.h>
#include <pcl/surface/gp3.h>
#include <pcl/surface/poisson.h>
#include<pcl/features/boundary.h>
#define BCNum 30
#define NORMAL_VECTOR_NUM 5
using namespace std;
class PointCloudData
{
public:
	PointCloudData();
	~PointCloudData();
private://特性
	int m_PointSumNumber;//点云总数
	PCPoint m_MidPoint;//模型的中点
	PCPoint* m_PointCloud;//存储点云数据
	PCPoint* m_OriginPointCloud;//置于显示中心后的点云数据
	PCPoint* m_AddedRepairingPointCloud;//增加修补点之后的点云数据
	double m_MaxX;//点云数据中最大的x值
	double m_MinX;//点云数据中最小的x值
	double m_MaxY;//点云数据中最大的y值
	double m_MinY;//点云数据中最小的y值
	double m_MaxZ;//点云数据中最大的z值
	double m_MinZ;//点云数据中最小的z值
	vector<PCPoint> RkNearestPoints;//用于存储k近邻点
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud; // PCL 点云存储空间
	pcl::KdTreeFLANN<pcl::PointXYZ> kdtree;
	int m_CloudClusterNum;//聚类个数
	vector<PCPoint>* m_BoundaryPointClusters;//存储边界聚类集合
	vector<PCPoint> m_AddedPoingCLoud;//新增的修补点
public:
	pcl::PolygonMesh triangles;//三角化后的模型
	double  Threadshold;//边界保留参数
	double m_clusterDistace;//最小聚类距离
	double m_clusterNum;//最小聚类个数
	double m_trianMaxDistace;//最大连接距离
	double m_trianMaxAngle;//最大角
	double m_trianMinAngle;//最小角
	double m_trianMaxSAngle;//最远搜索距离
	vector<PCPoint> m_TestSeletedPointCloud;//test
	vector<PCPoint> m_SelectedPointsNormalPoints;//用于显示数据点的法矢量
	vector<PCPoint> m_SelectedPointsNormalPoints2;//用于显示数据点的法矢量
	//一键修复模式
	bool b_OneStepMode;
	//一键修补并表面生成
	bool b_AllPoint;
public://操作
	int GetPointSumNumber();//获取模型的点云数量
	void SetPointSumNumber(int number);//设置模型的点云数量
	PCPoint* GetPointCloud();//获取点云数据
	void InitPointCloud(PCPoint* pointCloud);//初始化点云数据
	PCPoint* GetOriginPointCloud();//获取置于显示中心后的点云数据
	PCPoint* GetAddedRepairingPointCloud();//获取修补之后的模型点云数据
	bool ReadPointCloudFile(char* fileName);//从文件中读取点云数据
	void PointMoveToOrigin(PCPoint*& pointCloud);//将模型置于显示中心
	void PCLKDtreeNKSearch(PCPoint lpoint, int Knum);//PCL KDtree k近邻搜索
	bool BoundaryCheck(PCPoint lpoint);//边界检测
	bool HoleRepair();//孔洞修补
	double* GetRBFCoefficients(vector<PCPoint>& m_SelectedPoint);//获取径向基函数的系数
	void RectifyNormals(PCPoint* additionalPoint, int RBFNum);//法向矫正
	double* GetNormalVector(PCPoint lpoint);//获取数据点的法矢量
	void GetAdditionalPoint(PCPoint* lpoint);//获取附加约束点的值
	double GetTwoPointsDistance(PCPoint p,PCPoint q);//获取两个数据点之间的距离
	bool InsertRepairPoint(double* coefficients, vector<PCPoint> & repairedPoint, vector<PCPoint> m_SelectedBoundaryPoint, vector<PCPoint> m_SelectedPoint);//插入新的修补点
	PCPoint GetInsertPoint(PCPoint p, PCPoint q, double l);//获取新的插入修补点
	vector<PCPoint>* GetNewAddedPoint();//获取修补点集合
	//vector<PCPoint>* GetSelectedPoint();//获取约束点集合
	void RepairedPointDisjust(double* coefficients, PCPoint& repairedPoint, vector<PCPoint> m_SelectedPoint);//调整孔洞修补点
	void BoundaryClustering();//边界聚类
	int GetCloudClusterNum();//获取聚类数量
	vector<PCPoint>* GetBoundaryPointClusters();//获取边界聚类集合
	void AutomaticHoleRepair();//自动进行孔洞修补
	bool NotIn(PCPoint lpoint, vector<PCPoint> m_SelectedBoundaryPoint, vector<PCPoint> m_SelectedPoint);//是否在选中的边界集合中
	void SurfaceGenerating();//点云三角化
	void GetSelectedPointsNormalVectors();//获取选取数据点的法矢量
	void PCLBoundaryKeep();//PCL中的边界保留算法
	void TensorVoting(vector<PCPoint>& repairedPoint);//张量投票算法
	void PCLCoefficientsKDtreeNKSearch(PCPoint lpoint, int Knum, pcl::PointCloud<pcl::PointXYZ>::Ptr coeCloud
		, pcl::KdTreeFLANN<pcl::PointXYZ> coeKdtree);//带参数的PCL近邻搜索
	double* GetTVNormalVector(PCPoint lpoint, pcl::PointCloud<pcl::PointXYZ>::Ptr coeCloud
		, pcl::KdTreeFLANN<pcl::PointXYZ> coeKdtree);//求得张量投票算法的特征点法矢量
};

