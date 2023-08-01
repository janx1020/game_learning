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

class BellmanFord {
public:
	class Edge {
	public:
		int from;
		int to;
		float weight;
		Edge(int f, int t, float w) {
			from = f;
			to = t;
			weight = w;
		}
	};

	vector<float> bellmanFord(vector<Edge>& edges, int V, int start) {
		vector<float> dist(V, INFINITY);
		dist[start] = 0;

		// For each vertex, apply relaxation for all the edges
		for (int i = 0; i < V - 1; ++i)
			for (auto& edge : edges)
				if (dist[edge.from] + edge.weight < dist[edge.to])
					dist[edge.to] = dist[edge.from] + edge.weight;

		// Run algorithm a second time to detect which nodes are part
		// of a negative cycle. A negative cycle has occurred if we
		// can find a better path beyond the optimal solution.
		for (int i = 0; i < V - 1; ++i)
			for (auto& edge : edges)
				if (dist[edge.from] + edge.weight < dist[edge.to])
					dist[edge.to] = -INFINITY;

		return dist;
	}
};

void testBellmanFord() {
	cout << "BellmanFord:" << endl;
	BellmanFord bf;
	vector<BellmanFord::Edge> edges = {
		BellmanFord::Edge(0, 1, 4),
		BellmanFord::Edge(0, 2, 1),
		BellmanFord::Edge(1, 3, 1),
		BellmanFord::Edge(2, 1, 2),
		BellmanFord::Edge(2, 3, 5),
		BellmanFord::Edge(3, 4, 3),
	};
	int start = 0, V = 5;
	vector<float> dist = bf.bellmanFord(edges, V, start);
	for (float d : dist)
		cout << d << "  ";
	cout << endl;
}

int main() {
	testBellmanFord();
	
	return 0;
}