#include <iostream>
#include <stdio.h>
#include <cstring>
#define maxn 105
using namespace std;

int f[maxn];

void getf(char *p) {
	int len = strlen(p);
	memset(f, -1, sizeof f);
	for (int i = 0, j = -1; i < len;) {
		while (~j && p[i] != p[j]) j = f[j];
		i++; j++;
		f[i] = p[i] == p[j] ? f[j] : j;
	}
}

int kmp(char *t, char *p) {
	int len = strlen(t), lenp = strlen(p);
	getf(p);
	for (int i = 0, j = 0; i < len;) {
		while (~j && t[i] != p[j]) j = f[j];
		i++; j++;
		if (j == lenp) {
			return i - j;
			// j = 0;
		}
	}
	return -1;
}

int main() {
	char t[100], p[10];
	cout << kmp(t, p);
	return 0;
}
