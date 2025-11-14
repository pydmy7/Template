#include <algorithm>
#include <limits>
#include <vector>
#include <array>
#include <set>
#include <queue>
#include <cassert>
#include <iostream>

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

std::vector<int> bfs(const auto& adj, auto seen, auto st, auto ed) {
    std::queue<std::vector<int>> queue;
    queue.emplace(st);
    seen[st] = true;
    while (!queue.empty()) {
        auto path = queue.front();
        queue.pop();
        auto u = path.back();
        if (u == ed) {
            return path;
        }
        for (auto&& edge : adj[u]) {
            auto v = edge.v;
            if (seen[v] || std::tie(u, v) == std::tie(st, ed) || std::tie(u, v) == std::tie(ed, st)) {
                continue;
            }
            auto newPath = path;
            newPath.push_back(v);
            queue.emplace(newPath);
            seen[v] = true;
        }
    }

    return {};
}

std::vector<std::vector<int>> solve(std::vector<Info> infos) {
    const int idCount = infos.size();
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
        assert(static_cast<int>(idSet.size()) == idCount);
    }

    // filter closed coil (self-loop)
    // {
    //     auto dividIter = std::partition(infos.begin(), infos.end(), [](const auto& info) {
    //         return !isSamePoint(info.p1, info.p2);
    //     });
    //     for (auto iter = dividIter; iter != infos.end(); ++iter) {
    //         groups.emplace_back(iter->id);
    //     }
    //     infos.erase(dividIter, infos.end());
    // }

    // build graph
    std::vector<std::vector<Edge>> adj(idCount);
    for (auto i = 0; i < static_cast<int>(infos.size()); ++i) {
        for (auto j = i + 1; j < static_cast<int>(infos.size()); ++j) {
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

    std::vector<bool> seen(idCount, false);
    for (auto u = 0; auto&& edges : adj) {
        if (seen[u]) {
            ++u;
            continue;
        }

        if (edges.empty()) {
            groups.emplace_back(u);
            seen[u] = true;
        } else {
            for (auto&& edge : edges) {
                auto v = edge.v;
                auto path = bfs(adj, seen, u, v);  // note: herd copy seen
                if (path.empty()) {
                    continue;
                }
                for (auto id : path) {
                    seen[id] = true;
                }
                groups.emplace_back(path);
            }
        }

        ++u;
    }

    // check result
    auto getInfoById = [&infos](auto id) -> const Info& {
        auto iter = std::find_if(infos.begin(), infos.end(), [id](const auto& info) {
            return info.id == id;
        });
        assert(iter != infos.end());
        return *iter;
    };
    std::set<int> checkResultValid;
    for (auto&& group : groups) {
        for (auto id : group) {
            checkResultValid.emplace(id);
        }
        for (auto i = 1; i < static_cast<int>(group.size()); ++i) {
            assert(intersection(getInfoById(group[i - 1]), getInfoById(group[i])));
        }
    }
    assert(static_cast<int>(checkResultValid.size()) == idCount);

    return groups;
}

int main() {

    auto ans = solve({});

    return 0;
}
