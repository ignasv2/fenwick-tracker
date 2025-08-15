#ifndef FENWICK_H
#define FENWICK_H

#include <vector>

class FenwickTree {
    private:
        // how many usable indices
        int n;
        // internal 1 based array to store cumulative sums
        std::vector<long long> tree;

    public:
        // builds a tree of all zeros and size "size"
        explicit FenwickTree(int size);
        // clears data to be zeros
        void reset();
        // adds delta to index idx
        void add(int idx, long long delta);
        // sum of 1 through idx
        long long prefix_sum(int idx) const;
        // sum of 1 through r
        long long range_sum(int leftIndex, int rightIndex) const;
        // returns max supported index
        int size() const;
};

#endif