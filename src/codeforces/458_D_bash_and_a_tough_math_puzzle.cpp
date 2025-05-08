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

struct St {
    int n;
    vector<int> st;

    St(const vector<int>& a) : n(a.size()) {
        st.resize(n << 1, 1);
        for (int i = 0; i < n; ++i) {
            st[n + i] = a[i];
        }
        for (int i = n - 1; i > 0; --i) {
            st[i] = gcd(st[i << 1], st[i << 1 | 1]);
        }
    }

    int first_index_of_non_multiple(int l, int r, int x) {
        if (l >= r) return -1;
        int resl, resr;
        resl = resr = -1;
        int rr = r;
        for (l += n,r += n; l < r ; l >>= 1, r >>= 1) {
            if (l & 1) {
                if (st[l] % x != 0) {
                    resl = l;
                    break;
                }
                ++l;
            }
            if (r & 1) {
                --r;
                if (st[r] % x != 0) {
                    resr = r;
                }
            }
        }
        if (resl == -1 && resr == -1) return -1;
        int i = resl == -1 ? resr : resl;
        while (i < n) {
            if (st[i << 1] % x != 0) {
                i = i << 1;
            } else {
                i = i << 1 | 1;
            }
        }
        return i - n;
    }

    void modify(int i, int x) {
        for (st[i += n] = x; i > 1; i >>= 1) {
            st[i >> 1] = gcd(st[i], st[i ^ 1]);
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