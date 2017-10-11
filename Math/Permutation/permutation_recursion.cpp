#include <iostream>
using namespace std;

int a[100], n£»

void permutation(int x) {
	if (x == n - 1) {
		for (int i = 0; i < n; i++)
			cout << a[i] << ' ';
		cout << endl;
		return;
	}
	int dup[100] = {};
	for (int i = x; i < n; i++) {
		if (dup[a[i]] == 1) continue;
		dup[a[i]] = 1;
		swap(a[i], a[x]);
		permutation(x + 1);
		swap(a[i], a[x]);
	}
}

int main() {
	n = 12;
	for (int i = 0; i < n; i++)
		a[i] = i + 1;
	permutation(0);
	return 0;
}
