#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <iostream>
#include <map>
#include <queue>
#include <unordered_map>
#include <algorithm> 
#include <stack>

using namespace std;


class SolutionCalculation {
public:
	int calculateStack(vector<char>& ops, vector<int>& nums) {
		int ret = nums.back();
		nums.pop_back();
		while (ops.size() > 0) {
			char op = ops.back();
			ops.pop_back();
			if (op == ')')
				return ret;
			int rightNumber = nums.back();
			nums.pop_back();
			if (op == '+')  ret += rightNumber;
			else if (op == '-') ret -= rightNumber;
		}
		return ret;
	}

	int calculate(string& s) {
		vector<char> ops;
		vector<int> nums;
		string tmp;

		int n = s.length();
		for (int i = n - 1; i >= 0; --i) {
			char c = s.at(i);
			if (isdigit(c))
				tmp += c;
			else if (c == ')')
				ops.push_back(c);
			else if (c == '(') {
				if (tmp.length() > 0) {
					nums.push_back(atoi(tmp.c_str()));
					tmp = "";
				}
				nums.push_back(calculateStack(ops, nums));
			}
			else if (c == '+' || c == '-') {
				if (tmp.length() > 0) {
					nums.push_back(atoi(tmp.c_str()));
					tmp = "";
				}
				ops.push_back(c);
			}
			if (i == 0 && tmp.length() > 0)
				nums.push_back(atoi(tmp.c_str()));
		}
		return calculateStack(ops, nums);

	}
};


class SolutionAllParenthesis {
	void recursive(int n, int l, int r, vector<string>& result, string s) {
		if (s.length() == 2 * n) {
			result.push_back(s);
			return;
		}
		if (l < n)
			recursive(n, l + 1, r, result, s + "(");
		if (r < l)
			recursive(n, l, r + 1, result, s + ")");
	}
public:
	vector<string> allParenthesis(int n) {
		vector<string> result;
		if (n <= 0) return result;
		recursive(n, 0, 0, result, string());
		return result;
	}
};


class SolutionTotalShapes {
public:
	vector<int> id;
	vector<int> sz;

	int find(int p) {
		int root = p;
		while (root != id[root])
			root = id[root];
		while (p != root) {
			int next = id[p];
			id[p] = root;
			p = next;
		}
		return root;
	}

	void unify(int p, int q) {
		p = find(p);
		q = find(q);
		if (p == q) return;
		if (sz[p] < sz[q]) swap(p, q);
		id[q] = p;
		sz[p] += sz[q];
	}

	int xShape(vector<vector<char>>& grid) {
		// Code here
		int m = grid.size();
		int n = grid[0].size();
		unordered_set<int> roots;
		id.resize(m *n);
		sz.resize(m *n);
		for (int i = 0; i < m * n; ++i) {
			id[i] = i;
			sz[i] = 1;
		}
		for (int i = 0; i < m; ++i)
			for (int j = 0; j < n; ++j) {
				if (grid[i][j] == 'O') continue;
				if (i > 0 && grid[i - 1][j] == 'X')
					unify((i - 1) * n + j, i * n + j);
				if (j > 0 && grid[i][j - 1] == 'X')
					unify(i * n + j - 1, i * n + j);
			}
		for (int i = 0; i < m; ++i)
			for (int j = 0; j < n; ++j) {
				if (grid[i][j] == 'O') continue;
				roots.insert(find(i * n + j));
			}
		return roots.size();
	}
};


class SolutionSudoku {
public:
#define N 9
	int isValid(vector<vector<int>> mat){
		// code here
		bool rowId[N] = { 0 };
		bool colId[N][N] = { 0 };
		bool boxId[N][N] = { 0 };
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				if (j == 0) memset(rowId, 0, sizeof(rowId));
				int box = (i / 3) * (N / 3) + (j / 3);
				int curr = mat[i][j];
				if (curr == 0)
					continue;
				--curr;
				if (rowId[curr])
					return 0;
				else
					rowId[curr] = true;
				if (boxId[box][curr])
					return 0;
				else
					boxId[box][curr] = true;
				if (colId[j][curr])
					return 0;
				else
					colId[j][curr] = true;
			}
		}
		return 1;
	}
};


class LRUCache {
public:
#define MAX_NUM 3000
	int capacity;
	int val[MAX_NUM];
	list<int>::iterator itr[MAX_NUM];
	list<int> keys;

	LRUCache(int capacity) {
		this->capacity = capacity;
		memset(val, -1, sizeof(val));
	}


	int get(int key) {
		if (val[key] >= 0) {
			keys.erase(itr[key]);
			keys.push_front(key);
			itr[key] = keys.begin();
		}
		return val[key];
	}

	void put(int key, int value) {
		if (val[key] >= 0)
			keys.erase(itr[key]);
		else if (keys.size() >= capacity) {
			int oldKey = keys.back();
			keys.pop_back();
			val[oldKey] = -1;
		}
		val[key] = value;
		keys.push_front(key);
		itr[key] = keys.begin();
	}
};

class SolutionLargestRectangleArea {
public:
    
