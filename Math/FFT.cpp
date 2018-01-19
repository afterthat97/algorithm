#include <iostream>
#include <stdio.h>
#include <math.h>
#define pi acos(-1.0)
using namespace std;

struct comp {
	double re, im;
	comp(double r = 0, double i = 0): re(r), im(i) {}
	comp operator + (comp x) { return comp(re + x.re, im + x.im); }
	comp operator - (comp x) { return comp(re - x.re, im - x.im); }
	comp operator * (comp x) { return comp(re * x.re - im * x.im, re * x.im + im * x.re); }
};

void FFT(comp* a, int* g, int n, int f) {
	for (int i = 0; i < n; i++)
		if (g[i] > i) swap(a[i], a[g[i]]);
	for (int i = 1; i < n; i <<= 1) {
		comp wn1(cos(pi / i), f * sin(pi / i));
		for (int j = 0; j < n; j += (i << 1)) {
			comp wnk(1, 0);
			for (int k = 0; k < i; k++, wnk = wnk * wn1) {
				comp x = a[j + k], y = wnk * a[j + k + i];
				a[j + k] = x + y; a[j + k + i] = x - y;
			}
		}
	}
	if (!(~f)) for (int i = 0; i < n; i++) a[i].re /= n;
}

void mul(comp* a, comp* b, comp* ans, int n) {
	int _n = 1, t = -1;
	while (_n < n) { _n <<= 1; t++; }
	int* g = new int[_n]; g[0] = 0;
	for (int i = 1; i < _n; i++)
		g[i] = (g[i >> 1] >> 1) | ((i & 1) << t);
	FFT(a, g, _n, 1); FFT(b, g, _n, 1);
	for (int i = 0; i < _n; i++) { ans[i] = a[i] * b[i]; b[i] = comp(0, 0); }
	FFT(ans, g, _n, -1);
	delete[] g;
}

void inv(comp* a, comp* b, int n) {
	if (n == 1) { b[0].re = 1 / a[0].re; return; }
	inv(a, b, (n + 1) >> 1);
	int _n = 1, t = -1;
	while (_n < n) { _n <<= 1; t++; }
	int* g = new int[_n]; g[0] = 0;
	for (int i = 1; i < _n; i++)
		g[i] = (g[i >> 1] >> 1) | ((i & 1) << t);
	comp* tmp = new comp[_n];
	for (int i = 0; i < n; i++) tmp[i] = a[i];
	FFT(tmp, g, _n, 1); FFT(b, g, _n, 1);
	for (int i = 0; i < _n; i++) {
		tmp[i] = comp(2, 0) - tmp[i] * b[i];
		b[i] = tmp[i] * b[i];
	}
	FFT(b, g, _n, -1);
}

void div(comp* a, comp* b, comp* d, comp* r, int n, int m) {
	int _n = 1, t = -1;
	while (_n < (n - m + 1) << 1) { _n <<= 1; t++; }
	int* g = new int[_n]; g[0] = 0;
	for (int i = 1; i < _n; i++)
		g[i] = (g[i >> 1] >> 1) | ((i & 1) << t);
	for (int i = 0; i < (n >> 1); i++) swap(a[i], a[n - i - 1]);
	for (int i = 0; i < (m >> 1); i++) swap(b[i], b[m - i - 1]);
	comp* invb = new comp[_n];
	inv(b, invb, n - m + 2);
	for (int i = 0; i < _n; i++) cout << invb[i].re << ' '; cout << endl;
	FFT(a, g, _n, 1); FFT(invb, g, _n, 1);
	for (int i = 0; i < _n; i++) d[i] = a[i] * invb[i];
	FFT(d, g, _n, -1);
	for (int i = 0; i < ((n - m + 1) >> 1); i++) swap(d[i], d[(n - m + 1) - i - 1]);
	for (int i = n - m; ~i; i--) {
		d[i - 1].re += (d[i].re - floor(d[i].re)) * 10;
		d[i].re = floor(d[i].re);
	}
}
