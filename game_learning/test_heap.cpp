#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <iostream>
#include <map>
#include <unordered_map>
#include <algorithm> 

using namespace std;



class SolutionFindKthLargest
{
	int heapSize;
	void heapifyRecursive(vector<int>& heap, int k) {
		if (k >= heapSize / 2)
			return;
		int left = (k << 1) + 1;
		int right = (k << 1) + 2;
		int smallest = heap[left] <= heap[k] ? left : k;
		if (right < heapSize)
			smallest = heap[right] <= heap[smallest] ? right : smallest;
		if (smallest != k) {
			swap(heap[smallest], heap[k]);
			heapifyRecursive(heap, smallest);
		}
	}

	void heapify(vector<int>& heap, int k) {
		while (true) {
			if (k >= heapSize / 2)
				break;
			int left = (k << 1) + 1;
			int right = (k << 1) + 2;
			int smallest = heap[left] <= heap[k] ? left : k;
			if (right < heapSize)
				smallest = heap[right] <= heap[smallest] ? right : smallest;
			if (smallest == k) break;
			swap(heap[smallest], heap[k]);
			k = smallest;
		}
	}

	void buildMinHeap(vector<int>& nums, int k) {
		heapSize = k;
		for (int i = k / 2 - 1; i >= 0; --i)
			heapifyRecursive(nums, i);
	}

public:
	vector<int> findKthLargest(vector<int>& nums, int k) {
		buildMinHeap(nums, k);
		for (int i = k; i < nums.size(); ++i) {
			if (nums[i] > nums[0]) {
				swap(nums[i], nums[0]);
				heapify(nums, 0);
			}
		}
		vector<int> result;
		for (int i = 0; i < k; ++i)
			result.push_back(nums[i]);
		sort(result.begin(), result.end());
		reverse(result.begin(), result.end());

		return result;
	}
};

/*
int main()
{
	//vector<int> nums = { 11, 3, 2, 1, 15, 5, 4, 45, 88, 96, 50, 45 };
	//int k = 3;
	vector<int> nums = {
		335, 501, 170, 725, 479, 359, 963, 465,
		706, 146, 282, 828, 962, 492, 996, 943,
		828, 437, 392, 605, 903, 154, 293, 383,
		422, 717, 719, 896, 448, 727, 772, 539,
		870, 913, 668, 300, 36, 895, 704, 812,
		323, 334
	};
	int k = 30;
	cout << "find_kth_largest: " << endl;
	vector<int> result = SolutionFindKthLargest().findKthLargest(nums, k);
	for (int i : result)
		cout << i << "  ";
	cout << endl;
	return 0;
}
*/