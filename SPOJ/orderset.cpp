# include <iostream>
#include <vector>

using namespace std;

struct St {
    int n, n2;
    vector<int> t;

    St(int n) : n(n) {
        n2 = 1;
        while (n2 < n) n2 <<= 1;
        t.resize(n2 << 1);
    }

    void update(int i, int x) {
        for (t[i += n2] = x; i > 1; i >>= 1) {
            t[i >> 1] = t[i] + t[i ^ 1];
        }
    }

    int prefixSum(int i) {
        int res = 0;
        for (i += n2; i > 0; i >>= 1) {
            res += t[i];
        }
        return res;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int q;
    cin >> q;
    vector<pair<char, int>> queries(q);
    vector<int> compressedIndices;
    for (int i = 0; i < q; ++i) {
        char type;
        int val;
        cin >> type >> val;
        queries[i] = {type, val};
        if (type != 'K') 
            compressedIndices.push_back(val);
    }
    sort(compressedIndices.begin(), compressedIndices.end());
    compressedIndices.erase(unique(compressedIndices.begin(), compressedIndices.end()), compressedIndices.end());
    int n = compressedIndices.size();

    const auto id = [&](int x) {
        return lower_bound(compressedIndices.begin(), compressedIndices.end(), x) - compressedIndices.begin();
    };

    St st(n);

    for (const auto& [type, val] : queries) {
        if (type == 'I') {

        } else if (type == 'D') {

        } else if (type == 'K') {
            
        } else if (type == 'C') {
            cout << id(val) + 1 << '\n';
        } else {
            exit(1);
        }
    }

    return 0;
}