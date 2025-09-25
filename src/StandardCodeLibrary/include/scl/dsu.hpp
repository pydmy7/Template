#pragma once

#include <cassert>
#include <numeric>
#include <vector>

namespace scl {

class DSU {
private:
    int n_{}, cnt_{};
    std::vector<int> p_, sz_;

public:
    explicit DSU(int n) {
        init(n);
    }

    void init(int n) {
        this->n_ = n;
        this->cnt_ = n;
        sz_.assign(n, 1);
        p_.resize(n);
        std::iota(p_.begin(), p_.end(), 0);
    }
    int find(int x) {
        assert(0 <= x && x < n_);
        return x == p_[x] ? x : p_[x] = find(p_[x]);
    }
    bool same(int a, int b) {
        assert(0 <= a && a < n_);
        assert(0 <= b && b < n_);
        return find(a) == find(b);
    }
    int size(int x) {
        assert(0 <= x && x < n_);
        return sz_[find(x)];
    }
    int count() {
        return cnt_;
    }
    bool merge(int a, int b) {
        assert(0 <= a && a < n_);
        assert(0 <= b && b < n_);
        int pa = find(a), pb = find(b);
        if (pa == pb) {
            return false;
        }
        sz_[pa] += sz_[pb];
        p_[pb] = pa;
        --cnt_;
        return true;
    }
};

}  // namespace scl
