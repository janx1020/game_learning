#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <iostream>
#include <map>
#include <unordered_map>
#include <algorithm> 

using namespace std;


class Node {
public:
	int val;
	vector<Node*> neighbors;
	Node() {
		val = 0;
		neighbors = vector<Node*>();
	}
	Node(int _val) {
		val = _val;
		neighbors = vector<Node*>();
	}
	Node(int _val, vector<Node*> _neighbors) {
		val = _val;
		neighbors = _neighbors;
	}
};


class SolutionCloneGraph {
public:
	unordered_map<Node*, Node*> mp;
	Node* cloneGraph(Node* node) {
		if (!node) return NULL;
		if (mp.find(node) != mp.end()) return mp[node];
		Node* cloneNode = new Node(node->val);
		mp[node] = cloneNode;
		for (Node* neighbor : node->neighbors) {
			cloneNode->neighbors.push_back(cloneGraph(neighbor));
		}
		return cloneNode;
	}
};

/*

int main()
{
	Node root(2);


	return 0;
}
*/