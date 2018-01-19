#include <iostream>
#include <cmath>
#include <stdio.h>
#include <algorithm>
#include <vector>

/////////////////////////////////////////////////////
//L = line, C = circle, S = segment, P = point, F = flat
/////////////////////////////////////////////////////

using namespace std;
const double eps = 1e-8;
const double pi = acos(-1.0);

/////////////////////////////////////////////////////
//点和向量的定义
/////////////////////////////////////////////////////

struct point {
	double x, y;
	point(double x = 0, double y = 0):x(x), y(y) {}
};
typedef point vec;//实现上，vector不过是point的别名
vec operator + (vec a, vec b) { return vec(a.x + b.x, a.y + b.y); }
vec operator - (point a, point b) { return vec(a.x - b.x, a.y - b.y); }
vec operator * (vec a, double p) { return vec(a.x * p, a.y * p); }
vec operator / (vec a, double p) { return vec(a.x / p, a.y / p); }
bool operator < (const point &a, const point &b) {//不要使用>,而使用<
	return a.x < b.x || (a.x == b.x && a.y < b.y);
}
int dcmp(double x) {//三态函数
	if (fabs(x) < eps)
		return 0;
	else
		return x < 0 ? -1 : 1;
}
bool operator == (const point &a, const point &b) {
	return dcmp(a.x - b.x) == 0 && dcmp(a.y - b.y == 0);
}

/////////////////////////////////////////////////////
//点和向量的基本运算
/////////////////////////////////////////////////////

double dot(vec a, vec b) { return a.x*b.x + a.y*b.y; };//点积
double length(vec a) { return sqrt(dot(a, a)); }
double angle(vec a, vec b) {//俩向量夹角
	double ans = dot(a, b) / length(a) / length(b);
	if (dcmp(ans - 1) == 0) ans = 1;
	if (dcmp(ans + 1) == 0) ans = -1;
	return acos(ans);
}
double angle(vec a) { return atan2(a.y, a.x); }//向量的极角
double cross(vec a, vec b) { return a.x * b.y - a.y * b.x; }//叉积
double area(point a, point b, point c) {//以a,b,c为顶点的三角形的面积
	return cross(b - a, c - a) / 2;
}
vec rotate(vec a, double rad) {//将向量a绕起点逆时针旋转rad角
	return vec(a.x*cos(rad) - a.y*sin(rad), a.x*sin(rad) + a.y*cos(rad));
}
vec normal(vec a) {//a的单位法线(即左转90度,再长度归一)
	double l = length(a);
	return vec(-a.y / l, a.x / l);
}

/////////////////////////////////////////////////////
//点和直线
/////////////////////////////////////////////////////

struct line {
	point p;
	vec v;
	double rad;//极角
	line() {}
	line(point p, vec v):p(p), v(v) { rad = angle(v); }
	bool operator < (const line& l) const {
		return rad < l.rad;
	}
};
point get_LL_intersection(point p, vec v, point q, vec w) {//俩直线交点
	vec u = p - q;//直线1:p+t*v,直线2:q+t*w
	double t = cross(w, u) / cross(v, w);
	return p + v * t;
}
double dis_of_PL(point p, point a, point b) {//点p到直线AB的距离
	vec v1 = b - a, v2 = p - a;
	return fabs(cross(v1, v2)) / length(v1);
}
double dis_of_PS(point p, point a, point b) {//点p到线段AB的距离
	if (a == b) return length(p - a);
	vec v1 = b - a, v2 = p - a, v3 = p - b;
	if (dcmp(dot(v1, v2)) < 0)
		return length(v2);//返回AP长度
	else if (dcmp(dot(v1, v3)) > 0)
		return length(v3);//返回BP长度
	else
		return fabs(cross(v1, v2)) / length(v1);//计算方法同直线方法
}
bool P_on_L_left(point a, point p, vec v) {//判断点a是否在线段p+tv左侧
	return cross(v, a - p) > 0;
}
point get_PL_projection(point p, point a, point b) {//p点在直线AB上的投影点
	vec v = b - a;
	return a + v*(dot(v, p - a) / dot(v, v));
}
bool SS_intersection(point a1, point a2, point b1, point b2) {//线段相交判定
	double c1 = cross(a2 - a1, b1 - a1), c2 = cross(a2 - a1, b2 - a1),
		c3 = cross(b2 - b1, a1 - b1), c4 = cross(b2 - b1, a2 - b1);
	return dcmp(c1) * dcmp(c2) < 0 && dcmp(c3) * dcmp(c4) < 0;
}
bool P_on_S(point p, point a1, point a2) {//判断点是否在线段上
	return dcmp(cross(a1 - p, a2 - p)) == 0 && dcmp(dot(a1 - p, a2 - p)) < 0;
}

/////////////////////////////////////////////////////
//多边形
/////////////////////////////////////////////////////

