#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    int jump(vector<int>& nums) {
        int n = nums.size();
        int farthest = 0;
        int curr_jump_end = 0;
        int res = 0;
        for (int i = 0; i < n-1; ++i) {
            farthest = max(farthest, i+nums[i]);
            if (i == curr_jump_end) {
                curr_jump_end = farthest;
                ++res;
            }
        }
        return res;
    }
};


void test_jump() {
    vector<int> nums = {2,3,1,1,4};
    cout << "jump result: " << Solution().jump(nums) << endl;
}


int main() {
    test_jump();
    return 0;
}