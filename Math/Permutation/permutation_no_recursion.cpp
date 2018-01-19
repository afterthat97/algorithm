//全排列，有重复，非递归版本
#include <iostream>
using namespace std;

inline void rev(int* from, int* to) {
	while (from < to) {
		swap(*from, *to);
		from++; to--;
	}
}

inline bool get_next_permutation(int* a, int size) {
	int i = size - 2;
	while ((i >= 0) && (a[i] >= a[i + 1])) i--;
	if (i < 0) return 0;
	int j = size - 1;
	while (a[j] <= a[i]) j--;
	swap(a[j], a[i]);
	rev(a + i + 1, a + size - 1);
	return 1;
}

int main() {
	int a[1005];
	int size = 12;
	for (int i = 0; i < size; i++)
		a[i] = i + 1;
	do {
		for (int i = 0; i < size; i++)
			cout << a[i] << ' ';
		cout << endl;
	} while (get_next_permutation(a, size));
	return 0;
}
