---
title: RMQ 问题总结
date: 2017-02-24 07:49:35
tags:
  - Algorithm
---

## 什么是 RMQ 问题

RMQ 全称为 Range Minimum Query，即区间最值查询。是指在给定数组 $a$ 的情况下，实现以下两种操作:

*   给定 $l$ 和 $r$，求 $a_l, a_{l + 1}, ..., a_r$ 的最小值/最大值
*   给定 $i$ 和 $x$，把 $a_i$ 的值改为 $x$（或者加上 $x$）

RSQ 全称为 Range Sum Query，即区间和查询。是指在给定数组 $a$ 的情况下，实现以下两种操作:

*   给定 $l$ 和 $r$，求 $a_l, a_{l + 1}, ..., a_r$ 的和
*   给定 $i$ 和 $x$，把 $a_i$ 的值改为 $x$（或者加上 $x$）

为了表述方便，在本文标题中将这两种类型的问题统一归为 RMQ 问题。

<!-- more -->

## 朴素做法

最容易想到的做法便是遍历，代码实现如下：

```cpp
int query(int* a, int l, int r) {
    int ans = a[l];
    for (int i = l; i <= r; i++)
        if (a[i] < ans) ans = a[i];
    return ans;
}

void update(int* a, int i, int x) {
    a[i] = x;
}
```

`query` 函数时间复杂度 $O(N)$，`update`函数时间复杂度 $O(1)$，在数据规模较大时效率偏低。

## 平方分割

分桶法是把一排物体或者平面分成桶，每个桶分别维护自己内部的信息，以达到高效计算的目的的方法。

其中，平方分割是把排成一排的 $n$ 个元素每 $\sqrt{n}$ 个分在一个桶内进行维护的方法的统称，这样的分割方法可以使对区间的操作的复杂度降至 $O(\sqrt{n})$，且同时支持 RMQ 和 RSQ 操作。

### 定义

定义结构体 `node` 为单位桶, `sumv`、`maxv`、`minv` 分别为该桶中元素的和、最大值、最小值，`dat` 为桶中所有的元素。

```cpp
struct node {
    ll sumv, minv, maxv;
    vector<int> dat;
    node() { sumv = 0; minv = INF; maxv = -INF; }
};
node* bkt;
```

### 预处理

令 `bkt_size = floor(sqrt(n))`, 把 $a$ 中的元素每 `bkt_size` 个分成一个桶, 并且计算出每个桶内的最小值、最大值、和。

```cpp
void init(int *a, int n) {
    bkt_size = floor(sqrt(n));
    bkt_n = n / bkt_size + (n % bkt_size ? 1 : 0);
    bkt = new node[bkt_n];
    for (int i = 0; i < n; i++) {
        int idx = i / bkt_size;
        if (a[i] < bkt[idx].minv) bkt[idx].minv = a[i];
        if (a[i] > bkt[idx].maxv) bkt[idx].maxv = a[i];
        bkt[idx].sumv += a[i];
        bkt[idx].dat.push_back(a[i]);
    }   
}
```

### 更新

在更新元素的值时，需要更新该元素所在桶的最大值、最小值、和。遍历一遍桶内元素即可，时间复杂度 $O(\sqrt{n})$。

```cpp
void add(int i, int x) {//给第i个元素的值加上x
    int idx = i / bkt_size;
    bkt[idx].sumv += x;
    bkt[idx].maxv = -INF;
    bkt[idx].minv = INF;
    bkt[idx].dat[i % bkt_size] += x;
    for (int k = 0; k < bkt[idx].dat.size(); k++) {
        if (bkt[idx].dat[k] < bkt[idx].minv) bkt[idx].minv = bkt[idx].dat[k];
        if (bkt[idx].dat[k] > bkt[idx].maxv) bkt[idx].maxv = bkt[idx].dat[k];
    }
}
```

### 查询

遍历相关的桶：

*   如果桶完全包含在区间内，则查询桶的最小值
*   如果元素所在的桶不完全被区间包含，则逐个检查最小值

