# include <iostream>
# include <vector>

using namespace std;

using ll = long long;

template<class T, auto op, auto e>
struct St {
    int n;
    vector<T> t;
    
    St(const vector<T>& a) : n(a.size()) {
        t = vector<T>(n << 1, e());
        build(a);
    }

    void build(const vector<T>& a) {
        for (int i = 0; i < n; ++i) {
            t[n + i] = a[i];
        }
        for (int i = n - 1; i > 0; --i) {
            t[i] = op(t[i << 1], t[i << 1 | 1]);
        }
    }
    
    void update(int i, const T& x) {
        for (t[i += n] = x; i >>=1; ) {
            t[i] = op(t[i << 1], t[i << 1 | 1]);
        }
    }

    T query(int l, int r) {
        T resl = e(), resr = e();
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) resl = op(resl, t[l++]);
            if (r & 1) resr = op(t[--r], resr);
        }
        return op(resl, resr);
    }

};

struct MaxSum {
    ll sum, maxl, maxr, max;
    MaxSum(ll sum = 0, ll maxl = 0, ll maxr = 0, ll max = 0) : sum(sum), maxl(maxl), maxr(maxr), max(max) {}
    MaxSum(int x) : sum(x), maxl(x), maxr(x), max(std::max(x, 0)) {}
};

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n, q;
    cin >> n >> q;
    vector<MaxSum> a(n);
    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;
        a[i] = MaxSum(x);
    }

    St<
        MaxSum,
        [](const MaxSum& a, const MaxSum& b) {
            return MaxSum(
                a.sum + b.sum,
                max(a.maxl, a.sum + b.maxl),
                max(b.maxr, b.sum + a.maxr),
                max({a.max, b.max, a.maxr + b.maxl})
            );
        },
        []() {
            return MaxSum(0);
        }>
        st(a);
        
    
    while (q--) {
        int k, x;
        cin >> k >> x;
        st.update(k - 1, MaxSum(x));
        cout << st.query(0, n).max << '\n';
    }
}