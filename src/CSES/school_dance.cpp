#include <iostream>
#include <vector>
#include <queue>
#include <cassert>

using namespace std;

struct MaxBipartiteMatching {
    int n, m;
    vector<vector<int>> adj;
    vector<int> match_from_left, match_from_right, dist;

    MaxBipartiteMatching(int n, int m) : n(n), m(m), adj(n) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
    }

    bool bfs() {
        queue<pair<int, int>> q;
        for (int i = 0; i < n; ++i) {
            if (match_from_left[i] == -1) {
                dist[i] = 0;
                q.emplace(i, 0);
            } else {
                dist[i] = -1;
            }
        }
        bool found_augmenting_path = false;
        int previous_level = -1;
        while (!q.empty()) {
            auto [u, level] = q.front();
            q.pop();
            if (found_augmenting_path && level != previous_level) break;
            for (int v : adj[u]) {
                if (match_from_right[v] == -1) {
                    found_augmenting_path = true;
                } else if (dist[match_from_right[v]] == -1) {
                    dist[match_from_right[v]] = level + 1;
                    q.emplace(match_from_right[v], level + 1);
                }
            }
        }
        return found_augmenting_path;
    }

    bool dfs(int u) {
        for (int v : adj[u]) {
            if (match_from_right[v] == -1 || 
                (dist[match_from_right[v]] == dist[u] + 1 && dfs(match_from_right[v]))) {
                match_from_left[u] = v;
                match_from_right[v] = u;
                return true;
            }
        }
        dist[u] = -1;
        return false;
    }

    int hopcroft_karp() {
        match_from_left.resize(n, -1);
        match_from_right.resize(m, -1);
        dist.resize(n);
        int res = 0;
        while (bfs()) for (int i = 0; i < n; ++i)
            if (match_from_left[i] == -1 && dfs(i))
                ++res;
        return res;
    }

    bool try_kuhn(int u, vector<bool>& visited) {
        if (visited[u]) return false;
        visited[u] = true;
        for (int v : adj[u]) {
            if (match_from_right[v] == -1 || try_kuhn(match_from_right[v], visited)) {
                match_from_right[v] = u;
                match_from_left[u] = v;
                return true;
            }
        }
        return false;
    }

    int kuhn() {
        match_from_left.resize(n, -1);
        match_from_right.resize(m, -1);
        int res = 0;
        for (int i = 0; i < n; ++i) {
            vector<bool> visited(n, false);
            if (try_kuhn(i, visited)) {
                ++res;
            }
        }
        return res;
    }
};

int main() {
    int n, m, k;
    cin >> n >> m >> k;
    MaxBipartiteMatching mbm(n, m);
    for (int i = 0; i < k; ++i) {
        int u, v;
        cin >> u >> v;
        mbm.add_edge(u - 1, v - 1);
    }
    int result = mbm.kuhn();
    cout << result << endl;
    for (int i = 0; i < n; ++i) {
        if (mbm.match_from_left[i] != -1) {
            cout << i + 1 << " " << mbm.match_from_left[i] + 1 << endl;
        }
    }
    return 0;
}