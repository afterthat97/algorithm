//LA 3942
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <string>
#include <math.h>
#include <stdlib.h>
#define maxn 4005
#define maxl 105
#define maxw 300005
using namespace std;

int size, trie[maxn * maxl][30], cnt[maxw];
bool flag[maxn * maxl];
char w[maxw];

void insert(char* s) {
    int p = 0, len = strlen(s);
    for (int i = 0; i < len; i++) {
        int c = s[i] - 'a';
        if (!trie[p][c])
            trie[p][c] = ++size;
        p = trie[p][c];
    }
    flag[p] = 1;
}

int main() {
    int idx = 0;
    while (scanf("%s", w) == 1) {
        size = 0;
        memset(trie, 0, sizeof trie);
        memset(cnt, 0, sizeof cnt);
        memset(flag, 0, sizeof flag);
        int n; char s[maxl];
        scanf("%d", &n);
        for (int i = 0; i < n; i++) {
            scanf("%s", s);
            insert(s);
        }
        int len = strlen(w); cnt[len] = 1;
        for (int i = len - 1; ~i; i--) {
            int p = 0;
            for (int j = i; j < len; j++) {
                int c = w[j] - 'a';
                if (!trie[p][c]) break;
                p = trie[p][c];
                if (flag[p])
                    cnt[i] = (cnt[i] + cnt[j + 1]) % 20071027;
            }
        }
        printf("Case %d: %d\n", ++idx, cnt[0]);
    }
    return 0;
}