    int largestRectangleArea(vector<int>& heights) {
        stack<int> st;
        int largest = 0;
        heights.push_back(INT_MIN);
        for (int i = 0; i < heights.size(); i++) {
            while (!st.empty() && heights[st.top()] > heights[i]) {
                int top = st.top();
                st.pop();
                largest = max(largest, heights[top] * (st.empty() ? i : i - 1 - st.top()));
            }
            st.push(i);
        }
        return largest;
    }
};



void testCalculation() {
	string s("1 + 1");
	cout << "SolutionCalculation " << s.c_str() << ": " << SolutionCalculation().calculate(s) << endl;
	s = " 2-1 + 2 ";
	cout << "SolutionCalculation " << s.c_str() << ": " << SolutionCalculation().calculate(s) << endl;
	s = "(1+(4+5+2)-3)+(6+8)";
	cout << "SolutionCalculation " << s.c_str() << ": " << SolutionCalculation().calculate(s) << endl;
}

void testAllParenthesis() {
	int n = 1;
	int i = 0;
	vector<string> result;
	result = SolutionAllParenthesis().allParenthesis(n);
	cout << "SolutionAllParenthesis: n=" << n << endl;
	for (string& s : result)
		cout << ++i << ":" << s.c_str() << "  ";
	cout << endl;
	n = 2;
	i = 0;
	result = SolutionAllParenthesis().allParenthesis(n);
	cout << "SolutionAllParenthesis: n=" << n << endl;
	for (string& s : result)
		cout << ++i << ":" << s.c_str() << "  ";
	cout << endl;
	n = 3;
	i = 0;
	result = SolutionAllParenthesis().allParenthesis(n);
	cout << "SolutionAllParenthesis: n=" << n << endl;
	for (string& s : result)
		cout << ++i << ":" << s.c_str() << "  ";
	cout << endl;
	n = 4;
	i = 0;
	result = SolutionAllParenthesis().allParenthesis(n);
	cout << "SolutionAllParenthesis: n=" << n << endl;
	for (string& s : result)
		cout << ++i << ":" << s.c_str() << "  ";
	cout << endl;
}

void testXShapes() {
	vector<vector<char>> grid = {
		{ 'O', 'X', 'O', 'X', 'X' },
		{ 'O', 'O', 'X', 'X', 'X' },
		{ 'O', 'X', 'X', 'O', 'O' },
		{ 'X', 'X', 'O', 'O', 'O' },
		{ 'O', 'X', 'X', 'X', 'O' },
		{ 'X', 'X', 'O', 'O', 'X' },
	};
	cout << "XShape: " << SolutionTotalShapes().xShape(grid) << endl;
}

void testSudoku() {
	/*
	vector<vector<int>> mat = {
	{ 3, 0, 6, 5, 0, 8, 4, 0, 0 },
	{ 5, 2, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 8, 7, 0, 0, 0, 0, 3, 1 },
	{ 0, 0, 3, 0, 1, 0, 0, 8, 0 },
	{ 9, 0, 0, 8, 6, 3, 0, 0, 5 },
	{ 0, 5, 0, 0, 9, 0, 6, 0, 0 },
	{ 1, 3, 0, 0, 0, 0, 2, 5, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 7, 4 },
	{ 0, 0, 5, 2, 0, 6, 3, 0, 0 },
	};
	*/

	vector<vector<int>> mat = {
		{ 0, 0, 0, 0, 0, 0, 0, 0, 7 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 7, 0, 0 },
		{ 0, 0, 0, 0, 3, 0, 0, 0, 0 },
		{ 3, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 8, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 3, 0, 0, 0, 0, 0, 0, 0, 0 },
	};

	cout << "SolutionSudoku isValid: " << SolutionSudoku().isValid(mat) << endl;
}

void testLRUCache() {
	cout << "LRUCache: " << endl;
	LRUCache lRUCache(2);
	lRUCache.put(1, 1); // cache is {1=1}
	lRUCache.put(2, 2); // cache is {1=1, 2=2}
	cout << "get(1): " << lRUCache.get(1) << endl;    // return 1
	lRUCache.put(3, 3); // LRU key was 2, evicts key 2, cache is {1=1, 3=3}
	cout << "get(2): " << lRUCache.get(2) << endl;    // returns -1 (not found)
	lRUCache.put(4, 4); // LRU key was 1, evicts key 1, cache is {4=4, 3=3}
	cout << "get(1): " << lRUCache.get(1) << endl;    // returns -1 (not found)
	cout << "get(3): " << lRUCache.get(3) << endl;    // returns 3 
	cout << "get(4): " << lRUCache.get(4) << endl;    // returns 3 
}

void testLargestRectangleArea() {
    //vector<int> heights = {2, 1, 5, 6, 2, 3};
    //vector<int> heights = {1, 1};
    //vector<int> heights = {4, 2, 0, 3, 2, 4, 3, 4};
    vector<int> heights = {2, 1, 2};
    cout << "LargestRectangleArea: " << SolutionLargestRectangleArea().largestRectangleArea(heights) << endl;
}

/*
int main()
{
	testCalculation();
	testAllParenthesis();
	testXShapes();
	testSudoku();
	testLRUCache();
    testLargestRectangleArea();
	return 0;
}
*/