# include <iostream>
# include <vector>

using namespace std;
using ull = unsigned long long;

const int MOD = 998244353;

template<typename T, auto op, auto e>
struct St {
    int n;
    vector<T> t;
    St(const vector<T>&a) : n(a.size()) {
        t = vector<T>(n << 1, e());
        for (int i = 0; i < n; ++i) {
            t[n + i] = T(a[i]);
        }
        for (int i = n - 1; i > 0; --i) {
            t[i] = op(t[i << 1], t[i << 1 | 1]);
        }
    }

    void update(int i, int c, int d) {
        for (t[i += n] = T(c, d); i >>= 1;) {
            t[i] = op(t[i << 1], t[i << 1 | 1]);
        }
    }

    T query(int l, int r) {
        T resl{e()}, resr{e()};
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) resl = op(resl, t[l++]);
            if (r & 1) resr = op(t[--r], resr);
        }
        return op(resl, resr);
    }
};

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n, q;
    cin >> n >> q;
    vector<pair<ull, ull>> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i].first >> a[i].second;
    }
    St<
        pair<ull, ull>,
        [](const pair<ull, ull>&a, const pair<ull, ull>&b) {
            return make_pair((a.first * b.first) % MOD, (a.second * b.first + b.second) % MOD);
        },
        []() {
            return make_pair(1, 0);
        }> 
        st(a);
    while (q--) {
        int type;
        cin >> type;
        if (type == 0) {
            int p, c, d;
            cin >> p >> c >> d;
            st.update(p, c, d);
        } else if (type == 1) {
            int l, r, x;
            cin >> l >> r >> x;
            pair<ull, ull> res = st.query(l, r);
            cout << (res.first * x + res.second) % MOD << '\n';
        }
    }
}