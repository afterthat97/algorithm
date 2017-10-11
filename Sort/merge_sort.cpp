#include <iostream>
#include <cstring>
#include <stdio.h>
#define MAXN 10000005
using namespace std;

int temp[MAXN];

void merge_sort(int* a, int low, int high) {
	if (low >= high) return;
	int mid = (low + high) >> 1;
	merge_sort(a, low, mid);
	merge_sort(a, mid + 1, high);
	
	int i = low, j = mid + 1, size = 0;
	for (; (i <= mid) && (j <= high); size++)
		if (a[i] < a[j]) temp[size] = a[i++];
		else temp[size] = a[j++];
	memcpy(temp + size, a + i, (mid - i + 1) << 2);
	memcpy(a + low, temp, (size + mid - i + 1) << 2);
}

int main() {
	int n;
	cin >> n;
	int* a = new int[n];
	srand(0);
	for (int i = 0; i < n; i++)
		a[i] = rand();
	clock_t start = clock();
	merge_sort(a, 0, n - 1);
	clock_t end = clock();
	
	cout << (end - start) / 1000 << "ms\n";
	return 0;
}
