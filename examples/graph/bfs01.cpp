// Problem: Grid with walls '#'. Moving to a neighbor cell costs 1; teleports "r1 c1 r2 c2" cost 0
//   (one way). Print the distance from the top-left to the bottom-right, ignoring teleports (BFS
//   preset) and using them (0-1 BFS), and the path with teleports.
// Input:
//   3 4 1
//   ..#.
//   ..#.
//   ....
//   0 1 0 3
// Output:
//   5 3
//   (0,0)(0,1)(0,3)(1,3)(2,3)
#include <bits/stdc++.h>
using namespace std;

#include "graph/bfs01.cpp"  // in a solution: <leader>rl -> graph/bfs01

int main() {
    int R, C, k;
    cin >> R >> C >> k;
    vector<string> grid(R);
    for (auto &row : grid) cin >> row;
    auto id = [&](int r, int c) { return r * C + c; };
    vector<vector<int>> g(R * C);
    vector<vector<pair<int, int>>> wg(R * C);
    for (int r = 0; r < R; r++)
        for (int c = 0; c < C; c++) {
            if (grid[r][c] == '#') continue;
            int dr[] = {1, -1, 0, 0}, dc[] = {0, 0, 1, -1};
            for (int d = 0; d < 4; d++) {
                int nr = r + dr[d], nc = c + dc[d];
                if (nr < 0 || nr >= R || nc < 0 || nc >= C || grid[nr][nc] == '#') continue;
                g[id(r, c)].push_back(id(nr, nc));
                wg[id(r, c)].push_back({id(nr, nc), 1});
            }
        }
    while (k--) {
        int r1, c1, r2, c2;
        cin >> r1 >> c1 >> r2 >> c2;
        wg[id(r1, c1)].push_back({id(r2, c2), 0});
    }
    int s = id(0, 0), t = id(R - 1, C - 1);
    BFS plain(g, s);  // preset: unweighted
    BFS01 b(wg, s);
    cout << plain.dist[t] << ' ' << b.dist[t] << '\n';
    for (int v : b.path(t)) cout << '(' << v / C << ',' << v % C << ')';
    cout << '\n';
}
