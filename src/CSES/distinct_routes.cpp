#include <iostream>
#include <ostream>
#include <vector>
#include <queue>
#include <sstream>
#include <climits>

using namespace std;

struct MaxFlow {
    int n;
    vector<vector<int>> capacity, inMaxFlow, residualWeight;
    vector<vector<int>> adj;
    vector<int> parent;
    ostringstream output;

    MaxFlow(int n) : n(n), capacity(n, vector<int>(n, 0)), inMaxFlow(n, vector<int>(n)), residualWeight(n, vector<int>(n)), adj(n), parent(n) {}

    void add_edge(int u, int v, int cap) {
        capacity[u][v] += cap;
        residualWeight[u][v] += cap;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int bfs(int s, int t) {
        fill(parent.begin(), parent.end(), -1);
        parent[s] = -2;
        queue<pair<int, int>> q;
        q.emplace(s, INT_MAX);
        while (!q.empty()) {
            auto [u, min_weight] = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (parent[v] == -1 && residualWeight[u][v] > 0) {
                    parent[v] = u;
                    int new_min_weight = min(min_weight, residualWeight[u][v]); 
                    if (v == t) {
                        return new_min_weight;
                    }
                    q.emplace(v, min(min_weight, new_min_weight));
                }
            }
        }
        return 0;
    }

    int edmond_karp(int s, int t) {
        int max_flow = 0, flow;
        output = ostringstream();
        while ((flow = bfs(s, t))) {
            max_flow += flow;
            vector<int> path;
            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                residualWeight[u][v] -= flow;
                residualWeight[v][u] += flow;
                path.push_back(v);
            }
            path.push_back(s);

            output << path.size() << endl;
            for (auto it = path.rbegin(); it != path.rend(); ++it) {
                if (it + 1 != path.rend()) {
                    inMaxFlow[*it][*(it + 1)] += flow;
                    inMaxFlow[*(it + 1)][*it] -= flow;
                }
                output << *it + 1 << " ";
            }
            output << endl;
        }
        return max_flow;
    }

    void printMaxFlow(int s, int t) {
        edmond_karp(s, t);
        for (int u = 0; u < n; ++u) {
            for (int v : adj[u]) {
                residualWeight[u][v] = inMaxFlow[u][v] - inMaxFlow[v][u] ? capacity[u][v] : 0;
            }
        }
        cout << edmond_karp(s, t) << endl;
        cout << output.str();
    }
};

int main() {
    int n, m;
    cin >> n >> m;
    MaxFlow mf(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        --u, --v;
        mf.add_edge(u, v, 1);
    }
    mf.printMaxFlow(0, n - 1);
    return 0;
}