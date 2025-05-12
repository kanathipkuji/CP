#include <algorithm>
# include <iostream>
# include <vector>

using namespace std;

template <typename T, auto op, auto e>
struct St {
    int n;
    vector<T> t;

    St (const vector<T>& a) : n(a.size()) {
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
        for (t[i += n] = x; i >>= 1; ) {
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

struct Bin {
    int maxPair = 0, r0 = 0, l1 = 0;
    Bin() = default;
    Bin(int maxPair, int r0, int l1) : maxPair(maxPair), r0(r0), l1(l1) {}
    Bin(int c) : maxPair(0), r0(c == 0), l1(c == 1) {}
};
  
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    string s;
    cin >> s;
    int n = s.size();
    vector<int> si(n);
    vector<Bin> a(n);
    for (int i = 0; i < n; ++i) {
        si[i] = s[i] - '0';
        a[i] = Bin(si[i]);
    }

    St <Bin,
        [](const Bin& a, const Bin& b) {
            int nPair = min(a.r0, b.l1);
            return Bin(
                    a.maxPair + b.maxPair + nPair, 
                    b.r0 + a.r0 - nPair, 
                    a.l1 + b.l1 - nPair);
        },
        []() { return Bin(); }>
        st(a);

    int m;
    cin >> m;
    while (m--) {
        int k;
        cin >> k;
        if (k == 1) {
            int x;
            cin >> x;
            --x;
            st.update(x, Bin(si[x] ^= 1));
        } else {
            int l, r;
            cin >> l >> r;
            --l;
            cout << r - l - 2 * st.query(l, r).maxPair << '\n';
        }
    }
    return 0;
}