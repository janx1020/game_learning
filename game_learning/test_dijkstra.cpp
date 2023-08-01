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


class DijkstraAlgorithm {
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

	class Compare {
	public:
		bool operator() (const Edge& eg1, const Edge& eg2) const {
			return eg1.weight > eg2.weight;
		}
	};

	struct OptPath {
		int minCost;
		vector<int> path;
	};

private:
	map<int, vector<Edge>> adj;
	map<int, int> mp;
	map<int, int> rmp;


	void process(vector<Edge>& edges) {
		int hash = 0;
		for (Edge& edge : edges) {
			if (mp.find(edge.from) == mp.end()) {
				rmp[hash] = edge.from;
				mp[edge.from] = hash++;
			}
			if (mp.find(edge.to) == mp.end()) {
				rmp[hash] = edge.to;
				mp[edge.to] = hash++;
			}
			int from = mp[edge.from];
			int to = mp[edge.to];
			if (adj.find(from) == adj.end())
				adj[from] = vector<Edge>({ Edge(from, to, edge.weight) });
			else
				adj[from].push_back(Edge(from, to, edge.weight));
		}
	}

public:
	OptPath dijkstra(vector<Edge>& edges, int start, int end) {
		OptPath op;
		process(edges);
		priority_queue<Edge, vector<Edge>, Compare> pq;
		int numNodes = mp.size();
		vector<int> dist(numNodes, INT_MAX);
		vector<bool> vis(numNodes, false);
		vector<int> prev(numNodes, -1);
		dist[mp[start]] = 0;
		prev[mp[start]] = mp[start];
		pq.push(Edge(mp[start], -1, 0)); // the to node is not used, so set to any value
		while (!pq.empty()) {
			Edge node = pq.top();
			pq.pop();
			vis[node.from] = true;

			if (node.weight < dist[node.from]) continue; // optimization

			for (auto& neighbor : adj[node.from])
				if (!vis[neighbor.to]) {
					int newDist = dist[node.from] + neighbor.weight;
					if (newDist < dist[neighbor.to]) {
						dist[neighbor.to] = newDist;
						prev[neighbor.to] = node.from;
						pq.push(Edge(neighbor.to, -1, newDist));
					}
				}

			if (node.from == mp[end]) break; // stop early, greedy
		}
		op.minCost = dist[end];
		int node = end;
		while (1) {
			op.path.push_back(node);
			if (node == start) break;
			node = prev[node];
		}
		reverse(op.path.begin(), op.path.end());
		return op;
	}
};

void testDijkstra() {
	cout << "Dijkstra:" << endl;
	DijkstraAlgorithm dj;
	vector<DijkstraAlgorithm::Edge> edges = {
		DijkstraAlgorithm::Edge(0, 1, 4),
		DijkstraAlgorithm::Edge(0, 2, 5),
		DijkstraAlgorithm::Edge(1, 3, 1),
		DijkstraAlgorithm::Edge(2, 1, -2),
		DijkstraAlgorithm::Edge(2, 3, 5),
		DijkstraAlgorithm::Edge(3, 4, 3),
	};
	int start = 0, end = 4;
	/*
	vector<DijkstraAlgorithm::Edge> edges = {
		DijkstraAlgorithm::Edge(1, 2, 2),
		DijkstraAlgorithm::Edge(1, 3, 12),
		DijkstraAlgorithm::Edge(1, 5, 11),
		DijkstraAlgorithm::Edge(2, 4, 3),
		DijkstraAlgorithm::Edge(3, 4, 13),
		DijkstraAlgorithm::Edge(3, 6, 6),
		DijkstraAlgorithm::Edge(4, 6, 5),
		DijkstraAlgorithm::Edge(4, 7, 4),
		DijkstraAlgorithm::Edge(5, 6, 10),
		DijkstraAlgorithm::Edge(5, 7, 9),
		DijkstraAlgorithm::Edge(6, 7, 8),
	};
	int start = 1, end = 7;
	*/
	DijkstraAlgorithm::OptPath op = dj.dijkstra(edges, start, end);
	cout << "minCost: " << op.minCost << endl;
	cout << "min path: ";
	for (int d : op.path)
		cout << d << "  ";
	cout << endl;

}

/*
int main() {
	testDijkstra();
	return 0;
}
*/