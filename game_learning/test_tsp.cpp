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


// travelling salesman problem
class TSPBruteForce {
public:
	struct OptPath {
		vector<int> path;
		float minCost;
		OptPath() {
			minCost = INFINITY;
		}
	};

private:
	float computeCost(vector<vector<float>>& matrix, vector<int>& permutation) {
		float cost = 0;
		for (int i = 1; i < permutation.size(); ++i) {
			int from = permutation[i - 1];
			int to = permutation[i];
			cost += matrix[from][to];
		}
		// add the cost of the last back to start
		int last = permutation.back();
		int start = permutation.front();
		cost += matrix[last][start];

		return cost;
	}

	// Generates the next ordered permutation in-place (skips repeated permutations).
	// Calling this when the array is already at the highest permutation returns false.
	// Recommended usage is to start with the smallest permutations and use a do while
	// loop to generate each successive permutations (see main for example).
	bool nextPermutation(vector<int>& permutation) {
		int first = getFirst(permutation);
		if (first == -1) return false;
		int toSwap = permutation.size() - 1;
		while (permutation[first] >= permutation[toSwap])
			--toSwap;
		swap(permutation, first++, toSwap);
		toSwap = permutation.size() - 1;
		while (first < toSwap)
			swap(permutation, first++, toSwap--);
		return true;
	}

	int getFirst(vector<int>& permutation) {
		for (int i = permutation.size() - 2; i >= 0; --i)
			if (permutation[i] < permutation[i + 1])
				return i;
		return -1;
	}

	void swap(vector<int>& permutation, int i, int j) {
		int temp = permutation[i];
		permutation[i] = permutation[j];
		permutation[j] = temp;
	}

	void recursive(vector<vector<float>>& matrix, OptPath& op, vector<int>& permutation, int l, int r) {
		if (l == r) {
			float cost = computeCost(matrix, permutation);
			if (cost < op.minCost) {
				op.minCost = cost;
				op.path = permutation;
			}
		}
		else
			for (int i = l; i <= r; ++i) {
				swap(permutation, l, i);
				recursive(matrix, op, permutation, l + 1, r);
				swap(permutation, l, i);
			}
	}

public:
	OptPath tspBruteForceMyCode(vector<vector<float>>& matrix) {
		auto start = chrono::high_resolution_clock::now();

		int n = matrix.size();
		OptPath op;
		op.path.resize(n, -1);
		vector<int> permutation(n);
		for (int i = 0; i < n; ++i)
			permutation[i] = i;

		recursive(matrix, op, permutation, 0, n - 1);

		auto stop = chrono::high_resolution_clock::now();
		auto micro = chrono::duration_cast<chrono::microseconds>(stop - start);
		auto seconds = chrono::duration_cast<chrono::seconds>(stop - start);

		// To get the value of duration use the count() 
		// member function on the duration object 
		cout << "time spent: " << seconds.count() << " seconds | " << micro.count() << " microseconds." << endl;
		return op;
	}

	OptPath tspBruteForce(vector<vector<float>>& matrix) {
		auto start = chrono::high_resolution_clock::now();

		int n = matrix.size();
		OptPath op;
		op.path.resize(n, -1);
		vector<int> permutation(n);
		for (int i = 0; i < n; ++i)
			permutation[i] = i;

		do {

			float cost = computeCost(matrix, permutation);
			if (cost < op.minCost) {
				op.minCost = cost;
				op.path = permutation;
			}
		} while (nextPermutation(permutation));

		auto stop = chrono::high_resolution_clock::now();
		auto micro = chrono::duration_cast<chrono::microseconds>(stop - start);
		auto seconds = chrono::duration_cast<chrono::seconds>(stop - start);

		// To get the value of duration use the count() 
		// member function on the duration object 
		cout << "time spent: " << seconds.count() << " seconds | " << micro.count() << " microseconds." << endl;
		return op;
	}

};

