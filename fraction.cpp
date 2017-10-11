#include <iostream>
#include <stdio.h>
#include <cstring>
#include <cmath>
#define sign(x) ((x) > 0 ? 1 : -1)
using namespace std;

inline int gcd(int a, int b) {
	while (b) { int t = a % b; a = b; b = t; }
	return a;
}

inline int lcm(int a, int b) { return (a * b) / gcd(a, b); }

struct frac {
	int x, y; // x / y
	frac(int val = 0) { x = val; y = 1; }
	frac(int a, int b) {
		int t = gcd(abs(a), abs(b));
		x = (a / t) * sign(b);
		y = (x == 0 ? 1 : (abs(b) / t));
	}
	frac(char* s) {
		int len = strlen(s), a, b, t;
		for (int i = 0; i < len; i++)
			if (s[i] == '/') {
				sscanf(s, "%d/%d", &a, &b); *this = frac(a, b); return;
			} else if (s[i] == '.') {
				sscanf(s, "%d.%d", &a, &t);
				for (int j = 0; j < len - i - 1; j++) b *= 10;
				a *= b; a += t; *this = frac(a, b); return;
			}
		sscanf(s, "%d", &a); x = a; y = 1;
	}
	frac operator + (const frac& f) { return frac(x * f.y + f.x * y, y * f.y); }
	frac operator - (const frac& f) { return frac(x * f.y - f.x * y, y * f.y); }
	frac operator * (const frac& f) { return frac(x * f.x, y * f.y); }
	frac operator / (const frac& f) { return frac(x * f.y, y * f.x); }
};

inline void print(const frac& f) {
	printf("%d", f.x); if (f.y != 1) printf("/%d", f.y);
}

int main() {
	
	return 0;
}
