#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <math.h>
#define max(x, y) ((x) < (y) ? (y) : (x))
#define min(x, y) ((x) > (y) ? (y) : (x))
#define abs(x) ((x) < 0 ? (-x) : (x))
#define maxlen 40005
#define ll long long
#define pi (acos(-1))
using namespace std;

struct comp {
	double re, im;
	comp(double r = 0, double i = 0): re(r), im(i) {}
	comp operator + (comp x) { return comp(re + x.re, im + x.im); }
	comp operator - (comp x) { return comp(re - x.re, im - x.im); }
	comp operator * (comp x) { 
		return comp(re * x.re - im * x.im, re * x.im + im * x.re); 
	}
};

class bint {
public:
	int len, s[maxlen];
	void inc() { 
		int flag = neg(); s[0] += 1; 
		for (int i = 0; i < len && s[i] == 10; s[i++] = 0) s[i + 1]++;
		if (neg() > flag) s[len++] = 0;
	}
	void dec() {
		int flag = neg(); s[0] -= 1; 
		for (int i = 0; i < len && !(~s[i]); s[i++] = 9) s[i + 1]--;
		if (neg() < flag) s[len++] = 9;
	}
	void inv() { for (int i = 0; i < len; i++) s[i] = 9 - s[i]; inc(); }
	int neg() const { return s[len - 1] > 4 ? 9 : 0; }
	void clean() { 
		while (len > 2 && s[len - 1] == 0 && s[len - 2] < 5) len--; 
		while (len > 2 && s[len - 1] == 9 && s[len - 2] > 4) len--;
	}

public:
	bint(const char* raw) {
		memset(s, 0, sizeof s);
		const char *str = raw[0] == '-' ? raw + 1 : raw;
		len = (int)strlen(str) + 1;
		for (int i = 0; str[i]; i++) s[len - i - 2] = str[i] - '0';
		if (raw[0] == '-') inv();
	}
	bint(int v = 0) {
		memset(s, 0, sizeof s); 
		int val = abs(v); if (!val) { len = 2; return; }
		for (len = 0; val; val /= 10) s[len++] = val % 10; len++;
		if (v < 0) inv();
	}
	bint(const bint& a) { len = a.len; memcpy(s, a.s, sizeof(int) * len); }
	bint& operator = (const bint& a) { 
		len = a.len; memcpy(s, a.s, sizeof(int) * len); return *this; 
	}
	int to_int() {
		int ans = 0; bint tmp = *this; if (neg()) tmp.inv();
		for (int i = 0, b = 1; i < len; i++, b *= 10) ans += b * tmp.s[i];
		return neg() ? -ans : ans;
	}
	char* to_str() {
		char *ans = new char[len + 5]; int cnt = 0;
		bint tmp = *this; if (neg()) { tmp.inv(); ans[cnt++] = '-'; }
		for (int i = tmp.len - 1; ~i; i--) 
			if (tmp.s[i] || i == 0) {
				for (int j = i; ~j; j--) ans[cnt++] = tmp.s[j] + '0'; break;
		}
		ans[cnt++] = 0; return ans;
	}
	friend bint operator + (bint a, bint b);
	friend bint operator - (bint a, bint b);
	friend bint operator * (bint a, bint b);
	friend bint operator * (bint a, int b);
	friend bint operator / (bint a, bint b);
	friend bint operator / (bint a, int b);
	friend bint operator % (bint a, bint b);
	friend bint operator % (bint a, int b);
	friend bint operator << (bint a, int k);
	friend bint operator >> (bint a, int k);
	bint operator <<= (int k);
	bint operator >>= (int k);
	bint operator += (bint a) 	{ *this = *this + a; return *this; }
	bint operator -= (bint a) 	{ *this = *this - a; return *this; }
	bint operator *= (bint a)	{ *this = *this * a; return *this; }
	bint operator *= (int b);
	bint operator /= (bint a) 	{ *this = *this / a; return *this; }
	bool operator < (bint a)  	{ return (*this - a).neg() > 0; }
	bool operator > (bint a)  	{ return (a - *this).neg() > 0; }
	bool operator <= (bint a) 	{ return (a - *this).neg() <= 0; }
	bool operator >= (bint a) 	{ return (*this - a).neg() <= 0; }
	bool operator == (bint a) 	{ bint t = *this - a; return t.len < 10 && !t.to_int(); }
	bool operator != (bint a) 	{ return !(*this == a); }
	bint operator ++ (int)     	{ bint tmp(*this); inc(); return tmp; }
	bint operator ++ ()			{ inc(); return *this; }
	bint operator -- (int)		{ bint tmp(*this); dec(); return tmp; }
	bint operator -- ()			{ dec(); return *this; }
	bint operator - () 			{ bint tmp(*this); tmp.inv(); return tmp; }
	friend std::ostream& operator << (std::ostream&, const bint&);
	friend std::istream& operator >> (std::istream&, bint&);
};

