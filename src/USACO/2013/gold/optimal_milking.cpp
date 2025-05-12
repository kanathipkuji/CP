# include <iostream>
# include <vector>
# include <fstream>

using namespace std;

ofstream fout("optmilk.out");
ifstream fin("optmilk.in");

struct MaxWithGap {
    int xx = 0, xo = 0, ox = 0, oo = 0;

    MaxWithGap() = default;
    MaxWithGap(int xx, int xo, int ox, int oo) : xx(xx), xo(xo), ox(ox), oo(oo) {}
};

template<typename T, typename Op, typename E>
struct St {
    int n;
    vector<T> t;
    Op op;
    E e;

    St(const vector<T>& a) : n(a.size()) {
        t = vector<T>(n << 1, e());
        build(a);
    }

    St(const vector<T>& a, Op op, E e) : n(a.size()), op(op), e(e) {
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

MaxWithGap combine(const MaxWithGap& a, const MaxWithGap& b) {
    MaxWithGap res;
    res.xx = max(a.xo + b.xx, a.xx + b.ox);
    res.xo = max(a.xo + b.xo, a.xx + b.oo);
    res.ox = max(a.oo + b.xx, a.ox + b.ox);
    res.oo = max(a.oo + b.xo, a.ox + b.oo);
    return res;
}

MaxWithGap ident() {
    return MaxWithGap();
}

int main() {
    int n, m;
    fin >> n >> m;
    vector<MaxWithGap> a(n);
    for (int i = 0; i < n; ++i) {
        fin >> a[i].oo;
    }
    St<MaxWithGap, decltype(&combine), decltype(&ident)> st(a, combine, ident);
    long long res = 0;
    while (m--) {
        int i, x;
        fin >> i >> x;
        --i;
        st.update(i, {0, 0, 0, x});
        res += st.query(0, n).oo;
    }
    fout << res << endl;
}