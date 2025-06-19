#include <iostream>
#include <vector>

using namespace std;

struct Vote {
    int sum = 0, maxPrefixSum = 0, maxSuffixSum = 0, ans = 0;
    Vote() = default;
    Vote(char ch) : Vote() {
        if (ch == 'C') {
            sum = -1;
        } else {
            sum = maxPrefixSum = maxSuffixSum = ans = 1;
        }
    }

    Vote operator+(const Vote& other) const {
        Vote res;
        res.sum = sum + other.sum;
        res.maxPrefixSum = max(maxPrefixSum, sum + other.maxPrefixSum);
        res.maxSuffixSum = max(other.maxSuffixSum, other.sum + maxSuffixSum);
        res.ans = max({ans + other.sum, sum + other.ans, 
                       maxPrefixSum + other.maxSuffixSum});
        return res;
    }
};

struct St {
    int n;
    vector<Vote> t; 
    St(int n) : n(n), t(n << 1) {}

    void build(const string& s) {
        for (size_t i = 0; i < s.size(); ++i) {
            t[n + i] = Vote(s[i]);
        }
        for (int i = n - 1; i > 0; --i) {
            t[i] = t[i << 1] + t[i << 1 | 1];
        }
    }

    Vote query(int l, int r) {
        Vote resl, resr;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) resl = resl + t[l++];
            if (r & 1) resr = t[--r] + resr;
        }
        return resl + resr;
    }
};

int main() {
    ios::sync_with_stdio(false);
	cin.tie(0);
    int n;
    cin >> n;
    string s;
    cin >> s;
    St st(n);
    st.build(s);
    int q;
    cin >> q;
    while (q--) {
        int l, r;
        cin >> l >> r;
        Vote res = st.query(l - 1, r);
        cout << res.ans << endl;
    }
    return 0;
}