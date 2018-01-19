#include <iostream>
#include <stdio.h>
#define MAXN 1000000
using namespace std;

int heap[MAXN + 1], heapsize, n;
//heap[]: start from 1 to heapsize; heap[1] is the MAX one.

inline void up(int x) {
	int fa = x >> 1, tmp = heap[x];
	while (fa) {
		if (tmp > heap[fa])//cmp
			heap[x] = heap[fa];
		else break;
		x = fa; fa = x >> 1;
	}
	heap[x] = tmp;
}

inline void down(int x) {
	int ch = x << 1, tmp = heap[x];
	while (ch <= heapsize) {
		if (ch < heapsize && heap[ch + 1] > heap[ch]) ch++;//cmp
		if (heap[ch] > tmp)//cmp
			heap[x] = heap[ch];
		else break;
		x = ch; ch = x << 1;
	}
	heap[x] = tmp;
}

inline void push(int val) {
	heap[++heapsize] = val;
	up(heapsize);
}

inline int top() { return heap[1]; }
inline void pop() {
	heap[1] = heap[heapsize--];
	down(1);
}

void build() {
	heapsize = n;
	for (int i = n; i > 0; i--)
		down(i);
}

int main() {
	cin >> n;
	for (int i = 0; i < n; i++) {
		int x;
		scanf("%d", &x);
		push(x);
	}
	for (int i = 0; i < n; i++) {
		printf("%d ", top());
		pop();
	}
	cout << endl;
	return 0;
}
