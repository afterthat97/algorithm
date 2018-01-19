#include <iostream>
#include <stdio.h>
#include <cstring>
#include <queue>
#define maxn 1
#define maxl 1000010
#define maxw 1000010
#define LETTER 26
using namespace std;

struct node {
	bool flag;
	int num, fail, match, next[LETTER];
}pool[maxn * maxl];

node* const trie = pool + 1;
int tsize, n;
char w[maxw], t[maxl];

inline void init() {
	tsize = 0;
	memset(pool, 0, 2 * sizeof(node));
	trie[0].fail = -1;
}

inline int convert(char ch) { return ch - 'a'; }
inline char convert2(int x) { return x + 'a'; }

inline void ins(const char *s) {
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

inline void build() {
	queue<int> q; q.push(0);
	while (!q.empty()) {
		int cnt = q.front(); q.pop();
		for (int i = 0; i < LETTER; i++) {
			int &pos = trie[cnt].next[i];
			if (pos) {
				q.push(pos);
				trie[pos].fail = trie[trie[cnt].fail].next[i];
				trie[pos].match = trie[pos].num ? pos : trie[trie[pos].fail].match;
			} else pos = trie[trie[cnt].fail].next[i];
		}
	}
}

inline int query(const char* s) {
	int tot = 0, cnt = 0;
	for (int i = 0; s[i]; i++) {
		cnt = trie[cnt].next[convert(s[i])];
		for (int tmp = trie[cnt].match; tmp;) {
			//ans += trie[tmp].num;
			if (trie[tmp].num && trie[tmp].flag == 0) {
				trie[tmp].flag = 1;
				++tot;
			} else if (trie[tmp].flag) break;
			tmp = trie[trie[tmp].fail].match;
		}
	}
	return tot;
}

int main() {
	while (scanf("%d", &n) == 1) {
        init();
        for (int i = 0; i < n; i++) {
            scanf("%s", t);
            ins(t);
        }
        scanf("%s", w);
		build();
		printf("%d\n", query(w));
    }
    return 0;
}
