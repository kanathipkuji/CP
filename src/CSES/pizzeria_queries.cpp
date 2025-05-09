# include <climits>
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

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n, q;
    cin >> n >> q;
    vector<int> l(n), r(n);
    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;
        l[i] = x - i;
        r[i] = x + i;
    }

    St<
        int,
        [](int a, int b) {
            return min(a, b);
        },
        []() {
            return INT_MAX;
        }>
        segl(l), segr(r);
        
    
    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int k, x;
            cin >> k >> x;
            --k;
            segl.update(k, x - k);
            segr.update(k, x + k);
        } else {
            int k;
            cin >> k;
            --k;
            int l = segl.query(0, k + 1) + k;
            int r = segr.query(k, n) - k;
            cout << min(l, r) << '\n';
        }
    }
}