// Problem: Grid with walls '#'; moving to a neighbor cell costs 1. Print the distance from the
//   top-left to the bottom-right and the path.
// Input:
//   3 4
//   ..#.
//   ..#.
//   ....
// Output:
//   5
//   (0,0)(1,0)(2,0)(2,1)(2,2)(2,3)
#include <bits/stdc++.h>
using namespace std;

#include "graph/bfs01.bfs.cpp"  // in a solution: <leader>rl -> graph/bfs01 -> bfs

int main() {
    int R, C;
    cin >> R >> C;
    vector<string> grid(R);
    for (auto &row : grid) cin >> row;
    auto id = [&](int r, int c) { return r * C + c; };
    vector<vector<int>> g(R * C);
    for (int r = 0; r < R; r++)
        for (int c = 0; c < C; c++) {
            if (grid[r][c] == '#') continue;
            int dr[] = {1, -1, 0, 0}, dc[] = {0, 0, 1, -1};
            for (int d = 0; d < 4; d++) {
                int nr = r + dr[d], nc = c + dc[d];
                if (nr < 0 || nr >= R || nc < 0 || nc >= C || grid[nr][nc] == '#') continue;
                g[id(r, c)].push_back(id(nr, nc));
            }
        }
    BFS b(g, id(0, 0));
    int t = id(R - 1, C - 1);
    cout << b.dist[t] << '\n';
    for (int v : b.path(t)) cout << '(' << v / C << ',' << v % C << ')';
    cout << '\n';
}
