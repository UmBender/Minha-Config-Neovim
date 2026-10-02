// Title: Planar graph faces
// Description: Faces of a straight-line planar graph, each as the cycle of vertices around it.
// Usage:
//   auto faces = planarFaces(pts, g);   // pts[v]: position of v; g: adjacency lists, each edge in both
//   faces[k]: vertices in walking order; every directed edge u -> v is used by exactly one face
//   bounded faces are counterclockwise (polyArea2 > 0); the outer face of each connected component
//   is clockwise (area <= 0; a tree has only that face); isolated vertices are in no face
//   needs a planar embedding: edges meet only at shared endpoints, no repeated edges
//   per component: faces = edges - vertices + 2 (Euler)
// Complexity: O(m log m), m = number of edges.
// Requires: geometry/point
template <class T> vector<vector<int>> planarFaces(const vector<Point<T>> &p, vector<vector<int>> g) {
    int n = (int)g.size();
    auto half = [](Point<T> v) { return sgn(v.y) < 0 || (sgn(v.y) == 0 && sgn(v.x) < 0); };
    map<pair<int, int>, int> at;  // at[{u, v}]: index of v in g[u]
    for (int u = 0; u < n; u++) {
        // neighbors counterclockwise by direction, starting at angle 0
        sort(g[u].begin(), g[u].end(), [&](int a, int b) {
            Point<T> da = p[a] - p[u], db = p[b] - p[u];
            return half(da) != half(db) ? half(db) : sgn(da.cross(db)) > 0;
        });
        for (int i = 0; i < (int)g[u].size(); i++) at[{u, g[u][i]}] = i;
    }
    vector<vector<int>> faces;
    vector<vector<char>> seen(n);
    for (int u = 0; u < n; u++) seen[u].assign(g[u].size(), 0);
    for (int s = 0; s < n; s++)
        for (int e = 0; e < (int)g[s].size(); e++) {
            if (seen[s][e]) continue;
            // walk u -> v, then leave v by the edge just clockwise of v -> u (the sharpest left
            // turn): the face stays on the left
            vector<int> f;
            for (int u = s, i = e; !seen[u][i];) {
                seen[u][i] = 1, f.push_back(u);
                int v = g[u][i], d = (int)g[v].size();
                i = (at[{v, u}] + d - 1) % d, u = v;
            }
            faces.push_back(f);
        }
    return faces;
}
