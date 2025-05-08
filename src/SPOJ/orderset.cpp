# include <iostream>
# include <vector>
# include <algorithm>

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

    int prefixSum(int r) {
        int res = 0;
        int l = n2;
        for (r += n2; l < r; l >>= 1, r >>= 1) {
            if (l & 1) res += t[l++];
            if (r & 1) res += t[--r];
        }
        return res;
    }
    
    int kth(int k) {
        int i = 1;
        while (i < n2) {
            if (t[i << 1] >= k) {
                i <<= 1;
            } else {
                k -= t[i << 1];
                i = (i << 1) | 1;
            }
        }
        // cout << "i: " << i << " k: " << k << endl;
        return k > 1 || t[i] == 0 ? -1 : i - n2 + 1;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int q;
    cin >> q;
    vector<pair<char, int>> queries(q);
    vector<int> indices;
    for (int i = 0; i < q; ++i) {
        char type;
        int val;
        cin >> type >> val;
        queries[i] = {type, val};
        if (type != 'K') 
            indices.push_back(val);
    }
    sort(indices.begin(), indices.end());
    indices.erase(unique(indices.begin(), indices.end()), indices.end());
    int n = indices.size();

    const auto id = [&](int x) {
        return lower_bound(indices.begin(), indices.end(), x) - indices.begin();
    };

    St st(n);
    vector<int> inSet(n);

    for (const auto& [type, val] : queries) {
        if (type == 'I') {
            if (!inSet[id(val)]) {
                inSet[id(val)] = 1;
                st.update(id(val), 1);
            }
        } else if (type == 'D') {
            if (inSet[id(val)]) {
                inSet[id(val)] = 0;
                st.update(id(val), 0);
            }
        } else if (type == 'K') {
            int res = st.kth(val);
            if (res == -1) {
                cout << "invalid\n";
            } else {
                cout << indices[res - 1] << '\n';
            }
        } else if (type == 'C') {
            cout << st.prefixSum(id(val)) << '\n';
        } else {
            exit(1);
        }
    }

    return 0;
}