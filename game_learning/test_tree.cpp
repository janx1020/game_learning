#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <iostream>
#include <map>
#include <queue>
#include <unordered_map>
#include <algorithm> 
#include <math.h>

using namespace std;


class TreeIsomorphism {
public:
	class TreeNode {
		int id;
		TreeNode* parent;
		vector<TreeNode*> children;
	public:
		TreeNode(int id) {
			this->id = id;
			parent = NULL;
		}

		TreeNode(int id, TreeNode* parent) {
			this->id = id;
			this->parent = parent;
		}

		~TreeNode() {
			for (TreeNode* child : children)
				delete child;
			children.clear();
		}

		int getId() {
			return id;
		}

		TreeNode* getParent() {
			return parent;
		}

		vector<TreeNode*>& getChildren() {
			return children;
		}
	};

	bool treesAreIsomorphic(vector<vector<int>>& tree1, vector<vector<int>>& tree2) {
		vector<int> centers1 = findTreeCenters(tree1);
		vector<int> centers2 = findTreeCenters(tree2);

		TreeNode* rootedTree1 = rootTree(tree1, centers1[0]);
		string tree1Encoding = encode(rootedTree1);
		cout << "encoded tree1: " << tree1Encoding.c_str() << endl;
		delete rootedTree1;

		for (int center2 : centers2) {
			TreeNode* rootedTree2 = rootTree(tree2, center2);
			string tree2Encoding = encode(rootedTree2);
			cout << "encoded tree2: " << tree2Encoding.c_str() << endl;
			delete rootedTree2;
			if (tree1Encoding == tree2Encoding)
				return true;
		}
		return false;
	}

	vector<int> findTreeCenters(vector<vector<int>>& tree) {
		int n = tree.size();
		vector<int> degree(n, 0);
		vector<int> leaves;

		// Find the first outer layer of leaf nodes
		for (int i = 0; i < n; ++i) {
			vector<int>& edges = tree[i];
			degree[i] = edges.size();
			if (degree[i] <= 1) {
				leaves.push_back(i);
				degree[i] = 0;
			}
		}

		int processedLeafs = leaves.size();
		while (processedLeafs < n) {
			vector<int> newLeaves;
			for (int node : leaves) {
				for (int neighbor : tree[node])
					if (--degree[neighbor] <= 1)
						newLeaves.push_back(neighbor);
				degree[node] = 0;
			}
			processedLeafs += newLeaves.size();
			leaves = newLeaves;
		}
		return leaves;
	}

	TreeNode* rootTree(vector<vector<int>>& graph, int rootId) {
		TreeNode * root = new TreeNode(rootId);
		return buildTree(graph, root);
	}

	TreeNode* buildTree(vector<vector<int>>& graph, TreeNode* node) {
		if (!node) return node;
		for (int neighbor : graph[node->getId()]) {
			// ignore adding an edge pointing back to parent.
			if (node->getParent() && neighbor == node->getParent()->getId())
				continue;
			TreeNode* child = new TreeNode(neighbor, node);
			node->getChildren().push_back(child);

			buildTree(graph, child);
		}
		return node;
	}

	string encode(TreeNode* node) {
		if (!node) return "";
		vector<string> labels;
		for (TreeNode* child : node->getChildren())
			labels.push_back(encode(child));
		sort(labels.begin(), labels.end());
		string s("(");
		for (auto& label : labels)
			s += label;
		s += ")";
		return s;
	}

	// Create a graph as a adjacency list with 'n' nodes.
	vector<vector<int>> createEmptyGraph(int n) {
		vector<vector<int>> graph(n);
		return graph;
	}

	void addUndirectedEdge(vector<vector<int>>& graph, int from, int to) {
		graph[from].push_back(to);
		graph[to].push_back(from);
	}

};


// lowest common ancestor
class TreeLCA {
public:
	class TreeNode {
		int id;
		TreeNode* left;
		TreeNode* right;
	public:
		TreeNode(int id, TreeNode* left = NULL, TreeNode* right = NULL) {
			this->id = id;
			this->left = left;
			this->right = right;
		}

