#include <iostream>
#include <stdio.h>
#include <cstring>
#define MAXLEN 5005
#define MOD 100000000
using namespace std;

char a[MAXLEN], b[MAXLEN];
int dp[2][MAXLEN], sum[2][MAXLEN];

int main() {
	scanf("%s %s", a + 1, b + 1);
	int len1 = strlen(a + 1), len2 = strlen(b + 1);
	for (int j = 0; j < len2; j++) sum[0][j] = 1;
	sum[1][0] = 1;
	int now = 1, pre = 0;
	for (int i = 1; i < len1; i++) {
		for (int j = 1; j < len2; j++) {
			if (a[i] == b[j]) {
				dp[now][j] = dp[pre][j - 1] + 1;
				sum[now][j] = sum[pre][j - 1]
					+ (dp[now][j] == dp[pre][j]) * sum[pre][j]
					+ (dp[now][j] == dp[now][j - 1]) * sum[now][j - 1];
			} else {
				dp[now][j] = max(dp[pre][j], dp[now][j - 1]);
				sum[now][j] = 0
					+ (dp[now][j] == dp[pre][j]) * sum[pre][j]
					+ (dp[now][j] == dp[now][j - 1]) * sum[now][j - 1]
					- (dp[now][j] == dp[pre][j - 1]) * sum[pre][j - 1];
			}
			sum[now][j] %= MOD;
		}
		swap(now, pre);
	}
	cout << dp[pre][len2 - 1] << endl
		<< sum[pre][len2 - 1] << endl;

	return 0;
}

