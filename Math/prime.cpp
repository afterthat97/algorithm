#include <iostream>
#include <stdio.h>
#include <vector>
#include <map>
#include <cstring>
#define ll long long
#define MAXN 100005
using namespace std;

ll prime[MAXN];
int pnum;
bool notp[MAXN];//e.g. notp[2] = 0, notp[4] = 1
bool notp_big[MAXN];//for segment sieve

bool judge(ll x) {//logN
	for (ll i = 2; i * i <= x; i++)
		if (x % i == 0) return 0;
	return (x != 1) && (x != 0);
}

vector<ll> divisor(ll x) {//logN
	vector<ll> ans;
	for (ll i = 2; i * i <= x; i++)
		if (x % i == 0) {
			ans.push_back(i);
			if (i != x / i) ans.push_back(x / i);
		}
	return ans;
}

map<ll, int> factor(ll x) {//logN
	map<ll, int> ans;
	for (ll i = 2; i * i <= x; i++)
		while (x % i == 0) {
			ans[i]++;
			x /= i;
		}
	if (x != 1) ans[x]++;
	return ans;
}

int sieve(int x) {//NloglogN ~= N
	memset(notp, 0, sizeof notp);
	pnum = 0;
	notp[0] = notp[1] = 1;
	for (int i = 2; i < x; i++)
		if (!notp[i]) {
			prime[pnum++] = (ll)i;
			for (int j = 2; i * j < x; j++)
				notp[i * j] = 1;
		}
	return pnum;
}

int segment_sieve(ll a, ll b) {//get primes in [a, b)
	pnum = 0;
	memset(notp, 0, sizeof notp);
	memset(notp_big, 0, sizeof notp_big);
	//notp_big[x - a] == 0 -> x is prime
	for (int i = 2; (ll)i * i < b; i++)
		if (!notp[i]) {
			//sieve [2, sqrt(b))
			for (int j = (i << 1); (ll)j * j < b; j += i)
				notp[j] = 1;
			//sieve [a, b)
			for (ll j = max(2LL, (a + i - 1) / i) * i; j < b; j += i)
				notp_big[j - a] = 1;
		}
	for (ll j = 0; j < b - a; j++)
		if (!notp_big[j])
			prime[pnum++] = a + j;
	return pnum;
}

int main() {
	int a, b;
	while (scanf("%d %d", &a, &b) == 2) {
		segment_sieve(a, b);
		for (int i = 0; i < pnum; i++)
			cout << prime[i] << ' ';
	}
	return 0;
}
