#include <stdio.h>
#include <cstring>
#include <iostream>
#include <algorithm>
#define maxn 4005
#define maxl 105
#define LETTER 26
using namespace std;

struct node {
	int num, next[LETTER];
}trie[maxn * maxl];

int tsize;

inline void init() {
	tsize = 0;
	memset(&trie[0], 0, sizeof(node));
}

inline int convert(char ch) {
	return ch - 'a';
}

inline char convert2(int x) {
	return x + 'a';
}

inline void ins(const char* s) {
    int cnt = 0;
    for (int i = 0; s[i]; i++) {
		int &pos = trie[cnt].next[convert(s[i])];
		if (pos == 0) {
			pos = ++tsize;
			memset(&trie[tsize], 0, sizeof(node));
		}
		cnt = pos;
    }
	++trie[cnt].num;
}

inline int query(const char* s) {
	int cnt = 0;
	for (int i = 0; s[i]; i++) {
		cnt = trie[cnt].next[convert(s[i])];
		if (cnt == 0) return 0; // Not found
	}
	return trie[cnt].num;
}

inline void del(const char* s) {
	int cnt = 0;
	for (int i = 0; s[i]; i++)
		cnt = trie[cnt].next[convert(s[i])];
	--trie[cnt].num;
}

char str[maxl];

void dfs(int cnt, int h) {
	if (trie[cnt].num) {
		str[h] = 0;
		printf("%s: %d\n", str, trie[cnt].num);
	}
	for (int i = 0; i < LETTER; i++) {
		if (trie[cnt].next[i]) {
			str[h] = convert2(i);
			dfs(trie[cnt].next[i], h + 1);
		}
	}
}

int main() {
    return 0;
}
