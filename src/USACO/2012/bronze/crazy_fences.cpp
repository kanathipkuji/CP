// # include <iostream>
// # include <vector>

// using namespace std;

// struct Endpoint {
//     int x, y, id;
//     bool valid;
//     int gaps;
//     bool operator<(const Endpoint& other) const {
//         return x < other.x || (x == other.x && y < other.y);
//     }

//     Endpoint(int x, int y, int id, bool valid) : x(x), y(y), id(id), valid(valid) {}
// };

// struct Segment {
//     int x1, y1, x2, y2;
// };

// int main() {
//     ios::sync_with_stdio(false);
//     cin.tie(0);

//     int n, c;
//     cin >> n >> c;
//     vector<Endpoint> endpoints;
//     vector<Segment> segments;
//     endpoints.reserve(3 * n);

//     for (int i = 0; i < n; ++i) {
//         int x1, y1, x2, y2;
//         cin >> x1 >> y1 >> x2 >> y2;
//         if (x1 > x2) swap(x1, x2);
//         if (y1 > y2) swap(y1, y2);
//         segments.emplace_back(x1, y1, x2, y2);
//         if (y1 == y2) {
//             endpoints.emplace_back(x1, y1, i, false);
//             endpoints.emplace_back(x2, y2, i, false);
//             endpoints.emplace_back(x1, y1 - 1, i, true);
//             endpoints.emplace_back(x1, y1 + 1, i, true);
//             endpoints.emplace_back(x2, y2 - 1, i, true);
//             endpoints.emplace_back(x2, y2 + 1, i, true);
//         } else if (x1 == x2) {
//             endpoints.emplace_back(x1, y1, i);
//             endpoints.emplace_back(x2, y2, i);
//             endpoints.emplace_back(x1 - 1, y1, i, true);
//             endpoints.emplace_back(x1 + 1, y1, i, true);
//             endpoints.emplace_back(x2 - 1, y2, i, true);
//             endpoints.emplace_back(x2 + 1, y2, i, true);
//         }
//     }
//     return 0;
// }