#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <iostream>
#include <map>
#include <queue>
#include <unordered_map>
#include <algorithm> 
#include <chrono> 

using namespace std;


class MinimumSpanningTree {
public:
	struct Edge {
		int from;
		int to;
		int cost;
		Edge()
		{

		}
		Edge(int f, int t, int c) {
			from = f;
			to = t;
			cost = c;
		}
	};

	struct Comp {
		bool operator() (const Edge& e1, const Edge& e2) {
			return e1.cost > e2.cost;
		}
	};

	struct OptPath {
		int minCost;
		vector<Edge> path;
	};


private:
	unordered_map<int, vector<Edge>> adj;
	priority_queue <Edge, vector<Edge>, Comp> pq;
	vector<int> vis;
	int numNodes;

	void preprocess(vector<Edge>& edges) {
		numNodes = 0;
		for (Edge& e : edges) {
			if (adj.find(e.from) == adj.end()) {
				adj[e.from] = vector<Edge>({ e });
				++numNodes;
			}
			else
				adj[e.from].push_back(e);

			if (adj.find(e.to) == adj.end()) {
				adj[e.to] = vector<Edge>({ Edge(e.to, e.from, e.cost) });
				++numNodes;
			}
			else
				adj[e.to].push_back(Edge(e.to, e.from, e.cost));
		}
	}

	void addEdges(int node) {
		vis[node] = true;
		for (auto e : adj[node])
			if (!vis[e.to])
				pq.push(e);
	}

public:
	OptPath mst(vector<Edge>& edges) {
		preprocess(edges);
		vis.resize(numNodes, false);
		int m = numNodes - 1;
		int edgeCount = 0;
		addEdges(0); // first node is 0
		OptPath op;
		op.minCost = 0;
		op.path.resize(m);
		while (!pq.empty() && edgeCount != m) {
			Edge e = pq.top();
			pq.pop();

			if (!vis[e.to]) {
				op.minCost += e.cost;
				op.path[edgeCount++] = e;
				addEdges(e.to);
			}
		}

		if (edgeCount != m) {
			op.minCost = INFINITY;
			op.path.clear();
		}

		return op;
	}

};


void testMST() {
	vector<MinimumSpanningTree::Edge> edges = {
		MinimumSpanningTree::Edge(0, 1, 5),
		MinimumSpanningTree::Edge(1, 2, 4),
		MinimumSpanningTree::Edge(2, 9, 2),
		MinimumSpanningTree::Edge(0, 4, 1),
		MinimumSpanningTree::Edge(0, 3, 4),
		MinimumSpanningTree::Edge(1, 3, 2),
		MinimumSpanningTree::Edge(2, 7, 4),
		MinimumSpanningTree::Edge(2, 8, 1),
		MinimumSpanningTree::Edge(9, 8, 0),
		MinimumSpanningTree::Edge(4, 5, 1),
		MinimumSpanningTree::Edge(5, 6, 7),
		MinimumSpanningTree::Edge(6, 8, 4),
		MinimumSpanningTree::Edge(4, 3, 2),
		MinimumSpanningTree::Edge(5, 3, 5),
		MinimumSpanningTree::Edge(3, 6, 11),
		MinimumSpanningTree::Edge(6, 7, 1),
		MinimumSpanningTree::Edge(3, 7, 2),
		MinimumSpanningTree::Edge(7, 8, 6),
	};
	MinimumSpanningTree::OptPath op = MinimumSpanningTree().mst(edges);
	cout << "MinimumSpanningTree: " << endl;
	cout << "minCost: " << op.minCost << endl;
	for (auto& e : op.path)
		cout << "from: "<< e.from << ", to: " << e.to << ", cost: " << e.cost << endl;
}


int main() {
	testMST();
	return 0;
}
