#include <iostream>
#include <climits>
#include <vector>

using namespace std;

int vMax;

struct F {
    int a = 0, b = vMax, c = 0;
    F() = default;
    F(int a, int b, int c) : a(a), b(b), c(c) {}
    F(bool isIncrease) {
        if (isIncrease) {
            a = 0, b = vMax - 1, c = 1;
        } else {
            a = 1, b = vMax, c = 0;
        }
    }
    F operator+(const F& other) const {
        F res;
        // cout << "a: " << a << " b: " << b << " c: " << c << "\n";
        // cout << "other.a: " << other.a << " other.b: " << other.b << " other.c: " << other.c << "\n";
        res.b = other.b <= c + b - a ? a + other.b - c: b;
        if (c <= other.a) {
            res.c = other.c;
            if (other.a < c + b - a) {
                res.a = a + other.a - c;
            } else {
                res.b = 0;
            }
        } else {
            res.c = other.c + min(c, other.b) - other.a;
            if (c < other.b) {
                res.a = a;
            } else {
                res.b = 0;
            }
        }
        // cout << "res.a: " << res.a << " res.b: " << res.b << " res.c: " << res.c << "\n";
        return res;
    }

    pair<int, int> getRange() const {
        return {c, c + b - a};
    }

    int getRightmostAt(int y) const {
        if (y >= c + b - a) return vMax;
        if (y <= c) return a;
        return y - c + a;
    }
};

struct St {
    int n;
    vector<F> t;
    St(int n) : n(n), t(n << 1) {}

    void update(int i, const F& x) {
        for (t[i += n] = x; i >>= 1;) {
            t[i] = t[i << 1] + t[i << 1 | 1];
        }
    }

    F query(int l, int r) {
        F resl, resr;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) resl = resl + t[l++];
            if (r & 1) resr = t[--r] + resr;
        }
        return resl + resr;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, v2;
    cin >> n >> vMax >> v2;
    --n;
    vector<int> indices(n);
    vector<int> times(n, INT_MAX);
    vector<char> s(n);
    char tmp;
    int prevSec;
    cin >> tmp >> prevSec;
    for (int i = 0; i < n; i++) {
        int sec;
        cin >> s[i] >> sec;
        indices[i] = i;
        times[i] = sec - prevSec;
        prevSec = sec;
    }
    sort(indices.begin(), indices.end(), [&times](int a, int b) {
        return times[a] < times[b];
    });

    int resI = -1, resV1 = v2;
    if (n == 0) {
        cout << "0 " << v2 << endl;
        return 0;
    }
    St st(n);

    for (int i = 0; i < n; ++i) {
        int idx = indices[i];
        cout << endl;
        // cout << "i: " << i << "\n";
        // cout << "idx: " << idx << "\n";
        // cout << "s[idx]: " << s[idx] << "\n";
        // cout << "times[idx]: " << times[idx] << "\n";
        st.update(idx, F(s[idx] == '+'));
        auto f = st.query(0, n);
        auto [l, r] = f.getRange();
        
        // cout << "f: " << f.a << " " << f.b << " " << f.c << "\n";
        // cout << "l: " << l << " r: " << r << "\n";
        if ((i == n - 1 || times[idx] != times[indices[i + 1]]) && l <= v2 && v2 <= r) {
            resI = i;
            resV1 = f.getRightmostAt(v2);
            // cout << endl;
            // cout << "\tresI: " << resI << "\n";
            // cout << "\tresV1: " << resV1 << "\n";
        }
    }
    // cout << "resI: " << resI << "\n";
    // cout << "resV1: " << resV1 << "\n";
    if (resI == -1) {
        cout << "0 " << v2 << "\n";
    } else if (resI == n - 1) {
        cout << "infinity\n";
    } else {
        cout << times[indices[resI + 1]] - 1 << " " << resV1 << "\n";
    }
    return 0;
}