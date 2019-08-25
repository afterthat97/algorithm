---
title: 区间 kth 问题的多种解法对比
date: 2017-08-28 22:34:51
tags:
 - Algorithm
---

区间 kth 问题是一个经典的问题，即给出一个由 $n$ 个数组成的序列 $a_1, a_2,..., a_n$，之后有 $m$ 个询问，每次询问区间 $[l, r]$ 中按从小到大排序第 $k$ 个元素的值。暴力解法即每次将 $[l, r]$ 中的数排序后查找，单次查询复杂度 $O(nlogn)$，在查询数量众多时效率极低。这类问题的解法众多，主要涉及各类数据结构，本文在此做个简介与比较。

<!-- more -->

## 分块

将 $n$ 个数分为 $\sqrt{n}$ 块，每块单独排序。对于每次查询，二分答案后，在各个被包含的块中再二分查询比其小的数的个数，边界枚举即可。

* 预处理（分块排序）：$O(nlogn)$
* 单次询问：$O(\sqrt{n}\cdot log^2n)$
* 总复杂度：$O(nlogn + m\sqrt{n}\cdot log^2n)$

### C++ 实现

{% include_code acm-solutions/POJ/2104/block.cpp %}

## 归并树

归并树，又名区间树，其实质是线段树。线段树的每个节点存储了其对应区间所有的数的升序排列，它的构造实质就是归并排序。（详见《挑战程序设计竞赛》Page 188）

查询时，二分答案后，在线段树的结点二分查找比其小的数的个数即可。

* 预处理（建树）：$O(nlogn)$
* 单次询问：$O(log^3n)$
* 总复杂度：$O(nlogn + m \cdot log^3n)$

### C++ 实现

{% include_code acm-solutions/POJ/2104/merge-tree.cpp %}

## 主席树

这类问题是主席树（可持久化线段树）最擅长的之一，具体原理请见[可持久化线段树（主席树）简介](/chairman-tree)。

* 预处理（建树）：$O(nlogn)$
* 单次询问：$O(logn)$
* 总时间复杂度：$O(nlogn + mlogn)$

### C++ 实现

{% include_code acm-solutions/POJ/2104/chairman-tree.cpp %}

## 莫队 + 分块

莫队算法将询问离线处理（具体原理自行 Google），其时间复杂度的瓶颈主要在于转移和查询。采用分块处理查询的好处在于使得转移时的单次增删操作的时间复杂度为 $O(1)$。

* 预处理（离散化）：$O(nlogn)$
* 莫队状态转移：$O(m\sqrt{m})$
* 单次查询：$O(\sqrt{n})$
* 总复杂度：$O(nlogn+m(\sqrt{m}+\sqrt{n}))$

### C++ 实现

{% include_code acm-solutions/POJ/2104/md-block.cpp %}

## 莫队 + 树状数组

具体原理与上一个方法大致相同，主要将查询用的数据结构换为树状数组，其单次查询时间复杂度为 $O(logn)$，但单次增删操作的复杂度也增加到 $O(logn)$。

* 预处理（离散化）：$O(nlogn)$
* 莫队状态转移：$O(m\sqrt{m}\cdot logn)$
* 单次查询：$O(logn)$
* 总复杂度：$O(nlogn+m\sqrt{m}\cdot logn)$

### C++ 实现

{% include_code acm-solutions/POJ/2104/md-bit.cpp %}

## aqx 二分

在本题，aqx 二分实际就是整体二分的循环版。江湖套路不解释。

* $O(nlogn+logmlogn(n+m))$

### C++ 实现

{% include_code acm-solutions/POJ/2104/binary-search.cpp %}


## 性能对比

使用以下两道题进行评测（评测难免有误差，题目数据也未必随机，仅供参考）：

* [POJ 2104 K-th Number](http://poj.org/problem?id=2104)
* [POJ 2761 Feed the dogs](http://poj.org/problem?id=2761)

> POJ 2104 中的数据范围为 $n \leq 10^6, m \leq 5 \cdot10^3$；
> POJ 2761 中的数据范围为 $n \leq 10^6, m \leq 5 \cdot10^4$。

|             | POJ 2104 | POJ 2761 |
|-------------|----------|----------|
| 分块         | TLE      | TLE      |
| 归并树       | 2219ms    | TLE      |
| 主席树       | 954ms     | 2563ms   |
| 莫队+分块    | 1532ms     | 1844ms  |
| 莫队+树状数组 | 2610ms    | 1907ms   |
| aqx 二分     | 1094ms    | 2594ms   |
