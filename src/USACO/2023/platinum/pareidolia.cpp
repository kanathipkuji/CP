#include <iostream>
#include <vector>

using namespace std;

struct At {
    static constexpr string pattern = "bes";
    static constexpr int n = pattern.size();
    vector<long long> prefix, suffix;
    vector<int> count;
    int len = 0;
    long long sum = 0;

    At() {
        prefix.resize(n + 2);
        suffix.resize(n + 2);
        count.resize(n + 2);
    }

    At(char c) : At() {
        len = 1;
        // cout << "c: " << c << endl;
        for (int i = 1; i <= n; ++i) {
            if (c == pattern[i - 1]) {
                count[i] = 1;
            }
        }
        if (c == pattern[0]) {
            prefix[1] = 0;
        } else {
            count[0] = 1;
        }
        if (c == pattern[n - 1]) {
            suffix[n] = 0;
        } else {
            count[n + 1] = 1;
        }
        // cout << "count: ";
        // for (int i = 0; i <= n + 1; ++i) {
        //     cout << count[i] << " ";
        // }
        // cout << endl;
        // cout << "prefix: ";
        // for (int i = 0; i <= n + 1; ++i) {
        //     cout << prefix[i] << " ";
        // }
        // cout << endl;
    }

    At operator+(const At &other) const {
        At res;
        res.len = len + other.len;
        res.sum =  sum + other.sum;
        // res.sum += prefix[1] * other.len + other.suffix[n] * len;

        cout << "count:\t\t";
        for (int i = 0; i <= n + 1; ++i) {
            cout << count[i] << " ";
        }
        cout << endl;
        cout << "other.count:\t";
        for (int i = 0; i <= n + 1; ++i) {
            cout << other.count[i] << " ";
        }
        cout << endl;
        cout << "prefix:\t\t";
        for (int i = 0; i <= n + 1; ++i) {
            cout << prefix[i] << " ";
        }
        
        cout << endl;
        cout << "other.prefix:\t";
        for (int i = 0; i <= n + 1; ++i) {
            cout << other.prefix[i] << " ";
        }
        cout << endl;
        cout << "suffix:\t\t";
        for (int i = 0; i <= n + 1; ++i) {
            cout << suffix[i] << " ";
        }
        cout << endl;
        cout << "other.suffix:\t";
        for (int i = 0; i <= n + 1; ++i) {
            cout << other.suffix[i] << " ";
        }
        cout << endl;
        cout << "sum:\t\t" << sum << endl;
        cout << "other.sum:\t" << other.sum << endl;
        long long countl = len * other.count[0], countr = other.len * count[n + 1];
        for (int i = 1; i <= n; ++i) {
            if (i < n)
                res.sum += prefix[i] * other.suffix[i + 1];
            
            res.prefix[i] = prefix[i] + other.prefix[i] + countl;
            countl *= other.count[i];

            res.suffix[n - i + 1] = suffix[n - i + 1] + other.suffix[n - i + 1] + countr;
            countr *= count[n - i + 1];

            res.count[i] = count[i] + other.count[i];
        }

        cout << "res.count:\t";
        for (int i = 0; i <= n + 1; ++i) {
            cout << res.count[i] << " ";
        }
        cout << endl;
        cout << "res.prefix:\t";
        for (int i = 0; i <= n + 1; ++i) {
            cout << res.prefix[i] << " ";
        }
        cout << endl;
        cout << "res.sum:\t" << res.sum << endl;
        cout << endl;
        return res;
    }
};

template <typename T, typename inT>
struct St {
    int n;
    vector<T> t;

    St(int n) : n(n), t(n << 1) {}

    void build(const inT &a) {
        for (int i = 0; i < n; ++i) t[i + n] = T(a[i]);
        for (int i = n - 1; i > 0; --i) t[i] = t[i << 1] + t[i << 1 | 1];
    }

    void update(int p, const T& v) {
        for (t[p += n] = v; p >>= 1;) t[p] = t[p << 1] + t[p << 1 | 1];
    }

    T query(int l, int r) {
        T resl, resr;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) resl = resl + t[l++];
            if (r & 1) resr = t[--r] + resr;
        }
        return resl + resr;
    }
};

int main() {
    string s;
    cin >> s;
    int m;
    cin >> m;
    St<At, string> st(s.size());
    st.build(s);
    cout << st.query(0, s.size()).sum << endl;
    while (m--) {
        int p;
        char c;
        cin >> p >> c;
        st.update(p - 1, At(c));
        cout << st.query(0, s.size()).sum << endl;
    }
    
    return 0;
}