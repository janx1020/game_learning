#include <cstring>
#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <iostream>
#include <map>
#include <queue>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Node
{
public:
    int val;
    vector<Node *> neighbors;
    Node()
    {
        val = 0;
        neighbors = vector<Node *>();
    }
    Node(int _val)
    {
        val = _val;
        neighbors = vector<Node *>();
    }
    Node(int _val, vector<Node *> _neighbors)
    {
        val = _val;
        neighbors = _neighbors;
    }
};

class SolutionCloneGraph
{
public:
    unordered_map<Node *, Node *> mp;
    Node *cloneGraph(Node *node)
    {
        if (!node)
            return NULL;
        if (mp.find(node) != mp.end())
            return mp[node];
        Node *cloneNode = new Node(node->val);
        mp[node] = cloneNode;
        for (Node *neighbor : node->neighbors)
        {
            cloneNode->neighbors.push_back(cloneGraph(neighbor));
        }
        return cloneNode;
    }
};

class SolutionCanFinish
{
private:
    vector<vector<int>> adj, radj;
    vector<int> st, who;
    vector<bool> vis, vis2;
    int nComponent;

public:
    void dfs1(int u)
    {
        vis[u] = true;
        for (int v : adj[u])
            if (!vis[v])
                dfs1(v);
        st.push_back(u);
    }
    void dfs2(int u, int rep)
    {
        vis2[u] = true;
        who[u] = rep;
        if (u != rep)
            --nComponent;
        for (int v : radj[u])
            if (!vis2[v])
                dfs2(v, rep);
    }

    bool canFinish(int numCourses, vector<vector<int>> &prerequisites)
    {
        nComponent = numCourses;
        adj = vector<vector<int>>(numCourses, vector<int>());
        radj = vector<vector<int>>(numCourses, vector<int>());
        vis = vector<bool>(numCourses, false);
        vis2 = vector<bool>(numCourses, false);
        who = vector<int>(numCourses, -1);
        for (auto &req : prerequisites)
        {
            int c1 = req[0];
            int c2 = req[1];
            adj[c1].push_back(c2);
            radj[c2].push_back(c1);
        }
        for (int i = 0; i < numCourses; ++i)
            if (!vis[i])
                dfs1(i);
        while (st.size() > 0)
        {
            int u = st.back();
            st.pop_back();
            if (!vis2[u])
                dfs2(u, u);
        }
        return nComponent == numCourses;
    }
};

class SolutionMinAddedEdgeNum
{
private:
#define mxN 100
    vector<int> adj[mxN], radj[mxN], st;
    bool vis[mxN], vis2[mxN];
    int who[mxN], deg[mxN], nComponent;

public:
    void init()
    {
        for (int i = 0; i < mxN; ++i)
        {
            vis[i] = false;
            vis2[i] = false;
            deg[i] = 0;
            who[i] = -1;
        }
    }

    void dfs1(int u)
    {
        vis[u] = true;
        for (int v : adj[u])
            if (!vis[v])
                dfs1(v);
        st.push_back(u);
    }

    void dfs2(int u, int rep)
    {
        vis2[u] = true;
        who[u] = rep;
        if (rep != u)
            --nComponent;
        for (int v : radj[u])
            if (!vis2[v])
                dfs2(v, rep);
    }

    int minAddedEdge(vector<vector<string>> routes)
    {
        init();
        unordered_map<string, int> mp;
        int n = 0;
        // process input
        for (auto &route : routes)
        {
            for (string &node : route)
                if (mp.find(node) == mp.end())
                    mp[node] = n++;
            adj[mp[route[0]]].push_back(mp[route[1]]);
            radj[mp[route[1]]].push_back(mp[route[0]]);
        }
        nComponent = n;

        for (int i = 0; i < n; ++i)
        {
            if (!vis[i])
                dfs1(i);
        }

        while (st.size() > 0)
        {
            int u = st.back();
            st.pop_back();
            if (!vis2[u])
                dfs2(u, u);
        }

        for (int i = 0; i < n; ++i)
            for (int j : adj[i])
                if (who[i] != who[j])
                    ++deg[j];

        int ans = nComponent;
        for (int i = 0; i < n; ++i)
            if (/*who[i] == i && */ deg[i] > 0)
                --ans;
        return ans;
    }
};

class SolutionDungeon
{
#define DIRECTION 4
    int row;
    int col;
    int startXY;
    int endXY;
    unordered_map<int, vector<int>> adj;

