#include <iostream>
#include <stdio.h>
#define ll long long
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
ll extgcd(ll a, ll b, ll& x, ll& y) {
	if (b == 0) {
		x = 1; y = 0; return a;
	} else {
		ll r = extgcd(b, a % b, y, x);
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
	return 0;
}
