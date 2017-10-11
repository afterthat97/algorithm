#include <iostream>
#include <cstdio>
#include <stdlib.h>
using namespace std;

void qsort(int *begin, int *end) {
    if (end - begin <= 1) return;
    int key = *(begin + rand() % (end - begin - 1));
    int *i = begin, *j = end - 1;
    while (i <= j) {
        while (*i < key) i++;
        while (*j > key) j--;
        if (i <= j) swap(*(i++), *(j--));
    }
    qsort(begin, i);
    qsort(i, end);
}

void mid_qsort(int* begin, int* end) {//faster when random
    if (end - begin <= 1) return;
    int key = *(begin + ((end - begin - 1) >> 1));
    int *i = begin, *j = end - 1;
	while (i <= j) {
        while (*i < key) i++;
        while (*j > key) j--;
        if (i <= j) swap(*(i++), *(j--));
	}
    mid_qsort(begin, i);
    mid_qsort(i, end);
 }

int main() {
	srand(2017);
	int n;
    scanf("%d", &n);
    int *arr = new int[n];
    for (int i = 0; i < n; i++)
        arr[i] = rand();
    clock_t start = clock();
	qsort(arr, arr + n);
	//mid_qsort(arr, arr + n);
	clock_t end = clock();

    cout << (end - start) / 1000 << "ms\n";
    return 0;
}
