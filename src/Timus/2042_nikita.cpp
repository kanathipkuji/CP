# include <iostream>
# include <vector>

using namespace std;

struct NumPalindrome {
    int l, r, m;
    NumPalindrome(int l = 0, int r = 0, int m = 0) : l(l), r(r), m(m) {}
    NumPalindrome operator+(const NumPalindrome &b) {
        NumPalindrome c;
        int x = min(l, b.r);
        c.l = l + b.l - x;
        c.r = r + b.r - x;
        c.m = m + b.m + 2 * x;
        return c;
    }
};

struct St {
    int n, k;
    string s;
    vector<NumPalindrome> st;

    St(const string& s, int k) : s(s), n(s.size()), st(n << 1), k(k) {
        build();
    }

    void build() {
        for (int i = 0; i < n; ++i) {
            st[n + i] = NumPalindrome(s[i] == '(', s[i] == ')', 0);
        }
        for (int i = n - 1; i > 0; --i) {
            st[i] = st[i << 1] + st[i << 1 | 1];
        }
    }

    NumPalindrome query(int l, int r) {
        NumPalindrome resl, resr;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) resl = resl + st[l++];
            if (r & 1) resr = st[--r] + resr;
        }
        return resl + resr;
    }

};

ostream& operator<<(ostream &os, const NumPalindrome &b) {
    os << b.m;
    return os;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    string s;
    int k, m;
    cin >> s >> k >> m;
    St st(s, k);
    while (m--) {
        int t, l ,r;
        cin >> t >> l >> r;
        if (t == 1) {
            char c;
            cin >> c;

        } else {
            cout << st.query(l - 1, r) << '\n';
        }
    }
}