bint bint::operator <<= (int k) {	
	for (int i = len - 1; ~i; i--) s[i + k] = s[i];
	len += k; clean(); return *this;	
}

bint bint::operator >>= (int k) {
	for (int i = 0; i < len - k; i++) s[i] = s[i + k];
	len -= k; if (len <= 0) *this = 0; clean(); return *this;
}

bint operator + (bint a, bint b) {
	bint ans; ans.len = max(a.len, b.len) + 1;
	for (int i = a.len, ta = a.neg(); i < ans.len; i++) a.s[i] = ta;
	for (int i = b.len, tb = b.neg(); i < ans.len; i++) b.s[i] = tb;
	for (int i = 0; i <= a.len || i <= b.len; i++) {
		ans.s[i] += a.s[i] + b.s[i];
		if (ans.s[i] > 9) { ans.s[i] -= 10; ans.s[i + 1]++; }
	}
	ans.clean(); return ans;
}

bint operator - (bint a, bint b) { b.inv(); return a + b; }

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

bint operator * (bint a, bint b) {
	if (b.len < 9) return a * b.to_int();
	bool flag = (a.neg() > 0) ^ (b.neg() > 0);
	if (a.neg()) a.inv(); if (b.neg()) b.inv();
	int n = max(a.len, b.len) << 1, _n = 1, t = -1;
	while (_n < n) { _n <<= 1; t++; }
	int* g = new int[_n]; g[0] = 0;
	comp *tmpa = new comp[_n], *tmpb = new comp[_n];
	for (int i = 1; i < _n; i++)
		g[i] = (g[i >> 1] >> 1) | ((i & 1) << t);
	for (int i = 0; i < a.len; i++) tmpa[i] = comp(a.s[i], 0);
	for (int i = 0; i < b.len; i++) tmpb[i] = comp(b.s[i], 0);
	FFT(tmpa, g, _n, 1); FFT(tmpb, g, _n, 1);
	for (int i = 0; i < _n; i++) tmpa[i] = tmpa[i] * tmpb[i];
	FFT(tmpa, g, _n, -1);
	bint ans; ans.len = a.len + b.len + 1;
	for (int i = 0; i < min(_n, ans.len); i++) ans.s[i] = tmpa[i].re + 0.01;
	for (int i = 0; i < ans.len - 2; ans.s[i++] %= 10) 
		ans.s[i + 1] += ans.s[i] / 10;
	ans.clean(); return flag ? -ans : ans;
}

bint operator * (bint a, int b) {
	if (b > 99999999) return a * bint(b);
	bool flag = (a.neg() > 0) ^ (b < 0);
	if (a.neg()) a.inv(); if (b < 0) b = -b;
	bint ans; ans.len = a.len + 10;
	for (int i = 0; i < a.len; i++)
		ans.s[i] += a.s[i] * b;
	for (int i = 0; i < ans.len - 2; ans.s[i++] %= 10)
		ans.s[i + 1] += ans.s[i] / 10;
	ans.clean(); return flag ? -ans : ans;
}

bint bint::operator *= (int b) {
	if (b > 99999999) return (*this) * bint(b);
	bool flag = (neg() > 0) ^ (b < 0);
	if (neg()) inv(); if (b < 0) b = -b;
	for (int i = 0; i < len; i++) s[i] *= b;
	len += 10;
	for (int i = 0; i < len - 2; s[i++] %= 10)
		s[i + 1] += s[i] / 10;
	clean(); return flag ? -(*this) : *this;
}

