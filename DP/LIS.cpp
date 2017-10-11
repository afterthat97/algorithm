#include <stdio.h>
#define maxn 100005

int n, a[maxn], l[maxn];//l: LIS
int pre[maxn], sub[maxn] = {-1};

int LIS() {
	int len = 0;
	for (int i = 0; i < n; i++) {
		int l = 1, r = len;
		while (l <= r) {
			int mid = (l + r + 1) >> 1;
			if (a[sub[mid]] < a[i]) 
				l = mid + 1;
			else 
				r = mid - 1;
		}
		pre[i] = sub[l - 1]; 
		sub[l] = i;
		if (l > len) len = l;
	}
	return len;
}

int main() {
	scanf("%d", &n);
	for (int i = 0; i < n; i++) 
		scanf("%d", &a[i]);
	int len = LIS();
	printf("%d\n", len);
	for (int p = sub[len], cnt = len; ~p; p = pre[p])
		l[--cnt] = a[p];
	for (int i = 0; i < len; i++) 
		printf("%d ", l[i]);
	return 0;
}
