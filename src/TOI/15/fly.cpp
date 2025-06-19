#include <iostream>
#include <utility>
#include <vector>

using namespace std;

struct Wall {
    int pos, n;
    bool dir;

    Wall() : pos(0), n(0), dir(' ') {}
    Wall(int pos, int n, char dir) : pos(pos), n(n), dir(dir == 'L') {}

    Wall shift(int t) {
        Wall res = *this;
        int shift = dir ? -t : t;
        res.pos += shift;
        if (res.pos < 0) {
            res.pos = -res.pos;
            res.dir = !res.dir;
        }
        if (res.pos / n % 2) {
            res.pos = n - res.pos % n;
            res.dir = !res.dir;
        } else {
            res.pos = res.pos % n;
        }
        if (res.pos == 0) {
            res.dir = 1;
        }
        if (res.pos == n) {
            res.dir = 0;
        }
        return res;
    }

    int timeToMeet(int x) {
        int t = 0;
        if (dir) {
            if (pos < x) {
                t = x - pos;
            } else {
                t = n - pos + x;
            }
        } else {
            if (pos > x) {
                t = pos - x;
            } else {
                t = n - x + pos;
            }
        }
        return t;
    }

    friend ostream& operator<<(ostream& os, const Wall& wall) {
        os << " " << wall.pos << ", " << (wall.dir ? 'L' : 'R');
        return os;
    }
};

struct WallPair {
    Wall left, right;
    int n;


    WallPair() : left(), right(), n() {}
    WallPair(Wall left, Wall right, int n) : left(left), right(right), n(n) {} 

    WallPair shift(int t) {
        WallPair res = *this;
        t %= 2 * n;
        res.left = left.shift(t);
        res.right = right.shift(t);
        return res;
    }

    int timeTilOpen(int x) {
        int t1 = left.timeToMeet(x);
        int t2 = right.timeToMeet(x);
        if (t1 > t2) swap(t1, t2);
        return t1 + (t2 - t1) / 2;
    }

    pair<pair<int, int>, pair<int, int>> getOpenTimeRange(int x, int t) {

    }

    friend ostream& operator<<(ostream& os, const WallPair& wallPair) {
        os << "left: " << wallPair.left << ", right: " << wallPair.right;
        return os;
    }
};

int main() {
    int R, C;
    cin >> R >> C;
    vector<WallPair> walls(R);
    while (true) {
        // int pos;
        // char dir;
        // cin >> pos >> dir;
        // Wall wall(pos, C, dir);
        // for (int t = - (C + 1); t < 4 * (C + 1); ++t) {
        //     cout << "t: " << t << endl;
        //     cout << '\t' << wall.shift(t) << endl;
        // }
        int l, r;
        char dirl, dirr;
        cin >> l >> dirl >> r >> dirr;
        WallPair wall(Wall(l, C, dirl), Wall(r, C, dirr), C);
        for (int t = - (C + 1); t < 2 * (C + 1); ++t) {
            cout << "t: " << t << endl;
            cout << '\t' << wall.shift(t) << endl;
        }
    }
    vector<pair<int,int>> flytime(C);
    for (int i = 0; i < R; ++i) {
        int l, r;
        char dirl, dirr;
        cin >> l >> dirl >> r >> dirr;
        walls[i] = WallPair(Wall(l, C, dirl), Wall(r, C, dirr), C);

        for (int j = 0; j < C; ++j) {

        }
    }
}