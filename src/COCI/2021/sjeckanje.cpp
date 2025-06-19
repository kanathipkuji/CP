#include <iostream>
#include <vector>

using namespace std;

using ll = long long;
const ll INF = 4.5e18;

struct MaxVal {
    ll dp[2][2];
    ll l, r;

    MaxVal(ll x = 0) {
        dp[0][0] = 0;
        dp[0][1] = dp[1][0] = -INF;
        dp[1][1] = abs(x);
        l = r = x;
    }

    static int sgn(ll x) {
        if (x > 0) return +1;
        if (x < 0) return -1;
        return 0;
    }
    
    MaxVal operator+(const MaxVal &other) const {
        MaxVal res;
        res.l = l;
        res.r = other.r;
        for (int lb: {0, 1}) for (int rb: {0, 1}) {
            ll& T = res.dp[lb][rb];
            T = max(T, dp[lb][0] + other.dp[0][rb]);
            T = max(T, dp[lb][1] + other.dp[0][rb]);
            T = max(T, dp[lb][0] + other.dp[1][rb]);
            if (sgn(r) * sgn(other.l) != -1) {
                T = max(T, dp[lb][1] + other.dp[1][rb]);
            }
        }
        return res;
    }
};

template<typename T>
struct St {
    int n;
    vector<T> t;

    St(int n) : n(n), t(n << 1) {}

    void build(const vector<int> &a) {
        for (int i = 0; i < n; ++i) {
            t[i + n] = MaxVal(a[i]);
        }
        for (int i = n - 1; i > 0; --i) {
            t[i] = t[i << 1] + t[i << 1 | 1];
        }
    }

    T query(int l, int r) {
        T resl, resr;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) resl = resl + t[l++];
            if (r & 1) resr = t[--r] + resr;
        }
        return resl + resr;
    }

    void update(int p, const T &v) {
        for (t[p += n] = v; p >>= 1;) {
            t[p] = t[p << 1] + t[p << 1 | 1];
        }
    }
};

int main() {
    int n, m;
    cin >> n >> m;
    vector<int> a(n);
    vector<int> d(n - 1);
    cin >> a[0];
    for (int i = 1; i < n; ++i) {
        cin >> a[i];
        d[i - 1] = a[i] - a[i - 1];
    }
    St<MaxVal> st(n - 1);
    st.build(d);
    while (m--) {
        int l, r, x;
        cin >> l >> r >> x;
        --l, --r;
        if (l > 0) {
            d[l - 1] += x;
            st.update(l - 1, MaxVal(d[l - 1]));
        }
        if (r < n - 1) {
            d[r] -= x;
            st.update(r, MaxVal(d[r]));
        }
        MaxVal res = st.query(0, n - 1);
        ll ans = max({res.dp[0][0], res.dp[0][1], res.dp[1][0], res.dp[1][1]});
        cout << ans << endl;
    }
    return 0;
}