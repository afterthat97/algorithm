#include <iostream>
#include <string>
using namespace std;

string rm_blank(string s) {
	int j = 0;
	for (int i = 0; i < s.size();i++)
		if (s[i] != ' ') {
			if (i != j)
				s[j] = s[i];
			j++;
		}
	s.erase(j, s.size() - j);
	return s;
}

int main() {
	string a = "  i have   a dream";
	cout << rm_blank(a);
	return 0;
}
