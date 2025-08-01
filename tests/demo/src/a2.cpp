int maxProfit(vector<int>& inventory, int orders) {
    constexpr auto mod = static_cast<int>(1E9 + 7);
    const auto n = static_cast<int>(inventory.size());

    auto check = [&](auto m, auto& ans) {
        auto cnt = 0;
        for (auto inv : inventory) {
            cnt += std::max(0, inv - m + 1);
        }

        ans = 0;
        for (auto inv : inventory) {
            if (inv < m) {
                continue;
            }
            auto cur = 1LL * (m + inv) * (inv - m + 1) / 2 % mod;
            ans = (ans + cur) % mod;
        }
        for (auto inv : inventory) {
            if (cnt >= orders) {
                break;
            }
            ans = (ans + inv) % mod;
        }

        return cnt <= orders;
    };

    auto ans = 0;
    auto lo = 0, hi = n;
    while (lo < hi) {
        auto mid = (lo + hi) / 2;
        if (check(mid, ans)) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }

    return ans;
}