它们的最小值就是整个区间的最小值了，时间复杂度 $O(\sqrt{n})$。

```cpp
node query(int l, int r) {
    int idx1 = l / bkt_size, idx2 = r / bkt_size;
    node ans;
    if (idx1 == idx2) {
        for (int k = l % bkt_size; k <= r % bkt_size; k++) {
            if (bkt[idx1].dat[k] > ans.maxv) ans.maxv = bkt[idx1].dat[k];
            if (bkt[idx1].dat[k] < ans.minv) ans.minv = bkt[idx1].dat[k];
            ans.sumv += bkt[idx1].dat[k];
        }   
        return ans;
    }
    for (int i = idx1 + 1; i < idx2; i++) {
        if (bkt[i].maxv > ans.maxv) ans.maxv = bkt[i].maxv;
        if (bkt[i].minv < ans.minv) ans.minv = bkt[i].minv;
        ans.sumv += bkt[i].sumv;
    }
    for (int k = l % bkt_size; k < bkt[idx1].dat.size(); k++) {
        if (bkt[idx1].dat[k] > ans.maxv) ans.maxv = bkt[idx1].dat[k];
        if (bkt[idx1].dat[k] < ans.minv) ans.minv = bkt[idx1].dat[k];
        ans.sumv += bkt[idx1].dat[k];
    }
    for (int k = 0; k <= r % bkt_size; k++) {
        if (bkt[idx2].dat[k] > ans.maxv) ans.maxv = bkt[idx2].dat[k];
        if (bkt[idx2].dat[k] < ans.minv) ans.minv = bkt[idx2].dat[k];
        ans.sumv += bkt[idx2].dat[k];
    }
    return ans; 
}
```

## 稀疏表

基于稀疏表（Sparse Table）的 RMQ 实现也比较常见，稀疏表可以做到 $O(nlogn)$ 预处理以及 $O(1)$ 查询，但仅支持 RMQ 查询。对于数列 `a = {5, 3, 7, 9, 6, 4, 1, 2}` 构建的稀疏表如下所示：

| st | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
|----|---|---|---|---|---|---|---|---|
| 0  | 5 | 3 | 7 | 9 | 6 | 4 | 1 | 2 |
| 1  | 3 | 3 | 7 | 6 | 4 | 1 | 1 |   |
| 2  | 3 | 3 | 4 | 1 | 1 |   |   |   |
| 3  | 1 |   |   |   |   |   |   |   |

其中 $st[i][j]$ 表示的是 $a[j], a[j + 1], ..., a[j + 2^i - 1]$ 的最小值。

### 预处理

稀疏表在预处理的时间复杂度和空间复杂度都达到了 $O(nlogn)$。

```cpp
void init(int* a, int n) {
    for (int j = 0; j < n; j++) st[0][j] = a[j];
    for (int i = 1; (1 << i) <= n; i++)
        for (int j = 0; j <= n - (1 << i); j++)
            st[i][j] = min(st[i - 1][j], st[i - 1][j + (1 << (i - 1))]);
}
```

### RMQ 查询

于是，在给定 $x$ 和 $y$ 的情况下，通过这个表可以快速求出 $a[x], a[x + 1], ..., a[y]$ 的最小值：

*   首先，求出使得 $2^i$ 不超过 $y - x + 1$ 的最大的 $i$
*   然后，$min(t[i][x], t[i][y - 2^i + 1])$ 就是 $a[x], a[x + 1], ..., a[y]$ 的最小值了

查询操作时间复杂度 $O(1)$。

```cpp
int query(int l, int r) {//[l, r]
    int i = 0;
    while ((1 << (i + 1)) <= r - l + 1) i++;
    return min(st[i][l], st[i][r - (1 << i) + 1]);
}
```

### 更新

每次更新都需要重新计算 `st` 数组，即进行 $O(nlogn)$ 的预处理操作。因此稀疏表无法高效地对值进行更新。

## 二叉索引树（树状数组）