    // variables used to track the number of steps taken and layers
    int moveCount = 0;
    int nodesLeftInLayer = 1;
    int nodesInNextLayer = 0;
    bool reachEnd = false;

    void preprocess(vector<vector<char>> &dungeon)
    {
        // north, south, east, west
        int dx[DIRECTION] = {-1, 1, 0, 0};
        int dy[DIRECTION] = {0, 0, -1, 1};
        row = dungeon.size();
        col = dungeon[0].size();
        for (int i = 0; i < row; ++i)
            for (int j = 0; j < col; ++j)
            {
                if (dungeon[i][j] == '#') // block
                    continue;
                vector<int> point({i, j});
                if (dungeon[i][j] == 'S') // start
                    startXY = i * col + j;
                else if (dungeon[i][j] == 'E') // end
                    endXY = i * col + j;
                vector<int> neighbors;
                for (int k = 0; k < DIRECTION; ++k)
                {
                    int ii = i + dx[k];
                    int jj = j + dy[k];
                    if (ii >= 0 && ii < row && jj >= 0 && jj < col && dungeon[ii][jj] != '#')
                        neighbors.push_back(ii * col + jj);
                }
                adj[i * col + j] = neighbors;
            }
    }

    vector<int> bfs()
    {
        vector<bool> visit(row * col, false);
        vector<int> prev(row * col, -1);
        queue<int> q;
        q.push(startXY);
        visit[startXY] = true;
        while (!q.empty())
        {
            int point = q.front();
            if (point == endXY)
                break;
            q.pop();
            vector<int> &neighbors = adj[point];
            for (int i = 0; i < neighbors.size(); ++i)
            {
                if (!visit[neighbors[i]])
                {
                    q.push(neighbors[i]);
                    visit[neighbors[i]] = true;
                    prev[neighbors[i]] = point;
                }
            }
        }
        return prev;
    }

    void exploreNeighbor(vector<vector<char>> &dungeon, queue<int> &q, int x, int y, vector<bool> &visit, vector<int> &prev)
    {
        // north, south, east, west
        int dx[DIRECTION] = {-1, 1, 0, 0};
        int dy[DIRECTION] = {0, 0, -1, 1};
        for (int i = 0; i < DIRECTION; ++i)
        {
            int xx = x + dx[i], yy = y + dy[i];
            int neighbor = xx * col + yy;
            if (xx < 0 || xx >= row || yy < 0 || yy >= col || visit[neighbor] || dungeon[xx][yy] == '#')
                continue;
            q.push(neighbor);
            visit[neighbor] = true;
            prev[neighbor] = x * col + y;
            ++nodesInNextLayer;
        }
    }
    vector<int> bfsV2(vector<vector<char>> &dungeon, int start)
    {
        row = dungeon.size();
        col = dungeon[0].size();

        vector<bool> visit(row * col, false);
        vector<int> prev(row * col, -1);
        queue<int> q;
        q.push(start);
        visit[start] = true;
        while (!q.empty())
        {
            int point = q.front();
            q.pop();
            int x = point / col, y = point % col;
            if (dungeon[x][y] == 'E')
            {
                endXY = point;
                reachEnd = true;
                break;
            }
            exploreNeighbor(dungeon, q, x, y, visit, prev);
            if (--nodesLeftInLayer == 0)
            {
                nodesLeftInLayer = nodesInNextLayer;
                nodesInNextLayer = 0;
                ++moveCount;
            }
        }
        if (reachEnd)
            cout << "reach End, took " << moveCount << " steps." << endl;
        return prev;
    }

    vector<vector<int>> reconstructPath(vector<int> &prev)
    {
        vector<vector<int>> path;
        for (int at = endXY; at >= 0; at = prev[at])
            path.insert(path.begin(), vector<int>({at / col, at % col}));
        return path;
    }

public:
    vector<vector<int>> solveDungeon(vector<vector<char>> &dungeon)
    {
        // preprocess the dungeon
        preprocess(dungeon);
        vector<int> prev = bfs();
        // use bfs to find the path
        return reconstructPath(prev);
    }

    vector<vector<int>> solveDungeonV2(vector<vector<char>> &dungeon)
    {
        vector<int> prev = bfsV2(dungeon, 0);
        return reconstructPath(prev);
    }
};

class SolutionRedundantDirectedConnection
{
public:
#define MAX_N 20

    int id[MAX_N];

    int find(int node)
    {
        int root = id[node];
        while (root != id[root])
            root = id[root];
        return root;
    }

