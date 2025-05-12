# include <iostream>
# include <vector>
# include <fstream>
# include <algorithm>

using namespace std;

ofstream fout("cardgame.out");
ifstream fin("cardgame.in");

struct CoveredCard {
    int coverable = 0, cover = 0;
    int point = 0;

    CoveredCard() = default;
    CoveredCard(int coverable, int cover, int point) : coverable(coverable), cover(cover), point(point) {}
    CoveredCard(bool shouldCover) : coverable(!shouldCover), cover(shouldCover) {}
    CoveredCard operator+(const CoveredCard& other) const {
        
        int numPair = min(coverable, other.cover);
        return CoveredCard(
            coverable + other.coverable - numPair,
            cover + other.cover - numPair,
            point + other.point + numPair
        );
    }
};

struct St {
    int n;
    vector<CoveredCard> t;

    St(int n) : n(n), t(n << 1) {}
    
    void update(int i, bool shouldCover) {
        t[i += n] = CoveredCard(shouldCover);
        while(i >>= 1) {
            t[i] = t[i << 1] + t[i << 1 | 1];
        }
    }

    CoveredCard query(int l, int r) {
        CoveredCard resl, resr;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) resl = resl + t[l++];
            if (r & 1) resr = t[--r] + resr;
        }
        return resl + resr;
    }
};

int main() {
    int n;
    fin >> n;
    vector<int> a(n), b;
    vector<int> idA(n << 1, -1);
    for (int i = 0; i < n; ++i) {
        fin >> a[i];
        --a[i];
        idA[a[i]] = i;
    }
    for (int i = 2 * n - 1; i >= 0; --i) {
        if (idA[i] == -1) {
            b.push_back(i);
        }
    }
    St st(n << 1), st2(n << 1);
    vector<int> maxl(n, 0), maxr(n, 0);
    for (int i = 0; i < n; ++i) {
        st.update(a[i], false);
        st.update(b[i], true);
        maxl[i] = st.query(0, n << 1).point;

        st2.update(a[n - i - 1], true);
        st2.update(b[n - i - 1], false);
        maxr[n - i - 1] = st2.query(0, n << 1).point;
    }
    int res = max(maxl[n - 1], maxr[0]);
    for (int i = 1; i < n; ++i) {
        res = max(res, maxl[i - 1] + maxr[i]);
    }
    fout << res << endl;
}