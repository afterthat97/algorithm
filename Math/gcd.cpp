#include <iostream>
#include <stdio.h>
using namespace std;

int gcd(int a, int b) {
	while (b) {
		int tmp = a % b;
		a = b; b = tmp;
	}
	return a;
}

//find x and y to satisfy ax + by = gcd(a, b)
//x will <= b, y will <= a
int extgcd(int a, int b, int& x, int& y) {
	if (b == 0) {
		x = 1; y = 0; return a;
	} else {
		int r = extgcd(b, a % b, y, x);
		y -= (a / b) * x;
		return r;
	}
}

ll inv(ll a, ll m) {
	ll x, y;
	extgcd(a, m, x, y);
	return (m + x % m) % m;
}

int main() {
	int a, b;
	while (scanf("%d %d", &a, &b) == 2) {
		int x, y;
		cout << extgcd(a, b, x, y) << ' ' << x << ' ' << y << endl;
	}
	return 0;
}
