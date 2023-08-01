#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <iostream>
#include <map>
#include <unordered_map>
#include <algorithm> 

using namespace std;


class SegmentTree
{
public:
	SegmentTree(vector<int>& nums) {
		n = nums.size();
		if (n > 0) {
			tree.resize(2 * n, 0);
			buildSegmentTree(nums);
		}
	}

	void update(int idx, int val) {
		if (!validIndex(idx))
			return;
		idx += n;
		tree[idx] = val;
		// O(N)
		/*
		for (int i = n - 1; i > 0; --i)
		tree[i] = tree[2 * i] + tree[2 * i + 1];
		*/
		// O(log(N))
		while (idx > 0) {
			int left = idx;
			int right = idx;
			if (idx % 2 == 0) ++right;
			else --left;
			tree[idx / 2] = tree[left] + tree[right];
			idx /= 2;
		}
	}

	int sumRange(int left, int right) {
		if (!validIndex(left) || !validIndex(right))
			return 0;
		left += n;
		right += n;
		if (left == right)
			return tree[left];
		int sum = 0;
		while (left <= right) {
			if (left % 2 == 1) {
				sum += tree[left];
				++left;
			}
			if (right % 2 == 0) {
				sum += tree[right];
				--right;
			}
			left /= 2;
			right /= 2;
		}

		return sum;
	}

	void printTree() {
		cout << "segment tree: " << endl;
		for (int i = 0; i < tree.size(); ++i)
			cout << tree[i] << "  ";
		cout << endl;
	}

private:
	bool validIndex(int idx) {
		return 0 <= idx < n;
	}
	void buildSegmentTree(vector<int>& nums) {
		for (int i = n, j = 0; i < 2 * n; ++i, ++j)
			tree[i] = nums[j];
		for (int i = n - 1; i > 0; --i)
			tree[i] = tree[2 * i] + tree[2 * i + 1];
	}

private:
	int n; // length of nums
	vector<int> tree;
};

class SolutionMaxEvents {
	vector<int> tree;
	int n;
	void buildSegmentTree(int l, int r) {
		n = r - l + 1;
		tree.resize(n << 1, n + 1);
		for (int i = n, j = l; i < 2 * n; ++i, ++j)
			tree[i] = j;
		for (int i = n - 1; i > 0; --i)
			tree[i] = min(tree[i << 1], tree[(i << 1) + 1]);
	}

	void update(int idx) {
		idx += n;
		tree[idx] = INT_MAX;
		while (idx > 0) {
			int left = idx;
			int right = idx;
			if (idx % 2 == 0) ++right;
			else              --left;
			tree[idx / 2] = min(tree[left], tree[right]);
			idx /= 2;
		}
	}

	int queryRange(int left, int right) {
		left += n;
		right += n;
		int val = INT_MAX;
		while (left <= right) {
			if (left % 2 == 1) {
				val = min(val, tree[left]);
				++left;
			}
			if (right % 2 == 0) {
				val = min(val, tree[right]);
				--right;
			}
			left /= 2;
			right /= 2;
		}
		return val == INT_MAX ? 0 : val;
	}


public:
	int maxEvents_SegmentTree(vector<vector<int>>& events) {
		int l = events[0][0];
		int r = events[0][1];
		sort(events.begin(), events.end(), [&](const vector<int>& a, const vector<int>&b) {
			l = min(l, min(a[0], b[0]));
			r = max(r, max(a[1], b[1]));
			if (a[1] == b[1]) return a[0] < b[0];
			return a[1] < b[1];
		});
		buildSegmentTree(l, r);
		int cnt = 0;
		for (auto e : events) {
			int q = queryRange(e[0] - l, e[1] - l);
			if (q != 0 && q <= e[1]) {
				++cnt;
				update(q - l);
			}
		}
		return cnt;
	}

	int maxEvents_Greedy(vector<vector<int>>& events) {
		int l = events[0][0];
		int r = events[0][1];
		sort(events.begin(), events.end(), [&](const vector<int> & a, const vector<int>&b) {
			l = min(l, min(a[0], b[0]));
			r = max(r, max(a[1], b[1]));
			if (a[1] == b[1]) return a[0] < b[0];
			return a[1] < b[1]; 
		});
		int n = events.size();
		set<int> days;
		for (int i = l; i <= r; ++i)
			days.insert(i);
		int cnt = 0;
		for (auto e : events) {
			auto itr = days.lower_bound(e[0]);
			if (itr == days.end() || *itr > e[1])
				continue;
			days.erase(*itr);
			++cnt;
   		    if (cnt == n) break;
		}
		return cnt;
	}
};

/*
int main()
{
	vector<int> nums = { 2, 5, 7, 9, 6, 10 };
	SegmentTree segTree(nums);
	segTree.printTree();
	segTree.update(2, 6);
	segTree.printTree();
	cout << "sumRange(1, 4): " << segTree.sumRange(1, 4) << endl;
	cout << "sumRange(5, 5): " << segTree.sumRange(5, 5) << endl;

	//[[1,7],[1,6],[1,5],[1,4],[1,3],[1,2],[1,1]]
	//[[1,2],[2,3],[3,4]]
	//[[1,2],[2,3],[3,4],[1,2]]
	//[[1,4],[4,4],[2,2],[3,4],[1,1]]
	//[[1,100000]]
	//[[1,1],[1,2],[1,3],[1,4],[1,5],[1,6],[1,7]]
	//[[7,11],[7,11],[7,11],[9,10],[9,11]]
	//expect:
	//7
	//3
	//4
	//4
	//1
	//7
	vector<vector<int>> in1 = { { 1, 7 }, { 1, 6 }, { 1, 5 }, { 1, 4 }, { 1, 3 }, { 1, 2 }, { 1, 1 } };
	cout << "maxEvents_Greedy: " << SolutionMaxEvents().maxEvents_SegmentTree(in1) << endl;
	vector<vector<int>> in2 = { { 1, 2 }, { 2, 3 }, { 3, 4 } };
	cout << "maxEvents_Greedy: " << SolutionMaxEvents().maxEvents_SegmentTree(in2) << endl;
	vector<vector<int>> in3 = { { 1, 2 }, { 2, 3 }, { 3, 4 }, { 1, 2 } };
	cout << "maxEvents_Greedy: " << SolutionMaxEvents().maxEvents_SegmentTree(in3) << endl;
	vector<vector<int>> in4 = { { 1, 4 }, { 4, 4 }, { 2, 2 }, { 3, 4 }, { 1, 1 } };
	cout << "maxEvents_Greedy: " << SolutionMaxEvents().maxEvents_SegmentTree(in4) << endl;
	vector<vector<int>> in5 = { { 1, 10 } };
	cout << "maxEvents_Greedy: " << SolutionMaxEvents().maxEvents_SegmentTree(in5) << endl;
	vector<vector<int>> in6 = { { 1, 1 }, { 1, 2 }, { 1, 3 }, { 1, 4 }, { 1, 5 }, { 1, 6 }, { 1, 7 } };
	cout << "maxEvents_Greedy: " << SolutionMaxEvents().maxEvents_SegmentTree(in6) << endl;
	vector<vector<int>> in7 = { { 7, 11 }, { 7, 11 }, { 7, 11 }, { 9, 10 }, { 9, 11 } };
	cout << "maxEvents_Greedy: " << SolutionMaxEvents().maxEvents_SegmentTree(in7) << endl;

	return 0;
}
*/