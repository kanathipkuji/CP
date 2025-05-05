# include <iostream>
# include <vector>

using namespace std;

int gcd(int a, int b) {
    while (b) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

struct RangeGCD {
    int x;
    RangeGCD(int x = 1) : x(x) {}
    RangeGCD operator+(const RangeGCD &b) {
        RangeGCD c;
        c.x = gcd(x, b.x);
        return c;
    }
};

struct St {
    int n, n2;
    vector<RangeGCD> st;

    St(const vector<int>& a) : n(a.size()) {
        n2 = 1;
        while (n2 < n) n2 <<= 1;
        st.resize(n2 << 1, 1);
        for (int i = 0; i < n; ++i) {
            st[n2 + i] = RangeGCD(a[i]);
        }
        for (int i = n2 - 1; i > 0; --i) {
            st[i] = st[i << 1] + st[i << 1 | 1];
        }
    }

    int first_index_of_non_multiple(int l, int r, int x) {
        if (l >= r) return -1;
        for (l += n2; l > 1 ; l >>= 1) {
            if (l & 1) {
                if (st[l].x % x != 0) {
                    break;
                }
                ++l;
            }
        }
        while (l < n2) {
            if (st[l << 1].x % x != 0) {
                l = l << 1;
            } else {
                l = l << 1 | 1;
            }
        }
        return l - n2 < r ? l - n2 : -1;
    }

    void modify(int i, int x) {
        for (st[i += n2] = x; i > 1; i >>= 1) {
            st[i >> 1] = st[i] + st[i ^ 1];
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    St st(a);
    cin >> m;
    while (m--) {
        int q;
        cin >> q;
        if (q == 1) {
            int l, r, x;
            cin >> l >> r >> x;
            --l;
            int resl = st.first_index_of_non_multiple(l, r, x);
            if (resl == -1) {
                cout << "YES\n";
                continue;
            }
            resl = st.first_index_of_non_multiple(resl + 1, r, x);
            if (resl == -1) {
                cout << "YES\n";
            } else {
                cout << "NO\n";
            }

            
        } else {
            int i, y;
            cin >> i >> y;
            st.modify(i - 1, y);
        }
    }
}