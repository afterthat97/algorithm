struct node {
	ll setv, sumv;
	node(): sumv(0), setv(-INF) {}
};

int n, m, _n;
node* tree;

void init() {
	_n = 1;
	while (_n < n) _n <<= 1;
	tree = new node[_n << 1];
	for (int i = _n; i < _n + n; i++) {/*read tree[i]*/}
	for (int p = _n >> 1; p; p >>= 1)
		for (int i = p; i < (p << 1); i++) {
			int lt = i << 1, rt = (i << 1) + 1;
			//tree[i].sumv = tree[lt].sumv + tree[rt].sumv;
		}
}

inline void maintain(int k, int l, int r) {}

inline void push_down(int k) {}

inline void flag(int k, int v) {}

inline void update(int k, int l, int r, int a, int b, int v) {
	int lt = k << 1, rt = (k << 1) + 1;
	if (a <= l && r <= b)
		flag(k, v);
	else {
		push_down(k);
		int mid = (l + r) >> 1;
		if (a <= mid) update(lt, l, mid, a, b, v); 
			else maintain(lt, l, mid);
		if (mid < b) update(rt, mid + 1, r, a, b, v); 
			else maintain(rt, mid + 1, r);
	}
	maintain(k, l, r);
}

inline int query(int k, int l, int r, int a, int b) {
	if (r < a || l > b) return 0;//or INF, -INF
	//if flag...(e.g. setv)
	if (a <= l && r <= b) {}
	int mid = (r + l) >> 1, lt = k << 1, rt = (k << 1) + 1;
	return query(lt, l, mid, ans) + query(rt, mid + 1, r, ans);
}