		int getId() {
			return id;
		}
		TreeNode* getLeft() {
			return left;
		}
		TreeNode* getRight() {
			return right;
		}
	};


	TreeNode* dfs(TreeNode* root, int id1, int id2, bool& f1, bool& f2) {
		if (!root) return NULL;
		if (root->getId() == id1) f1 = true;
		if (root->getId() == id2) f2 = true;
		TreeNode* left = dfs(root->getLeft(), id1, id2, f1, f2);
		TreeNode* right = dfs(root->getRight(), id1, id2, f1, f2);
		if (f1&&f2 || left&&right) return root;
		return left ? left : right;
	}

	TreeNode* lca(TreeNode* root, int id1, int id2) {
		bool f1 = false, f2 = false;
		TreeNode* found = dfs(root, id1, id2, f1, f2);
		return f1 && f2 ? found : NULL;
	}


	bool dfsV2(TreeNode* root, int id1, int id2, TreeNode* & found) {
		if (!root) return false;
		int count = 0;
		if (root->getId() == id1) ++count;
		if (root->getId() == id2) ++count;

		bool left = dfsV2(root->getLeft(), id1, id2, found);
		bool right = dfsV2(root->getRight(), id1, id2, found);
		if (left) ++count;
		if (right) ++count;
		if (count == 2) found = root;
		return count > 0;
	}

	TreeNode* lcaV2(TreeNode* root, int id1, int id2) {
		TreeNode* found = NULL;
		dfsV2(root, id1, id2, found);
		return found;
	}

};

class TreeEulerTour
{
public:
	class TreeNode {
		int val;
		TreeNode* l;
		TreeNode* r;
	public:

		// id should start from 0
		TreeNode(int id, TreeNode* left = NULL, TreeNode* right = NULL) {
			this->val = id;
			this->l = left;
			this->r = right;
		}

		int id() {
			return val;
		}
		TreeNode* left() {
			return l;
		}
		TreeNode* right() {
			return r;
		}
	};

	class MinSparseTable {
		// The number of elements in the original input array.
		int n;

		// The maximum power of 2 needed. This value is floor(log2(n))
		int P;

		// Fast log base 2 logarithm lookup table, 1 <= i <= n
		vector<int> log2;

		// The sparse table values.
		vector<vector<int>> dp;

		// Index Table (IT) associated with the values in the sparse table.
		vector<vector<int>> it;

	public:
		MinSparseTable(vector<int>& values) {
			init(values);
		}

		// Returns the index of the minimum element in the range [l, r].
		int queryIndex(int l, int r) {
			int len = r - l + 1;
			int p = log2[len];
			int leftInterval = dp[p][l];
			int rightInterval = dp[p][r - (1 << p) + 1];
			if (leftInterval <= rightInterval)
				return it[p][l];
			else
				return it[p][r - (1 << p) + 1];
		}

	private:
		void init(vector<int>& values) {
			n = values.size();
			P = (int)(log(n) / log(2));
			dp.resize(P + 1, vector<int>(n));
			it.resize(P + 1, vector<int>(n));

			for (int i = 0; i < n; ++i) {
				dp[0][i] = values[i];
				it[0][i] = i;
			}

			log2.resize(n + 1, 0);
			for (int i = 2; i <= n; ++i)
				log2[i] = log2[i / 2] + 1;

			// Build sparse table combining the values of the previous intervals.
			for (int p = 1; p <= P; ++p)
				for (int i = 0; i + (1 << p) <= n; ++i) {
					int leftInterval = dp[p - 1][i];
					int rightInterval = dp[p - 1][i + (1 << (p-1))];
					dp[p][i] = min(leftInterval, rightInterval);

					// propagate the index of the best value
					if (leftInterval <= rightInterval)
						it[p][i] = it[p - 1][i];
					else
						it[p][i] = it[p - 1][i + (1 << (p - 1))];
				}
		}
	};