bint operator / (bint a, bint b) {
	if (b.len < 10) return a / b.to_int();
	bool flag = (a.neg() > 0) ^ (b.neg() > 0);
	if (a.neg()) a.inv(); if (b.neg()) b.inv();
    bint invb = 1; int lim = a.len + 5, fac = b.len;
    for (int i = 0; i < 100; i++) {
        bint two(0), last = invb; two.len = fac + 1; two.s[fac] = 2;
        invb = invb * (two - b * invb); fac <<= 1;
        if (invb.len > lim) {
            fac -= invb.len - lim;
            for (int i = 0; i < lim; i++)
                invb.s[i] = invb.s[i + invb.len - lim];
            for (int i = lim; i < invb.len; i++)
                invb.s[i] = 0;
            invb.len = lim;
        }
        if (last == invb) break;
    }
    bint ans = a * invb; if (ans.len < fac) return 0;
    for (int i = 0; i < ans.len - fac; i++)
        ans.s[i] = ans.s[i + fac];
    for (int i = ans.len - fac; i < ans.len; i++)
        ans.s[i] = 0;
    ans.len -= fac; if (ans.len == 0) ans = 0;
    bint mod = a - b * ans;
    while (mod >= b) { ans++; mod -= b;}
	ans.clean(); return flag ? -ans : ans;
}

bint operator / (bint a, int b) {
	bool flag = (a.neg() > 0) ^ (b < 0);
	if (a.neg()) a.inv(); if (b < 0) b = -b;
	bint ans; ans.len = a.len;
	long long mod = 0;
	for (int i = a.len - 1; ~i; i--) {
		mod *= 10; mod += a.s[i];
		ans.s[i] = (int)(mod / (ll)b); mod %= b;
	}
	ans.clean(); return flag ? -ans : ans;
}

bint operator % (bint a, bint b) {
	if (b.len < 10) return a % b.to_int();
	bool flag = (a.neg() > 0);
	if (a.neg()) a.inv(); if (b.neg()) b.inv();
    bint invb = 1; int lim = a.len + 5, fac = b.len;
    for (int i = 0; i < 100; i++) {
        bint two(0), last = invb; two.len = fac + 1; two.s[fac] = 2;
        invb = invb * (two - b * invb); fac <<= 1;
        if (invb.len > lim) {
            fac -= invb.len - lim;
            for (int i = 0; i < lim; i++)
                invb.s[i] = invb.s[i + invb.len - lim];
            for (int i = lim; i < invb.len; i++)
                invb.s[i] = 0;
            invb.len = lim;
        }
        if (last == invb) break;
    }
    bint ans = a * invb; if (ans.len < fac) return 0;
    for (int i = 0; i < ans.len - fac; i++)
        ans.s[i] = ans.s[i + fac];
    for (int i = ans.len - fac; i < ans.len; i++)
        ans.s[i] = 0;
    ans.len -= fac; if (ans.len == 0) ans = 0;
    bint mod = a - b * ans;
    while (mod >= b) { ans++; mod -= b;}
	mod.clean(); return flag ? -mod : mod;
}

bint operator % (bint a, int b) {
	bool flag = (a.neg() > 0);
	if (a.neg()) a.inv(); if (b < 0) b = -b;
	bint ans; ans.len = a.len;
	long long mod = 0;
	for (int i = a.len - 1; ~i; i--) {
		mod *= 10; mod += a.s[i];
		ans.s[i] = (int)(mod / (ll)b); mod %= b;
	}
	return flag ? -int(mod) : int(mod);
}

bint operator << (bint a, int k) {
	a.len += k;
	for (int i = a.len - 1; ~i; i--) a.s[i + k] = a.s[i];
	a.clean(); return a;
}

bint operator >> (bint a, int k) {
	a.len -= k;
	for (int i = 0; i < a.len; i++) a.s[i] = a.s[i + k];
	if (a.len <= 0) a = 0; a.clean(); return a;
}

std::ostream& operator << (std::ostream& out, const bint& a) {
	bint tmp = a; if (a.neg()) { out << '-'; tmp.inv(); }
	for (int i = tmp.len - 1; ~i; i--) 
		if (tmp.s[i] || i == 0) {
			for (int j = i; ~j; j--) out << (char)(tmp.s[j] + '0');
			break;
		}
	return out;
}

std::istream& operator >> (std::istream& cin, bint& a) {
	char s[maxlen]; cin >> s; a = bint(s);
	return cin;
}

int main() {
	bint a, b;
	cin >> a >> b;
	cout << a / b << endl;
	return 0;
}