    void preprocess(vector<vector<int>> &edges)
    {
        for (int i = 0; i < MAX_N; ++i)
        {
            id[i] = i;
        }
    }

    vector<int> findRedundantDirectedConnection(vector<vector<int>> &edges)
    {
        preprocess(edges);
        vector<int> lastCircleEdge, firstCircleEdge, doubledEdge;
        for (auto &edge : edges)
        {
            int from = edge[0];
            int to = edge[1];
            int fromRoot = find(from);
            int toRoot = find(to);
            if (fromRoot == toRoot)
            {
                if (!doubledEdge.empty())
                    return firstCircleEdge;
                lastCircleEdge = edge;
            }
            else if (toRoot != to)
            {
                firstCircleEdge = {id[to], to};
                doubledEdge = edge;
                if (!lastCircleEdge.empty())
                    return firstCircleEdge;
            }
            else
                id[to] = from;
        }
        if (!doubledEdge.empty())
            return doubledEdge;
        else
            return lastCircleEdge;
    }
};

class SolutionConnectedTrios
{
public:
#define MAX_N 408

    int deg[MAX_N];
    bool adj[MAX_N][MAX_N];

    void preprocess(int n, vector<vector<int>> &edges)
    {
        memset(deg, 0, sizeof(deg));
        memset(adj, 0, sizeof(adj));

        for (auto &edge : edges)
        {
            int from = edge[0];
            int to = edge[1];
            ++deg[from];
            ++deg[to];
            adj[from][to] = true;
            adj[to][from] = true;
        }
    }

    int getTrios(int n)
    {
        int ans = INT_MAX;
        for (int i = 1; i <= n; ++i)
            for (int j = i + 1; j <= n; ++j)
            {
                if (!adj[i][j])
                    continue;
                for (int k = j + 1; k <= n; ++k)
                {
                    if (adj[i][k] && adj[j][k])
                        ans = min(ans, deg[i] + deg[j] + deg[k] - 6);
                }
            }
        return ans == INT_MAX ? -1 : ans;
    }

    int minTrioDegree(int n, vector<vector<int>> &edges)
    {
        preprocess(n, edges);
        return getTrios(n);
    }
};

class SolutionRobot
{
public:
    int m;
    int n;
    int start;
    vector<bool> vis;
    int longest = -1;

    int pos(int x, int y)
    {
        return x * n + y;
    }

    vector<int> getNeighbors(int curr, int prev)
    {
        vector<int> neighbors;
        int x = curr / n;
        int y = curr % n;
        int px = prev / n;
        int py = prev % n;
        if (x < m - 1 && (prev == -1 || (py == y && px < x) || (px == x && py < y)))
            neighbors.push_back(pos(x + 1, y));
        if (x > 0 && (prev == -1 || (py == y && px > x) || (px == x && py > y)))
            neighbors.push_back(pos(x - 1, y));
        if (y < n - 1 && (prev == -1 || (py == y && px > x) || (px == x && py < y)))
            neighbors.push_back(pos(x, y + 1));
        if (y > 0 && (prev == -1 || (py == y && px < x) || (px == x && py > y)))
            neighbors.push_back(pos(x, y - 1));
        return neighbors;
    }

    void dfs(int curr, int prev, int path)
    {
        if (curr == start && path > 0)
        {
            longest = max(longest, path);
            return;
        }
        if (vis[curr])
            return;
        if (curr != start)
            vis[curr] = true;

        vector<int> neighbors = getNeighbors(curr, prev);
        for (int neighbor : neighbors)
        {
            if (!vis[neighbor])
                dfs(neighbor, curr, path + 1);
        }
        vis[curr] = false;
    }

    int maxDistance(int m, int n, int startX, int startY)
    {
        this->m = m;
        this->n = n;
        vis.resize(m * n, false);
        start = pos(startX, startY);
        int path = 0;
        int prev = -1;
        dfs(start, prev, path);

        return longest;
    }
};

