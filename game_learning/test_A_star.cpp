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

class AStar {
public:
	struct Edge {
		int from;
		int to;
		float cost;
		Edge(int f, int t, float c) {
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
		float minCost;
		vector<int> path;
	};

private:
	vector<int> prev;
	vector<float> globalCost;
	vector<float> localCost;
	vector<bool> vis;
	priority_queue<Edge, vector<Edge>, Comp> pq;
	unordered_map<int, vector<Edge>> adj;
	int numNodes;

	void preprocess(vector<Edge>& edges) {
		numNodes = 0;
		for (auto& edge : edges) {
			if (adj.find(edge.from) == adj.end()) {
				++numNodes;
				adj[edge.from] = vector<Edge>({ edge });
			}
			else
				adj[edge.from].push_back(edge);

			if (adj.find(edge.to) == adj.end()) {
				++numNodes;
				adj[edge.to] = vector<Edge>();
			}
		}
		globalCost.resize(numNodes, INFINITY);
		localCost.resize(numNodes, INFINITY);
		prev.resize(numNodes, -1);
		vis.resize(numNodes, false);
	}

	float heuristic(int a, int b) {
		//return abs(a - b);
		return 0;
	}

public:
	OptPath astar(vector<Edge>& edges, int start, int end) {
		OptPath op;
		preprocess(edges);
		localCost[start] = 0;
		globalCost[start] = localCost[start] + heuristic(start, end);
		prev[start] = start;
		pq.push(Edge(start, -1, globalCost[start]));

		while (!pq.empty()) {
			Edge e = pq.top();
			pq.pop();
			vis[e.from] = true;

			for (auto neighbor : adj[e.from]) {
				float possibleLowerLocal = localCost[e.from] + neighbor.cost;
				if (possibleLowerLocal < localCost[neighbor.to]) {
					localCost[neighbor.to] = possibleLowerLocal;
					prev[neighbor.to] = e.from;
					globalCost[neighbor.to] = localCost[neighbor.to] + heuristic(neighbor.to, end);
				}
				if (!vis[neighbor.to])
					pq.push(Edge(neighbor.to, -1, globalCost[neighbor.to]));
			}
		}
		op.minCost = localCost[end];
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


// only could turn right
class AStarNegative {
public:
	struct OptPath {
		float minCost;
		vector<pair<int, int>> path;
	};

	struct Grid {
		int x;
		int y;
		bool visited;
		Grid* parent;
		vector<Grid*> neighbors;
		float globalCost;
		float localCost;
		Grid() {
			x = -1;
			y = -1;
			visited = false;
			parent = NULL;
			globalCost = INFINITY;
			localCost = INFINITY;

		}
	};

	struct Comp {
		bool operator() (const Grid* a, const Grid* b) {
			if (abs(a->globalCost - b->globalCost) < DBL_EPSILON)
				return a->localCost > b->localCost;
			return a->globalCost > b->globalCost;
		}
	};

private:
	int nWidth;
	int nHeight;
	vector<Grid> grids;
	// priority queue could NOT be modified the compare value if it's in queue, otherwise crash
	//priority_queue<Grid*, vector<Grid*>, Comp> pq;
	list<Grid*> pq;
	Grid* start;
	Grid* end;


	void init(int width, int height) {
		nWidth = width;
		nHeight = height;
		grids.resize(width * height);
		for (int x = 0; x < width; ++x)
			for (int y = 0; y < height; ++y) {
				Grid grid;
				grid.x = x;
				grid.y = y;
				grids[y * width + x] = grid;
			}
		for (int x = 0; x < width; ++x)
			for (int y = 0; y < height; ++y) {
				if (x > 0)
					grids[y * width + x].neighbors.push_back(&grids[y * width + x - 1]);
				if (x < width - 1)
					grids[y * width + x].neighbors.push_back(&grids[y * width + x + 1]);
				if (y > 0)
					grids[y * width + x].neighbors.push_back(&grids[(y - 1) * width + x]);
				if (y < height - 1)
					grids[y * width + x].neighbors.push_back(&grids[(y + 1) * width + x]);
			}
		start = &grids[0];
		end = &grids[1];
	}

	float distance(Grid* a, Grid* b) {
		return -sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y)*(a->y - b->y));
	}

