#define BUF_SIZE 100000
#define eps 1e-12
#define ll long long
ll pow10[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000,
		1000000000, 10000000000LL, 100000000000LL, 1000000000000LL, 10000000000000LL,
		100000000000000LL, 1000000000000000LL, 10000000000000000LL, 100000000000000000LL };

char bufin[BUF_SIZE], *startin = bufin, *endin = bufin;
char bufout[BUF_SIZE], *startout = bufout;

inline char getch() {
	if (startin == endin)
		endin = (startin = bufin) + fread(bufin, 1, BUF_SIZE, stdin);
	return *startin++;
}

inline bool blank(char ch) {
	return ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t';
}

inline int getint() {
	bool sign = 0; char ch = getch(); int x = 0;
	for (; blank(ch); ch = getch());
	if (ch == '-') { sign = 1; ch = getch(); }
	for (; ch >= '0' && ch <= '9'; ch = getch())
		x = x * 10 + ch - '0';
	return sign ? -x : x;
}

inline long long getll() {
	bool sign = 0; char ch = getch(); long long x = 0;
	for (; blank(ch); ch = getch());
	if (ch == '-') { sign = 1; ch = getch(); }
	for (; ch >= '0' && ch <= '9'; ch = getch())
		x = x * 10 + ch - '0';
	return sign ? -x : x;
}

inline double getdb() {
	bool sign = 0; char ch = getch(); double x = 0;
	for (; blank(ch); ch = getch());
	if (ch == '-') { sign = 1; ch = getch(); }
	for (; ch >= '0' && ch <= '9'; ch = getch())
		x = x * 10 + ch - '0';
	if (ch == '.') {
		double tmp = 1; ch = getch();
		for (; ch >= '0' && ch <= '9'; ch = getch())
			tmp /= 10.0, x += tmp * (ch - '0');
	}
	return sign ? -x : x;
}

inline void getstr(char *s) {
	char ch = getch();
	for (; blank(ch); ch = getch());
	for (; !blank(ch); ch = getch()) *s++ = ch;
	*s = 0;
}

inline void putch(char ch) {
	if (startout == bufout + BUF_SIZE)
		fwrite(startout = bufout, 1, BUF_SIZE, stdout);
	*startout++ = ch;
}

inline void putint(int x) {
	if (x < 0) putch('-');
	if (x < 10) putch(x + '0');
	else {
		putint(x / 10);
		putch(x % 10 + '0');
	}
}

inline void putll(long long x) {
    if (x < 0) putch('-');
    if (x < 10) putch(x + '0');
    else {
        putll(x / 10);
        putch(x % 10 + '0');
    }
}

inline void putdb(double x, int y) {
	if (x < -eps) { putch('-'); x = -x; }
	x = x * pow10[y];
	long long x1 = (long long) floor(x);
	if (x - floor(x) >= 0.5) ++x1;
	long long x2 = x1 / pow10[y], x3 = x1 - x2 * pow10[y];
	putll(x2);
	if (y > 0) {
		putch('.');
		for (int i = 1; i < y && x3 * pow10[i] < pow10[y]; putch('0'), ++i);
		putll(x3);
	}
}

inline void putstr(char *s) { while (*s) putch(*s++); }

inline void flush() {
	if (startout != bufout) {
		fwrite(bufout, 1, startout - bufout, stdout);
		startout = bufout;
	}
}
