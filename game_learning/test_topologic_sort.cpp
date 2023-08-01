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

class TopologicSort {

public:
	// Finds a topological ordering of the nodes in a Directed Acyclic Graph (DAG)
	// The input to this function is an adjacency list for a graph and the number
	// of nodes in the graph.
	//
	// NOTE: 'numNodes' is not necessarily the number of nodes currently present
	// in the adjacency list since you can have singleton nodes with no edges which
	// wouldn't be present in the adjacency list but are still part of the graph!
	// the node number in adjacency list is start from 0.
	//
	vector<int> topologicSortV1(unordered_map<int, vector<int> >& adj, int numNodes) {
		vector<int> ordering(numNodes);
		int idx = 0;
		vector<bool> visited(numNodes, false);
		for (int i = 0; i < numNodes; ++i) {
			if (!visited[i]) {
				vector<int> path;
				dfsV1(adj, visited, path, i);
				for (int j = path.size() - 1; j >= 0; --j) {
					ordering[idx++] = path[j];
				}
			}
		}
		return ordering;
	}

	// V2 only make difference from V1 that don't need to use the temp path vector
	//
	vector<int> topologicSortV2(unordered_map<int, vector<int> >& adj, int numNodes) {
		vector<int> ordering(numNodes);
		int idx = numNodes - 1;
		vector<bool> visited(numNodes, false);
		for (int at = 0; at < numNodes; ++at) {
			if (!visited[at]) {
				idx = dfsV2(adj, visited, ordering, at, idx);
			}
		}
		return ordering;
	}
private:
	void dfsV1(unordered_map<int, vector<int> >& adj, vector<bool>& visited, vector<int>& path, int node) {
		visited[node] = true;
		if (adj.find(node) != adj.end()) {
			vector<int>& neighbors = adj[node];
			for (int neighbor : neighbors)
				if (!visited[neighbor])
					dfsV1(adj, visited, path, neighbor);
		}
		path.push_back(node);
	}

	int dfsV2(unordered_map<int, vector<int> >& adj, vector<bool>& visited, vector<int>& ordering, int node, int idx) {
		visited[node] = true;
		if (adj.find(node) != adj.end()) {
			vector<int>& neighbors = adj[node];
			for (int neighbor : neighbors)
				if (!visited[neighbor])
					idx = dfsV2(adj, visited, ordering, neighbor, idx);
		}
		ordering[idx] = node;
		return idx - 1;
	}
};

class Kahns {
public:
	// Kahns Algorithm
	// Finds a topological ordering of the nodes in a Directed Acyclic Graph (DAG)
	// The input to this function is an adjacency list for a graph and the number
	// of nodes in the graph.
	//
	// NOTE: 'numNodes' is not necessarily the number of nodes currently present
	// in the adjacency list since you can have singleton nodes with no edges which
	// wouldn't be present in the adjacency list but are still part of the graph!
	// the node number in adjacency list is start from 0.
	//
	vector<int> kahns(unordered_map<int, vector<int> >& adj, int numNodes) {
		vector<int> inDegree(numNodes, 0);
		queue<int> q;
		vector<bool> visited(numNodes, false);
		vector<int> ordering;

		for (auto it = adj.begin(); it != adj.end(); ++it)
			for (auto neighbor : it->second)
				++inDegree[neighbor];

		for (int i = 0; i < inDegree.size(); ++i)
			if (inDegree[i] == 0)
				q.push(i);

		while (!q.empty()) {
			int node = q.front();
			q.pop();
			visited[node] = true;
			ordering.push_back(node);

			if (adj.find(node) != adj.end())
				for (int neighbor : adj[node])
					if (!visited[neighbor] && (--inDegree[neighbor] == 0))
						q.push(neighbor);
		}

		if (ordering.size() != numNodes)
			ordering.clear();
		return ordering;
	}
};


