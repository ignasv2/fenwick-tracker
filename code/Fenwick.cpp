#include "Fenwick.h"

// initializing Fenwick Tree of a given size and setting up 1-based internal array
FenwickTree::FenwickTree(int size) {
    if (size < 0) {
        n=0;
    }
    else {
        n = size;
    }
    tree.assign(n + 1, 0);
}

// setting all of the values to zero by using a for loop
void FenwickTree::reset() {
    for (int i=0; i<=n; i++) {
        tree[i] = 0;
    }
}

// add the delta to idx and all of the relevant parents
void FenwickTree::add(int idx, long long delta) {
    while (idx > 0 && idx <= n) {
        tree[idx] += delta;
        idx += (idx & -idx);
    }
}

// function to return the sum of all elements from first index to idx
long long FenwickTree::prefix_sum(int idx) const {
    if (idx <= 0) return 0;
    if (idx > n) idx = n;

    long long total = 0;
    while (idx > 0) {
        total += tree[idx];
        idx -= (idx & -idx);
    }
    return total;
}

// calculating the sum between left index and right index by swapping order subtracting prefix sum
long long FenwickTree::range_sum(int leftIndex, int rightIndex) const {
    if (leftIndex > rightIndex) {
        int tmp=leftIndex;
        leftIndex=rightIndex;
        rightIndex=tmp;
    }
    if (rightIndex<1 || leftIndex>n) return 0;
    if (leftIndex < 1) leftIndex = 1;
    if (rightIndex > n) rightIndex = n;
    return prefix_sum(rightIndex) - prefix_sum(leftIndex-1);
}

// returning max index n from the tree
int FenwickTree::size() const {
    return n;
}