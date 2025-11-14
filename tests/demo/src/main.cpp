#include <algorithm>
#include <limits>
#include <vector>
#include <array>
#include <set>
#include <queue>
#include <cassert>
#include <iostream>

struct Info {
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

std::vector<int> bfs(const std::vector<std::vector<int>>& adj, std::vector<bool> seen, int start) {
    if (!adj[start].empty() && adj[start][0] == start) {
        return {start};
    }

    std::queue<std::vector<int>> que;
    que.push({start});
    seen[start] = true;
    while (!que.empty()) {
        auto path = que.front();
        que.pop();
        auto u = path.back();
        if (u == start) {
            return path;
        }
        for (auto v : adj[u]) {
            if (seen[v]) {
                if (v == start) {
                    path.push_back(v);
                    return path;
                }
                continue;
            }
            auto newPath = path;
            newPath.push_back(v);
            que.push(newPath);
            seen[v] = true;
        }
    }
}

std::vector<std::vector<int>> solve(std::vector<Info> infos) {
    const int n = infos.size();

    // build graph
    std::vector<std::vector<int>> adj(n);
    for (auto i = 0; i < n; ++i) {
        if (isSamePoint(infos[i].p1, infos[i].p2)) {
            adj[i].push_back(i);
        }
        for (auto j = i + 1; j < n; ++j) {
            if (!intersection(infos[i], infos[j])) {
                continue;
            }
            adj[i].push_back(j);
            adj[j].push_back(i);
        }
    }

    std::vector<std::vector<int>> groups;
    std::vector<bool> seen(n, false);
    for (auto i = 0; i < n; ++i) {
        if (seen[i]) {
            continue;
        }
        auto path = bfs(adj, seen, i);  // hard copy to seen
        if (path.empty()) {
            path = {i};
        }
        groups.push_back(path);
        for (auto id : path) {
            seen[id] = true;
        }
    }

    // check result
    std::set<int> checkResultValid;
    for (auto&& group : groups) {
        for (auto id : group) {
            checkResultValid.emplace(id);
        }
        for (auto i = 0, m = static_cast<int>(group.size()); i < m; ++i) {
            assert(intersection(infos[i], infos[(i + 1) % m]));
        }
    }
    assert(static_cast<int>(checkResultValid.size()) == n);

    return groups;
}

int main() {

    auto ans = solve({});

    return 0;
}
