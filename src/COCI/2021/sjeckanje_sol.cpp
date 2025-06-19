#include <iostream>
#include <vector>

using namespace std;

#define TRACE(x) cerr << #x << " = " << x << endl
#define _ << " _ " <<

#define fi first
#define se second

typedef long long ll;
typedef long double ld;
typedef pair<int, int> pii;
typedef vector<int> vi;

const int OFF = 1 << 18;
const ll INF = 4.5e18;

int sgn(ll x) {
    if (x > 0) return +1;
    if (x < 0) return -1;
    return 0;
}

void maxeq(ll& x, ll y) {
    x = max(x, y);
}

ll t[2 * OFF][2][2], d[OFF];
int l[2 * OFF], r[2 * OFF], m[2 * OFF];

void update(int i, ll x) {
    d[i] += x;

    i += OFF;
    t[i][0][0] = 0;
    t[i][1][1] = abs(d[m[i]]);
    t[i][0][1] = t[i][1][0] = -INF;

    for (i /= 2; i > 0; i /= 2) {
        for (int lb : {0, 1}) for (int rb : {0, 1}) {
            ll& T = t[i][lb][rb];
            T = -INF;
            maxeq(T, t[2 * i][lb][0] + t[2 * i + 1][0][rb]);
            maxeq(T, t[2 * i][lb][1] + t[2 * i + 1][0][rb]);
            maxeq(T, t[2 * i][lb][0] + t[2 * i + 1][1][rb]);
            if (sgn(d[m[i] - 1]) * sgn(d[m[i]]) != -1)
                maxeq(T, t[2 * i][lb][1] + t[2 * i + 1][1][rb]);
        }
    }
}

ll query() {
    return max(max(t[1][0][0], t[1][0][1]),
               max(t[1][1][0], t[1][1][1]));
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    l[1] = 0, r[1] = OFF;
    for (int i = 1; i < 2 * OFF; i++) {
        m[i] = (l[i] + r[i]) / 2;
        if (i < OFF) {
            l[2 * i + 0] = l[i];
            r[2 * i + 0] = l[2 * i + 1] = m[i];
            r[2 * i + 1] = r[i];
        }
    }

    int n, q;
    cin >> n >> q;
    ll x;
    for (int i = 0; i < n; i++) {
        ll y;
        cin >> y;
        if (i > 0) update(i - 1, x - y);
        x = y;
    }

    while (q--) {
        int lq, rq;
        ll w;
        cin >> lq >> rq >> w;

        if (lq > 1) update(lq - 2, -w);
        if (rq < n) update(rq - 1, +w);

        cout << query() << '\n';
    }

    return 0;
}

