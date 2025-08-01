int maxPointsInsideSquare(vector<vector<int>>& points, string s) {
    const int n = points.size();

    auto check = [&](auto m, auto& ans) {
        auto flag = 0;
        for (auto&& position : points) {
            auto x = position[0], y = position[1];
            if (std::abs(x) > m || std::abs(y) > m) {
                continue;
            }
            if (flag >> (s[i] - 'a') & 1) {
                return false;
            }
            flag |= 1 << (s[i] - 'a');
        }
        ans = std::popcount(flag);
    };

    auto max_xy = 0;
    for (auto&& position : points) {
        max_xy = std::max({max_xy, position[0], position[1]});
    }

    auto ans = -1;
    auto lo = 0, hi = max_xy;
    while (lo < hi) {
        auto mid = (lo + hi + 1) / 2;
        if (check(mid, ans)) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }

    return ans;
}
