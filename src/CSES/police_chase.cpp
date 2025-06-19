#include <cstddef>
#include <iostream>
#include <queue>
#include <vector>
#include <climits>

using namespace std;

using ll = long long;

struct MaxFlow {
private:
    int n;
    vector<vector<int>> adj;
    vector<vector<ll>> capacity;
    vector<ll> excess;
    vector<int> parent, height;
    vector<vector<int>::iterator> current_arc;
    queue<int> excess_vertices;

    ll bfs(int s, int t) {
        fill(parent.begin(), parent.end(), -1);
        parent[s] = -2;
        queue<pair<int, ll>> q;
        q.emplace(s, LLONG_MAX);

        while (!q.empty()) {
            auto [u, flow] = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (parent[v] == -1 && capacity[u][v] > 0) {
                    parent[v] = u;
                    ll new_flow = min(flow, capacity[u][v]);
                    if (v == t) {
                        return new_flow;
                    }
                    q.emplace(v, new_flow);
                }
            }
        }
        return 0;
    }

    void init_preflow(int s, bool to_push = true) {
        height[s] = n;
        excess[s] = LLONG_MAX;
        for (int v : adj[s]) {
            if (push(s, v) && to_push) {
                excess_vertices.push(v);
            }
        }
    }

    bool push(int u, int v) {
        ll d = min(excess[u], capacity[u][v]);
        capacity[u][v] -= d;
        capacity[v][u] += d;
        excess[u] -= d;
        excess[v] += d;
        return d && excess[v] == d;
    }

    void relabel(int u) {
        int d = INT_MAX;
        for (int v: adj[u]) {
            if (capacity[u][v] > 0) {
                d = min(d, height[v]);
            }
        }
        if (d < INT_MAX) {
            height[u] = d + 1;
        }
    }

    void discharge(int u) {
        auto& adj = this->adj[u];
        auto& it = current_arc[u];
        while (excess[u] > 0) {
            if (it == adj.end()) {
                relabel(u);
                it = adj.begin();
            } else {
                int v = *it;
                if (capacity[u][v] > 0 && height[u] == height[v] + 1) {
                    if (push(u, v)) excess_vertices.push(v);
                } else {
                    ++it;
                }
            }
        }
    }

    vector<int> find_max_height_vertices(int s, int t) {
        vector<int> max_height_vertices;
        for (int i = 0; i < n; ++i) {
            if (i == s || i == t || !excess[i]) continue;
            if (!max_height_vertices.empty() && height[i] > height[max_height_vertices.back()]) {
                max_height_vertices.clear();
            }
            if (max_height_vertices.empty() || height[i] == height[max_height_vertices.back()]) {
                max_height_vertices.push_back(i);
            }
        }
        return max_height_vertices;
    }

    

public:
    MaxFlow(int n) : n(n), adj(n), capacity(n, vector<ll>(n, 0)) {}

    void add_edge(int u, int v, ll cap) {
        if (capacity[u][v] == 0) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        capacity[u][v] += cap;
        capacity[v][u] += cap;
    }

    ll edmond_karp(int s, int t) {
        parent.resize(n);
        ll flow = 0, new_flow;
        while ((new_flow = bfs(s, t))) {
            flow += new_flow;
            for (int prev, cur = t; cur != s; cur = prev) {
                prev = parent[cur];
                capacity[prev][cur] -= new_flow;
                capacity[cur][prev] += new_flow;
            }
        }
        return flow;
    }

    ll push_relabel(int s, int t) {
        excess.resize(n, 0);
        height.resize(n, 0);
        current_arc.resize(n);
        while (!excess_vertices.empty()) {
            excess_vertices.pop();
        }
        for (int i = 0; i < n; ++i) {
            current_arc[i] = adj[i].begin();
        }
        init_preflow(s);

        while (!excess_vertices.empty()) {
            int u = excess_vertices.front();
            excess_vertices.pop();
            if (u == s || u == t) continue;
            discharge(u);
        }
        return excess[t];
    }

    ll push_relabel_max_height(int s, int t) {
        excess.resize(n, 0);
        height.resize(n, 0);
        init_preflow(s);
        
        vector<int> current;
        while (true) {
            current = find_max_height_vertices(s, t);
            if (current.empty()) break;

            for (int i: current) {
                bool pushed = false;
                for (int v : adj[i]) {
                    if (capacity[i][v] > 0 && height[i] == height[v] + 1) {
                        push(i, v);
                        pushed = true;
                    }
                }
                if (!pushed) {
                    relabel(i);
                    break;
                }
            }
        }
        return excess[t];
    }

    vector<pair<int, int>> get_minimum_cut_edges(int s) {
        queue<int> q;
        vector<bool> visited(n, false);
        q.push(s);
        visited[s] = true;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (!visited[v] && capacity[u][v] > 0) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
        vector<pair<int, int>> cut_edges;
        for (int u = 0; u < n; ++u) {
            for (int v : adj[u]) {
                if (visited[u] && !visited[v] && capacity[u][v] == 0) {
                    cut_edges.emplace_back(u, v);
                }
            }
        }
        return cut_edges;
    }
};

int main() {
    int n, m;
    cin >> n >> m;
    MaxFlow mf(n);
    
    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        --a; --b;
        mf.add_edge(a, b, 1);
    }
    cout << mf.push_relabel(0, n - 1) << endl;
    auto cut_edges = mf.get_minimum_cut_edges(0);
    for (const auto& edge : cut_edges) {
        cout << edge.first + 1 << " " << edge.second + 1 << endl;
    }
}