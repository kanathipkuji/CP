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
    int n;
    vector<MaxElement> t;

    St(vector<int>& a) : n(a.size()) {
        t.resize(n << 1);
        for (int i = 0; i < n; ++i) {
            t[n + i] = MaxElement(a[i]);
        }
        for (int i = n - 1; i > 0; --i) {
            t[i] = t[i << 1] + t[i << 1 | 1];
        }
    }

    int walk(int l, int r, int x) {
        if (l >= r) return -1;
        int resl, resr;
        resl = resr = -1;
        for (l += n,r += n; l < r ; l >>= 1, r >>= 1) {
            if (l & 1) {
                if (t[l].x >= x) {
                    resl = l;
                    break;
                }
                ++l;
            }
            if (r & 1) {
                --r;
                if (t[r].x >= x) {
                    resr = r;
                }
            }
        }
        if (resl == -1 && resr == -1) return -1;
        int i = resl == -1 ? resr : resl;
        while (i < n) {
            if (t[i << 1].x >= x) {
                i = i << 1;
            } else {
                i = i << 1 | 1;
            }
        }
        return i - n;
    }

    int queryUpdate(int x) {
        int j = walk(0, n, x);
        if (j == -1) return 0;
        int res = j + 1;
        j += n;
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