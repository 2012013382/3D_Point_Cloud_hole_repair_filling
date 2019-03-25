#include<iostream>
#include"PCPoint.h"
#define MAX 10
extern int gProgress;
extern int gProgressAu;
extern bool b_Saved;//保存判断
extern bool b_FIP;//文件导入判断
void Inverse(double *matrix1[], double *matrix2[], int n, double d);
double Determinant(double* matrix[], int n);
double AlCo(double* matrix[], int jie, int row, int column);
double Cofactor(double* matrix[], int jie, int row, int column);
void colunmPrincipleGauss(int N, double** a);//列主元高斯消去法
double GetRBFValue(double a);//简单径向基函数的值
double GetDistance(double* q, double* p);//获取两点之间的距离
bool ThreePointsCollinear(PCPoint p1,PCPoint p2,PCPoint p3);//判断三点是否共线
double Guss(double s, double x);
void sortValue(double &value1, double &value2, double &value3);