二叉索引树（Binary Indexed Tree，BIT）又称树状数组，其初衷是解决数据压缩里的累积频率的计算问题，现多用于高效解决 RSQ 问题（也可稍作修改以支持 RMQ 操作，本文不做赘述）。它可以 $O(logn)$ 的时间得到任意前缀和 $a_1 + a_2+ ... + a_j, 1 \leq j \leq N$，并同时支持在 $O(logn)$ 时间内支持动态单点值的修改，空间复杂度 $O(n)$。

### 预备知识

对于正整数 $x$，定义函数 $lowbit(x)$ 为 $x$ 的二进制表达式中最右边的 `1` 所对应的值（而非序号）。例如，`34` 的二进制码为 `0010 0010`，则`lowbit(34)` 将返回 `2`。

在程序实现中，`lowbit(x) = x & -x`。由于计算机中的整数全部以补码形式存储，因此 $-x$ 实际是 $x$ 按位取反，末位加 `1` 后的结果，例如：

*   `+38288` = `1001 0101 1001 0000`
*   `-38288` = `0110 1010 0111 0000`

二者按位取与之后，将返回 `10000` 对应的十进制值，即 `16`。

### 定义

下图为一个侧置的二叉索引树，树中的每一个结点都对应了 $a$ 中的一段连续和（即图中的灰色长条），灰色长条的长度取决于该节点编号的 `lowbit` 值。对于奇数号结点，其 `lowbit` 值为1，因此其仅对应 $a$ 中的一个值。而对于编号 `12` 的结点，由于 `lowbit(12) = 4`，因此它对应了 $a_9 + a_{10} + a_{11} + a_{12}$ 四个数的和。

