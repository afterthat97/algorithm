#include <stdio.h>
#include <string.h>
#include <iostream>
#include <math.h>
#define max(x, y) ((x) < (y) ? (y) : (x))
#define min(x, y) ((x) > (y) ? (y) : (x))
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

class ubint {
public:
    int len, s[maxlen];
    void clean() { while (len > 1 && s[len - 1] == 0) len--; }
    void inc() {
        s[0]++; for (int i = 0; i < len && s[i] == 10; s[i++] = 0) s[i + 1]++;
        if (s[len]) len++;
    }
    void dec() {
        s[0]--; for (int i = 0; i < len && !(~s[i]); s[i++] = 9) s[i + 1]--;
        if (s[len - 1] == 0) len--;
    }
    int cmp(const ubint& a) {
        if (len != a.len) return len - a.len;
        for (int i = len - 1; ~i; i--)
            if (s[i] != a.s[i]) return s[i] - a.s[i];
        return 0;
    }
    
public:
    ubint(const char* str) {
        memset(s, 0, sizeof s); len = (int)strlen(str);
        for (int i = 0; str[i]; i++) s[len - i - 1] = str[i] - '0';
    }
    ubint(int v = 0) {
        memset(s, 0, sizeof s);
        if (!v) { len = 1; s[0] = 0; return; }
        for (len = 0; v; v /= 10) s[len++] = v % 10;
    }
    ubint(const ubint& a) {
        len = a.len; memcpy(s, a.s, sizeof s); clean();
    }
    ubint& operator = (const ubint& a) {
        len = a.len; memcpy(s, a.s, sizeof s); clean(); return *this;
    }
    int to_int() {
        int ans = 0;
        for (int i = 0, b = 1; i < len; i++, b *= 10) ans += b * s[i];
        return ans;
    }
    char* to_str() {
        char *ans = new char[len + 3]; int cnt = 0; clean();
        for (int i = len - 1; ~i; i--) ans[cnt++] = s[i] + '0';
        ans[cnt++] = 0; return ans;
    }
    friend ubint operator + (ubint a, ubint b);
    friend ubint operator - (ubint a, ubint b);
    friend ubint operator * (ubint a, ubint b);
    friend ubint operator * (ubint a, int b);
    friend ubint operator / (ubint a, ubint b);
    friend ubint operator / (ubint a, int b);
    friend ubint operator % (ubint a, ubint b);
    friend ubint operator % (ubint a, int b);
    ubint operator += (ubint a) { *this = *this + a; return *this; }
    ubint operator -= (ubint a) { *this = *this - a; return *this; }
    ubint operator *= (ubint a)	{ *this = *this * a; return *this; }
    ubint operator *= (int b);
    ubint operator /= (ubint a) { *this = *this / a; return *this; }
    bool operator < (ubint a)  	{ return cmp(a) < 0; }
    bool operator > (ubint a)  	{ return cmp(a) > 0; }
    bool operator <= (ubint a) 	{ return cmp(a) <= 0; }
    bool operator >= (ubint a) 	{ return cmp(a) >= 0; }
    bool operator == (ubint a) 	{ return cmp(a) == 0; }
    bool operator != (ubint a) 	{ return cmp(a) != 0; }
    ubint operator ++ (int)     { ubint tmp(*this); inc(); return tmp; }
    ubint operator ++ ()		{ inc(); return *this; }
    ubint operator -- (int)		{ ubint tmp(*this); dec(); return tmp; }
    ubint operator -- ()		{ dec(); return *this; }
    friend std::ostream& operator << (std::ostream&, const ubint&);
    friend std::istream& operator >> (std::istream&, ubint&);
};

ubint operator + (ubint a, ubint b) {
    ubint ans; ans.len = max(a.len, b.len) + 1;
    for (int i = 0; i < a.len || i < b.len; i++) {
        ans.s[i] += a.s[i] + b.s[i];
        if (ans.s[i] > 9) { ans.s[i] -= 10; ans.s[i + 1]++; }
    }
    ans.clean(); return ans;
}