	int size(TreeNode* root) {
		if (!root) return 0;
		return size(root->left()) + size(root->right()) + 1;
	}

	void setup(TreeNode* root) {
		if (!root) return;
		int n = size(root);
		int eulerTourSize = 2 * n - 1;
		nodeDepth.resize(eulerTourSize);
		nodeOrder.resize(eulerTourSize);
		last.resize(n);
		// Do depth first search to construct Euler tour.
		dfs(root, 0);
	}

	// Construct Euler Tour by populating the `nodeDepth` and `nodeOrder` arrays.
	void dfs(TreeNode* node, int depth) {
		if (!node) return;
		visit(node, depth);
		if (node->left()) {
			dfs(node->left(), depth + 1);
			visit(node, depth);
		}
		if (node->right()) {
			dfs(node->right(), depth + 1);
			visit(node, depth);
		}
	}

	void visit(TreeNode* node, int depth) {
		if (!node) return;
		nodeOrder[tourIndex] = node;
		nodeDepth[tourIndex] = depth;
		last[node->id()] = tourIndex;
		++tourIndex;
	}

	TreeNode* lca(TreeNode* root, int id1, int id2) {
		if (!root) return NULL;
		setup(root);
		// Initialize and build sparse table on the `nodeDepth` array which will
		// allow us to index into the `nodeOrder` array and return the LCA.
		MinSparseTable sparseTable(nodeDepth);
		int l = min(last[id1], last[id2]);
		int r = max(last[id1], last[id2]);
		int i = sparseTable.queryIndex(l, r);
		return nodeOrder[i];
	}

private:
	vector<TreeNode*> nodeOrder;
	vector<int> nodeDepth;
	vector<int> last;
	int tourIndex = 0;

};

class TreeNodeDepth {
public:
	struct TreeNode {
		int val;
		int numSubNodes;
		TreeNode* left;
		TreeNode* right;
		TreeNode(int v=0, TreeNode* l=NULL, TreeNode* r=NULL) {
			val = v;
			left = l;
			right = r;
			numSubNodes = 0;
		}
	};

	void dfsRootDepth(TreeNode* node, int dep) {
		rootDepth += dep;
		if (node->left)
			dfsRootDepth(node->left, dep+1);
		if (node->right)
			dfsRootDepth(node->right, dep+1);
	}

	// first = the number of nodes in sub-tree
	// second = the sum of depth in sub-tree
	pair<int, int> dfsRootSumDepth(TreeNode* node) {
		pair<int, int> p = make_pair(1, 0);
		if (node->left) {
			pair<int, int> left = dfsRootSumDepth(node->left);
			p.second += left.second + left.first;
			p.first += left.first;
		}
		if (node->right) {
			pair<int, int> right = dfsRootSumDepth(node->right);
			p.second += right.second + right.first;
			p.first += right.first;
		}
		rootSumDepth += p.second;
		node->numSubNodes = p.first;
		return p;
	}

	void dfsSumDist(TreeNode* node, int sumDists, TreeNode* target) {
		if (node == target) {
			sumDistsAns = sumDists;
			return;
		}
		if (node->left) {
			int newSumDists = sumDists - node->left->numSubNodes + (numNodes - node->left->numSubNodes);
			dfsSumDist(node->left, newSumDists, target);
		}
		if (node->right) {
			int newSumDists = sumDists - node->right->numSubNodes + (numNodes - node->right->numSubNodes);
			dfsSumDist(node->right, newSumDists, target);
		}
	}

	int rootDepth;
	int numNodes;
	int calcRootDepth(TreeNode* root) {
		rootDepth = 0;
		dfsRootDepth(root, 0);
		return rootDepth;
	}

	int rootSumDepth;
	int calcRootSumDepth(TreeNode* root) {
		rootSumDepth = 0;
		dfsRootSumDepth(root);
		return rootSumDepth;
	}

