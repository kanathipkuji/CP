# include <iostream>
# include <utility>
# include <vector>
# include <fstream>

using namespace std;

ofstream fout("marathon.out");
ifstream fin("marathon.in");

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

int dist(const pair<int, int>&a, const pair<int, int>&b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}

int dist2box(const pair<int, int>&a, const pair<int, int>&b, const pair<int, int>&x) {
    int dx, dy;
    dx = dy = 0;
    if (max(a.first, b.first) < x.first) {
        dx = x.first - max(a.first, b.first);
    } else if (min(a.first, b.first) > x.first) {
        dx = min(a.first, b.first) - x.first;
    }
    if (max(a.second, b.second) < x.second) {
        dy = x.second - max(a.second, b.second);
    } else if (min(a.second, b.second) > x.second) {
        dy = min(a.second, b.second) - x.second;
    }
    return dx + dy;
}

int add(int a, int b) {
    return a + b;
}

int maximum(int a, int b) {
    return max(a, b);
}

int ident() {
    return 0;
}



int main() {    
    int n, q;
    fin >> n >> q;
    vector<pair<int, int>> p(n);
    vector<int> d(n), d2(n);
    for (int i = 0; i < n; ++i) {
        int x, y;
        fin >> x >> y;
        p[i] = {x, y};
        if (i) {
            d[i] = dist(p[i], p[i - 1]);
            if (i > 1) {
                d2[i - 1] = dist2box(p[i - 2], p[i], p[i - 1]);
            }
        }
    }
    St <int, decltype(&add), decltype(&ident)> st1(d, &add, &ident);
    St <int, decltype(&maximum), decltype(&ident)> st2(d2, &maximum, &ident);
    

    while (q--) {
        char type;
        fin >> type;
        if (type == 'Q') {
            int l, r;
            fin >> l >> r;
            int res = st1.query(l, r) - 2 * st2.query(l, r - 1);
            fout << res << '\n';
        } else {
            int i, x, y;
            fin >> i >> x >> y;
            --i;
            p[i] = {x, y};
            if (i > 0) {
                d[i] = dist(p[i], p[i - 1]);
                st1.update(i, d[i]);
                if (i < n - 1) {
                    d2[i] = dist2box(p[i - 1], p[i + 1], p[i]);
                    st2.update(i, d2[i]);
                }
                if (i > 1) {
                    d2[i - 1] = dist2box(p[i - 2], p[i], p[i - 1]);
                    st2.update(i - 1, d2[i - 1]);
                }
            }
            if (i < n - 1) {
                d[i + 1] = dist(p[i + 1], p[i]);
                st1.update(i + 1, d[i + 1]);
                if (i < n - 2) {
                    d2[i + 1] = dist2box(p[i], p[i + 2], p[i + 1]);
                    st2.update(i + 1, d2[i + 1]);
                }
            }
            
        }
    }
}