![](https://img.masterliu.net/rmq-summary/bit.jpg)

树中 `lowbit = 1` 的结点为叶子，它们的父亲是 `lowbit = 2` 的结点，再往上是 `lowbit = 4, 8, 16...` 的结点。这就是 BIT 的层次结构。不难发现，对于结点 $i$，如果它是左子结点，那么他的父亲结点的的编号就是 `i + lowbit(i)`；如果它是右子结点，那么他的父亲结点的的编号就是 `i - lowbit(i)`。

### 查询

给定一个正整数 $x$，求前缀和 $a_1 + a_2 + ... + a_x$：沿着结点 $x$ 往左下方向走（即`x = x - lowbit(x)`），把沿途经过的结点对应的值累加即可。时间复杂度$O(logN)$。以上图为例，求 `a[11]` 的前缀和，走的路线是 `11` --> `10` --> `8`，即`ans = BIT[11] + BIT[10] + BIT[8]`。

```cpp
int sum(int x) {
    int ans = 0;
    for (; x > 0; x -= lowbit(x))
        ans += BIT[x];
    return ans;
}
```

### 更新

如果修改了一个值 $a_x$，则需要沿着结点 $x$ 往左上方向走（即 `x = x + lowbit(x)`），修改沿途经过的结点对应的值即可。时间复杂度 $O(logn)$。以上图为例，修改 $a_3$ 后，走的路线是 `3`-->`4`-->`8`，即修改 `BIT[3]`, `BIT[4]`, `BIT[8]`即可。

```cpp
void add(int x, int val) {//a[x] += val
    for (; x <= n; x += lowbit(x))
        BIT[x] += val;
}
```

### 预处理

预处理只需要将 `BIT[]` 清空，然后执行 $n$ 次 `add` 操作即可，时间复杂度$O(nlogn)$。

## 线段树 -- 单点修改

线段树，是一棵完全二叉树，它在各个结点保存一条线段（数组中的一段子数组），同时支持 RMQ 和 RSQ 操作。它的查询与更新操作的时间复杂度均为 $O(logn)$。

例如，对于数组 $a = \{18, 17, 13, 19, 15, 11, 20, 12, 33, 25\}$，构建的 RSQ（区间求和）线段树如下所示：

![](https://img.masterliu.net/rmq-summary/segtree1.png)

可以看到，绿色结点为叶子结点，存储 $a$ 中的各个值；每个橙色的结点的值都等于它的两个孩子的值之和。这就意味着根结点的值即为整个数组之和，即对应区间 $[0, 9]$；根结点的左孩子对应 $a$ 中区间 $[0, 4]$ 所有元素之和，根结点的右孩子对应 $a$ 中区间 $[5, 9]$ 所有元素之和，以此类推。

**性质**：若父亲的区间是 $[a, b]$，令 $mid = \frac{a + b}{2}$，则左孩子对应的区间是 $[a, mid]$，右孩子对应的区间是 $[mid + 1, b]$。

**索引**：若父亲的索引是 $idx$，则左孩子的索引为 $idx * 2 + 1$，右孩子的索引为 $idx * 2 + 2$。

![](https://img.masterliu.net/rmq-summary/segtree2.png)

### 定义

以下的示例线段树均支持 RMQ 和 RSQ 所有操作，即结点中存储了三个值：对应区间的和(`sumv`)、最大值(`maxv`)、最小值(`minv`)。
为了方便，根结点编号为 `1`，左孩子编号为 `父亲编号 * 2`，右孩子编号为 `父亲编号 * 2 + 1`。

```cpp
#define INF 0x3f3f3f3f3f3f3f3f
#define ll long long

struct node {
    ll sumv, maxv, minv;
    node(): sumv(0), maxv(-INF), minv(INF) {}
};//root from 1

node *tree;
```

### 预处理（建树）

为了操作方便，我们将线段树建立为一棵满二叉树。因此，若 $a$ 中元素数量不恰为 `2` 次幂，则需要将其补全（补上 $sumv = 0, minv = INF, maxv = -INF$ 的结点即可）

```cpp
void init() {
    _n = 1;
    while (_n < n) _n <<= 1;
    tree = new node[_n << 1];
    for (int i = _n; i < _n + n; i++) {
        scanf("%lld", &tree[i].sumv);
        tree[i].minv = tree[i].maxv = tree[i].sumv;
    }
    int p = _n;
    while (p > 1) { // build tree from bottom
        p >>= 1;
        for (int i = p; i < (p << 1); i++) {
            tree[i].sumv = tree[i << 1].sumv + tree[(i << 1) + 1].sumv;
            tree[i].minv = min(tree[i << 1].minv, tree[(i << 1) + 1].minv);
            tree[i].maxv = max(tree[i << 1].maxv, tree[(i << 1) + 1].maxv);
        }
    }
}
```

### 查询

以 $a = {18, 17, 13, 19, 15, 11, 20, 12, 33, 25}$ 构造的 RSQ 线段树为例，若要查询 $[2, 8]$ 的和，只需查询图中的紫色结点，并将它们的值累加即可。由于这四个紫色结点分别对应区间 $[2, 2]$, $[3, 4]$, $[5, 7]$, $[8, 8]$，因此 $13 + 34 + 43 + 33 = 123$ 即为区间 $[2, 8]$ 的和。时间复杂度 $O(logn)$。

![](https://img.masterliu.net/rmq-summary/segtree4.png)

```cpp
void query(int k, int l, int r, int a, int b) { // node k in [l, r]
    if (a <= l && r <= b) {
        ans.sumv += tree[k].sumv;
        ans.minv = min(ans.minv, tree[k].minv);
        ans.maxv = max(ans.maxv, tree[k].maxv);
    } else {
        int mid = (l + r) >> 1;
        if (a <= mid) query(k << 1, l, mid, a, b);
        if (mid < b) query((k << 1) + 1, mid + 1, r, a, b);
    }
}
```

### 更新

更新操作很简单，首先修改数组元素对应的叶子结点，再向上追溯更新它的祖先即可。时间复杂度 $O(logn)$。

下图展示了三次更新操作后的情景：

![](https://img.masterliu.net/rmq-summary/segtree5.png)

```cpp
void update(int i, ll x) {//the ith in a[], starts from 0
    i += _n;
    tree[i].sumv = tree[i].minv = tree[i].maxv = x;
    while (i > 1) { // trace to root
        i >>= 1;
        tree[i].sumv = tree[i << 1].sumv + tree[(i << 1) + 1].sumv;
        tree[i].minv = min(tree[i << 1].minv, tree[(i << 1) + 1].minv);
        tree[i].maxv = max(tree[i << 1].maxv, tree[(i << 1) + 1].maxv);
    }
}
```

## 小结

对于单点修改、区间查询的 RMQ 问题：

| 算法 | RMQ | RSQ | 空间 | 预处理 | 查询 | 更新 |
|-----|-----|-----|------|-------|-----|-----|
| 朴素 | 是  | 是  |$O(1)$| $O(1)$|$O(N)$|$O(1)$|
|平方分割|是|是|$O(N)$|$O(N)$|$O(\sqrt{N})$|$O(\sqrt{N})$|
|稀疏表|是|否|$O(NlogN)$|$O(NlogN)$|$O(1)$|$O(NlogN)$|
|树状数组|是|是|$O(N)$|$O(NlogN)$|$O(logN)$|$O(logN)$|
|线段树|是|是|$O(N)$|$O(N)$|$O(logN)$|$O(logN)$|

## 线段树 -- 区间 ADD

现在要求实现一个数据结构，能支持以下两种操作：

* $add(l, r, v)$: 将 $a_l, a_{l + 1}, ..., a_r$ 全部加上值 $v$
* $query(l, r)$: 查询区间 $[l, r]$ 的元素之和、最大值、最小值

即将单点修改变为区间修改操作。

区间修改线段树支持在 $O(logN)$ 的时间复杂度内完成区间修改以及 RMQ 和 RSQ 查询，空间复杂度与点修改线段树相同。

### 定义

区间修改线段树与单点修改线段树结构相似，只不过需要在结点中附加一个值 `addv`，表示对该结点对应区间所有元素全部加上值 `addv`。

```cpp
#define INF 0x3f3f3f3f

struct node {
    int sumv, minv, maxv, addv;
    node(): sumv(0), addv(0), maxv(-INF), minv(INF) {}
};//root from 1

node* tree;
```

### 初始化

初始化代码与点修改线段树几乎相同，仅仅多了 `addv = 0` 的赋值。

```cpp
void init() {
    _n = 1;
    while (_n < n) _n <<= 1;
    tree = new node[_n << 1];
    for (int i = _n; i < _n + n; i++) {
        scanf("%d", &tree[i].addv);
        tree[i].maxv = tree[i].minv = tree[i].sumv = tree[i].addv;
    }
    int p = _n;
    while (p > 1) {
        p >>= 1;
        for (int i = p; i < (p << 1); i++) {
            tree[i].sumv = tree[i << 1].sumv + tree[(i << 1) + 1].sumv;
            tree[i].minv = min(tree[i << 1].minv, tree[(i << 1) + 1].minv);
            tree[i].maxv = max(tree[i << 1].maxv, tree[(i << 1) + 1].maxv);
            tree[i].addv = 0;
        }
    }
}
```

### ADD

例如对下图线段树进行两次操作 `ADD(0, 7, 2)` 和 `ADD(4, 8, 5)` 之后，得到的结果如下：

![](https://img.masterliu.net/rmq-summary/segtree6.png)

注意值为 `43` 的结点中执行了两个 `ADD` 操作，这相当于一个 `ADD(7)`。

则结点中维护的 `sumv`, `maxv`, `minv` 的定义也需要修改为“如果只执行该结点以及其子孙结点中的 `ADD` 操作，该结点对应区间的元素和、最大值、最小值”。

因此需要实现维护操作（时间复杂度 $O(1)$）：

```cpp
void maintain(int k, int l, int r) { // node k with [l, r]
    int lt = k << 1, rt = (k << 1) + 1;
    tree[k].maxv = tree[k].minv = tree[k].sumv = 0;
    if (r > l) {
        tree[k].sumv = tree[lt].sumv + tree[rt].sumv;
        tree[k].minv = min(tree[lt].minv, tree[rt].minv);
        tree[k].maxv = max(tree[lt].maxv, tree[rt].maxv);
    }
    tree[k].minv += tree[k].addv;
    tree[k].maxv += tree[k].addv;
    tree[k].sumv += tree[k].addv * (r - l + 1);
}
```

维护操作需要在 `ADD` 操作递归访问到的所有结点中调用，且在递归完成后调用：

```cpp
void update(int k, int l, int r, int &v) {
    if (a <= l && b >= r)
        tree[k].addv += v;
    else {
        int mid = (r + l) >> 1;
        if (a <= mid) update(k << 1, l, mid, v);
        if (mid < b) update((k << 1) + 1, mid + 1, r, v);
    }
    maintain(k, l, r);
}
```

### Query

查询操作依然是把查询区间递归分解为若干不相交的子区间，把每个子区间的查询结果加以合并，但需要注意每个子区间的结果不能直接使用，还要考虑祖先结点的 `addv` 值对它的影响。

因此，我们需要在递归查询中增加一个参数 `add`，表示当前区间所有祖先结点的 `addv` 值之和：

```cpp
void query(int k, int l, int r, int add, node& ans) {
    if (a <= l && r <= b) {
        ans.sumv += tree[k].sumv + add * (r - l + 1);
        ans.minv = min(ans.minv, tree[k].minv + add);
        ans.maxv = max(ans.maxv, tree[k].maxv + add);
    } else {
        int mid = (r + l) >> 1;
        if (a <= mid) query(k << 1, l, mid, add + tree[k].addv, ans);
        if (mid < b) query((k << 1) + 1, mid + 1, r, add + tree[k].addv, ans);
    }
}
```

## 线段树 -- 区间SET

现在要求实现一个数据结构，能支持以下两种操作：

* $set(l, r, v)`: 将`a[L], a[L + 1], ... , a[R]`全部赋值为 $v$
* $query(l, r)$: 查询区间 $[l, r]$ 的元素之和、最大值、最小值

即将区间 `ADD` 变为区间 `SET`。

区间修改线段树支持在 $O(logN)$ 的时间复杂度内完成区间修改以及 RMQ 和 RSQ 查询，空间复杂度与点修改线段树相同。

### 定义

区间修改线段树与单点修改线段树结构相似，只不过需要在结点中附加一个值 `setv`，表示对该结点对应区间所有元素全部赋值为 `setv`。

```cpp
#define INF 0x3f3f3f3f3f3f3f3f

struct node {
    ll setv, sumv, minv, maxv;
    node(): sumv(0), setv(-INF), maxv(-INF), minv(INF) {}
};

node* tree;
```

### 初始化

初始化代码与区间 `ADD` 线段树几乎相同，仅仅将 `addv` 改为 `setv`。

```cpp
void init() {
    _n = 1;
    while (_n < n) _n <<= 1;
    tree = new node[_n << 1];
    for (int i = _n; i < _n + n; i++) {
        scanf("%lld", &tree[i].sumv);
        tree[i].maxv = tree[i].minv = tree[i].sumv;
    }
    int p = _n;
    while (p > 1) {
        p >>= 1;
        for (int i = p; i < (p << 1); i++) {
            tree[i].sumv = tree[i << 1].sumv + tree[(i << 1) + 1].sumv;
            tree[i].minv = min(tree[i << 1].minv, tree[(i << 1) + 1].minv);
            tree[i].maxv = max(tree[i << 1].maxv, tree[(i << 1) + 1].maxv);
        }
    }
}
```

### SET

不难想到将 `SET` 操作也分解，然而 `ADD` 操作的时间顺序不会影响结果，而 `SET` 会。

解决方案是这样的：除了对本次操作进行分解以外，还要修改以前分解好的操作，使得任意两个 `SET` 操作都不存在祖先后代关系。

例如对下图线段树进行操作 `SET(0, 7, 1)` 之后，得到的结果如下：

![](https://img.masterliu.net/rmq-summary/segtree7.png)

再进行操作 `SET(0, 2, 2)` 时，首先需要将值为82的结点上的 `SET(1)` 标记向下传，即将值为 `48` 和 `34` 的结点标记 `SET(1)`。由于 `SET(0, 2, 2)` 完整覆盖了值为 `48` 的结点对应的区间，所以只需要该结点的标记修改为 `SET(2)` 即可。

![](https://img.masterliu.net/rmq-summary/segtree8.png)

维护操作也要做少许修改，我们约定 `setv = -INF` 意味着没有 `SET` 标记：

```cpp
void maintain(int k, int l, int r) {
    int lt = k << 1, rt = (k << 1) + 1;
    if (tree[k].setv > -INF) {
        tree[k].maxv = tree[k].minv = tree[k].setv;
        tree[k].sumv = tree[k].setv * (r - l + 1);
    } else {
        tree[k].sumv = tree[lt].sumv + tree[rt].sumv;
        tree[k].maxv = max(tree[lt].maxv, tree[rt].maxv);
        tree[k].minv = min(tree[lt].minv, tree[rt].minv);
    }
}
```

在 `update` 操作中多了两处 `maintain` 调用，这是因为只要标记下传，该子树的信息就需要重新计算：

```cpp
void update(int k, int l, int r, int& v) {
    int lt = k << 1, rt = (k << 1) + 1;
    if (a <= l && r <= b)
        tree[k].setv = v;
    else {
        if (tree[k].setv > -INF) { //push down
            tree[lt].setv = tree[rt].setv = tree[k].setv;
            tree[k].setv = -INF;
        }
        int mid = (l + r) >> 1;
        if (a <= mid) update(lt, l, mid, v); else maintain(lt, l, mid);
        if (mid < b) update(rt, mid + 1, r, v); else maintain(rt, mid + 1, r);
    }
    maintain(k, l, r);
}
```

### Query

由于任意两个 `SET` 操作均不存在祖孙后代关系，因此我们约定，以祖先结点上的 `SET` 操作为准。在递归时，一旦遇到 `setv` 标记或者区间被完全覆盖就停止：

```cpp
void query(int k, int l, int r, node& ans) {
    if (tree[k].setv > -INF) { // SET found
        ans.sumv += tree[k].setv * (min(r, b) - max(l, a) + 1);
        ans.minv = min(ans.minv, tree[k].minv);
        ans.maxv = max(ans.maxv, tree[k].maxv);
    } else if (a <= l && r <= b) {
        ans.sumv += tree[k].sumv;
        ans.minv = min(ans.minv, tree[k].minv);
        ans.maxv = max(ans.maxv, tree[k].maxv);
    } else {
        int mid = (r + l) >> 1;
        if (a <= mid) query((k << 1), l, mid, ans);
        if (mid < b) query((k << 1) + 1, mid + 1, r, ans);
    }
}
```

## 双标记线段树

现在要求实现一个数据结构，能同时支持上述的区间 `ADD`，区间 `SET`，和区间查询功能。做法就是将上述的两种线段树结合起来，使用 `addv` 和 `setv` 双标记的线段树依然可以在 $O(logN)$ 的时间复杂度内完成上述三个操作。

### 定义

我们约定，当结点中同时存在两个标记时，先执行 `SET` 操作后执行 `ADD` 操作：

```cpp
#define INF 0x3f3f3f3f3f3f3f3f

struct node {
    ll setv, sumv, minv, maxv, addv;
    node(): setv(-INF), sumv(0), minv(INF), maxv(-INF), addv(0) {};
};

node* tree;
```

### 初始化

初始化代码没多大变化：

```cpp
void init() {
    _n = 1;
    while (_n < n) _n <<= 1;
    tree = new node[_n << 1];
    for (int i = _n; i < _n + n; i++) {
        scanf("%lld", &tree[i].addv);
        tree[i].maxv = tree[i].minv = tree[i].sumv = tree[i].addv;
    }
    int p = _n;
    while (p > 1) {
        p >>= 1;
        for (int i = p; i < (p << 1); i++) {
            tree[i].sumv = tree[i << 1].sumv + tree[(i << 1) + 1].sumv;
            tree[i].minv = min(tree[i << 1].minv, tree[(i << 1) + 1].minv);
            tree[i].maxv = max(tree[i << 1].maxv, tree[(i << 1) + 1].maxv);
        }
    }
}
```

### Maintain

定义全局枚举类型 `enum { ADD, SET, QUERY } OPT`，在维护操作中，需要先考虑 `setv` 标记再考虑 `addv` 标记：

```cpp
void maintain(int k, int l, int r) {
    int lt = k << 1, rt = (k << 1) + 1;
    tree[k].maxv = tree[k].minv = tree[k].sumv = 0;
    if (r > l) {
        tree[k].sumv = tree[lt].sumv + tree[rt].sumv;
        tree[k].minv = min(tree[lt].minv, tree[rt].minv);
        tree[k].maxv = max(tree[lt].maxv, tree[rt].maxv);
    }
    if (tree[k].setv > -INF) { // if found SET
        tree[k].setv += tree[k].addv;
        tree[k].addv = 0;
        tree[k].maxv = tree[k].minv = tree[k].setv;
        tree[k].sumv = tree[k].setv * (r - l + 1);
    } else {
        tree[k].minv += tree[k].addv;
        tree[k].maxv += tree[k].addv;
        tree[k].sumv += tree[k].addv * (r - l + 1);
    }
}
```

### Update

在 `update` 操作中需要注意两点：

* `push down` 完成后，左右子树的信息都需要重新计算，且必须在递归之前完成。
* 若为 `SET`，则需要清除 `addv` 标记；而 `ADD` 操作不需要清除 `setv` 标记。

```cpp
void update(int k, int l, int r, ll &v) {
    int lt = k << 1, rt = (k << 1) + 1;
    if (a <= l && b >= r) {
        if (OPT == ADD)
            tree[k].addv += v;
        else {
            tree[k].setv = v;
            tree[k].addv = 0;
        }
    } else {
        if (tree[k].setv > -INF) { // push down
            tree[lt].setv = tree[rt].setv = tree[k].setv;
            tree[k].setv = -INF;
            tree[lt].addv = tree[rt].addv = 0;
        } else {
            tree[lt].addv += tree[k].addv;
            tree[rt].addv += tree[k].addv;
            tree[k].addv = 0;
        }
        int mid = (l + r) >> 1;
        maintain(lt, l, mid);
        maintain(rt, mid + 1, r);
        if (a <= mid) update(lt, l, mid, v);
        if (mid < b) update(rt, mid + 1, r, v);
    }
    maintain(k, l, r);
}
```

### Query

需要注意两点：

* 依然以祖先结点上的 `SET` 操作为准。即在递归时，一旦遇到 `setv` 标记或者区间被完全覆盖就停止
* 同时需要考虑祖先的 `addv` 标记对当前区间的影响

```cpp
void query(int k, int l, int r, ll add, node &ans) {
    if (tree[k].setv > -INF) {
        ans.sumv += (tree[k].setv + add) * (min(r, b) - max(l, a) + 1);
        ans.minv = min(ans.minv, tree[k].minv + add);
        ans.maxv = max(ans.maxv, tree[k].maxv + add);
    } else if (a <= l && r <= b) {
        ans.sumv += tree[k].sumv + add * (r - l + 1);
        ans.minv = min(ans.minv, tree[k].minv + add);
        ans.maxv = max(ans.maxv, tree[k].maxv + add);
    } else {
        int mid = (r + l) >> 1;
        if (a <= mid) query(k << 1, l, mid, add + tree[k].addv, ans);
        if (mid < b) query((k << 1) + 1, mid + 1, r, add + tree[k].addv, ans);
    }
}
```

## 总结

显然线段树能完美解决本文所有的 RMQ 和 RSQ 问题，但其代码相对复杂难懂，易于出错。因此，在实际中，要根据需要选择合适的数据结构。比如对于单点修改 RMQ 问题，当修改操作数量远小于查询操作时，使用稀疏表会获得较好的性能；当修改操作数量与查询操作数量相当时，树状数组的性能往往优于线段树，且代码易于编写；而对于区间修改问题，线段树往往是最好的选择。
