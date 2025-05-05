# include <iostream>
# include <vector>

using namespace std;

struct MaxCorrectBracket {
    int l, r, m;
    MaxCorrectBracket(int l = 0, int r = 0, int m = 0) : l(l), r(r), m(m) {}
    MaxCorrectBracket operator+(const MaxCorrectBracket &b) {
        MaxCorrectBracket c;
        int x = min(l, b.r);
        c.l = l + b.l - x;
        c.r = r + b.r - x;
        c.m = m + b.m + 2 * x;
        return c;
    }
};

struct St {
    int n;
    string s;
    vector<MaxCorrectBracket> st;

    St(const string& s) : s(s), n(s.size()), st(n << 1) {
        st.resize(n << 1);
        build();
    }

    void build() {
        for (int i = 0; i < n; ++i) {
            st[n + i] = MaxCorrectBracket(s[i] == '(', s[i] == ')', 0);
        }
        for (int i = n - 1; i > 0; --i) {
            st[i] = st[i << 1] + st[i << 1 | 1];
        }
    }

    MaxCorrectBracket query(int l, int r) {
        MaxCorrectBracket resl, resr;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) resl = resl + st[l++];
            if (r & 1) resr = st[--r] + resr;
        }
        return resl + resr;
    }

};

ostream& operator<<(ostream &os, const MaxCorrectBracket &b) {
    os << b.m;
    return os;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    string s;
    int n;
    cin >> s >> n;
    St st(s);
    while (n--) {
        int l ,r;
        cin >> l >> r;
        cout << st.query(l - 1, r) << '\n';
    }
}