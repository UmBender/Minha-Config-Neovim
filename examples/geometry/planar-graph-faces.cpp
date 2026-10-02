// Problem: a planar drawing of a graph (points and straight non-crossing edges): print its bounded
//   faces (regions) and their areas.
// Input:
//   5 6
//   0 0
//   2 0
//   2 2
//   0 2
//   5 5
//   0 1
//   1 2
//   2 3
//   3 0
//   0 2
//   2 4
// Output:
//   0 1 2 area 2
//   0 2 3 area 2
#include <bits/stdc++.h>
using namespace std;

#include "geometry/point.cpp"               // required by geometry/planar-graph-faces (inserted with it)
#include "geometry/planar-graph-faces.cpp"  // in a solution: <leader>rl -> geometry/planar-graph-faces

using P = Point<long long>;

int main() {
    int n, m;
    cin >> n >> m;
    vector<P> pts(n);
    for (auto &q : pts) cin >> q;
    vector<vector<int>> g(n);
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        g[a].push_back(b), g[b].push_back(a);
    }
    for (auto &f : planarFaces(pts, g)) {
        long long a2 = 0;  // twice the signed area: > 0 for bounded faces
        for (int i = 0; i < (int)f.size(); i++) a2 += pts[f[i]].cross(pts[f[(i + 1) % f.size()]]);
        if (a2 <= 0) continue;  // the outer face
        for (int v : f) cout << v << ' ';
        cout << "area " << a2 / 2.0 << '\n';
    }
}
