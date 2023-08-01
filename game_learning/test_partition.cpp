#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <iostream>
#include <map>
#include <unordered_map>
#include <algorithm> 

using namespace std;

class SolutionPartition
{
public:
	int partition(vector<int>& arr, int left, int right) {
		int pivotIdx = arr[right];
		int idx = left;
		for (int i = left; i < right; ++i)
			if (arr[i] <= pivotIdx)
				swap(arr[i], arr[idx++]);
		swap(arr[idx], arr[right]);
		return idx;
	}

	int findKthLargest(vector<int>& arr, int k) {
		int left = 0, right = arr.size()-1;
		while (left <= right) {
			int pivotIdx = partition(arr, left, right);
			if (pivotIdx == arr.size() - k)
				return arr[pivotIdx];
			else if (pivotIdx > arr.size() - k)
				right = pivotIdx - 1;
			else
				left = pivotIdx + 1;
		}
		return -1;
	}
};

void testPartition() {
	vector<int> arr = {
		5, 3, 7, 2, 4, 1, 6
	};
	int k = 3;
	cout << "findKthLargest: " << SolutionPartition().findKthLargest(arr, k) << endl;
}

int main()
{
	testPartition();
	return 0;
}