ubint operator - (ubint a, ubint b) {
    ubint ans; ans.len = a.len + 1;
    for (int i = 0; i < a.len; i++) {
        ans.s[i] += a.s[i] - b.s[i];
        if (ans.s[i] < 0) { ans.s[i] += 10; ans.s[i + 1]--; }
    }
    ans.clean(); return ans;
}

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

ubint operator * (ubint a, ubint b) {
    if (b.len < 9) return a * b.to_int();
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
    ubint ans; ans.len = a.len + b.len + 1;
    for (int i = 0; i < min(_n, ans.len); i++) ans.s[i] = tmpa[i].re + 0.01;
    for (int i = 0; i < ans.len - 2; ans.s[i++] %= 10)
        ans.s[i + 1] += ans.s[i] / 10;
    ans.clean(); return ans;
}

ubint operator * (ubint a, int b) {
    if (b > 99999999) return a * ubint(b);
    ubint ans; ans.len = a.len + 10;
    for (int i = 0; i < a.len; i++)
        ans.s[i] += a.s[i] * b;
    for (int i = 0; i < ans.len - 2; ans.s[i++] %= 10)
        ans.s[i + 1] += ans.s[i] / 10;
    ans.clean(); return ans;
}

ubint ubint::operator *= (int b) {
    if (b > 99999999) return (*this) * ubint(b);
    for (int i = 0; i < len; i++) s[i] *= b;
    len += 10;
    for (int i = 0; i < len - 2; s[i++] %= 10)
        s[i + 1] += s[i] / 10;
    clean(); return *this;
}

ubint operator / (ubint a, ubint b) {
    if (b.len < 10) return a / b.to_int();
    ubint invb = 1; int lim = a.len + 5, fac = b.len;
    for (int i = 0; i < 100; i++) {
        ubint two(0), last = invb; two.len = fac + 1; two.s[fac] = 2;
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
    ubint ans = a * invb; if (ans.len < fac) return 0;
    for (int i = 0; i < ans.len - fac; i++)
        ans.s[i] = ans.s[i + fac];
    for (int i = ans.len - fac; i < ans.len; i++)
        ans.s[i] = 0;
    ans.len -= fac; if (ans.len == 0) ans = 0;
    ubint mod = a - b * ans;
    while (mod >= b) { ans++; mod -= b;}
    ans.clean(); return ans;
}

ubint operator / (ubint a, int b) {
    ubint ans; ans.len = a.len; ll mod = 0;
    for (int i = a.len - 1; ~i; i--) {
        mod *= 10; mod += a.s[i];
        ans.s[i] = (int)(mod / (ll)b); mod %= b;
    }
    ans.clean(); return ans;
}

ubint operator % (ubint a, ubint b) {
    if (b.len < 10) return a % b.to_int();
    ubint invb = 1; int lim = a.len + 5, fac = b.len;
    for (int i = 0; i < 100; i++) {
        ubint two(0), last = invb; two.len = fac + 1; two.s[fac] = 2;
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
    ubint ans = a * invb; if (ans.len < fac) return 0;
    for (int i = 0; i < ans.len - fac; i++)
        ans.s[i] = ans.s[i + fac];
    for (int i = ans.len - fac; i < ans.len; i++)
        ans.s[i] = 0;
    ans.len -= fac; if (ans.len == 0) ans = 0;
    ubint mod = a - b * ans;
    while (mod >= b) { ans++; mod -= b;}
    ans.clean(); return mod;
}

ubint operator % (ubint a, int b) {
    ubint ans; ans.len = a.len; ll mod = 0;
    for (int i = a.len - 1; ~i; i--) {
        mod *= 10; mod += a.s[i];
        ans.s[i] = (int)(mod / (ll)b); mod %= b;
    }
    return int(mod);
}

std::ostream& operator << (std::ostream& out, const ubint& a) {
    for (int i = a.len - 1; ~i; i--) out << (char)(a.s[i] + '0');
    return out;
}

std::istream& operator >> (std::istream& cin, ubint& a) {
    char s[maxlen]; cin >> s; a = ubint(s);
    return cin;
}

int main() {
    ubint a, b; cin >> a >> b;
    cout << a % b << endl;
    return 0;
}
