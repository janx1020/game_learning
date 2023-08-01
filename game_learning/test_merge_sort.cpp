#include <iostream>
#include <vector>
using namespace std;

class MergeSort {

public:
    void merge(vector<int>& arr, int left, int mid, int right) {
        // create left temp arr: l[left...mid], right temp arr: r[mid+1...right]
        int len1 = mid - left + 1, len2 = right - mid;
        int L[len1], R[len2];
        for (int i = 0; i < len1; ++i)
            L[i] = arr[left+i];
        for (int j = 0; j < len2; ++j)
            R[j] = arr[mid+1+j];
        int i = 0, j = 0, k = left;
        while (i < len1 && j < len2) {
            if (L[i] <= R[j])
                arr[k++] = L[i++];
            else
                arr[k++] = R[j++];
        }
        while (i < len1)
            arr[k++] = L[i++];
        while (j < len2)
            arr[k++] = R[j++];

    }

    void merge_sort(vector<int>& arr, int left, int right) {
        if (left >= right) return;

        int mid = left + (right - left) / 2;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid+1, right);
        merge(arr, left, mid, right);
    }

    void merge_sort(vector<int>& arr) {
        merge_sort(arr, 0, arr.size()-1);
    }
};

class CountSmallerSolution {

public:
    /* old sort will cause TLE
    void merge(vector<int>& arr, int left, int mid, int right, vector<int>& res, vector<int>& indices) {
        // create left temp arr: l[left...mid], right temp arr: r[mid+1...right]
        int len1 = mid - left + 1, len2 = right - mid;
        int L[len1], R[len2];
        for (int i = 0; i < len1; ++i)
            L[i] = arr[left+i];
        for (int j = 0; j < len2; ++j)
            R[j] = arr[mid+1+j];
        // create temporary index arrary
        vector<int> new_indices(right-left+1);
        int i = 0, j = 0, k = left, new_idx = 0;
        while (i < len1 && j < len2) {
            if (L[i] <= R[j]) {
                res[indices[left+i]] += j;
                new_indices[new_idx++] = indices[left + i];
                arr[k++] = L[i];
                ++i;
            }
            else {
                new_indices[new_idx++] = indices[j + mid + 1];
                arr[k++] = R[j];
                ++j;
            }
        }
        while (i < len1) {
            res[indices[left+i]] += j;
            new_indices[new_idx++] = indices[left + i];
            arr[k++] = L[i];
            ++i;
        }
        while (j < len2) {
            new_indices[new_idx++] = indices[j + mid + 1];
            arr[k++] = R[j];
            ++j;
        }
        for (int i = left; i <= right; ++i)
            indices[i] = new_indices[i-left];
        cout << "indices: ";
        for (auto it = indices.begin(); it != indices.end(); ++it)
            cout << *it << " ";
        cout << endl;
    }
    */
    void merge(vector<int>& nums, int left, int mid, int right, vector<int>& res, vector<int>& indices) {
        int n1 = mid - left + 1, n2 = right - mid;
        vector<int> new_indices(right-left+1);
        
        int i = 0, j = 0, new_idx = 0;
        while (i < n1 || j < n2) {
            if (j >= n2 || (i < n1 && (nums[indices[left+i]] <= nums[indices[j+mid+1]]))) {
                new_indices[new_idx++] = indices[left+i];
                res[indices[left+i]] += j;
                ++i;
            } else {
                new_indices[new_idx++] = indices[j+mid+1];
                ++j;
            }
        }
       
        for (int i = left; i <= right; ++i)
            indices[i] = new_indices[i-left];
    }

    void merge_sort(vector<int>& arr, int left, int right, vector<int>& res, vector<int>& indices) {
        if (left >= right) return;

        int mid = left + (right - left) / 2;
        merge_sort(arr, left, mid, res, indices);
        merge_sort(arr, mid+1, right, res, indices);
        merge(arr, left, mid, right, res, indices);
    }

    vector<int> countSmaller(vector<int>& arr) {
        vector<int> res(arr.size());
        vector<int> indices(arr.size());
        for (int i = 0; i < arr.size(); ++i)
            indices[i] = i;
        merge_sort(arr, 0, arr.size()-1, res, indices);
        return res;
    }
};

void test_merge_sort() {
    vector<int> arr = {3, 1, 2, 5, 9, 7, 6, 4};
    cout << "original: ";
    for (auto it = arr.begin(); it != arr.end(); ++it)
        cout << *it << "  ";
    cout << endl;
    cout << "merge sort result: ";
    MergeSort().merge_sort(arr);
    for (auto it = arr.begin(); it != arr.end(); ++it)
        cout << *it << "  ";
    cout << endl;
}

void test_count_smaller() {
    //vector<int> arr = {3, 1, 2, 5, 9, 7, 6, 4};
    vector<int> arr = {0, 2, 1};
    cout << "original: ";
    for (auto it = arr.begin(); it != arr.end(); ++it)
        cout << *it << "  ";
    cout << endl;
    vector<int> res = CountSmallerSolution().countSmaller(arr);
    cout << "merge sort result: ";
    for (auto it = arr.begin(); it != arr.end(); ++it)
        cout << *it << "  ";
    cout << endl << "count smaller result: ";
    for (auto it = res.begin(); it != res.end(); ++it)
        cout << *it << "  ";
    cout << endl;
}


int main() {
    //test_merge_sort();
    test_count_smaller();
    return 0;
}