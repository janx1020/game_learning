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

class FordFulkerson {
public:
    struct Edge {
        int from;
        int to;
        int capacity;
        int flow;
        Edge(int f, int t, int c, int fw = 0) {
            from = f;
            to = t;
            capacity = c;
            flow = fw;
        }

        bool isResidual() {
            return capacity == 0;
        }

        int remainCapacity() {
            return capacity - flow;
        }

        void augment(int bottleNeck) {
            flow += bottleNeck;
        }
    };

private:
    unordered_map<int, vector<Edge>> adj;
    vector<bool> vis;
    int numNodes;
    int sourceNode;
    int sinkNode;

    void markAllNodesAsUnvisited() {
        fill(vis.begin(), vis.end(), false);
    }

    int dfs(int node, int flow) {
        // at sink node, return augmented path flow.
        if (node == sinkNode) return flow;
        vis[node] = true;

        for (Edge& e : adj[node]) {
            int rcap = e.remainCapacity();
            if (rcap > 0 && !vis[e.to]) {
                int bottleNeck = dfs(e.to, min(flow, rcap));

                // if we made it from s->t (a.k.a bettleNeck > 0)
                if (bottleNeck > 0) {
                    // augment flow with bottle neck value
                    e.augment(bottleNeck);
                    // find residual edge
                    auto & residualEdges = adj[e.to];
                    for (auto& it = residualEdges.begin(); it != residualEdges.end(); ++it) {
                        if (it->to == e.from) {
                            it->augment(-bottleNeck);
                            break;
                        }
                    }
                    return bottleNeck;
                }
            }
        }

        return 0;
    }

    void preprocess(vector<vector<int>>& cap, int s, int t) {
        numNodes = cap.size();
        sourceNode = s;
        sinkNode = t;
        vis.resize(numNodes, false);
        for (int i = 0; i < numNodes; ++i)
            for (int j = 0; j < numNodes; ++j) {
                if (cap[i][j] != 0) {
                    Edge e = Edge(i, j, cap[i][j]);
                    if (adj.find(i) != adj.end())
                        adj[i].push_back(e);
                    else
                        adj[i] = vector<Edge>({ e });

                    Edge residualEdge = Edge(j, i, 0);
                    if (adj.find(j) != adj.end()) // residual edge
                        adj[j].push_back(residualEdge);
                    else
                        adj[j] = vector<Edge>({ residualEdge });
                }
            }
    }

public:
    int fordFulkerson(vector<vector<int>>& cap, int s, int t) {
        preprocess(cap, s, t);
        int maxFlow = 0;
        for (int f = dfs(sourceNode, INT_MAX); f != 0; f = dfs(sourceNode, INT_MAX)) {
            markAllNodesAsUnvisited();
            maxFlow += f;
        }
        return maxFlow;
    }

};


void testFordFulkersonMaxFlow() {
    /*
    vector<vector<int>> cap = {
        { 0, 0, 25, 0, 0, 0 },
        { 0, 0, 0, 15, 0, 0 },
        { 0, 0, 0, 0, 0, 10 },
        { 6, 0, 0, 0, 0, 10 },
        { 10, 10, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0 },
    };
    */
    vector<vector<int>> cap = {
        { 0, 2, 4, 8, 0, 0 },
        { 0, 0, 0, 9, 0, 0 },
        { 0, 0, 0, 0, 0, 10},
        { 0, 0, 6, 0, 0, 10},
        { 10, 10, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0 },
    };
    int sourceNode = 4;
    int sinkNode = 5;
    cout << "FordFulkerson max flow: " << FordFulkerson().fordFulkerson(cap, sourceNode, sinkNode) << endl;
}

/*
int main() {
    testFordFulkersonMaxFlow();
    return 0;
}
*/