	float heuristic(Grid*a, Grid* b) {
		return distance(a, b);
	}

	bool validNeighbor(Grid* node, Grid* neighbor) {
		if (neighbor == start)
			return false;
		if (!node->parent)
			return true;
		int px = node->parent->x;
		int py = node->parent->y;
		int x = node->x;
		int y = node->y;
		if (x == px)
			if (y == py - 1 && neighbor->y != y - 1 && neighbor->x != x + 1
				|| y == py + 1 && neighbor->y != y + 1 && neighbor->x != x - 1
				)
				return false;
		if (y == py)
			if (x == px - 1 && neighbor->x != x - 1 && neighbor->y != y - 1
				|| x == px + 1 && neighbor->x != x + 1 && neighbor->y != y + 1)
				return false;
		return true;
	}

	void astar() {
		start->localCost = 0;
		start->globalCost = start->localCost + heuristic(start, end);
		pq.push_back(start);
		while (!pq.empty()) {
			pq.sort([](const Grid* a, const Grid* b) {return a->globalCost < b->globalCost; });

			// Front of listNotTestedNodes is potentially the lowest distance node. Our
			// list may also contain nodes that have been visited, so ditch these...
			while (!pq.empty() && pq.front()->visited)
				pq.pop_front();

			// ...or abort because there are no valid nodes left to test
			if (pq.empty())
				break;

			Grid* node = pq.front();
			pq.pop_front();
			node->visited = true;
			for (Grid* neighbor : node->neighbors) {
				if (!validNeighbor(node, neighbor))
					continue;
				float cost = distance(node, neighbor);
				if (node->localCost + cost < neighbor->localCost) {
					neighbor->localCost = node->localCost + cost;
					neighbor->parent = node;
					neighbor->globalCost = neighbor->localCost + heuristic(neighbor, end);
				}
				if (!neighbor->visited) {
					pq.push_back(neighbor);
				}
			}
		}
	}

public:
	OptPath astarNegative(int width, int height) {
		OptPath op;
		init(width, height);
		astar();
		Grid* end1 = &grids[1];
		Grid* end2 = &grids[width];
		op.minCost = min(end1->globalCost, end2->globalCost);
		Grid* tmp = end1;
		/*
		while (tmp) {
			op.path.push_back(make_pair(tmp->y, tmp->x));
			tmp = tmp->parent;
		}
		*/
		return op;
	}

};


void testAStar() {
	cout << "AStar:" << endl;
	AStar dj;
	vector<AStar::Edge> edges = {
		AStar::Edge(0, 1, 4),
		AStar::Edge(0, 2, 5),
		AStar::Edge(1, 3, 1),
		AStar::Edge(2, 1, -2),
		AStar::Edge(2, 3, 5),
		AStar::Edge(3, 4, 3),
	};
	int start = 0, end = 4;
	
	AStar::OptPath op = dj.astar(edges, start, end);
	cout << "minCost: " << op.minCost << endl;
	cout << "min path: ";
	for (int d : op.path)
		cout << d << "  ";
	cout << endl;

}

void testAStarNegative() {
	int width = 4; 
	int height = 3;
	cout << "AStarNegative: " << endl;
	AStarNegative::OptPath op = AStarNegative().astarNegative(width, height);
	cout << "minCost: " << op.minCost << endl;
	cout << "min path: ";
	for (auto& d : op.path)
		cout << "(" << d.first << ", " << d.second << "),  ";
	cout << endl;
}

/*
int main()
{
	testAStar();
	testAStarNegative();
	return 0;
}
*/