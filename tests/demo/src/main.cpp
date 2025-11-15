#include <vector>
#include <array>
#include <set>
#include <queue>
#include <cassert>
#include <iostream>

void printVector(const std::vector<std::vector<int>>& vec) {
    std::cout << "=======================================\n";
    std::cout << "size() == " << vec.size() << '\n';
    for (auto idx = 0; auto&& v : vec) {
        std::cout << "idx == " << idx << ": ";
        for (auto vi : v) {
            std::cout << vi << ' ';
        }
        std::cout << '\n';
        ++idx;
    }
}

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

bool isSamePosition(const Info& a, const Info& b) {
    return (isSamePoint(a.p1, b.p1) && isSamePoint(a.p2, b.p2))
        || (isSamePoint(a.p1, b.p2) && isSamePoint(a.p2, b.p1));
}

std::vector<int> bfs(const std::vector<std::vector<int>>& adj, std::vector<bool> seen, int start, int end) {
    std::queue<std::vector<int>> que;
    que.push({ start });
    seen[start] = true;
    while (!que.empty()) {
        auto path = que.front();
        que.pop();
        auto u = path.back();
        if (u == end) {
            return path;
        }
        for (auto v : adj[u]) {
            if (seen[v] || std::tie(u, v) == std::tie(start, end)) {
                continue;
            }
            auto newPath = path;
            newPath.push_back(v);
            que.push(newPath);
            seen[v] = true;
        }
    }

    return {};
}

std::vector<std::vector<int>> solve(std::vector<Info> infos) {
    const int n = infos.size();

    std::vector<std::vector<int>> groups;
    std::vector<bool> seen(n, false);

    // cycle = 1
    for (auto i = 0; i < n; ++i) {
        if (isSamePoint(infos[i].p1, infos[i].p2)) {
            groups.push_back({i});
            seen[i] = true;
        }
    }

    // cycle = 2
    for (auto i = 0; i < n; ++i) {
        if (seen[i]) {
            continue;
        }
        for (auto j = i + 1; j < n; ++j) {
            if (seen[j]) {
                continue;
            }
            if (isSamePosition(infos[i], infos[j])) {
                groups.push_back({i, j});
                seen[i] = true;
                seen[j] = true;
                break;
            }
        }
    }

    // general
    std::vector<std::vector<int>> adj(n);
    for (auto i = 0; i < n; ++i) {
        if (seen[i]) {
            continue;
        }
        for (auto j = i + 1; j < n; ++j) {
            if (seen[j]) {
                continue;
            }
            if (intersection(infos[i], infos[j])) {
                adj[i].push_back(j);
                adj[j].push_back(i);
            }
        }
    }
    printVector(adj);

    for (auto i = 0; i < n; ++i) {
        if (seen[i]) {
            continue;
        }

        std::vector<int> path;
        for (auto j : adj[i]) {
            if (seen[j]) {
                continue;
            }
            auto res = bfs(adj, seen, i, j);
            if (!res.empty()) {
                path = res;
                break;
            }
        }

        if (path.empty()) {
            path = { i };
        }
        groups.push_back(path);
        for (auto id : path) {
            seen[id] = true;
        }
    }
    printVector(groups);

    // check result
    std::set<int> checkResultValid;
    for (auto&& group : groups) {
        for (auto id : group) {
            checkResultValid.emplace(id);
        }
        for (auto i = 0, m = static_cast<int>(group.size()); i < m; ++i) {
            assert(intersection(infos[group[i]], infos[group[(i + 1) % m]]));
        }
    }
    assert(static_cast<int>(checkResultValid.size()) == n);

    return groups;
}

int main() {

    std::vector<Info> infos = {
        Info {
            std::array<double, 3> { -1, 0, 0 },
            std::array<double, 3> { -1, 0, 0 },
        },

        Info {
            std::array<double, 3> { 0, 0, 0 },
            std::array<double, 3> { 0, 0, 1 },
        },

        Info {
            std::array<double, 3> { 1, 0, 0 },
            std::array<double, 3> { 2, 0, 0 },
        },
        Info {
            std::array<double, 3> { 2, 0, 0 },
            std::array<double, 3> { 1, 0, 0 },
        },

        Info {
            std::array<double, 3> { 3, 0, 0 },
            std::array<double, 3> { 3, 1, 0 },
        },
        Info {
            std::array<double, 3> { 4, 0, 0 },
            std::array<double, 3> { 3, 0, 0 },
        },
        Info {
            std::array<double, 3> { 4, 0, 0 },
            std::array<double, 3> { 3, 1, 0 },
        },

        Info {
            std::array<double, 3> { 5, 0, 0 },
            std::array<double, 3> { 6, 0, 0 },
        },
        Info {
            std::array<double, 3> { 6, 0, 0 },
            std::array<double, 3> { 6, 1, 0 },
        },
        Info {
            std::array<double, 3> { 6, 1, 0 },
            std::array<double, 3> { 5, 0, 0 },
        },
        Info {
            std::array<double, 3> { 6, 0, 0 },
            std::array<double, 3> { 6, 1, 0 },
        },
        Info {
            std::array<double, 3> { 7, 0, 0 },
            std::array<double, 3> { 6, 0, 0 },
        },
        Info {
            std::array<double, 3> { 7, 0, 0 },
            std::array<double, 3> { 6, 1, 0 },
        },
    };
    auto ans = solve(infos);
    printVector(ans);

    return 0;
}