class TSPDynamicProgramming
{
public:
	struct OptPath {
		vector<int> path;
		float minCost;
		OptPath() {
			minCost = INFINITY;
		}
	};

private:
	int startNode;
	int endState;
	float dp(vector<vector<float>>& matrix, int node, int state, vector<vector<float>>& memo, vector<vector<int>>& prev) {
		if (state == endState)
			return matrix[node][startNode];
		if (memo[node][state] != INFINITY)
			return memo[node][state];
		int index = -1;
		float minCost = INFINITY;;
		for (int next = 0; next < matrix.size(); ++next) {
			// Skip if the next node has already been visited.
			if ((state & (1 << next)) != 0) continue;

			int nextState = state | (1 << next);
			float cost = matrix[node][next] + dp(matrix, next, nextState, memo, prev);
			if (cost < minCost) {
				minCost = cost;
				index = next;
			}
		}
		memo[node][state] = minCost;
		prev[node][state] = index;
		return minCost;
	}

public:
	OptPath tspDPRecursive(vector<vector<float>>& matrix) {
		auto start = chrono::high_resolution_clock::now();

		int n = matrix.size();
		OptPath op;
		startNode = 0;
		endState = (1 << n) - 1;
		int state = 1 << startNode;
		vector<vector<float>> memo(n, vector<float>(1 << n, INFINITY));
		vector<vector<int>> prev(n, vector<int>(1 << n, -1));
		op.minCost = dp(matrix, startNode, state, memo, prev);

		// regenerate path
		int index = startNode;
		while (1) {
			op.path.push_back(index);
			int nextIndex = prev[index][state];
			if (nextIndex == -1) break;
			int nextState = state | (1 << nextIndex);
			state = nextState;
			index = nextIndex;
		}

		auto stop = chrono::high_resolution_clock::now();
		auto micro = chrono::duration_cast<chrono::microseconds>(stop - start);
		auto seconds = chrono::duration_cast<chrono::seconds>(stop - start);

		// To get the value of duration use the count() 
		// member function on the duration object 
		cout << "time spent: " << seconds.count() << " seconds | " << micro.count() << " microseconds." << endl;
		return op;
	}
};

void testTSPBruteForce() {
	int n = 10;
	vector<vector<float>> matrix(n, vector<float>(n, 100));
	float edgeCost = 5.0;
	vector<int> optimalTour = { 2, 7, 6, 1, 9, 8, 5, 3, 4, 0, 2 };
	for (int i = 1; i < n; ++i)
		matrix[optimalTour[i - 1]][optimalTour[i]] = edgeCost;

	cout << "TSPBruteForce:" << endl;
	//TSPBruteForce::OptPath op = TSPBruteForce().tspBruteForce(matrix);
	TSPBruteForce::OptPath op = TSPBruteForce().tspBruteForceMyCode(matrix);
	cout << "minCost: " << op.minCost << endl;
	cout << "optimal path: " << op.minCost << endl;
	for (auto node : op.path)
		cout << node << "  ";
	cout << endl;

}

void testTSPDPRecursive() {
	int n = 10;
	vector<vector<float>> matrix(n, vector<float>(n, 100));
	float edgeCost = 5.0;
	vector<int> optimalTour = { 2, 7, 6, 1, 9, 8, 5, 3, 4, 0, 2 };
	for (int i = 1; i < n; ++i)
		matrix[optimalTour[i - 1]][optimalTour[i]] = edgeCost;

	cout << "TSPDynamicProgramming::tspDPRecursive:" << endl;
    TSPDynamicProgramming::OptPath op = TSPDynamicProgramming().tspDPRecursive(matrix);
	cout << "minCost: " << op.minCost << endl;
	cout << "optimal path: " << op.minCost << endl;
	for (auto node : op.path)
		cout << node << "  ";
	cout << endl;

}


/*
int main()
{
	testTSPBruteForce();
	testTSPDPRecursive();
	return 0;
}
*/