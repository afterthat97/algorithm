#include <iostream>
#include <stdio.h>
#include <cstring>
#define BASE (1 << 16)
#define MAXN 10000005
using namespace std;

int a[MAXN];
int tmp[MAXN], bkt[BASE + 5];
void radix_sort(int* begin, int* end) {
	int n = end - begin;
	for (int k = BASE - 1, i = 0; i < 2; i++, k <<= 16) {
		for (int j = 0; j < n; j++) 
			bkt[((*(begin + j)) & k) >> (i * 16)]++;
		for (int j = 1; j < BASE; j++) 
			bkt[j] += bkt[j - 1];
		for (int j = n - 1; j >= 0; j--)
			tmp[--bkt[((*(begin + j)) & k) >> (i * 16)]] = *(begin + j);
		memcpy(begin, tmp, n * sizeof(int));
		memset(bkt, 0, sizeof bkt);
	}
}

int main() {
	srand(0);

    radix_sort(a, a + MAXN);
    return 0;
}
