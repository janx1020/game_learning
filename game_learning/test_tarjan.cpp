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

class Tarjan {
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
	const int UNVISITED = -1;
	int id = 0;
	vector<bool> onStack;
	vector<bool> vis;
	vector<int> low;
	vector<int> ids;
	vector<int> st;

	void dfs(int at) {
		low[at] = ids[at] = id++;
		st.push_back(at);
		onStack[at] = true;

		for (int to : adj[at]) {
			if (ids[to] == UNVISITED)
				dfs(to);

			if (onStack[to])
				low[at] = min(low[at], low[to]);
		}

		// On recursive callback, if we're at the root node (start of SCC)
		// empty the seen stack until back to root.
		if (low[at] == ids[at])
			while (st.size() > 0) {
				int node = st.back();
				st.pop_back();
				onStack[node] = false;
				if (node == at) break;
			}
	}

	void dfsMyCode(int i) {
		vis[i] = true;
		st.push_back(i);
		onStack[i] = true;
		for (int to : adj[i]) {
			if (!vis[to])
				dfsMyCode(to);

			if (onStack[to])
				low[i] = min(low[i], low[to]);
		}

		// On recursive callback, if we're at the root node (start of SCC)
		// empty the seen stack until back to root.
		if (low[i] == i)
			while (st.size() > 0) {
				int node = st.back();
				st.pop_back();
				onStack[node] = false;
				if (node == i) break;
			}
	}

public:
	vector<int> tarjan(vector<Edge>& edges, int numNodes) {
		onStack.resize(numNodes, false);
		low.resize(numNodes, UNVISITED);
		ids.resize(numNodes, UNVISITED);

		for (auto& edge : edges)
			if (adj.find(edge.from) == adj.end())
				adj[edge.from] = vector<int>({ edge.to });
			else
				adj[edge.from].push_back(edge.to);

		for (int i = 0; i < numNodes; ++i)
			if (ids[i] == UNVISITED)
				dfs(i);

		return low;
	}

	vector<int> tarjanMyCode(vector<Edge>& edges, int numNodes) {
		onStack.resize(numNodes, false);
		low.resize(numNodes);
		vis.resize(numNodes, false);
		for (int i = 0; i < numNodes; ++i)
			low[i] = i;

		for (auto& edge : edges)
			if (adj.find(edge.from) == adj.end())
				adj[edge.from] = vector<int>({ edge.to });
			else
				adj[edge.from].push_back(edge.to);

		for (int i = 0; i < numNodes; ++i)
			if (!vis[i])
				dfsMyCode(i);

		return low;
	}
};


class Kosaraju {
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
	unordered_map<int, vector<int>> radj;
	vector<bool> vis;
	vector<bool> vis2;
	vector<int> who;
	vector<int> st;

	void dfs1(int i) {
		vis[i] = true;
		for (int to : adj[i])
			if (!vis[to])
				dfs1(to);
		st.push_back(i);
	}

	void dfs2(int node, int rep) {
		vis2[node] = true;
		who[node] = rep;
		for (int from : radj[node])
			if (!vis2[from])
				dfs2(from, rep);
	}
public:
	vector<int> kosaraju(vector<Edge>& edges, int numNodes) {
		who.resize(numNodes, -1);
		vis.resize(numNodes, false);
		vis2.resize(numNodes, false);
		for (Edge& edge : edges) {
			if (adj.find(edge.from) == adj.end())
				adj[edge.from] = vector<int>({ edge.to });
			else
				adj[edge.from].push_back(edge.to);
			if (radj.find(edge.to) == radj.end())
				radj[edge.to] = vector<int>({ edge.from });
			else
				radj[edge.to].push_back(edge.from);
		}

		for (int i = 0; i < numNodes; ++i) {
			who[i] = i;
			if (!vis[i])
				dfs1(i);
		}

		while (st.size() > 0) {
			int node = st.back();
			st.pop_back();
			if (!vis2[node])
				dfs2(node, node);
		}
		return who;
	}
};

void testTarjan() {
	vector<Tarjan::Edge> edges = {
		{ 6, 0 },
		{ 6, 2 },
		{ 3, 4 },
		{ 6, 4 },
		{ 2, 0 },
		{ 0, 1 },
		{ 4, 5 },
		{ 5, 6 },
		{ 3, 7 },
		{ 7, 5 },
		{ 1, 2 },
		{ 7, 3 },
		{ 5, 0 },
	};
	int numNodes = 8;
	//vector<int> lowLink = Tarjan().tarjan(edges, numNodes);
	vector<int> lowLink = Tarjan().tarjanMyCode(edges, numNodes);
	cout << "Tarjan:" << endl;
	for (int low : lowLink)
		cout << low << "  ";
	cout << endl;
}

void testKosaraju() {
	vector<Kosaraju::Edge> edges = {
		{ 6, 0 },
		{ 6, 2 },
		{ 3, 4 },
		{ 6, 4 },
		{ 2, 0 },
		{ 0, 1 },
		{ 4, 5 },
		{ 5, 6 },
		{ 3, 7 },
		{ 7, 5 },
		{ 1, 2 },
		{ 7, 3 },
		{ 5, 0 },
	};
	int numNodes = 8;
	vector<int> lowLink = Kosaraju().kosaraju(edges, numNodes);
	cout << "Kosaraju:" << endl;
	for (int low : lowLink)
		cout << low << "  ";
	cout << endl;
}


/*
int main() {
	testTarjan();
	testKosaraju();

	return 0;
}
*/