class SolutionBipartite
{
public:
    vector<vector<int>> adjList;
    int m;
    const int RED = 0, BLACK = 1;
    vector<int> colors;
    void preprocess(vector<vector<int>> &adjMatrix)
    {
        m = adjMatrix.size();
        colors.resize(m, -1);
        adjList.resize(m);
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < m; ++j)
            {
                if (adjMatrix[i][j])
                {
                    adjList[i].push_back(j);
                    adjList[j].push_back(i);
                }
            }
    }

    // Do a depth first search coloring the nodes of the graph as we go.
    // This method returns the count of the number of nodes visited while
    // coloring the graph or -1 if this graph is not bipartite.
    int ColorGraph(int node, int color)
    {
        colors[node] = color;
        // Toggles the color between RED and BLACK by exploiting the
        // binary respresentation of the constants
        int nextColor = !color;
        int visitCount = 1;
        for (int neighbor : adjList[node])
        {
            // contradiction found. In a biaprtite graph no two nodes
            // of the same color can be next to each other!
            if (colors[neighbor] == color)
                return -1;
            if (colors[neighbor] == nextColor)
                continue;

            // If a contradiction is found propagate return -1
            // otherwise keep track of the number of visited nodes.
            int count = ColorGraph(neighbor, nextColor);
            if (count == -1)
                return -1;
            visitCount += count;
        }
        return visitCount;
    }

    bool IsBipartite(vector<vector<int>> &adjMatrix)
    {
        preprocess(adjMatrix);
        int visitedCount = ColorGraph(0, RED);
        return visitedCount == m;
    }
};

void testDungeon()
{
    vector<vector<int>> prerequisites = {{1, 0}, {0, 2}, {2, 1}};
    //vector<vector<int>> prerequisites = { { 1, 0 }, { 2, 0 }, { 1, 2 } };
    //vector<vector<int>> prerequisites = { { 1, 0 }, { 0, 1 } };
    //vector<vector<int>> prerequisites = { { 1, 0 } };
    int numCourses = 3;
    cout << "canFinish: " << SolutionCanFinish().canFinish(numCourses, prerequisites) << endl;
    vector<vector<string>> routes = {{"A", "B"}, {"Z", "B"}, {"B", "C"}, {"C", "D"}, {"D", "E"}, {"E", "B"}};
    //vector<vector<string>> routes = { { "A", "B" }, { "Z", "B" }, { "B", "C" }, { "C", "D" }, { "D", "E" } };
    cout << "minAddedEdge: " << SolutionMinAddedEdgeNum().minAddedEdge(routes) << endl;

    vector<vector<char>> dungeon = {
        {'S', ' ', ' ', '#', ' ', ' ', ' '},
        {' ', '#', ' ', ' ', ' ', '#', ' '},
        {' ', '#', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', '#', '#', ' ', ' ', ' '},
        {'#', ' ', '#', 'E', ' ', '#', ' '},
    };
    auto path = SolutionDungeon().solveDungeonV2(dungeon);
    cout << "solveDungeon:" << endl;
    for (int i = 0; i < path.size(); ++i)
    {
        auto axis = path[i];
        int x = axis[0];
        int y = axis[1];
        cout << "(" << x << "," << y << ")";
        if (i != path.size() - 1)
            cout << " => ";
    }
    cout << endl;
}

void testRedundantDirectedConnection()
{
    cout << "RedundantDirectedConnection: " << endl;
    vector<vector<int>> edges = {
        //{ 2, 1 }, { 3, 1 }, { 4, 2 }, { 1, 4 }
        //{ { 4, 2 }, { 1, 5 }, { 5, 2 }, { 5, 3 }, { 2, 4 } }
        {3, 4},
        {4, 1},
        {1, 2},
        {2, 3},
        {5, 1}};
    vector<int> ans = SolutionRedundantDirectedConnection().findRedundantDirectedConnection(edges);
    for (int r : ans)
        cout << r << " ";
    cout << endl;
}

void testConnectedTrios()
{
    int n = 6;
    vector<vector<int>> edges = {{1, 2}, {1, 3}, {3, 2}, {4, 1}, {5, 2}, {3, 6}};
    cout << "ConnectedTrios minTrioDegree: " << SolutionConnectedTrios().minTrioDegree(n, edges) << endl;
}

void testRobotMaxDistance()
{
    int m = 3, n = 1;
    int startX = 0, startY = 0;
    cout << "SolutionRobot: " << endl;
    cout << SolutionRobot().maxDistance(m, n, startX, startY) << endl;
    ;
}

void testBipartite()
{
    vector<vector<int>> adjMatrix = {
        {0, 1, 0, 1},
        {1, 0, 1, 0},
        {0, 1, 0, 1},
        {1, 0, 1, 0}};
    SolutionBipartite bip;
    cout << "SolutionBipartite: " << bip.IsBipartite(adjMatrix) << endl;
}

int main()
{
    testDungeon();
    testRedundantDirectedConnection();
    testConnectedTrios();
    testRobotMaxDistance();
    testBipartite();
    return 0;
}
