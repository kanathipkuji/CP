# include <iostream>
# include <vector>

using namespace std;
using ll = long long;

struct RangeQueryPointUpdateBIT {
    int n;
    vector<ll> t;

    RangeQueryPointUpdateBIT(int n) : n(n), t(n + 1) {}

    void update(int i, ll x) {
        for (; i <= n; i += i & -i) {
            t[i] += x;
        }
    }

    ll prefixSum(int i) {
        ll res = 0;
        for (; i > 0; i -= i & -i) {
            res += t[i];
        }
        return res;
    }

    ll query(int l, int r) {
        return prefixSum(r) - prefixSum(l - 1);
    }
};

struct BIT {
    int n;
    RangeQueryPointUpdateBIT t1, t2;

    BIT(int n) : n(n), t1(n + 1), t2(n + 1) {}

    void update(int l, int r, ll x) {
        t1.update(l, x);
        t1.update(r + 1, -x);
        t2.update(l, x * (l - 1));
        t2.update(r + 1, -x * r);
    }

    ll prefixSum(int i) {
        return i * t1.prefixSum(i) - t2.prefixSum(i);
    }

    ll query(int l, int r) {
        return prefixSum(r) - prefixSum(l - 1);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int t;
    cin >> t;
    while (t--) {
        int n, c;
        cin >> n >> c;
        BIT BIT(n);
        while (c--) {
            int c, p, q;
            cin >> c >> p >> q;
            if (c == 0) {
                int v;
                cin >> v;
                BIT.update(p, q, v);
            } else {
                cout << BIT.query(p, q) << '\n';
            }
        }
    }
    return 0;
}