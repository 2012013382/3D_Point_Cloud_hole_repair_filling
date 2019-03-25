#include"Guass.h"
using namespace std;
int gProgress = 0;
int gProgressAu = 0;
bool b_Saved = true;
bool b_FIP = false;
void Inverse(double *matrix1[], double *matrix2[], int n, double d)
{
	int i, j;
	for (i = 0; i<n; i++)
		matrix2[i] = (double *)malloc(n*sizeof(double));
	for (i = 0; i<n; i++)
		for (j = 0; j<n; j++)
			*(matrix2[j] + i) = (AlCo(matrix1, n, i, j) / d);
}

double Determinant(double* matrix[], int n)
{
	double result = 0, temp;
	int i;
	if (n == 1)
		result = (*matrix[0]);
	else
	{
		for (i = 0; i<n; i++)
		{
			temp = AlCo(matrix, n, n - 1, i);
			result += (*(matrix[n - 1] + i))*temp;
		}
	}
	return result;
}

double AlCo(double* matrix[], int jie, int row, int column)
{
	double result;
	if ((row + column) % 2 == 0)
		result = Cofactor(matrix, jie, row, column);
	else result = (-1)*Cofactor(matrix, jie, row, column);
	return result;
}

double Cofactor(double* matrix[], int jie, int row, int column)
{
	double result;
	int i, j;
	double* smallmatr[MAX - 1];
	for (i = 0; i<jie - 1; i++)
		smallmatr[i] = new double[jie - 1];
	for (i = 0; i<row; i++)
		for (j = 0; j<column; j++)
			*(smallmatr[i] + j) = *(matrix[i] + j);
	for (i = row; i<jie - 1; i++)
		for (j = 0; j<column; j++)
			*(smallmatr[i] + j) = *(matrix[i + 1] + j);
	for (i = 0; i<row; i++)
		for (j = column; j<jie - 1; j++)
			*(smallmatr[i] + j) = *(matrix[i] + j + 1);
	for (i = row; i<jie - 1; i++)
		for (j = column; j<jie - 1; j++)
			*(smallmatr[i] + j) = *(matrix[i + 1] + j + 1);
	result = Determinant(smallmatr, jie - 1);
	for (i = 0; i<jie - 1; i++)
		delete[] smallmatr[i];
	return result;
}
//列主元高斯消去法
void colunmPrincipleGauss(int N, double** a)
{
	int k = 0, i = 0, r = 0, j = 0;
	double t;
	for (k = 0; k<N - 1; k++)
	{

		for (i = k; i<N; i++)
		{
			r = i;
			t = (double)fabs(a[r][k]);
			if (fabs(a[i][k])>t)
			{
				r = i;
			}
		}

		if (a[r][k] == 0)//列主元不能为0
		{
			break;
		}

		for (j = k; j<N + 1; j++)//交换第K行和第r行
		{
			t = a[r][j];
			a[r][j] = a[k][j];
			a[k][j] = t;
		}
		for (i = k + 1; i<N; i++)
		{
			for (j = k + 1; j<N + 1; j++)
			{
				a[i][j] = a[i][j] - a[i][k] / a[k][k] * a[k][j];
			}
		}
	}
	double m = 0;
	for (k = N - 1; k >= 0; k--)
	{
		m = 0;
		for (j = k + 1; j<N; j++)
		{
			m += a[k][j] * a[j][N];
		}

		a[k][N] = (a[k][N] - m) / a[k][k];
	}
}
//简单径向基函数的值
double GetRBFValue(double a)
{
	return fabs(a*a*a);
}
//获得两点之间的距离
double GetDistance(double* q,double* p)
{
	return sqrt((q[0] - p[0]) * (q[0] - p[0])
		+ (q[1] - p[1]) * (q[1] - p[1])
		+ (q[2] - p[2]) * (q[2] - p[2]));
}
//判断三点是否共线
bool ThreePointsCollinear(PCPoint p1, PCPoint p2, PCPoint p3)
{
	double p12[3];
	double p13[3];
	p12[0] = p2.m_Coordinate[0] - p1.m_Coordinate[0];
	p12[1] = p2.m_Coordinate[1] - p1.m_Coordinate[1];
	p12[2] = p2.m_Coordinate[2] - p1.m_Coordinate[2];

	p13[0] = p3.m_Coordinate[0] - p1.m_Coordinate[0];
	p13[1] = p3.m_Coordinate[1] - p1.m_Coordinate[1];
	p13[2] = p3.m_Coordinate[2] - p1.m_Coordinate[2];
	
	double ppu = p12[0] * p13[0] + p12[1] * p13[1] + p12[2] * p13[2];
	double ppd = sqrt(p12[0] * p12[0] + p12[1] * p12[1] + p12[2] * p12[2])
		* sqrt(p13[0] * p13[0] + p13[1] * p13[1] + p13[2] * p13[2]);
	double ttt = fabs(ppu / ppd);
	if ( ttt - 1 < 0.0001)
	{
		return true;
	}
	
	return false;

}
double Guss(double s, double x)
{
	double e = 2.71828;
	double di = -(s*s) / (x*x);
	return pow(e, di);
}
void sortValue(double &value1, double &value2, double &value3)
{
	double temp = 0;
	if (value2 > value1)
	{
		temp = value1;
		value1 = value2;
		value2 = temp;
	}
	if (value3 > value1)
	{
		temp = value1;
		value1 = value3;
		value3 = temp;
	}
	if (value3 > value2)
	{
		temp = value2;
		value2 = value3;
		value3 = temp;
	}
}