double polygon_area(point *p, int n) {
	double area = 0;
	for (int i = 1; i < n - 1; i++)
		area += cross(p[i] - p[0], p[i + 1] - p[0]);
	return area / 2;
}

/////////////////////////////////////////////////////
//圆的相关计算
/////////////////////////////////////////////////////

struct circle {
	point c;
	double r;
	circle(point c, double r):c(c), r(r) {}
	point get_point(double rad) {//获得圆上一点的坐标
		return point(c.x + cos(rad)*r, c.y + sin(rad)*r);
	}
};
int get_LC_intersection(point p, vec v, circle cir, //p+tv为直线参数方程
	double &t1, double &t2, vector<point> &sol) {//t1,t2存放交点在直线上的参数,sol存放交点
	double a = v.x, b = p.x - cir.c.x, c = v.y, d = p.y - cir.c.y;
	double e = a*a + c*c, f = 2 * (a*b + c*d), g = b*b + d*d - cir.r*cir.r;
	double delta = f*f - 4 * e*g;
	if (dcmp(delta) < 0)return 0;//返回交点的个数
	if (dcmp(delta) == 0) {
		t1 = t2 = -f / (2 * e);
		sol.push_back(p + v*t1);
		return 1;
	}
	t1 = (-f - sqrt(delta)) / (2 * e); sol.push_back(p + v*t1);//相交,两个交点
	t2 = (-f + sqrt(delta)) / (2 * e); sol.push_back(p + v*t2);
	return 2;
}
int get_CC_intersection(circle cir1, circle cir2, vector<point> &sol) {//两圆相交
	double d = length(cir1.c - cir2.c);//圆心距
	if (dcmp(d) == 0) {
		if (dcmp(cir1.r - cir2.r) == 0) return -1;//两圆重合
		else return 0;//两圆内含
	}
	if (dcmp(cir1.r + cir2.r - d) < 0) return 0;//两圆相离
	if (dcmp(fabs(cir1.r - cir2.r) - d) > 0) return 0;//两圆内含
	double a = angle(cir2.c - cir1.c);//C1C2的极角
	double da = acos((cir1.r*cir1.r + d*d - cir2.r*cir2.r) / (2 * cir1.r*d));
	point p1 = cir1.get_point(a - da), p2 = cir1.get_point(a + da);
	sol.push_back(p1);
	if (p1 == p2) return 1;
	sol.push_back(p2); return 2;
}
int get_PC_tangents(point p, circle cir, vec *v) {//过点p到圆cir的切线,v[i]是第i条切线的向量
	vec u = cir.c - p;
	double dis = length(u);
	if (dis < cir.r) return 0;
	else if (dcmp(dis - cir.r) == 0) {
		v[0] = rotate(u, pi / 2);
		return 1;
	} else {
		double rad = asin(cir.r / dis);
		v[0] = rotate(u, -rad);
		v[1] = rotate(u, rad);
		return 2;
	}
}
int get_CC_tangents(circle cir1, circle cir2,//返回切线的条数,-1表示无穷 
	point *p1, point *p2) {//p1[i]和p2[i]分别表示第i条切线在圆1与圆2上的切点
	int cnt = 0;
	if (cir1.r < cir2.r) {
		swap(cir1, cir2);
		swap(p1, p2);
	}
	double d2 = (cir1.c.x - cir2.c.x)*(cir1.c.x - cir2.c.x)
		+ (cir1.c.y - cir2.c.y)*(cir1.c.y - cir2.c.y);//d^2为圆心距的平方
	double rdiff = cir1.r - cir2.r;//半径之差
	double rsum = cir1.r + cir2.r;//半径之和
	if (d2 < rdiff*rdiff) return 0;//两圆内含
	double base = angle(cir2.c - cir1.c);
	if (d2 == 0 && cir1.r == cir2.r) return -1;//无限条切线
	if (d2 == rdiff*rdiff) {//内切,1条切线
		p1[cnt] = cir1.get_point(base);
		p2[cnt] = cir2.get_point(base);
		cnt++; return 1;
	}
	double rad = acos((cir1.r - cir2.r) / sqrt(d2));//有外公切线,一定为2条
	p1[cnt] = cir1.get_point(base + rad);
	p2[cnt] = cir2.get_point(base + rad);
	cnt++;
	p1[cnt] = cir1.get_point(base - rad);
	p2[cnt] = cir2.get_point(base - rad);
	cnt++;
	if (d2 == rsum*rsum) {//1条内公切线
		p1[cnt] = cir1.get_point(base);
		p2[cnt] = cir2.get_point(base + pi);
		cnt++;
	} else if (d2>rsum*rsum) {//2条内公切线
		double rad = acos((cir1.r + cir2.r) / sqrt(d2));
		p1[cnt] = cir1.get_point(base + rad);
		p2[cnt] = cir2.get_point(base + rad + pi);
		cnt++;
		p1[cnt] = cir1.get_point(base - rad);
		p2[cnt] = cir2.get_point(base - rad + pi);
		cnt++;
	}
	return cnt;
}

