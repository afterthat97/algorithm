---
title: 动态开点线段树
date: 2017-12-11 19:01:36
tags:
 - Algorithm
---

用链表线段树的大佬请忽略此文...以及目前仅支持单点修改区间查询...

## 先看一道题目

在开始本文前，先看一道题目吧：[Codeforces 762E](http://codeforces.com/problemset/problem/762/E)

题目大意：一共有 $n$ 个电台，每个电台 $i$ 有三个参数：$x_i$, $r_i$, $f_i$，分别指它的一维坐标、作用半径和频率。如果两个电台的频率差值在 $k$ 内，并且它们的作用范围都能覆盖到彼此，那么就称这两个电台相互干扰。问这 $n$ 个站台中相互干扰的站台有多少对。

如果不考虑频率，坐标范围都在 $10^9$ 范围内，那么针对电台的坐标开一个权值线段树即可。但是现在还要考虑频率，虽然频率差值 $k \leq 10$，但是频率的数值范围可能达到 $10^4$。开 $10^4$ 个这样的线段树显然不现实。但是总共只有 $n$ 个站台，有意义的数据肯定是存得下的。

动态开点线段树就此诞生。

<!-- more -->

## 相比于传统线段树

### 定义

对于传统线段树中的某一个结点，若其编号为 $i$，那么它的左右儿子的编号通常为固定值，比如 $2i$ 和 $2i + 1$。在动态开点线段树中，只会给将要用到的结点进行编号，因此还需要 `lt[]` 和 `rt[]` 数组，分别记录某一编号的结点的左右儿子的编号（如果不存在就为 0 或 -1）：

```cpp
int tree[maxnode], lt[maxnode], rt[maxnode], head[maxn], tsize;
```

* `tsize` 为线段树中所有结点的个数
* `head[i]` 记录了第 $i$ 个线段树根结点在 `tree[]` 数组中的位置

### 修改

在单点修改操作（将第 `idx` 个值增加 `val`）中，如果该点不存在，就动态给它分配一个编号（同时修改其父亲的孩子指针）：

**注意第一个参数 `k` 是引用！**

```cpp
void add(int& k, int l, int r, int idx, int val) {
	if (k == 0) k = ++tsize;
	if (l == r) {
		tree[k] += val;
		return;
	}
	int mid = (l + r) / 2;
	if (idx <= mid) add(lt[k], l, mid, idx, val);
	else add(rt[k], mid + 1, r, idx, val);
	tree[k] = tree[lt[k]] + tree[rt[k]];
}
```

### 查询

同样，在查询时也要注意判断当前结点是否存在：

```cpp
int query(int& k, int l, int r, int a, int b) {
	if (k == 0 || a > r || b < l) return 0;
	if (a <= l && r <= b) return tree[k];
	int mid = (l + r) / 2;
	return query(lt[k], l, mid, a, b)
		+ query(rt[k], mid + 1, r, a, b);
}
```

单点修改、区间查询的动态结点线段树实现就完成了。可以看到，它动态开点的过程和主席树思想很像（或者说一致）。关于最初那道题的题解，请转到我的另一个题解专用博客。
