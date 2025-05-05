# include <iostream>
# include <vector>

using namespace std;

struct MaxElement {
    int x;
    MaxElement(int x = 0) : x(x) {}
    MaxElement operator+(const MaxElement &b) {
        MaxElement c;
        c.x = max(x, b.x);
        return c;
    }

    friend ostream& operator<<(ostream &os, const MaxElement &b) {
        os << b.x;
        return os;
    }
};

struct St {
    int n, n2;
    vector<MaxElement> t;

    St(vector<int>& a) : n(a.size()) {
        n2 = 1;
        while (n2 < n) n2 <<= 1;
        t.resize(n2 << 1);
        for (int i = 0; i < n; ++i) {
            t[n2 + i] = MaxElement(a[i]);
        }
        for (int i = n2 - 1; i > 0; --i) {
            t[i] = t[i << 1] + t[i << 1 | 1];
        }
    }

    int queryUpdate(int x) {
        int j = 1;
        while (j < n2) {
            if (t[j << 1].x >= x) {
                j <<= 1;
            } else if (t[j << 1 | 1].x >= x) {
                j = (j << 1) | 1;
            } else {
                return 0;
            }
        }
        if (t[j].x < x) return 0;
        int res = j - n2 + 1;
        for (t[j].x -= x; j > 1; j >>= 1) {
            t[j >> 1] = t[j] + t[j^1];
        }
        return res;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    St st(a);
    while (m--) {
        int x;
        cin >> x;
        cout << st.queryUpdate(x) << ' ';
    }
    cout << endl;
    return 0;
}