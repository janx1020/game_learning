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

class EulerianPath {
public:
	class Edge {
	public:
		int from;
		int to;
		Edge(int f, int t) {
			from = f;
			to = t;
		}
	};

private:
	unordered_map<int, vector<int>> adj;
	vector<int> inDegree;
	vector<int> outDegree;
	int numNodes;
	int edgeCount;


	void preprocess(vector<Edge>& edges, int numNodes) {
		this->numNodes = numNodes;
		edgeCount = 0;
		inDegree.resize(numNodes, 0);
		outDegree.resize(numNodes, 0);
		for (Edge& e : edges) {
			++inDegree[e.to];
			++outDegree[e.from];
			++edgeCount;
			if (adj.find(e.from) != adj.end())
				adj[e.from].push_back(e.to);
			else
				adj[e.from] = vector<int>({ e.to });
		}
	}

	bool hasEulerianPath() {
		if (edgeCount == 0) return false;
		int startNodes = 0, endNodes = 0;
		for (int node = 0; node < numNodes; ++node) {
			if (abs(inDegree[node] - outDegree[node]) > 1)
				return false;
			if (inDegree[node] > outDegree[node])
				++endNodes;
			else if (inDegree[node] < outDegree[node])
				++startNodes;
		}
		return ((startNodes == 0 && endNodes == 0) || (startNodes == 1 && endNodes == 1));
	}

	int findFirstNode() {
		int start = 0;
		for (int i = 0; i < numNodes; ++i) {
			if (inDegree[i] < outDegree[i])
				return i;
			// Start at a node with an outgoing edge.
			if (outDegree[i] > 0)
				start = i;
		}
		return start;
	}

	void dfs(int at, vector<int>& path) {
		while (outDegree[at] > 0) {
			vector<int>& tos = adj[at];
			int next = tos[tos.size() - outDegree[at]--];
			dfs(next, path);
		}
		path.push_back(at);
	}

public:
	vector<int> eulerianPath(vector<Edge>& edges, int numNodes) {
		vector<int> path;
		preprocess(edges, numNodes);
		if (!hasEulerianPath())
			return path;
		int firstNode = findFirstNode();
		dfs(firstNode, path);
		reverse(path.begin(), path.end());
		return path;
	}
};

void testEulerianPath() {
	int numNodes = 7;
	vector<EulerianPath::Edge> edges = {
		EulerianPath::Edge(1, 2),
		EulerianPath::Edge(1, 3),
		EulerianPath::Edge(2, 2),
		EulerianPath::Edge(2, 4),
		EulerianPath::Edge(2, 4),
		EulerianPath::Edge(3, 1),
		EulerianPath::Edge(3, 2),
		EulerianPath::Edge(3, 5),
		EulerianPath::Edge(4, 3),
		EulerianPath::Edge(4, 6),
		EulerianPath::Edge(5, 6),
		EulerianPath::Edge(6, 3),
	};
	vector<int> path = EulerianPath().eulerianPath(edges, numNodes);

	cout << "EulerianPath:" << endl;
	for (int node : path)
		cout << node << "  ";
	cout << endl;
}

/*
int main() {
	testEulerianPath();
	return 0;
}
*/