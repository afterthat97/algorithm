#include <iostream>
#include <string>
using namespace std;

void rev_str(string &s, int from, int to) {
	while (from < to) {
		char t = s[from];
		s[from++] = s[to];
		s[to--] = t;
	}
}

string left_rotate_str(string s, int m) {
	int n = s.size();
	m %= n;
	rev_str(s, 0, m - 1);
	rev_str(s, m, n - 1);
	rev_str(s, 0, n - 1);
	return s;
}

int main() {
	string a = "abcdef";
	cout << left_rotate_str(a, 21);
	return 0;
}
