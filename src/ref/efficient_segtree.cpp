#include <climits>
#include <iostream>
#include <vector>

using namespace std;

struct St {
    struct MinPrefixSum {
        int sum, minPrefixSum, lazySum;
        int maxIndexMinPrefixSum;

        MinPrefixSum(int x = 0, int min = 1e9, int idx = 0)
            : sum(x), minPrefixSum(min), lazySum(0), maxIndexMinPrefixSum(idx) {}

        MinPrefixSum operator+(const MinPrefixSum &other) const {
            MinPrefixSum res;
            res.sum = sum + other.sum;
            res.minPrefixSum = min(minPrefixSum, other.minPrefixSum);
            res.maxIndexMinPrefixSum = (minPrefixSum < other.minPrefixSum) ? 
                            maxIndexMinPrefixSum : other.maxIndexMinPrefixSum;
            return res;
        }

        void applyLazy(int k) {
            if (lazySum != 0) {
                sum += lazySum * k;
                minPrefixSum += lazySum * k;
            }
        }
    };
    int n, n2, h;
    vector<MinPrefixSum> t;

    St(vector<int> &a) : n(a.size()) {
        n2 = 1;
        h = 0;
        while (n2 < n) n2 <<= 1, ++h;
        t.resize(n2 << 1);
        for (int i = 0; i < n; ++i) {
            t[i + n2] = MinPrefixSum(a[i], a[i], i);
        }
        build(0, n2);
    }

    void calc(int p, int k) {
        t[p] = t[p << 1] + t[p << 1 | 1];
        t[p].applyLazy(k);
    }

    void build(int l, int r) {
        int k = 2;
        for (l += n2, r += n2; l > 1; k <<= 1) {
            l >>= 1, r >>= 1;
            for (int i = r; i >= l; --i) calc(i, k);
        }
    }

    void apply(int p, int val, int k) {
        t[p].sum += val * k;
        t[p].minPrefixSum += val * k;
        if (p < n2) t[p].lazySum += val;
    }

    void push(int l, int r) {
        int s = h, k = 1 << (h - 1);
        for (l += n, r += n - 1; s > 0; --s, k >>= 1) {
            for (int i = l >> s; i <= r >> s; ++i) if (t[i].lazySum != 0) {
                apply(i<<1, t[i].lazySum, k);
                apply(i<<1|1, t[i].lazySum, k);
                t[i].lazySum = 0;
            }
        }
    }

    void update(int l, int x) {
        if (x == 0) return;
        int r = n2;
        push(l, l + 1);
        push(r - 1, r);
        int l0 = l, r0 = r, k = 1;
        for (l += n2, r += n2; l < r; l >>= 1, r >>= 1, k <<= 1) {
            if (l & 1) apply(l++, x, k);
            if (r & 1) apply(--r, x, k);
        }
        build(l0, l0 + 1);
        build(r0 - 1, r0);
    }

    MinPrefixSum query(int l, int r) {
        push(l, l + 1);
        push(r - 1, r);
        MinPrefixSum resl, resr;
        for (l += n2, r += n2; l < r; l >>= 1, r >>= 1) {
            if (l & 1) resl = resl + t[l++];
            if (r & 1) resr = t[--r] + resr;
        }
        return resl + resr;
    }

    int getLastIndexOfMinPrefixSumBefore(int x) {
        auto res = query(0, x);
        cout << "MinPrefixSum: " << res.minPrefixSum << ", MaxIndexMinPrefixSum: " 
            << res.maxIndexMinPrefixSum << endl;
        return res.minPrefixSum > 0 ? -1 : res.maxIndexMinPrefixSum;
    }
};

int main() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    St st(a);
    int q;
    cin >> q;
    while (q--) {
        char type;
        cin >> type;
        if (type == 'U') {
            int p, x;
            cin >> p >> x;
            st.update(p - 1, x);
        } else if (type == 'Q') {
            int x, y;
            cin >> x >> y;
            cout << st.query(x - 1, y).minPrefixSum << endl;
        } else if (type == 'F') {
            int x;
            cin >> x;
            int index = st.getLastIndexOfMinPrefixSumBefore(x);
            cout << index + 1 << endl;
        }
    }
}