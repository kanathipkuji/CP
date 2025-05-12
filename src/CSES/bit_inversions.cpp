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
    int l0 = 0, r0 = 0, l1 = 0, r1 = 0, max_same_bit = 0, len = 0;
    Bin() = default;
    Bin(int l0, int r0, int l1, int r1, int max_same_bit, int len) : l0(l0), r0(r0), l1(l1), r1(r1), max_same_bit(max_same_bit), len(len) {}
    Bin(int c) : l0(0), r0(0), l1(0), r1(0), max_same_bit(1), len(1) {
        if (c == 0) {
            l0 = 1;
            r0 = 1;
        } else {
            l1 = 1;
            r1 = 1;
        }
    }

};

int main() {
    string s;
    cin >> s;
    int n = s.size();
    vector<int> si(n);
    for (int i = 0; i < n; ++i) {
        si[i] = s[i] - '0';
    }
    vector<Bin> a(n);
    for (int i = 0; i < n; ++i) {
        a[i] = Bin(si[i]);
    }
    int m;
    cin >> m;
    St <Bin, [](const Bin& a, const Bin& b) {
            return Bin(
                a.l0 == a.len ? a.l0 + b.l0 : a.l0,
                b.r0 == b.len ? a.r0 + b.r0 : b.r0,
                a.l1 == a.len ? a.l1 + b.l1 : a.l1,
                b.r1 == b.len ? a.r1 + b.r1 : b.r1,
                max({a.max_same_bit, b.max_same_bit, a.r0 + b.l0, a.r1 + b.l1}),
                a.len + b.len
            );
        },
        []() {
            return Bin(0, 0, 0, 0, 0, 0);
        }
        > st(a);
    while (m--) {
        int x;
        cin >> x;
        --x;
        si[x] ^= 1;
        st.update(x, Bin(si[x]));
        cout << st.query(0, n).max_same_bit << '\n';
    }
    return 0;
}