/////////////////////////////////////////////////////
//二维几何常用算法
/////////////////////////////////////////////////////

int P_in_polygon(point p, point *poly, int size) {//判断点是否在多边形中
	int wn = 0;
	for (int i = 0; i < size; i++) {
		if (P_on_S(p, poly[i], poly[(i + 1) % size])) return -1;
		int k = dcmp(cross(poly[(i + 1) % size] - poly[i], p - poly[i]));
		int d1 = dcmp(poly[i].y - p.y);
		int d2 = dcmp(poly[(i + 1) % size].y - p.y);
		if (k > 0 && d1 <= 0 && d2 > 0)wn++;
		if (k < 0 && d2 <= 0 && d1 > 0)wn--;
	}
	if (wn) return 1;//点在内部
	else return 0;//点在外部
}
int convex_hull(point *p, int n, point *ch) {//n个点组成的p[]的凸包,存放在ch[]中
	sort(p, p + n);
	int m = 0;
	for (int i = 0; i < n; i++) {
		while (m > 1 && cross(ch[m - 1] - ch[m - 2], p[i] - ch[m - 2]) <= 0) m--;
		ch[m++] = p[i];
	}
	int k = m;
	for (int i = n - 2; i >= 0; i--) {
		while (m > k && cross(ch[m - 1] - ch[m - 2], p[i] - ch[m - 2]) <= 0) m--;
		ch[m++] = p[i];
	}
	if (n > 1) m--;
	return m;
}
int half_plane_intersaction(line *l, int n, point *poly) {
	sort(l, l + n);
	int first, last;//构造双端队列
	point *p = new point[n];//p[i]是q[i] & q[i+1]的交点
	line *q = new line[n];//双端队列
	q[first = last = 0] = l[0];
	for (int i = 1; i < n; i++) {
		while (first < last && !P_on_L_left(p[last - 1], l[i].p, l[i].v)) last--;
		while (first < last && !P_on_L_left(p[first], l[i].p, l[i].v)) first++;
		q[++last] = l[i];
		if (fabs(cross(q[last].v, q[last - 1].v)) < eps) {
			last--;
			if (P_on_L_left(l[i].p, q[last].p, q[last].v)) q[last] = l[i];
		}
		if (first < last) p[last - 1] = get_LL_intersection(q[last].p, q[last].v, q[last - 1].p, q[last - 1].v);
	}
	while (first < last && !P_on_L_left(p[last - 1], q[first].p, q[first].v)) last--;//删除无用平面
	if (last - first <= 1) return 0;
	p[last] = get_LL_intersection(q[last].p, q[last].v, q[first].p, q[first].v);//计算首尾交点
	int m = 0;
	for (int i = first; i <= last; i++) poly[m++] = p[i];//拷贝结果
	return m;

}

/////////////////////////////////////////////////////
//三维点的定义
/////////////////////////////////////////////////////

struct point3 {
	double x, y, z;
	point3(double x = 0, double y = 0, double z = 0):x(x), y(y) {}
};
typedef point3 vec3;
vec3 operator + (vec3 a, vec3 b) {
	return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}
vec3 operator - (vec3 a, vec3 b) {
	return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}
vec3 operator * (vec3 a, double p) {
	return vec3(a.x * p, a.y * p, a.z * p);
}
vec3 operator / (vec3 a, double p) {
	return vec3(a.x / p, a.y / p, a.z / p);
}

/////////////////////////////////////////////////////
//三维点的运算
/////////////////////////////////////////////////////

double dot(vec3 a, vec3 b) { return a.x*b.x + a.y*b.y + a.z * b.z; }//三维点积
double length(vec3 a) { return sqrt(dot(a, a)); }
double angle(vec3 a, vec3 b) {//俩向量夹角
	double ans = dot(a, b) / length(a) / length(b);
	if (dcmp(ans - 1) == 0)ans = 1;
	if (dcmp(ans + 1) == 0)ans = -1;
	return acos(ans);
}
double dis_of_PF(const point3 &p, const point3 &p0, const point3 &n) {
	return fabs(dot(p - p0, n));
}
point3 get_PF_projection(const point3 &p, const point3 &p0, const point3 &n) {
	return p - n*dot(p - p0, n);
}
point3 get_LF_intersaction(point3 &p1, point3 &p2, point3 &p0, vec3 n) {
	vec3 v = p2 - p1;
	double t = (dot(n, p0 - p1) / dot(n, p2 - p1));
	return p1 + v*t;
}
vec3 cross(vec3 a, vec3 b) {
	return vec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}
double area2(point3 a, point3 b, point3 c) {
	return length(cross(b - a, c - a));
}