void testTopologicSort() {
	unordered_map<int, vector<int> > adj = {
		{ 0, { 1, 2, 5 } },
		{ 1, { 3, 2 } },
		{ 2, { 3, 4 } },
		{ 3, { 4 } },
		{ 5, { 4 } },
	};

	TopologicSort topSort;
	int numNodes = 7;
	//vector<int> ordering = topSort.topologicSortV1(adj, 7);
	vector<int> ordering = topSort.topologicSortV2(adj, 7);
	cout << "topologicSort: " << endl;
	for (int i = 0; i < ordering.size(); ++i) {
		cout << ordering[i];
		if (i != ordering.size() - 1)
			cout << " => ";
	}
	cout << endl;
}

// Directed Acyclic Graph
class DAGShortestPath {
public:
	class Edge {
	public:
		int from;
		int to;
		int weight;

		Edge(int f, int t, int w) {
			from = f;
			to = t;
			weight = w;
		}
	};

private:
	unordered_map<int, vector<Edge> > mp;

public:
	vector<int> topologicalSort(vector<Edge>& edges, int numNodes) {
		vector<int> inDegree(numNodes, 0);
		vector<bool> visited(numNodes, false);
		queue<int> q;
		vector<int> ordering;

		for (Edge& edge : edges) {
			++inDegree[edge.to];
			if (mp.find(edge.from) == mp.end())
				mp[edge.from] = vector<Edge>({ edge });
			else
				mp[edge.from].push_back(edge);
		}

		for (int node = 0; node < inDegree.size(); ++node)
			if (inDegree[node] == 0)
				q.push(node);

		while (!q.empty()) {
			int node = q.front();
			q.pop();
			visited[node] = true;
			ordering.push_back(node);
			if (mp.find(node) != mp.end())
				for (Edge neighbor : mp[node])
					if (!visited[neighbor.to] && (--inDegree[neighbor.to] == 0))
						q.push(neighbor.to);
		}

		if (ordering.size() != numNodes)
			ordering.clear();

		return ordering;
	}

	vector<int> dagShortestPath(vector<Edge>& edges, int start, int numNodes) {
		vector<int> ordering = topologicalSort(edges, numNodes);
		vector<int> dist(numNodes, INT_MAX);
		dist[start] = 0;

		for (int i = 0; i < numNodes; ++i) {
			int node = ordering[i];
			if (dist[node] != INT_MAX) {
				if (mp.find(node) != mp.end())
					for (auto& neighbor : mp[node]) {
						int newDist = dist[node] + neighbor.weight;
						dist[neighbor.to] = min(dist[neighbor.to], newDist);
					}
			}
		}
		return dist;
	}

};

void testKahns() {
	unordered_map<int, vector<int> > adj = {
		{ 0, { 1, 2, 5 } },
		{ 1, { 3, 2 } },
		{ 2, { 3, 4 } },
		{ 3, { 4 } },
		{ 5, { 4 } },
	};

	Kahns k;
	int numNodes = 7;
	vector<int> ordering = k.kahns(adj, 7);
	cout << "Kahns: " << endl;
	for (int i = 0; i < ordering.size(); ++i) {
		cout << ordering[i];
		if (i != ordering.size() - 1)
			cout << " => ";
	}
	cout << endl;
}

void testDAGShortestPath() {
	DAGShortestPath dag;
	vector<DAGShortestPath::Edge> edges = {
		DAGShortestPath::Edge(0, 1, 3),
		DAGShortestPath::Edge(0, 2, 2),
		DAGShortestPath::Edge(0, 5, 3),
		DAGShortestPath::Edge(1, 3, 1),
		DAGShortestPath::Edge(1, 2, 6),
		DAGShortestPath::Edge(2, 3, 1),
		DAGShortestPath::Edge(2, 4, 10),
		DAGShortestPath::Edge(3, 4, 5),
		DAGShortestPath::Edge(5, 4, 7),
	};
	int numNodes = 7;
	int start = 0;
	vector<int> dist = dag.dagShortestPath(edges, start, numNodes);
	cout << "DAGShortestPath:" << endl;
	for (auto d : dist)
		cout << d << "  ";
	cout << endl;
}

int main() {
	testTopologicSort();
	testKahns();
	testDAGShortestPath();

	return 0;
}