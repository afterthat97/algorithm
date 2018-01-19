#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#define MAXN 100
#define MAXM 200
//N * M
using namespace std;

template <class T>
struct matrix {
	int n, m;
	T dat[MAXN][MAXM];//both start from 1
	matrix(int nn = 0, int mm = 0): n(nn), m(mm) {}
	T* operator[](int i) { return dat[i]; }
};

template <class T>
void matcpy(matrix<T>& a, matrix<T>& b) {
	b.n = a.n; b.m = a.m;
	memcpy(b.dat, a.dat, sizeof(T) * MAXM * MAXN);
}

template <class T>
void matadd(matrix<T>& a, matrix<T>& b, matrix<T>& ans) {
	ans.n = a.n; ans.m = a.m;
	for (int i = 1; i <= a.n; i++)
		for (int j = 1; j <= a.m; j++)
			ans[i][j] = a[i][j] + b[i][j];
}

template <class T>
void matsub(matrix<T> &a, matrix<T> &b, matrix<T>& ans) {
	ans.n = a.n; ans.m = a.m;
	for (int i = 1; i <= a.n; i++)
		for (int j = 1; j <= a.m; j++)
			ans[i][j] = a[i][j] - b[i][j];
}

template <class T>
void matmul(matrix<T> &a, matrix<T> &b, matrix<T> &ans) {
	ans.n = a.n; ans.m = b.m;
	for (int i = 1; i <= a.n; i++)
		for (int j = 1; j <= b.m; j++) {
			T sum = 0;
			for (int k = 1; k <= a.m; k++)
				sum += a[i][k] * b[k][j];
			ans[i][j] = sum;
		}
}

template <class T>
void matmul(matrix<T> &a, int k, matrix<T> &ans) {
	ans.n = a.n; ans.m = a.m;
	for (int i = 1; i <= a.n; i++)
		for (int j = 1; j <= a.m; j++)
			ans[i][j] = a[i][j] * k;
}

template <class T>
void matdiv(matrix<T> &a, int k, matrix<T> &ans) {
	ans.n = a.n; ans.m = a.m;
	for (int i = 1; i <= a.n; i++)
		for (int j = 1; j <= a.m; j++)
			ans[i][j] = a[i][j] / k;
}

template <class T>
T det(matrix<T> &a) {
	if (a.m - a.n) exit(1);//error
	T ans = 1;
	for (int px = 1; px <= a.n; px++) {
		if (a[px][px] == 0) 
			for (int i = px + 1; i <= a.n; i++)
				if (a[i][px] != 0) {
					for (int j = px; j <= a.n; j++)
						swap(a[i][j], a[px][j]);
					ans = -ans;
					break;
				}
		if (a[px][px] == 0) return 0;
		ans *= a[px][px];
		for (int i = px + 1; i <= a.n; i++) {
			T k = a[i][px] / a[px][px];
			for (int j = px; j <= a.n; j++)
				a[i][j] -= a[px][j] * k;
		}
	}
	return ans;
}

template <class T>
void inv(matrix<T> &a, matrix<T> &ans) {
	if (a.n - a.m) exit(1);
	a.m <<= 1;
	for (int i = 1; i <= a.n; i++) {
		for (int j = a.n + 1; j <= a.m; j++) a[i][j] = 0;
		a[i][i + a.n] = 1;
	}
	for (int px = 1; px <= a.n; px++) {
		if (a[px][px] == 0)
			for (int i = px + 1; i <= a.n; i++)
				if (a[i][px] != 0) {
					for (int j = px; j <= a.m; j++)
						swap(a[i][j], a[px][j]);
					break;
				}
		if (a[px][px] == 0) {ans.n = -1; return;}
		for (int j = px + 1; j <= a.m; j++)
			a[px][j] /= a[px][px];
		a[px][px] = 1;
		for (int i = px + 1; i <= a.n; i++) {
			T k = a[i][px];
			for (int j = px; j <= a.m; j++)
				a[i][j] -= a[px][j] * k;
		}
	}
	for (int px = a.n; px >= 1; px--)
		for (int i = px - 1; i >= 1; i--)
			if (a[i][px] != 0) {
				T k = a[i][px];
				for (int j = px; j <= a.m; j++)
					a[i][j] -= a[px][j] * k;
			}
	ans.n = ans.m = a.n;
	for (int i = 1; i <= a.n; i++)
		for (int j = 1; j <= a.n; j++)
			ans[i][j] = a[i][j + a.n];
}

int main() {
	int n;
	cin >> n;
	matrix<double> a(n, n);
	//double a[3][3];
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			cin >> a[i][j];

	matrix<double> ans;
	inv(a, ans);
	for (int i = 1; i <= ans.n; i++) {
		for (int j = 1; j <= ans.m; j++)
			cout << round(ans[i][j]) << ' ';
		cout << endl;
	}
	return 0;
}
