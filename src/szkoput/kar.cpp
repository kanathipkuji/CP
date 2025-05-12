# include <iostream>
# include <vector>
# include <climits>

using namespace std;

struct Card {
    int la = 0, lb = INT_MAX, ra = 0, rb = INT_MAX, len = 1;
    bool aa = true, ab = true, ba = true, bb = true;
    Card() = default;
    Card(pair<int, int> p) {
        int x = min(p.first, p.second);
        int y = max(p.first, p.second);
        la = ra = x;
        lb = rb = y;
        ba = x == y;
    }
    Card operator+(const Card& other) const {
        Card res;
        res.la = la;
        res.lb = lb;
        res.ra = other.ra;
        res.rb = other.rb;
        res.aa = res.ab = res.ba = res.bb = false;
        if (ra <= other.la) {
            res.ab = res.ab || (aa && other.ab);
            res.aa = res.aa || (aa && other.aa);
            res.ba = res.ba || (ba && other.aa);
            res.bb = res.bb || (ba && other.ab);
        }
        if (rb <= other.la) {
            res.ab = res.ab || (ab && other.ab);
            res.aa = res.aa || (ab && other.aa);
            res.ba = res.ba || (bb && other.aa);
            res.bb = res.bb || (bb && other.ab);
        }
        if (ra <= other.lb) {
            res.ab = res.ab || (aa && other.bb);
            res.aa = res.aa || (aa && other.ba);
            res.ba = res.ba || (ba && other.ba);
            res.bb = res.bb || (ba && other.bb);
        }
        if (rb <= other.lb) {
            res.ab = res.ab || (ab && other.bb);
            res.aa = res.aa || (ab && other.ba);
            res.ba = res.ba || (bb && other.ba);
            res.bb = res.bb || (bb && other.bb);
        }
        return res;
    }

    bool getAns() const {
        return aa || ab || ba || bb;
    }
};

template<typename T>
struct St {
    int n;
    vector<T> t;

    St(vector<pair<int, int>> a) : n(a.size()), t(n << 1) {
        for (int i = 0; i < n; ++i) {
            t[i + n] = a[i];
        }
        build();
    }

    void build() {
        for (int i = n - 1; i > 0; --i) {
            t[i] = t[i << 1] + t[i << 1 | 1];
        }
    }

    void update(int i, T x) {
        for (t[i += n] = x; i >>=1; ) {
            t[i] = t[i << 1] + t[i << 1 | 1];
        }
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
    int n;
    cin >> n;
    vector<pair<int, int>> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i].first >> a[i].second;
    }
    int m;
    cin >> m;
    St<Card> st(a);
    while (m--) {
        int x, y;
        cin >> x >> y;
        --x;
        --y;
        swap(a[x], a[y]);
        st.update(x, Card(a[x]));
        st.update(y, Card(a[y]));
        Card res = st.query(0, n);
        if (res.getAns()) {
            cout << "TAK\n";
        } else {
            cout << "NIE\n";
        }
    }
    return 0;
}