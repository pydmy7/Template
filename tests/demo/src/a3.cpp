int minimizedMaximum(int n, vector<int>& quantities) {
    const int m = quantities.size();

    auto check = [&](auto k) {
        auto cnt = 0;
        for (auto quantity : quantities) {
            cnt += (quantity + k - 1) / k;
        }
        return cnt >= n;
    };

    auto lo = 1, hi = std::ranges::max(quantities);
    while (lo < hi) {
        auto mid = (lo + hi) / 2;
        if (check(mid)) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }

    return lo;
}
