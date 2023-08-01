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

class FloydWarshall {
public:
	vector<vector<float>> floydWarshall(vector<vector<float>>& matrix) {
		int numNodes = matrix.size();
		vector<vector<float>> dp = matrix; // optimized, original should be dp[k][i][j]
		vector<vector<int>> next(numNodes, vector<int>(numNodes, -1));
		for (int i = 0; i < numNodes; ++i)
			for (int j = 0; j < numNodes; ++j)
				if (matrix[i][j] != INFINITY)
					next[i][j] = j;

		// Compute all pairs shortest paths.
		for (int k = 0; k < numNodes; ++k)
			for (int i = 0; i < numNodes; ++i)
				for (int j = 0; j < numNodes; ++j) {
					// dp[k][i][j] = min(dp[k-1][i][j], dp[k-1][i][k] + dp[k-1][k][j]);
					if (dp[i][k] + dp[k][j] < dp[i][j]) {
						dp[i][j] = dp[i][k] + dp[k][j];
						next[i][j] = next[i][k];
					}
				}

		// Identify negative cycles by propagating the value 'NEGATIVE_INFINITY'
		// to every edge that is part of or reaches into a negative cycle.
		for (int k = 0; k < numNodes; ++k)
			for (int i = 0; i < numNodes; ++i)
				for (int j = 0; j < numNodes; ++j) {
					if (dp[i][k] + dp[k][j] < dp[i][j]) {
						dp[i][j] = -INFINITY;
						next[i][j] = -2; // reach negative cycle
					}
				}


		return dp;
	}
};


void testFloydWarshall() {
	vector<vector<float>> matrix = {
		{0, 2, 5, INFINITY, INFINITY, INFINITY, 10},
		{INFINITY, 0, 2, INFINITY, 11, INFINITY, INFINITY},
		{INFINITY, INFINITY, 0, INFINITY, INFINITY, INFINITY, 2},
		{INFINITY, INFINITY,INFINITY, 0, INFINITY, INFINITY, INFINITY},
		{INFINITY, INFINITY,INFINITY, INFINITY, 0, 1, INFINITY},
		{INFINITY, INFINITY,INFINITY, INFINITY, -2, 0, INFINITY},
		{INFINITY, INFINITY,INFINITY, INFINITY, INFINITY, 11, 0},
	};

	FloydWarshall fw;
	vector<vector<float>> dp = fw.floydWarshall(matrix);
	cout << "FloydWarshall: " << endl;
	for (int i = 0; i < dp.size(); ++i) {
		for (int j = 0; j < dp.size(); ++j)
			cout << dp[i][j] << "  ";
		cout << endl;
	}

}

/*
int main() {
	testFloydWarshall();
	return 0;
}
*/