	int sumDistsAns;
	int calcSumDist(TreeNode* root, TreeNode* target) {
		pair<int, int> p = dfsRootSumDepth(root);
		numNodes = p.first;
		dfsSumDist(root, p.second, target);
		return sumDistsAns;
	}
};

void testIsomorphic() {
	TreeIsomorphism test;
	vector<vector<int>> tree1 = test.createEmptyGraph(5);
	test.addUndirectedEdge(tree1, 2, 0);
	test.addUndirectedEdge(tree1, 3, 4);
	test.addUndirectedEdge(tree1, 2, 1);
	test.addUndirectedEdge(tree1, 2, 3);

	vector<vector<int>> tree2 = test.createEmptyGraph(5);
	test.addUndirectedEdge(tree2, 1, 0);
	test.addUndirectedEdge(tree2, 2, 4);
	test.addUndirectedEdge(tree2, 1, 3);
	test.addUndirectedEdge(tree2, 1, 2);

	if (test.treesAreIsomorphic(tree1, tree2))
		cout << "these trees are isomorphic." << endl;
	else
		cout << "Oops, these trees are NOT isomorphic." << endl;
}

void testLCA() {
	TreeLCA::TreeNode leaf1(4);
	TreeLCA::TreeNode leaf2(5);
	TreeLCA::TreeNode leaf3(6);
	TreeLCA::TreeNode leaf4(7);
	TreeLCA::TreeNode mid1(2, &leaf1, &leaf2);
	TreeLCA::TreeNode mid2(3, &leaf3, &leaf4);
	TreeLCA::TreeNode root(1, &mid1, &mid2);
	bool f1 = false;
	bool f2 = false;
	TreeLCA::TreeNode* found = TreeLCA().lca(&root, 7, 1);
	//TreeLCA::TreeNode* found = TreeLCA().lcaV2(&root, 7, 1);
	int val = found ? found->getId() : -1;
	cout << "testLCA: " << val << endl;
}

void testLCAEulerTour() {
	TreeEulerTour::TreeNode leaf1(3);
	TreeEulerTour::TreeNode leaf2(4);
	TreeEulerTour::TreeNode leaf3(5);
	TreeEulerTour::TreeNode leaf4(6);
	TreeEulerTour::TreeNode mid1(1, &leaf1, &leaf2);
	TreeEulerTour::TreeNode mid2(2, &leaf3, &leaf4);
	TreeEulerTour::TreeNode root(0, &mid1, &mid2);
	TreeEulerTour tour;
	TreeEulerTour::TreeNode* found = tour.lca(&root, 4, 5);
	int val = found ? found->id() : -1;
	cout << "testLCAEulerTour: " << val << endl;
}

void testTreeNodeDepth() {
	TreeNodeDepth::TreeNode leaf8(8);
	TreeNodeDepth::TreeNode leaf9(9);
	TreeNodeDepth::TreeNode leaf1(4, &leaf8, &leaf9);
	TreeNodeDepth::TreeNode leaf2(5);
	TreeNodeDepth::TreeNode leaf3(6);
	TreeNodeDepth::TreeNode leaf4(7);
	TreeNodeDepth::TreeNode mid1(2, &leaf1, &leaf2);
	TreeNodeDepth::TreeNode mid2(3, &leaf3, &leaf4);
	TreeNodeDepth::TreeNode root(1, &mid1, &mid2);
	cout << "TreeNodeDepth::calcRootDepth: " << TreeNodeDepth().calcRootDepth(&root) << endl;
	cout << "TreeNodeDepth::calcRootSumDepth: " << TreeNodeDepth().calcRootSumDepth(&root) << endl;
	cout << "TreeNodeDepth::calcSumDist: " << TreeNodeDepth().calcSumDist(&root, &leaf1) << endl;
}


int main()
{
	testIsomorphic();
	testLCA();
	testLCAEulerTour();
	testTreeNodeDepth();
	return 0;
}
