#include <algorithm>
#include <cassert>
#include <limits>
#include <vector>
#include <array>
#include <tuple>
#include <iostream>
#include <set>

struct Edge {
    int v, d;
    bool operator<(const auto& other) const {
        return !(d < other.d);
    }
};

struct Info {
    int id;
    std::array<double, 3> p1;
    std::array<double, 3> p2;
};

bool isSamePoint(const std::array<double, 3>& a, const std::array<double, 3>& b) {
    for (auto i = 0; i < 3; ++i) {
        if (std::abs(a[i] - b[i]) > std::numeric_limits<float>::epsilon()) {
            return false;
        }
    }
    return true;
}

bool intersection(const Info& a, const Info& b) {
    return isSamePoint(a.p1, b.p1)
        || isSamePoint(a.p1, b.p2)
        || isSamePoint(a.p2, b.p1)
        || isSamePoint(a.p2, b.p2);
}

std::vector<std::vector<int>> solve(std::vector<Info> infos) {
    const auto idCount = infos.size();
    std::vector<std::vector<int>> groups;

    // check input validity
    assert(std::all_of(infos.begin(), infos.end(), [idCount](const auto& info) {
        return info.id >= 0 && info.id < idCount;
    }));
    {
        std::set<int> idSet;
        for (auto&& info : infos) {
            idSet.emplace(info.id);
        }
        assert(idSet.size() == idCount);
    }

    // filter closed coil
    {
        auto dividIter = std::partition(infos.begin(), infos.end(), [](const auto& info) {
            return !isSamePoint(info.p1, info.p2);
        });
        for (auto iter = dividIter; iter != infos.end(); ++iter) {
            groups.emplace_back(iter->id);
        }
        infos.erase(dividIter, infos.end());
    }

    // build graph
    std::vector<std::vector<Edge>> adj(idCount);
    for (auto i = 0; i < infos.size(); ++i) {
        for (auto j = i + 1; j < infos.size(); ++j) {
            if (infos[i].id == infos[j].id
                || !intersection(infos[i], infos[j])
            ) {
                continue;
            }

            auto u = infos[i].id;
            auto v = infos[j].id;
            adj[u].emplace_back(v, 1);
            adj[v].emplace_back(u, 1);
        }
    }

    // 每条边只能用一次 == 每个点只能属于某一个最小环

    // check result
    std::set<int> checkResultValid;
    for (auto&& group : groups) {
        for (auto id : group) {
            checkResultValid.emplace(id);
        }
    }
    assert(checkResultValid.size() == idCount);

    return groups;
}

int main() {

    auto ans = solve({});

    return 0;
}
